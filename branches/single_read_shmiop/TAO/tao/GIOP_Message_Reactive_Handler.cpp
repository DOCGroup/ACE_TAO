// $Id$

#include "tao/GIOP_Message_Reactive_Handler.h"
#include "tao/GIOP_Message_Generator_Parser_Impl.h"
#include "tao/ORB_Core.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "Transport.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Reactive_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Message_Reactive_Handler, "$Id$")

TAO_GIOP_Message_Reactive_Handler::TAO_GIOP_Message_Reactive_Handler (TAO_ORB_Core * orb_core,
                                                                      TAO_GIOP_Message_Base *base,
                                                                      size_t input_cdr_size)
  : message_state_ (orb_core),
    mesg_base_ (base),
    message_status_ (TAO_GIOP_WAITING_FOR_HEADER),
    message_size_ (input_cdr_size),
    current_buffer_ (orb_core->data_block_for_message_block (input_cdr_size)),
    supp_buffer_ (orb_core->data_block_for_message_block (input_cdr_size))

{
  // NOTE: The message blocks here use a locked allocator which is not
  // from the TSS even if there is one. We are getting the allocators
  // from the global memory. We  shouldn't be using the TSS stuff for
  // the following reason
  // (a) The connection handlers are per-connection and not
  //     per-thread.
  // (b) The order of cleaning is important if we use allocators from
  //     TSS. The TSS goes away when the threads go away. But the
  //     connection handlers go away only when the ORB decides to shut
  //     it down.
  ACE_CDR::mb_align (&this->current_buffer_);

  // Calculate the effective message after alignment
  this->message_size_ -= this->rd_pos ();
}



int
TAO_GIOP_Message_Reactive_Handler::read_messages (TAO_Transport *transport)
{
  // Read the message from the transport. The size of the message read
  // is the maximum size of the buffer that we have less the amount of
  // data that has already been read in to the buffer.
  ssize_t n = transport->recv (this->current_buffer_.wr_ptr (),
                               this->current_buffer_.space ());

  if (n == -1)
    {
      if (errno == EWOULDBLOCK)
        return 0;

      return -1;
    }
  // @@ What are the other error handling here??
  else if (n == 0)
    {
      return -1;
    }

  if (TAO_debug_level == 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - GIOP_Message_Reactive_Handler::read_messages"
                  " received %d bytes\n",
                  n));

      size_t len;
      for (size_t offset = 0; offset < size_t(n); offset += len)
        {
          len = n - offset;
          if (len > 512)
            len = 512;
          ACE_HEX_DUMP ((LM_DEBUG,
                         this->current_buffer_.wr_ptr () + offset,
                         len,
                         "TAO (%P|%t) - read_messages "));
        }
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - received %d bytes \n", n));
    }


  // Now we have a succesful read. First adjust the write pointer
  this->current_buffer_.wr_ptr (n);


  // Success
  return 1;
}

int
TAO_GIOP_Message_Reactive_Handler::parse_message_header (void)
{
  // Check what message are we waiting for and take suitable action
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_HEADER)
    {
      size_t len = this->current_buffer_.length ();
      char *buf = this->current_buffer_.rd_ptr ();

      if (len > TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          // Parse the GIOP header
          if (this->parse_message_header_i (buf) == -1)

            return -1;

          int retval = this->parse_fragment_header (buf + TAO_GIOP_MESSAGE_HEADER_LEN,
                                                    len);

          // Set the pointer read pointer position in the
          // <current_buffer_>
          size_t pos = TAO_GIOP_MESSAGE_HEADER_LEN;

          if (retval)
            {
              // We had a fragment header, so the position should be
              // beyond that
              pos += TAO_GIOP_MESSAGE_FRAGMENT_HEADER;
            }

          this->current_buffer_.rd_ptr (pos);
          buf = this->current_buffer_.rd_ptr ();

          // The GIOP header has been parsed. Set the status to wait for
          // payload
          this->message_status_ = TAO_GIOP_WAITING_FOR_PAYLOAD;

          return 1;
        }

      // We dont have sufficient information to decipher the GIOP
      // header. Make sure that the reactor calls us back.
      return -2;
    }

  // The last read just "read" left-over messages
  return 0;
}

int
TAO_GIOP_Message_Reactive_Handler::is_message_ready (void)
{
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_PAYLOAD)
    {
      size_t len = this->current_buffer_.length ();
      char *buf = this->current_buffer_.rd_ptr ();

      // Set the buf pointer to the start of the GIOP header
      buf -= TAO_GIOP_MESSAGE_HEADER_LEN;

      // Dump the incoming message . It will be dumped only if the
      // debug level is greater than 5 anyway.
      this->mesg_base_->dump_msg (
              "Recv msg",
              ACE_reinterpret_cast (u_char *,
                                    buf),
              len + TAO_GIOP_MESSAGE_HEADER_LEN);
      if (len == this->message_state_.message_size)
        {
          // If the buffer length is equal to the size of the payload we
          // have exactly one message.
          this->message_status_ = TAO_GIOP_WAITING_FOR_HEADER;

          // Check whether we have received only the first part of the
          // fragment.
          return this->message_state_.is_complete (this->current_buffer_);
        }
      else if (len > this->message_state_.message_size)
        {
          // If the length is greater we have received some X messages
          // and a part of X + 1  messages (probably) with X varying
          // from  1 to N.
          this->message_status_ = TAO_GIOP_MULTIPLE_MESSAGES;

          // Clone the data that we read in.
          this->supp_buffer_.data_block (
            this->current_buffer_.data_block ()->clone ());

          // Set the read and write pointer for the supplementary
          // buffer.
          size_t rd_pos = this->rd_pos ();
          this->supp_buffer_.rd_ptr (rd_pos +
                                     this->message_state_.message_size);
          this->supp_buffer_.wr_ptr (this->wr_pos ());

          // Reset the current buffer
          this->current_buffer_.reset ();

          // Set the read and write pointers again for the current
          // buffer. We change the write pointer settings as we would
          // like to process a single message.
          this->current_buffer_.rd_ptr (rd_pos);
          this->current_buffer_.wr_ptr (rd_pos +
                                        this->message_state_.message_size);

          return this->message_state_.is_complete (this->current_buffer_);
        }
    }
  else if (this->message_status_ == TAO_GIOP_MULTIPLE_MESSAGES)
    {
      size_t len = this->supp_buffer_.length ();

      if (len > TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          // @@ What about fragment headers???
          this->current_buffer_.copy (this->supp_buffer_.rd_ptr (),
                                      TAO_GIOP_MESSAGE_HEADER_LEN);

          this->supp_buffer_.rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN);

          len = this->current_buffer_.length ();
          char *buf = this->current_buffer_.rd_ptr ();

          if (this->parse_message_header_i (buf) == -1)

            return -1;

          // Set the pointer read pointer position in the
          // <current_buffer_>
          this->current_buffer_.rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN);

          // The GIOP header has been parsed. Set the status to wait for
          // payload
          this->message_status_ = TAO_GIOP_WAITING_FOR_PAYLOAD;

          return this->get_message ();
        }
      else
        {
          // We have smaller than the header size left here. We
          // just copy the rest of the stuff and reset things so that
          // we can read the rest of the stuff from the socket.
          this->current_buffer_.copy (
              this->supp_buffer_.rd_ptr (),
              len);

          // Reset the supp buffer now
          this->supp_buffer_.reset ();

          this->message_status_ = TAO_GIOP_WAITING_FOR_HEADER;
        }

    }

  // Just send us back to the reactor so that we can for more data to
  // come in .
  return 0;
}

ACE_Data_Block *
TAO_GIOP_Message_Reactive_Handler::steal_data_block (void)
{
  ACE_Data_Block *db =
    this->current_buffer_.data_block ()->clone_nocopy ();

  ACE_Data_Block *old_db =
    this->current_buffer_.replace_data_block (db);

  ACE_CDR::mb_align (&this->current_buffer_);

  return old_db;
}


void
TAO_GIOP_Message_Reactive_Handler::reset (int reset_flag)
{
  // Reset the contents of the message state
  this->message_state_.reset (reset_flag);

  // Reset the current buffer
  this->current_buffer_.reset ();

  ACE_CDR::mb_align (&this->current_buffer_);

  if (this->message_status_ != TAO_GIOP_MULTIPLE_MESSAGES)
    {
      this->supp_buffer_.reset ();
      ACE_CDR::mb_align (&this->supp_buffer_);
    }

}


int
TAO_GIOP_Message_Reactive_Handler::parse_message_header_i (char *buf)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - parsing header\n"));
    }

  // Check whether we have a GIOP Message in the first place
  if (this->parse_magic_bytes (buf) == -1)
    return -1;

  // Let us be specific that this is for 1.0
  if (this->message_state_.giop_version.minor == 0 &&
      this->message_state_.giop_version.major == 1)
    {
      this->message_state_.byte_order =
        buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];

      if (this->message_state_.byte_order != 0 &&
          this->message_state_.byte_order != 1)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) invalid byte order <%d>")
                        ACE_TEXT (" for version <1.0>\n"),
                        this->message_state_.byte_order));
          return -1;
        }
    }
  else
    {
      // Read the byte ORDER
      this->message_state_.byte_order     =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x01);

      // Read the fragment bit
      this->message_state_.more_fragments =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x02);

      if ((buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          if (TAO_debug_level > 2)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) invalid flags for <%d>")
                      ACE_TEXT (" for version <%d %d> \n"),
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET],
                      this->message_state_.giop_version.major,
                      this->message_state_.giop_version.minor));
          return -1;
        }
    }

  // Get the message type
  this->message_state_.message_type =
    buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];

  // Get the payload size. If the payload size is greater than the
  // length then set the length of the message block to that
  // size. Move the rd_ptr to the end of the GIOP header
  this->message_state_.message_size = this->get_payload_size (buf);

  // If the message_size or the payload_size  is zero then something
  // is fishy. So return an error.
  if (this->message_state_.message_size == 0)
    return -1;

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n"),
                  this->message_state_.giop_version.major,
                  this->message_state_.giop_version.minor,
                  this->message_state_.byte_order,
                  this->message_state_.message_type,
                  this->message_state_.message_size));
    }

  return 1;
}


int
TAO_GIOP_Message_Reactive_Handler::parse_fragment_header (char *buf,
                                                          size_t length)
{
  size_t len =
    TAO_GIOP_MESSAGE_FRAGMENT_HEADER + TAO_GIOP_MESSAGE_HEADER_LEN;

  // By this point we are doubly sure that we have a more or less
  // valid GIOP message with a valid major revision number.
  if (this->message_state_.more_fragments &&
      this->message_state_.giop_version.minor == 2 &&
      length > len)
    {
      // Fragmented message in GIOP 1.2 should have a fragment header
      // following the GIOP header.  Grab the rd_ptr to get that
      // info.
      this->message_state_.request_id = this->read_ulong (buf);

      // As we parsed the header
      return 1;
    }

  return 0;
}


int
TAO_GIOP_Message_Reactive_Handler::parse_magic_bytes (char *buf)
{
  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      // For the present...
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) bad header, "
                              "magic word [%2.2x,%2.2x,%2.2x,%2.2x]\n"),
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3]));
      return -1;
    }

  // We have a GIOP message on hand. Get its revision numbers
  CORBA::Octet incoming_major =
    buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  CORBA::Octet incoming_minor =
    buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  if (TAO_GIOP_Message_Generator_Parser_Impl::check_revision (
          incoming_major,
          incoming_minor) == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t|%N|%l) bad version <%d.%d>\n"),
                      incoming_major, incoming_minor));
        }

      return -1;
    }

  // Set the version
  this->message_state_.giop_version.minor = incoming_minor;
  this->message_state_.giop_version.major = incoming_major;

  return 0;
}


CORBA::ULong
TAO_GIOP_Message_Reactive_Handler::get_payload_size (char *rd_ptr)
{
  // Move the read pointer
  rd_ptr += TAO_GIOP_MESSAGE_SIZE_OFFSET;

  CORBA::ULong x = this->read_ulong (rd_ptr);

  if ((x + TAO_GIOP_MESSAGE_HEADER_LEN) > this->message_size_)
      {
        if (ACE_CDR::grow (&this->current_buffer_,
                       x + TAO_GIOP_MESSAGE_HEADER_LEN) == -1)
          {
            if (TAO_debug_level > 0)
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P | %t) Unable to increase the size \n")
                          ACE_TEXT ("of the buffer \n")));
            return 0;
          }

       // New message size is the size of the now larger buffer.
       this->message_size_ = x +
         TAO_GIOP_MESSAGE_HEADER_LEN +
         ACE_CDR::MAX_ALIGNMENT;
      }

  // Set the read pointer to the end of the GIOP message
  //  this->current_buffer_.rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN);
  return x;
}

CORBA::ULong
TAO_GIOP_Message_Reactive_Handler::read_ulong (const char *ptr)
{
  size_t msg_size = 4;

  char *buf = ACE_ptr_align_binary (ptr,
                                    msg_size);

  CORBA::ULong x;
#if !defined (ACE_DISABLE_SWAP_ON_READ)
  if (!(this->message_state_.byte_order != ACE_CDR_BYTE_ORDER))
    {
      x = *ACE_reinterpret_cast (ACE_CDR::ULong*, buf);
    }
  else
    {
      ACE_CDR::swap_4 (buf, ACE_reinterpret_cast (char*, &x));
    }
#else
  x = *ACE_reinterpret_cast(ACE_CDR::ULong*, buf);
#endif /* ACE_DISABLE_SWAP_ON_READ */

  return x;
}




int
TAO_GIOP_Message_Reactive_Handler::get_message (void)
{
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_PAYLOAD)
    {
      size_t len = this->supp_buffer_.length ();
      char * buf =
        this->current_buffer_.rd_ptr ();

      buf -= TAO_GIOP_MESSAGE_HEADER_LEN;

      if (len == this->message_state_.message_size)
        {
          // If the buffer length is equal to the size of the payload we
          // have exactly one message. Check whether we have received
          // only the first part of the fragment.
          this->message_status_ = TAO_GIOP_WAITING_FOR_HEADER;
          this->current_buffer_.copy (this->supp_buffer_.rd_ptr (),
                                      this->message_state_.message_size);

          // The message will be dumped only if the debug level is
          // greater than 5 anyway.
          this->mesg_base_->dump_msg (
                                      "Recv msg",
                                      ACE_reinterpret_cast (u_char *,
                                                            buf),
                                      len +
                                      TAO_GIOP_MESSAGE_HEADER_LEN);

          this->supp_buffer_.rd_ptr (this->message_state_.message_size);
          return this->message_state_.is_complete (this->current_buffer_);
        }
      else if (len > this->message_state_.message_size)
        {
          // If the length is greater we have received some X messages
          // and a part of X + 1  messages (probably) with X varying
          // from  1 to N.
          this->message_status_ = TAO_GIOP_MULTIPLE_MESSAGES;

          this->current_buffer_.copy (this->supp_buffer_.rd_ptr (),
                                      this->message_state_.message_size);

          // The message will be dumped only if the debug level is
          // greater than 5 anyway.
          this->mesg_base_->dump_msg (
                                      "Recv msg",
                                      ACE_reinterpret_cast (u_char *,
                                                            buf),
                                      len +
                                      TAO_GIOP_MESSAGE_HEADER_LEN);

          this->supp_buffer_.rd_ptr (this->message_state_.message_size);
          return this->message_state_.is_complete (this->current_buffer_);
        }
      else
        {
          // The remaining message in the supp buffer
          this->current_buffer_.copy (this->supp_buffer_.rd_ptr (),
                                      this->supp_buffer_.length ());

          // Reset the supp buffer now
          this->supp_buffer_.reset ();
        }
    }

  return 0;
}
