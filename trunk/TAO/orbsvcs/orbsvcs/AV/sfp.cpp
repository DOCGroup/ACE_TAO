#include "orbsvcs/AV/sfp.h"
#include "ace/ARGV.h"

// $Id$

int 
operator< (const TAO_SFP_Fragment_Node& left,
           const TAO_SFP_Fragment_Node& right)
{
  return left.fragment_info_.frag_number < right.fragment_info_.frag_number;
}

// constructor.
TAO_SFP::TAO_SFP (CORBA::ORB_ptr orb,
                  ACE_Reactor* reactor,
                  ACE_Time_Value timeout1,
                  ACE_Time_Value timeout2,
                  SFP_Callback *callback)
  :orb_ (orb),
   reactor_ (reactor),
   timeout1_ (timeout1),
   timeout2_ (timeout2),
   start_tries_ (10),
   startReply_tries_ (10),
   callback_ (callback),
   sequence_num_ (0),
   credit_num_ (10),
   magic_number_len_ (sizeof (magic_number_)-1)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // fill in the default frameHeader fields.
      this->frame_header_.magic_number [0] = '=';
      this->frame_header_.magic_number [1] = 'S';
      this->frame_header_.magic_number [2] = 'F';
      this->frame_header_.magic_number [3] = 'P';
      this->frame_header_.flags = TAO_ENCAP_BYTE_ORDER;
      this->output_cdr_.reset ();
      this->output_cdr_.encode (flowProtocol::_tc_frameHeader,
                                &this->frame_header_,
                                0,
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->frame_header_len_ = this->output_cdr_.total_length ();
      // fill in the default fragment message fields.
      this->fragment_.magic_number [0] = 'F';
      this->fragment_.magic_number [1] = 'R';
      this->fragment_.magic_number [2] = 'A';
      this->fragment_.magic_number [3] = 'G';
      this->output_cdr_.reset ();
      this->output_cdr_.encode (flowProtocol::_tc_fragment,
                                &this->fragment_,
                                0,
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->fragment_len_ = this->output_cdr_.total_length ();
      // fill in the default Start message fields.
      this->start_.magic_number [0] = '=';
      this->start_.magic_number [1] = 'S';
      this->start_.magic_number [2] = 'T';
      this->start_.magic_number [3] = 'A';
      this->start_.major_version = TAO_SFP_MAJOR_VERSION;
      this->start_.minor_version = TAO_SFP_MINOR_VERSION;
      this->start_.flags = 0;
      this->start_len_ = sizeof (this->start_);
      // fill in the default StartReply message fields.
      this->start_reply_.magic_number [0] = '=';
      this->start_reply_.magic_number [1] = 'S';
      this->start_reply_.magic_number [2] = 'T';
      this->start_reply_.magic_number [3] = 'R';
      this->start_reply_.flags = 0;
      this->start_reply_len_ = sizeof (this->start_reply_);
      // fill in the default Credit message fields.
      this->credit_.magic_number [0] = '=';
      this->credit_.magic_number [1] = 'C';
      this->credit_.magic_number [2] = 'R';
      this->credit_.magic_number [3] = 'E';
      this->credit_len_ = sizeof (this->credit_);
      this->output_cdr_.reset ();
      //      this->output_cdr_ <<= this->credit_;
      this->output_cdr_.reset ();
      this->output_cdr_.encode (flowProtocol::_tc_credit,
                                &this->credit_,
                                0,
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->credit_len_ = this->output_cdr_.total_length ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"TAO_SFP constructor");
    }
  ACE_ENDTRY;
}

// Start the active end of the stream.
int
TAO_SFP::start_stream (const char *receiver_addr)
{
  int result;
  ACE_INET_Addr sender;
  result = this->connect_to_receiver (receiver_addr);
  if (result < 0)
    return result;
  while (this->start_tries_ > 0)
    {
      result = this->send_start ();
      if (result != 0)
        return result;
      // Timed recv.
      char magic_number [MAGIC_NUMBER_LEN];
      ssize_t n =this->dgram_.recv (magic_number,
                                    this->magic_number_len_,
                                    sender,
                                    MSG_PEEK,
                                    &this->timeout1_);
      //      ACE_DEBUG ((LM_DEBUG,"n = %d\n",n));
      if (n == -1) 
        {
          if (errno == ETIME)
            {
              ACE_DEBUG ((LM_DEBUG,"Timed out in reading StartReply"));
              this->start_tries_ --;
              continue;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,"dgram recv error:%d,%p",errno,"recv"),-1);
        }
      else if (n==0)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::start_stream -peek"),-1);
      // Null terminate the magic number.
      magic_number [this->magic_number_len_] = 0;
      // check if its startreply message.
      if (ACE_OS::strcmp (magic_number,TAO_SFP_STARTREPLY_MAGIC_NUMBER) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,"(%P|%t)StartReply message received\n"));
          flowProtocol::StartReply start_reply;
          n = this->dgram_.recv ((char *)&start_reply,
                                 sizeof (start_reply),
                                 sender);
          if (n != sizeof (start_reply))
            ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input-StartReply\n"),0);
          //  check for SFP version difference.??
          this->state_ = REPLY_RECEIVED;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,"Invalid message while StartReply expected\n"),0);
      // register the data handler.
      return this->register_dgram_handler ();
    }
  return 0;
}

// Start the passive end of the stream.
int
TAO_SFP::start_stream (const char *local_addr,int Credit)
{
  int result;
  ACE_INET_Addr sender;

  this->state_ = PASSIVE_START;
  ACE_INET_Addr myaddr (local_addr);
  result = this->dgram_.open (myaddr);

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::passive start- open failed\n"),-1);

  char magic_number[MAGIC_NUMBER_LEN];
  // Timed recv.
  ssize_t n =this->dgram_.recv (magic_number,
                                this->magic_number_len_,
                                sender,
                                MSG_PEEK,
                                &this->timeout2_);
  if ((n == -1) && (errno == ETIME))
    {
      ACE_ERROR_RETURN ((LM_ERROR,"Timedout in reading Start"),-1);
    }
  else if (n==0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::start_stream -peek"),-1);
  // Null terminate the magic_number.
  magic_number [this->magic_number_len_] = 0;
  if (ACE_OS::strcmp (magic_number,TAO_SFP_START_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"Start received:"));
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
      return this->register_dgram_handler ();
    }
  else
      ACE_ERROR_RETURN ((LM_ERROR,"Invalid messaged received while Start expected\n"),-1);
  return 0;
}

// Sends the ACE_Message_Block data as a frame, fragmenting if necessary.
int
TAO_SFP::send_frame (ACE_Message_Block *frame)
{
  ACE_TRY_NEW_ENV
    {
      if (this->credit_num_ > 0)
        {
          // if we have enough credit then we send.
          int total_length = 0;
          for (ACE_Message_Block *temp = frame;temp != 0;temp = temp->cont ())
            total_length += temp->length ();
          ACE_DEBUG ((LM_DEBUG,"total_length of frame=%d\n",total_length));
          if (total_length < (SFP_MAX_PACKET_SIZE -this->frame_header_len_))
            {  
              // clear the output cdr.
              this->output_cdr_.reset ();
              // CDR encode the frame header.
              //(<<= isAvailable only in compiled marshalling!)
              this->frame_header_.message_type = flowProtocol::SimpleFrame;
              this->frame_header_.message_size = frame->length ()+this->frame_header_len_;
              this->output_cdr_.encode (flowProtocol::_tc_frameHeader,
                                        &this->frame_header_,
                                        0,
                                        ACE_TRY_ENV);
              ACE_TRY_CHECK;
              //          this->output_cdr_ <<= this->frame_header_;
              this->send_cdr_buffer (this->output_cdr_,frame);
            }
          else // larger frame,fragment and send it.
            {
              // set the fragments bit.
              this->frame_header_.flags |= 2;
              // This is a good maximum, because Dgrams cannot be longer than
              // 64K and the usual size for a CDR fragment is 512 bytes.
              // @@ TODO In the future we may need to allocate some memory
              // from the heap.
              int message_len = this->frame_header_len_;
              iovec iov[TAO_WRITEV_MAX];
              int iovcnt = 1;// since first iov is for frameHeader. 
              flowProtocol::frame frame_info;
              frame_info.timestamp = 10;
              frame_info.synchSource = 10;
              frame_info.source_ids.length (1);
              frame_info.source_ids [0] = 1; // XXX random number.
              frame_info.sequence_num = this->sequence_num_;
              this->output_cdr_.reset ();
              this->output_cdr_.encode (flowProtocol::_tc_frame,
                                        &frame_info,
                                        0,
                                        ACE_TRY_ENV);
              ACE_TRY_CHECK;
              ACE_DEBUG ((LM_DEBUG,"frame info length:%d\n",this->output_cdr_.total_length ()));
              for (const ACE_Message_Block* b = this->output_cdr_.begin ()->clone ();
                   b != 0 && iovcnt < TAO_WRITEV_MAX;
                   b = b->cont ())
                {
                  //                  ACE_DEBUG ((LM_DEBUG,"iovcnt:%d\n",iovcnt));
                  iov[iovcnt].iov_base = b->rd_ptr ();
                  iov[iovcnt].iov_len =  b->length ();
                  message_len += b->length ();
                  ACE_DEBUG ((LM_DEBUG,"send_cdr_buffer:length=%d\n",b->length ()));
                  // print the buffer.
                  //                  DUMP_BUF (b->rd_ptr (),b->length ());
                  iovcnt++;
                }
              ACE_Message_Block *mb = frame;
              int prev_len;
              while (mb != 0)
                {
                  prev_len = message_len;
                  message_len += mb->length ();
                  if (message_len > SFP_MAX_PACKET_SIZE)
                    {
                      // get only the length that we can accomodate.
                      int current_len = SFP_MAX_PACKET_SIZE - prev_len;
                      if (current_len < mb->length ())
                        {
                          // The above condition is an assertion.
                          iov [iovcnt].iov_base = mb->rd_ptr ();
                          iov [iovcnt].iov_len = current_len;
                          message_len += (current_len-mb->length ());
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
                      mb = mb->cont ();
                    }
                }
              //  This can be either a simpleframe or a sequenced frame,other types of frames.
              this->frame_header_.message_type = flowProtocol::Frame;
              this->frame_header_.message_size = message_len;
              ACE_DEBUG ((LM_DEBUG,"first fragment of size:%d\n",message_len- this->frame_header_len_));
              this->output_cdr_.reset ();
              this->output_cdr_.encode (flowProtocol::_tc_frameHeader,
                                        &this->frame_header_,
                                        0,
                                        ACE_TRY_ENV);
              ACE_TRY_CHECK;
              // header will be only in the first cdr fragment.
              iov[0].iov_base = this->output_cdr_.begin ()->rd_ptr ();
              iov[0].iov_len = this->output_cdr_.begin ()->length ();
              ACE_DEBUG ((LM_DEBUG,"frame header len:%d\n",iov[0].iov_len));
              // send the first fragment.
              for (int i=0;i<iovcnt;i++)
                {
                  //                  DUMP_BUF (iov[i].iov_base,iov[i].iov_len);
                }
              ssize_t n = this->dgram_.send (iov,
                                             iovcnt,
                                             this->receiver_inet_addr_);
              if (n == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "send_frame (%t) fragment 0 send failed %p\n", ""),-1);
              else if (n == 0)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "send_Frame (%t) EOF on send \n"),-1);

              int frag_number = 1;
              // If there is any more data send those as fragments.
              while (mb != 0)
                {
                  message_len = this->fragment_len_;
                  iovcnt = 1;//  1 is for the frag header.
                  while (mb != 0)
                    {
                      prev_len = message_len;
                      message_len += mb->length ();
                      if (message_len > SFP_MAX_PACKET_SIZE)
                        {
                          // get only the length that we can accomodate.
                          int current_len = SFP_MAX_PACKET_SIZE - prev_len;
                          if (current_len < mb->length ())
                            {
                              // The above condition is an assertion.
                              iov [iovcnt].iov_base = mb->rd_ptr ();
                              iov [iovcnt].iov_len = current_len;
                              message_len += (current_len - mb->length ());
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
                          iovcnt++;
                          mb = mb->cont ();
                        }
                    }
                  this->fragment_.flags =  TAO_ENCAP_BYTE_ORDER;
                  if (mb == 0)
                    {
                      ACE_DEBUG ((LM_DEBUG,"sending the last fragment\n"));
                      // This is the last fragment so clear the fragments bit.
                    }
                  else
                    {
                      // set the more fragments flag
                      this->fragment_.flags |= 2;
                    }
                  // if there are no data blocks.
                  if (iovcnt == 1)
                    break;
                  this->fragment_.frag_number = frag_number++;
                  this->fragment_.sequence_num = this->sequence_num_;
                  this->fragment_.frag_sz = message_len;
                  this->fragment_.source_id = 0;
                  this->output_cdr_.reset ();
                  this->output_cdr_.encode (flowProtocol::_tc_fragment,
                                            &this->fragment_,
                                            0,
                                            ACE_TRY_ENV);
                  ACE_TRY_CHECK;
                  ACE_DEBUG ((LM_DEBUG,"sending a fragment numbered %d of size %d\n",
                              this->fragment_.frag_number,
                              this->fragment_.frag_sz));
                  // THe header will be only in the first cdr fragment.
                  iov[0].iov_base = this->output_cdr_.begin ()->rd_ptr ();
                  iov[0].iov_len = this->output_cdr_.begin ()->length ();
                  //   send the fragment now.
                  // without the sleep the fragments gets lost! 
                  // probably because the UDP buffer queue on the sender side
                  // is overflown it drops the packets.
                  // XXX: This is a hack.
                  ACE_OS::sleep (1);
                  ssize_t n = this->dgram_.send (iov,
                                                 iovcnt,
                                                 this->receiver_inet_addr_);
                  if ((n == -1) || (n==0))
                    ACE_ERROR_RETURN ((LM_ERROR,"TAO_SFP::send_framed failed:%p\n",""),-1);
                }
            }
        }
      else
        {
          // flow controlled so wait. 
        }
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("TAO_SFP::send_frame");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}


// creates a connected dgram.
int
TAO_SFP::connect_to_receiver (const char *receiver_addr)
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
  return 0;
}

// sends all the ACE_Message_Blocks in the current CDR stream.
int
TAO_SFP::send_cdr_buffer (TAO_OutputCDR &cdr,ACE_Message_Block *mb)
{
  // This is a good maximum, because Dgrams cannot be longer than
  // 64K and the usual size for a CDR fragment is 512 bytes.
  // @@ TODO In the future we may need to allocate some memory
  // from the heap.
  iovec iov[TAO_WRITEV_MAX];
  int iovcnt = 0;
  for (const ACE_Message_Block* b = cdr.begin ();
       b != cdr.end () && iovcnt < TAO_WRITEV_MAX;
       b = b->cont ())
    {
      iov[iovcnt].iov_base = b->rd_ptr ();
      iov[iovcnt].iov_len =  b->length ();
      //      ACE_DEBUG ((LM_DEBUG,"send_cdr_buffer:length=%d\n",b->length ()));
      // print the buffer.
      //      DUMP_BUF (b->rd_ptr (),b->length ());
      iovcnt++;
    }
  for (b = mb; b!=0 && iovcnt < TAO_WRITEV_MAX; b=b->cont ())
    {
      iov [iovcnt].iov_base = b->rd_ptr ();
      iov [iovcnt].iov_len = b->length ();
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
TAO_SFP::send_start (void)
{
  int result;
  // copy the magic number into the message
  this->state_ = ACTIVE_START;
  // Now send the network byte ordered start message.
  int n = this->dgram_.send ((char *)&this->start_,
                             this->start_len_,
                             this->receiver_inet_addr_);
  if (n!= this->start_len_)
    ACE_ERROR_RETURN ((LM_ERROR,"start send failed\n"),-1);

  ACE_DEBUG ((LM_DEBUG," Start sent\n"));
  return 0;
}

int
TAO_SFP::send_startReply (void)
{
  int result;
  int n = this->dgram_.send ((char *)&this->start_reply_,
                             this->start_reply_len_,
                             this->receiver_inet_addr_);
  if (n!= this->start_reply_len_)
    ACE_ERROR_RETURN ((LM_ERROR,"startreply send failed\n"),-1);

  ACE_DEBUG ((LM_DEBUG," startReply sent\n"));
  return 0;
}

int
TAO_SFP::handle_timeout (const ACE_Time_Value &tv,
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
TAO_SFP::handle_input (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_SFP::handle_input\n"));
  flowProtocol::MsgType msg_type;
  ACE_INET_Addr sender;
  char magic_number[MAGIC_NUMBER_LEN];
  ssize_t n =this->dgram_.recv (this->magic_number_,
                                this->magic_number_len_,
                                sender,
                                MSG_PEEK);
  this->magic_number_[this->magic_number_len_] = 0;
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),-1);
  else if (n==0)
    ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);

  if (ACE_OS::strcmp (this->magic_number_,TAO_SFP_START_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)Start message received\n"));
      msg_type = flowProtocol::start;
    }
  else if (ACE_OS::strcmp (this->magic_number_,TAO_SFP_STARTREPLY_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)StartReply message received\n"));
      msg_type = flowProtocol::startReply;
    }
  else if (ACE_OS::strcmp (this->magic_number_,TAO_SFP_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) frameHeader received\n"));
      msg_type = flowProtocol::SimpleFrame;
    }
  else if (ACE_OS::strcmp (this->magic_number_,TAO_SFP_FRAGMENT_MAGIC_NUMBER) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) fragment Header received\n"));
      msg_type = flowProtocol::Fragment;
    }
  else if (ACE_OS::strcmp (this->magic_number_,TAO_SFP_CREDIT_MAGIC_NUMBER) == 0)
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
            this->credit_num_ += credit.cred_num;
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
            break;
          }
        case flowProtocol::SimpleFrame:
          {
            ACE_Message_Block * mb =this->read_simple_frame ();
            if (mb != 0)
              this->callback_->receive_frame (mb);
            else
              {
                if (!this->more_fragments_)
                  {
                    char buf[BUFSIZ];
                    // consume the wrong UDP frame.
                    this->dgram_.recv (buf,
                                       BUFSIZ,
                                       sender);
                  }
              }
            break;
          }
        case flowProtocol::Fragment:
          {
            ACE_DEBUG ((LM_DEBUG,"Fragment received\n"));
            ACE_Message_Block *result = this->read_fragment ();
            // no more fragments.
            if (result != 0)
              this->callback_->receive_frame (result);
            break;
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

int
TAO_SFP::end_stream (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,"SFP - ending the stream\n"));
      // send the EndofStream message.
      this->frame_header_.flags = TAO_ENCAP_BYTE_ORDER;
      this->frame_header_.message_type = flowProtocol::EndofStream;
      this->frame_header_.message_type = 0;
      this->output_cdr_.reset ();
      this->output_cdr_.encode (flowProtocol::_tc_frameHeader,
                                &this->frame_header_,
                                0,
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;
//       ssize_t n = this->dgram_.send (this->output_cdr_.begin ()->rd_ptr (),
//                                      this->output_cdr_.begin ()->length (),
//                                      this->receiver_inet_addr_);
//       if ((n==-1) || (n==0))
//         ACE_ERROR_RETURN ((LM_ERROR,"Error sending endofstream message:%p",""),-1);
      int result = this->reactor_->remove_handler (this,
                                                   ACE_Event_Handler::READ_MASK);
      return result;
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("TAO_SFP::end_stream ()\n");
      return -1;
    }
  ACE_ENDTRY;
}
                                  
int
TAO_SFP::register_dgram_handler (void)
{
  int result;
  result = this->reactor_->register_handler (this,
                                             ACE_Event_Handler::READ_MASK);
  return result;
}

ACE_HANDLE
TAO_SFP::get_handle (void) const
{
  return this->dgram_.get_handle ();
}

ACE_Message_Block *
TAO_SFP::read_simple_frame (void)
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,"Reading simple frame\n"));
      // Check to see what the length of the message is.

      flowProtocol::frameHeader frame_header;
      ACE_INET_Addr sender;
      char *buf;
      ACE_NEW_RETURN (buf,
                      char [this->frame_header_len_+ACE_CDR::MAX_ALIGNMENT],
                      0);
      ssize_t n =this->dgram_.recv (buf,
                                    this->frame_header_len_,
                                    sender,
                                    MSG_PEEK);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame -peek:%p",""),0);
      else if (n==0)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame -peek:%p",""),0);
      else if (n != this->frame_header_len_)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame - not able to peek\n"),0);
      // print the buffer.
      //      DUMP_BUF (buf,n);
      ACE_Message_Block mb (n+ACE_CDR::MAX_ALIGNMENT);
      ACE_CDR::mb_align (&mb);
      int result
        = mb.copy (buf,n);
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"Message_Block::copy failed\n"),0);
      // buf[4] is the byte order.
      int byte_order = buf[4] & 0x1;
      ACE_DEBUG ((LM_DEBUG,"mb len = %d,byte_order=%d\n",mb.length (),byte_order));
      TAO_InputCDR cdr (&mb,byte_order);
      //  cdr >>= frame_header;
      cdr.decode (flowProtocol::_tc_frameHeader,
                  &frame_header,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,"message_type = %d, message_size = %d,message_flags = %d\n",
                  frame_header.message_type,frame_header.message_size,frame_header.flags));

      int message_len = frame_header.message_size;
      if (frame_header.message_size < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"Negative message size\n"),0);
      ACE_Message_Block *message_block;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (message_len),
                      0);
      n = this->dgram_.recv (message_block->wr_ptr (),message_len,sender);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
      else if (n==0)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::handle_input -peek"),0);
      else if (n != message_len)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame:message truncated\n"),0);
      // print the buffer.
      //      DUMP_BUF (message,n);
      // skip over the frame header.
      message_block->rd_ptr (this->frame_header_len_);
      message_block->wr_ptr (n);
      if (frame_header.flags & 0x2)
        {
          ACE_DEBUG ((LM_DEBUG,"fragmented frame:0th fragment\n"));
          this->more_fragments_ = 1;
          // read the frame info.
          ACE_Message_Block frame_info_mb (message_len-this->frame_header_len_+ACE_CDR::MAX_ALIGNMENT);
          ACE_CDR::mb_align (&frame_info_mb);
          frame_info_mb.copy (message_block->rd_ptr (),
                              message_block->length ());
          // print the buffer.
          //          DUMP_BUF (message_block->rd_ptr (),16);
          TAO_InputCDR frame_info_cdr (&frame_info_mb,byte_order);
          flowProtocol::frame frame_info;
          frame_info_cdr.decode (flowProtocol::_tc_frame,
                                 &frame_info,
                                 0,
                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;
          ACE_DEBUG ((LM_DEBUG,"frame.timestamp = %d, frame.synchsource = %d, frame.sequence_num = %d\n",
                      frame_info.timestamp,
                      frame_info.synchSource,
                      frame_info.sequence_num));
          // The remaining message in the CDR stream is the fragment data for frag.0
          ACE_Message_Block *data = 
            frame_info_cdr.start ()->clone ();
          ACE_DEBUG ((LM_DEBUG,"Length of 0th fragment= %d\n",data->length ()));
          TAO_SFP_Fragment_Table_Entry *fragment_entry = 0;
          TAO_SFP_Fragment_Node *new_node;
          ACE_NEW_RETURN (new_node,
                          TAO_SFP_Fragment_Node,
                          0);
          new_node->fragment_info_.frag_sz = data->length ();
          new_node->fragment_info_.frag_number = 0;
          new_node->fragment_info_.source_id = frame_info.source_ids [0];
          new_node->data_ = data;
          if (this->fragment_table_.find (frame_info.sequence_num,fragment_entry) == 0)
            {
              // This case can happen where a nth (n > 0)fragment is received before the 0th fragment.
              ACE_DEBUG ((LM_DEBUG,"fragment table entry found for 0th fragment:\n"));
              result = fragment_entry->fragment_set_.insert (*new_node);
              if (result != 0)
                ACE_ERROR_RETURN ((LM_ERROR,"insert for 0th fragment failed\n"),0);
              // check if all the fragments have been received.
              return check_all_fragments (fragment_entry);
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,"fragment table entry not found for 0th fragment\n"));
              TAO_SFP_Fragment_Table_Entry *new_entry;
              ACE_NEW_RETURN (new_entry,
                              TAO_SFP_Fragment_Table_Entry,
                              0);
              result = new_entry->fragment_set_.insert (*new_node);
              if (result != 0)
                ACE_ERROR_RETURN ((LM_ERROR,"insert for 0th fragment failed\n"),0);
              // not found. so bind a new entry.
              result = this->fragment_table_.bind (frame_info.sequence_num,new_entry);
              if (result != 0)
                ACE_ERROR_RETURN ((LM_ERROR,"fragment table bind failed\n"),0);
            }
        }
      else
          return message_block;
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("read_simple_frame");
      return 0;
    }
  ACE_ENDTRY;
}

ACE_Message_Block *
TAO_SFP::read_fragment (void)
{
  ACE_TRY_NEW_ENV
    {
      flowProtocol::fragment fragment;
      ACE_INET_Addr sender;
      char *buf;
      ACE_NEW_RETURN (buf,
                      char [this->fragment_len_+ACE_CDR::MAX_ALIGNMENT],
                      0);
      ssize_t n =this->dgram_.recv (buf,
                                    this->fragment_len_,
                                    sender,
                                    MSG_PEEK);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_fragment -peek:%p",""),0);
      else if (n==0)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame -peek:%p",""),0);
      else if (n != this->fragment_len_)
        ACE_ERROR_RETURN ((LM_ERROR,"SFP::read_simple_frame - not able to peek\n"),0);
      // print the buffer.
      DUMP_BUF (buf,n);
      ACE_Message_Block mb (n+ACE_CDR::MAX_ALIGNMENT);
      ACE_CDR::mb_align (&mb);
      int result
        = mb.copy (buf,n);
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"read_fragment::Message_Block::copy failed\n"),0);
      // buf[4] is the byte order.
      int byte_order = buf[4] & 0x1;
      ACE_DEBUG ((LM_DEBUG,"mb len = %d,byte_order=%d\n",mb.length (),byte_order));
      TAO_InputCDR cdr (&mb,byte_order);
      //  cdr >>= frame_header;
      cdr.decode (flowProtocol::_tc_fragment,
                  &fragment,
                  0,
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,"frag number = %d, frag size = %d,source id  = %d\n",
                  fragment.frag_number,fragment.frag_sz,fragment.source_id));

      if (fragment.frag_sz < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"negative fragment size:\n"),0);
      ACE_Message_Block *data;
      ACE_NEW_RETURN (data,
                      ACE_Message_Block(fragment.frag_sz),
                      0);
                       
      // Read the fragment.
      n = this->dgram_.recv (data->wr_ptr (),fragment.frag_sz,sender);
      if ((n == -1) || (n==0))
        ACE_ERROR_RETURN ((LM_ERROR,"TAO_SFP::read_fragment:%p",""),0);
      // move past the fragment header.
      data->rd_ptr (this->fragment_len_);
      data->wr_ptr (n);
      ACE_DEBUG ((LM_DEBUG,"length of %dth fragment is: %d\n",
                  fragment.frag_number,
                  data->length ()));
      TAO_SFP_Fragment_Table_Entry *fragment_entry;
      TAO_SFP_Fragment_Node *new_node;
      ACE_NEW_RETURN (new_node,
                      TAO_SFP_Fragment_Node,
                      0);
      new_node->fragment_info_ = fragment;
      new_node->data_ = data;
      if (this->fragment_table_.find (fragment.sequence_num,fragment_entry) == 0)
        {
          // Already an entry exists. Traverse the list and insert it at the right place.
          result = fragment_entry->fragment_set_.insert (*new_node);
          if (result != 0)
            ACE_ERROR_RETURN ((LM_ERROR,"insert for %dth node failed\n",fragment.frag_number),0);
          // check if all the fragments have been received.
        }
      else 
        {
          ACE_NEW_RETURN (fragment_entry,
                          TAO_SFP_Fragment_Table_Entry,
                          0);
          fragment_entry->fragment_set_.insert (*new_node);
          // bind a new entry for this sequence number.
          result = this->fragment_table_.bind (fragment.sequence_num,fragment_entry);
          if (result != 0)
            ACE_ERROR_RETURN ((LM_ERROR,"bind for %dth fragment failed\n",
                               fragment.frag_number),0);
        }
      if (!(fragment.flags & 0x2))
        {
          ACE_DEBUG ((LM_DEBUG,"Last fragment received\n"));
          // if bit 1 is not set then there are 
          // no more fragments.
          fragment_entry->last_received_ = 1;
          // since fragment number starts from 0 to n-1 we add 1.
          fragment_entry->num_fragments_ = fragment.frag_number + 1;
        }
      return check_all_fragments (fragment_entry);
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("TAO_SFP::read_fragment");
      return 0;
    }
  ACE_ENDTRY;
}

ACE_Message_Block*
TAO_SFP::check_all_fragments (TAO_SFP_Fragment_Table_Entry *fragment_entry)
{
  ACE_DEBUG ((LM_DEBUG,"table size: %d, num_fragments: %d\n",fragment_entry->fragment_set_.size (),fragment_entry->num_fragments_));
  // check to see if all the frames have been received.
  if (fragment_entry->fragment_set_.size () == fragment_entry->num_fragments_)
    {
      ACE_DEBUG ((LM_DEBUG,"all fragments have been received\n"));
      // all the fragments have been received
      // we can now chain the ACE_Message_Blocks in the fragment_set_ and then return them 
      // back.
      ACE_Message_Block *frame = 0,*head = 0;
      FRAGMENT_SET_ITERATOR frag_iterator (fragment_entry->fragment_set_);
      TAO_SFP_Fragment_Node *node;
      for (;frag_iterator.next (node) != 0;frag_iterator.advance ())
        {
//           ACE_Message_Block *block = node->data_;
//           char *buf =block->rd_ptr ();
//           ACE_DEBUG ((LM_DEBUG,"length of buf = %d\n",block->length ()));
//           for (int i=0;i<block->length ();i++)
//             ACE_DEBUG ((LM_DEBUG,"%c ",buf[i]));
//           ACE_DEBUG ((LM_DEBUG,"\n"));

          if (!head)
            {
              frame = node->data_;
              head = frame;
            }
          else
            {
              frame->cont (node->data_);
              frame = node->data_;
            }
        }
      return head;
    }
  return 0;
}
