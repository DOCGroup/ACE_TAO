#include "orbsvcs/AV/sfp.h"
#include "ace/ARGV.h"

// $Id$

// constructor.
SFP::SFP (CORBA::ORB_ptr orb,
          ACE_Reactor* reactor,
          ACE_Time_Value timeout1,
          ACE_Time_Value timeout2,
          SFP_Callback *callback)
  :orb_ (orb),
   reactor_ (reactor),
   encoder_ (0),
   decoder_ (0),
   timeout1_ (timeout1),
   timeout2_ (timeout2),
   start_tries_ (10),
   startReply_tries_ (10),
   callback_ (callback),
   sequence_num_ (0)
{

}

void
SFP::set_cdr_length (void)
{
  CORBA::ULong bodylen = encoder_->total_length ();
  char* buf = ACE_const_cast(char*,encoder_->buffer ());
  buf += 4;
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast(CORBA::ULong*,buf) = bodylen;
#else
  if (!encoder_->do_byte_swap ())
    {
      *ACE_reinterpret_cast(CORBA::ULong*, buf) = bodylen;
    }
  else
    {
      ACE_CDR::swap_4 (ACE_reinterpret_cast(char*,&bodylen), buf);
    }
#endif
}

// Copies length bytes from the given message into the
// CDR buffer. Returns 0 on success, -1 on failure
int
SFP::create_cdr_buffer (char *message,
                        size_t length)
{
  if (this->decoder_)
    delete this->decoder_;

  ACE_NEW_RETURN (this->decoder_,
                  TAO_InputCDR (message,
                                length),
                  -1);

  ACE_OS::memcpy (this->decoder_->rd_ptr (),
                  message,
                  length);

  return 0;
}

// Start the active end of the stream.
int
SFP::start_stream (const char *receiver_addr)
{
  // @@we have to do ACE_NTOHS for all the network-byte ordered fields.
  int result;
  ACE_INET_Addr sender;
  result = this->connect_to_receiver (receiver_addr);
  if (result < 0)
    return result;
  while (this->start_tries_ > 0)
    {
      if ((result = this->send_start ()) != 0)
        return result;
      char magic_number [4];
      // Timed recv.
      ssize_t n =this->dgram_.recv (magic_number,
                                    sizeof(magic_number),
                                    sender,
                                    MSG_PEEK,
                                    &this->timeout1_);
      ACE_DEBUG ((LM_DEBUG,"n = %d\n",n));
      if (n == -1)
        {
          if (errno == ETIME)
            {
              ACE_DEBUG ((LM_DEBUG,"Timed out in reading StartReply"));
              this->start_tries_ --;
              continue;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,"dgram recv error:%p","recv"),-1);
        }
      else if (n==0)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::start_stream -peek"),-1);
      // successful receive of dgram.
      ACE_DEBUG ((LM_DEBUG,"StartReply received"));
      // check if its startreply message.
      char *magic_string = this->magic_number_to_string (magic_number);
      if (ACE_OS::strcmp (magic_string,TAO_SFP_STARTREPLY_MAGIC_NUMBER) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,"(%P|%t)StartReply message received\n"));
          flowProtocol::StartReply start_reply;
          n = this->dgram_.recv ((char *)&start_reply,
                                 sizeof (start_reply),
                                 sender);
          if (n != sizeof (start_reply))
            ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input-StartReply\n"),0);
          //  check for SFP version difference.??
          // Call the application back.
          this->state_ = REPLY_RECEIVED;
        }
      // register the data handler.
      result = this->register_dgram_handler ();
      return result;
    }
  return 0;
}

// Start the passive end of the stream.
int
SFP::start_stream (const char *local_addr,int Credit)
{
  int result;
  ACE_INET_Addr sender;

  this->state_ = PASSIVE_START;
  ACE_INET_Addr myaddr (local_addr);
  result = this->dgram_.open (myaddr);

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::passive start- open failed\n"),-1);

  char magic_number[4];
  // Timed recv.
  ssize_t n =this->dgram_.recv (magic_number,
                                sizeof(magic_number),
                                sender,
                                MSG_PEEK,
                                &this->timeout2_);
  if ((n == -1) && (errno == ETIME))
    {
      ACE_ERROR_RETURN ((LM_ERROR,"Timedout in reading Start"),-1);
    }
  else if (n==0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::start_stream -peek"),-1);

  ACE_DEBUG ((LM_DEBUG,"Start received:"));
  char *magic_string = this->magic_number_to_string (magic_number);
  if (ACE_OS::strcmp (magic_string,TAO_SFP_START_MAGIC_NUMBER) == 0)
    {
      // Read the start message.
      flowProtocol::Start start;
      n = this->dgram_.recv ((char *)&start,
                             sizeof (start),
                             sender);
      if (n != sizeof (start))
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input - Start\n"),0);
      else
        ACE_DEBUG ((LM_DEBUG,"Start message consumed\n"));
      this->state_ = START_RECEIVED;
      this->receiver_inet_addr_.set (sender);
      // Now send a startReply message back.
      result = this->send_startReply ();
      if (result != 0)
        return result;
      // Now we register a timeout handler until we receive a data
      // frame.
      result = this->reactor_->schedule_timer (this,
                                               0,
                                               this->timeout1_);
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"schedule_timer failed\n"),result);

      // register the data handler.
      result = this->register_dgram_handler ();
      return result;
    }

  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,"Invalid messaged received"),-1);
    }
  return 0;
}

// Sends the ACE_Message_Block data as a frame, fragmenting if necessary.
int
SFP::send_simple_frame (ACE_Message_Block *frame)
{
  // Currently there is no fragmentation  handled, just a simple
  // frame.

  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block,
                  -1);
  if (this->credit_ > 0)
    {
      // if we have enough credit then we send.
      // Currently no fragmentation.
      int length = frame->length ();

      if (length > ACE_MAX_DGRAM_SIZE)
          ACE_ERROR_RETURN ((LM_ERROR,"sfp doesn't support fragmentation yet"),-1);

      flowProtocol::frameHeader frame_header;

      // The magic_number and flags are to be sent in network-byte order.
      frame_header.magic_number [0] = ACE_HTONS ('=');
      frame_header.magic_number [1] = ACE_HTONS ('S');
      frame_header.magic_number [2] = ACE_HTONS ('F');
      frame_header.magic_number [3] = ACE_HTONS ('P');

      // set the byte order and no fragments.
      frame_header.flags = 0;
      frame_header.flags |= TAO_ENCAP_BYTE_ORDER;

      frame_header.flags = ACE_HTONS (frame_header.flags);
      //set the size of the message block.
      int len = sizeof(frame_header.magic_number)+sizeof
        (frame_header.flags);
        mb->size (len);

        mb->rd_ptr ((char *)&frame_header);
        mb->wr_ptr ((char *)&frame_header+len);
        if (this->encoder_ != 0)
          delete this->encoder_;
        ACE_NEW_RETURN (this->encoder_,
                      TAO_OutputCDR,
                      -1);

      frame_header.message_type = flowProtocol::SimpleFrame;
      frame_header.message_size = frame->length ();
      this->encoder_->write_octet (frame_header.message_type);
      this->encoder_->write_ulong (frame_header.message_size);

      // This is a good maximum, because Dgrams cannot be longer than
      // 64K and the usual size for a CDR fragment is 512 bytes.
      // @@ TODO In the future we may need to allocate some memory
      // from the heap.
      const int TAO_WRITEV_MAX = 128;
      iovec iov[TAO_WRITEV_MAX];

      iov[0].iov_base = mb->rd_ptr ();
      iov[0].iov_len = mb->length ();
      ACE_DEBUG ((LM_DEBUG,"length: %d ",mb->length ()));
      int iovcnt = 1;
      for (const ACE_Message_Block* b = this->encoder_->begin ();
           b != this->encoder_->end () && iovcnt < TAO_WRITEV_MAX;
           b = b->cont ())
        {
          iov[iovcnt].iov_base = b->rd_ptr ();
          iov[iovcnt].iov_len =  b->length ();
          ACE_DEBUG ((LM_DEBUG,"length: %d ",b->length ()));
          iovcnt++;
        }
      iov[iovcnt].iov_base = frame->rd_ptr ();
      iov[iovcnt].iov_len = frame->length ();
      ACE_DEBUG ((LM_DEBUG,"length: %d ",frame->length ()));
      ssize_t n = this->dgram_.send (iov,
                                     iovcnt,
                                     this->receiver_inet_addr_);
      if (n == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "send_simple_frame (%t) send failed %p\n", ""),-1);
      else if (n == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "send_simple_Frame (%t) EOF on send \n"),-1);
    }
  return 0;
}

// This is used to send large frames with fragmentation.This is not
// complete yet.
int
SFP::send_frame (ACE_Message_Block *frame)
{
  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block,
                  -1);
  if (this->credit_ > 0)
    {
      // if we have enough credit then we send.
      // Do fragmentation if necessary.
      int length = frame->length ();

      int total_length = 0;
      ACE_Message_Block *temp = frame;
      while (temp != 0)
        {
          total_length += temp->length ();
          temp = temp->next ();
        }
      flowProtocol::frameHeader frame_header;

      // The magic_number and flags are to be sent in network-byte order.
      frame_header.magic_number [0] = ACE_HTONS ('=');
      frame_header.magic_number [1] = ACE_HTONS ('S');
      frame_header.magic_number [2] = ACE_HTONS ('F');
      frame_header.magic_number [3] = ACE_HTONS ('P');
      // sizeof (frameHeader) may have to be replaced with more
      // accurate size??.
      if (total_length > (ACE_MAX_DGRAM_SIZE- sizeof (flowProtocol::frameHeader)))
        {
          // If the message size is not okay including the headers i.e it
          // cannot fit in a dgram.

          // set the byte order and no fragments.
          frame_header.flags = 0;
          frame_header.flags |= TAO_ENCAP_BYTE_ORDER;
          // set the fragments bit.
          frame_header.flags |= 2;

          frame_header.flags = ACE_HTONS (frame_header.flags);

          // first fragment will have size to be
          //set the size of the message block.
          int len = sizeof(frame_header.magic_number)+sizeof
            (frame_header.flags);
          mb->size (len);

          mb->rd_ptr ((char *)&frame_header);
          mb->wr_ptr ((char *)&frame_header+len);
          if (this->encoder_ != 0)
            delete this->encoder_;
          ACE_NEW_RETURN (this->encoder_,
                          TAO_OutputCDR,
                          -1);


          // This is a good maximum, because Dgrams cannot be longer than
          // 64K and the usual size for a CDR fragment is 512 bytes.
          // @@ TODO In the future we may need to allocate some memory
          // from the heap.
          int message_len = 0;
          const int TAO_WRITEV_MAX = 128;
          iovec iov[TAO_WRITEV_MAX];

          iov[0].iov_base = mb->rd_ptr ();
          iov[0].iov_len = mb->length ();
          int header_len = mb->length ()+2* sizeof(CORBA::ULong);
          message_len += header_len;
          ACE_DEBUG ((LM_DEBUG,"length: %d ",mb->length ()));
          int iovcnt = 2;
          ACE_Message_Block *mb = frame;
          int prev_len;
          while (mb != 0)
            {
              prev_len = message_len;
              message_len += mb->length ();
              if (message_len > ACE_MAX_DGRAM_SIZE)
                {
                  // get only the length that we can accomodate.
                  int current_len = ACE_MAX_DGRAM_SIZE - prev_len;
                  if (current_len < mb->length ())
                    {
                      // The above condition is an assertion.
                      iov [iovcnt].iov_base = mb->rd_ptr ();
                      iov [iovcnt].iov_len = current_len;
                      message_len += current_len;
                      mb->rd_ptr (current_len);
                      iovcnt++;
                    }
                  break;
                }
              else
                {
                  // we can accomodate this message block
                      iov [iovcnt].iov_base = mb->rd_ptr ();
                      iov [iovcnt].iov_len = mb->length ();
                      message_len += mb->length ();
                      iovcnt++;
                      mb = mb->next ();
                }
            }
          frame_header.message_type = flowProtocol::SimpleFrame;
          frame_header.message_size = message_len - header_len;
          this->encoder_->write_octet (frame_header.message_type);
          this->encoder_->write_ulong (frame_header.message_size);

          // THe header will be only in the first cdr fragment.
          iov[1].iov_base = this->encoder_->begin ()->rd_ptr ();
          iov[1].iov_len = this->encoder_->begin ()->length ();

          // send the fragment 0.
          ssize_t n = this->dgram_.send (iov,
                                         iovcnt,
                                         this->receiver_inet_addr_);
          if (n == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "send_frame (%t) fragment 0 send failed %p\n", ""),-1);
          else if (n == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "send_Frame (%t) EOF on send \n"),-1);

          int frag_number = 0;
          // If there is any more data send those as fragments.
          while (mb != 0)
            {
              flowProtocol::fragment frag;

              // The magic_number and flags are to be sent in network-byte order.
              frag.magic_number [0] = ACE_HTONS ('F');
              frag.magic_number [1] = ACE_HTONS ('R');
              frag.magic_number [2] = ACE_HTONS ('A');
              frag.magic_number [3] = ACE_HTONS ('G');

              ACE_Message_Block *magic_block;
              ACE_NEW_RETURN (magic_block,
                              ACE_Message_Block,
                              -1);
              magic_block->size (5);// magic_number+flags size.
              magic_block->rd_ptr ((char *)&frag);
              magic_block->wr_ptr (5);

              iov [0].iov_base = magic_block->rd_ptr ();
              iov [0].iov_len = magic_block->length ();

              int header_len = 5 + 4 *sizeof (CORBA::ULong);
              message_len = header_len;
              // 5 for magic_number+flags and 4 ulongs in the fragment header.
              iovcnt = 2;//  1 is for the frag header.
              while (mb != 0)
                {
                  prev_len = message_len;
                  message_len += mb->length ();
                  if (message_len > ACE_MAX_DGRAM_SIZE)
                    {
                      // get only the length that we can accomodate.
                      int current_len = ACE_MAX_DGRAM_SIZE - prev_len;
                      if (current_len < mb->length ())
                        {
                          // The above condition is an assertion.
                          iov [iovcnt].iov_base = mb->rd_ptr ();
                          iov [iovcnt].iov_len = current_len;
                          mb->rd_ptr (current_len);
                          message_len += current_len;
                          iovcnt++;
                        }
                      break;
                    }
                  else
                    {
                      // we can accomodate this message block
                      iov [iovcnt].iov_base = mb->rd_ptr ();
                      iov [iovcnt].iov_len = mb->length ();
                      message_len += mb->length ();
                      iovcnt++;
                      mb = mb->next ();
                    }
                }
              // send this fragment.
              // set the more fragments flag
              if ((mb != 0) && (mb->length () != 0))
                frag.flags = ACE_HTONS (2);
              else
                break;
              if (this->encoder_ != 0)
                delete this->encoder_;
              ACE_NEW_RETURN (this->encoder_,
                              TAO_OutputCDR,
                              -1);
              frag.frag_number = frag_number++;
              frag.sequence_num = this->sequence_num_;
              frag.frag_sz = message_len - header_len;
              frag.source_id = 0;
              this->encoder_->write_ulong (frag.frag_number);
              this->encoder_->write_ulong (frag.sequence_num);
              this->encoder_->write_ulong (frag.frag_sz);
              this->encoder_->write_ulong (frag.source_id);

              // THe header will be only in the first cdr fragment.
              iov[1].iov_base = this->encoder_->begin ()->rd_ptr ();
              iov[1].iov_len = this->encoder_->begin ()->length ();

              //   send the fragment now.
              ssize_t n = this->dgram_.send (iov,
                                             iovcnt,
                                             this->receiver_inet_addr_);
              if (n == -1)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "SFP::send_frame (%t) send failed %p\n", ""));
                  return -1;
                }
              else if (n == 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "SFP::send_frame (%t) EOF on send \n"));
                  return -1;
                }
            }
        }
    }
  return 0;
}


// creates a connected dgram.
int
SFP::connect_to_receiver (const char *receiver_addr)
{
  this->receiver_addr_ = ACE_OS::strdup (receiver_addr);
  // Get the local UDP address
  if (this->dgram_.open (ACE_Addr::sap_any) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) datagram open failed %p\n"),1);

      // set the socket buffer sizes to 64k.
  int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;

  if (this->dgram_.set_option (SOL_SOCKET,
                               SO_SNDBUF,
                               (void *) &sndbufsize,
                               sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->dgram_.set_option (SOL_SOCKET,
                                    SO_RCVBUF,
                                    (void *) &rcvbufsize,
                                    sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return -1;

  this->receiver_inet_addr_.set (receiver_addr);
  if (ACE_OS::connect (this->dgram_.get_handle (),(sockaddr *) this->receiver_inet_addr_.get_addr (),
                       this->receiver_inet_addr_.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) datagram connect failed %p\n"),-1);
  return 0;
}

// sends all the ACE_Message_Blocks in the current CDR stream.
int
SFP::send_cdr_buffer (void)
{
  // This is a good maximum, because Dgrams cannot be longer than
  // 64K and the usual size for a CDR fragment is 512 bytes.
  // @@ TODO In the future we may need to allocate some memory
  // from the heap.
  const int TAO_WRITEV_MAX = 128;
  iovec iov[TAO_WRITEV_MAX];

  int iovcnt = 0;
  for (const ACE_Message_Block* b = this->encoder_->begin ();
       b != this->encoder_->end () && iovcnt < TAO_WRITEV_MAX;
       b = b->cont ())
    {
      iov[iovcnt].iov_base = b->rd_ptr ();
      iov[iovcnt].iov_len =  b->length ();
      iovcnt++;
    }
  // send the message.
  ssize_t n = this->dgram_.send (iov,
                                 iovcnt,
                                 this->receiver_inet_addr_);
  if (n == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "SFP::send_cdr_buffer (%t) send failed %p\n", ""));
      return -1;
    }
  else if (n == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "SFP::send_cdr_buffer (%t) EOF on send \n"));
      return -1;
    }
  return 0;
}

int
SFP::send_start (void)
{
  int result;
  // Start message is in network byte order.
  // construct the start message
  flowProtocol::Start start;

  // copy the magic number into the message
  start.magic_number [0] = ACE_HTONS ('=');
  start.magic_number [1] = ACE_HTONS ('S');
  start.magic_number [2] = ACE_HTONS ('T');
  start.magic_number [3] = ACE_HTONS ('A');

  // put the version number into the field
  start.major_version = ACE_HTONS (TAO_SFP_MAJOR_VERSION);
  start.minor_version = ACE_HTONS (TAO_SFP_MINOR_VERSION);

  // flags field is all zeroes
  start.flags = ACE_HTONS (0);

  this->state_ = ACTIVE_START;

  // Now send the network byte ordered start message.
  int n = this->dgram_.send ((char *)&start,
                             sizeof (start),
                             this->receiver_inet_addr_);
  if (n!= sizeof (start))
    ACE_ERROR_RETURN ((LM_ERROR,"start send failed\n"),-1);

  ACE_DEBUG ((LM_DEBUG," Start sent\n"));
//   // non-interval timer.
//   result = this->reactor_->schedule_timer (this,
//                                            0,
//                                            this->timeout1_);
//   if (result != 0)
//     return result;

  return 0;
}

int
SFP::send_startReply (void)
{
  int result;

  flowProtocol::StartReply start_reply;

  // copy the magic number into the message
  start_reply.magic_number [0] = ACE_HTONS ('=');
  start_reply.magic_number [1] = ACE_HTONS ('S');
  start_reply.magic_number [2] = ACE_HTONS ('T');
  start_reply.magic_number [3] = ACE_HTONS ('R');

  start_reply.flags = ACE_HTONS (0);

  // Now send the network byte ordered start message.
  int n = this->dgram_.send ((char *)&start_reply,
                             sizeof (start_reply),
                             this->receiver_inet_addr_);
  if (n!= sizeof (start_reply))
    ACE_ERROR_RETURN ((LM_ERROR,"startreply send failed\n"),-1);

  ACE_DEBUG ((LM_DEBUG," startReply sent\n"));
  return 0;
}

int
SFP::handle_timeout (const ACE_Time_Value &tv,
                     const void *arg)
{
  int result;
  // Handle the timeout for timeout1 and timeout2.
  switch (this->state_)
    {
    case ACTIVE_START:
    case PASSIVE_START:
      // Timingout for Start Messages.
      ACE_DEBUG ((LM_DEBUG,"Invalid state in handle_timeout\n"));
      break;
    case START_RECEIVED:
      // we need to reduce the startreply_tries and also reschedule
      // the timer.
      if (this->startReply_tries_ --)
        {
          ACE_DEBUG ((LM_DEBUG,"Timed out on receiving Data Frame\n"));
          // send startreply.
          result = this->send_startReply ();
          if (result != 0)
            ACE_ERROR_RETURN ((LM_ERROR,"Error in sending startreply"),0);
          this->reactor_->schedule_timer (this,
                                          0,
                                          this->timeout1_);
        }
      else
        {
          this->end_stream ();
        }
    }
  return 0;
}

// Handle_input is called when data arrives on the  dgram
// socket. Currently both the receiver and sender side input is
// handled in this same handle_input ().
int
SFP::handle_input (ACE_HANDLE fd)
{
  flowProtocol::MsgType msg_type;
  ACE_INET_Addr sender;
  char magic_number[4];
  ssize_t n =this->dgram_.recv (magic_number,
                                sizeof(magic_number),
                                sender,
                                MSG_PEEK);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),-1);
  else if (n==0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);

  // convert from network byte order to host order.

  magic_number [0] = ACE_NTOHS (magic_number [0]);
  magic_number [1] = ACE_NTOHS (magic_number [1]);
  magic_number [2] = ACE_NTOHS (magic_number [2]);
  magic_number [3] = ACE_NTOHS (magic_number [3]);

  char *magic_string = this->magic_number_to_string (magic_number);

  if (ACE_OS::strcmp (magic_string,TAO_SFP_START_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)Start message received\n"));
      msg_type = flowProtocol::start;
    }
  else if (ACE_OS::strcmp (magic_string,TAO_SFP_STARTREPLY_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)StartReply message received\n"));
      msg_type = flowProtocol::startReply;
    }
  else if (ACE_OS::strcmp (magic_string,TAO_SFP_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) frameHeader received\n"));
      msg_type = flowProtocol::SimpleFrame;
    }
  else if (ACE_OS::strcmp (magic_string,TAO_SFP_FRAGMENT_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) fragment Header received\n"));
      msg_type = flowProtocol::Fragment;
    }
  else if (ACE_OS::strcmp (magic_string,TAO_SFP_CREDIT_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) credit message received\n"));
      msg_type = flowProtocol::Credit;
    }
  switch (this->state_)
    {
    case ACTIVE_START:
      // Check if we received a StartReply back.
          ACE_DEBUG ((LM_DEBUG,"Unexpected message while StartReply expected\n"));
          break;
    case PASSIVE_START:
      // Check if we received a Start from the Sender.
      ACE_DEBUG ((LM_DEBUG,"Unexpected message while Start expected\n"));
      break;
    case START_RECEIVED:
      // In this state we check for credit frames.
      switch (msg_type)
        {
        case flowProtocol::Credit:
          {
            flowProtocol::credit credit;
            n = this->dgram_.recv ((char *)&credit,
                                   sizeof (credit),
                                   sender);
            if (n != sizeof (credit))
              ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input - Credit\n"),0);
            this->credit_ += credit.cred_num;
            break;
          }
        case flowProtocol::start:
          // consume the retransmitted start message.
          {
            flowProtocol::Start start;
            n = this->dgram_.recv ((char *)&start,
                                   sizeof (start),
                                   sender);
            if (n != sizeof (start))
              ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input - Start\n"),0);
            else
              ACE_DEBUG ((LM_DEBUG,"Start message consumed\n"));
            //          ACE_DEBUG ((LM_DEBUG,"Unexpected message while
            // Credit expected\n"));
            break;
          }
        case flowProtocol::SimpleFrame:
          {
            ACE_Message_Block * mb =this->read_simple_frame ();
            this->callback_->receive_frame (mb);
          }
        }
      break;
    case REPLY_RECEIVED:
      // In this state we check for Data frames.
      switch (msg_type)
        {
        case flowProtocol::startReply:
          {
            flowProtocol::StartReply start_reply;
            n = this->dgram_.recv ((char *)&start_reply,
                                   sizeof (start_reply),
                                   sender);
            if (n != sizeof (start_reply))
              ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input-StartReply\n"),0);
            else
              ACE_DEBUG ((LM_DEBUG,"start reply consumed\n"));
          }
        }
    }
  return 0;
}

char *
SFP::magic_number_to_string (char *magic_number)
{
  char *buf;
  ACE_NEW_RETURN (buf,
                  char [5],
                  0);
  for (int i=0;i<4;i++)
    {
      buf [i] = magic_number [i];
      ACE_DEBUG ((LM_DEBUG,"%c ",buf [i]));
    }
  buf[i] = 0;
  return buf;
}

int
SFP::end_stream (void)
{
  ACE_DEBUG ((LM_DEBUG,"SFP - ending the stream\n"));
  int result = this->reactor_->remove_handler (this,
                                               ACE_Event_Handler::READ_MASK);
  return result;
}

int
SFP::register_dgram_handler (void)
{
  int result;
  result = this->reactor_->register_handler (this,
                                             ACE_Event_Handler::READ_MASK);
  return result;
}

ACE_HANDLE
SFP::get_handle (void) const
{
  return this->dgram_.get_handle ();
}

ACE_Message_Block *
SFP::read_simple_frame (void)
{
  ACE_DEBUG ((LM_DEBUG,"Reading simple frame\n"));
  // Check to see what the length of the message is.

  flowProtocol::frameHeader frame_header;
  char *buf;
  ssize_t firstlen =sizeof (frame_header.magic_number)+sizeof (frame_header.flags);
  ssize_t buflen =firstlen+2*sizeof (CORBA::ULong)+3;// hack to ensure
  //  that buffer is aligned for CDR.

  ACE_DEBUG ((LM_DEBUG,"firstlen = %d,buflen =%d\n",firstlen,buflen));
  ACE_NEW_RETURN (buf,
                  char [buflen],
                  0);

  ACE_INET_Addr sender;

  buf +=3;
  ssize_t n =this->dgram_.recv (buf,
                                buflen,
                                sender,
                                MSG_PEEK);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
  else if (n==0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
  for (int i=0;i<4;i++)
    ACE_DEBUG ((LM_DEBUG,"%c ",buf[i]));
  //skip the magic_number..
  buf += 4 ;

  // Get the byte order from the flags.

  int byte_order = buf[0];
  ACE_DEBUG ((LM_DEBUG,"byte_order = %d\n",byte_order));
  // move past the flags.
  buf += 1;
  //  CORBA::ULong *header = (CORBA::ULong *) (buf+firstlen);
  //  ACE_DEBUG ((LM_DEBUG,"first ulong = %d, second ulong = %d",*(CORBA::ULong*)(buf+firstlen),
  //              *(CORBA::ULong *)(buf+firstlen+sizeof (CORBA::ULong))));

  //  ACE_DEBUG ((LM_DEBUG,"first ulong = %d, second ulong = %d",header [0],header[1]));


  ACE_Message_Block mb (buf,buflen-firstlen + ACE_CDR::MAX_ALIGNMENT);
  ACE_CDR::mb_align (&mb);
  mb.wr_ptr (buflen-firstlen);
  ACE_DEBUG ((LM_DEBUG,"mb len = %d\n",mb.length ()));
  TAO_InputCDR cdr (&mb,byte_order);
  cdr.read_octet (frame_header.message_type);
  cdr.read_ulong (frame_header.message_size);

  ACE_DEBUG ((LM_DEBUG,"message_type = %d, message_size = %d",
              frame_header.message_type,frame_header.message_size));

  char *message;
  int message_len = buflen+frame_header.message_size;
  ACE_NEW_RETURN (message,
                  char [message_len],
                  0);

  ACE_DEBUG ((LM_DEBUG,"message_len = %d\n",message_len));
  n = this->dgram_.recv (message,message_len,sender);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
  else if (n==0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);

  ACE_Message_Block *message_block;
  ACE_NEW_RETURN (message_block,
                  ACE_Message_Block (message+buflen,
                                     message_len-buflen),
                  0);
  message_block->wr_ptr (message_len-buflen);
  ACE_DEBUG ((LM_DEBUG,"messageblock length: ",message_block ->length ()));
  return message_block;
}
