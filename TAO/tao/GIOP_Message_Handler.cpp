// $Id$

#include "tao/GIOP_Message_Handler.h"
#include "tao/GIOP_Message_Generator_Parser_Impl.h"
#include "tao/ORB_Core.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "Transport.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Message_Handler, "$Id$")

TAO_GIOP_Message_Handler::TAO_GIOP_Message_Handler (TAO_ORB_Core * orb_core,
                                                    TAO_GIOP_Message_Base *base)
  : mesg_base_ (base),
    message_status_ (TAO_GIOP_WAITING_FOR_HEADER),
    message_size_ (ACE_CDR::DEFAULT_BUFSIZE),
    current_buffer_ (ACE_CDR::DEFAULT_BUFSIZE),
    // @@ This doesn't seem to work. The problem comes when we extract
    // data portion from this buffer in the skeleton. Why?? Needs
    // investigation.
    //current_buffer_ (orb_core->create_input_cdr_data_block (ACE_CDR::DEFAULT_BUFSIZE)),
    supp_buffer_ (ACE_CDR::DEFAULT_BUFSIZE),
    message_state_ (orb_core)
{
}


int
TAO_GIOP_Message_Handler::read_parse_message (TAO_Transport *transport)
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

  // Now we have a succesful read. First adjust the write pointer
  this->current_buffer_.wr_ptr (n);

  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - received %d bytes\n", n));
    }

  // Check what message are we waiting for and take suitable action
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_HEADER)
    {
      if (this->current_buffer_.length () >=
          TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          return this->parse_header ();
        }
    }

  return 0;
}

int
TAO_GIOP_Message_Handler::parse_header (void)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - parsing header\n"));
    }

  // Check whether we have a GIOP Message in the first place
  if (this->parse_magic_bytes () == -1)
    return -1;

  // Grab the read pointer
  char *buf = this->current_buffer_.rd_ptr ();

  // Let us be specific that it is for 1.0
  if (this->message_state_.giop_version.minor == 0 &&
      this->message_state_.giop_version.minor == 1)
    {
      this->message_state_.byte_order =
        buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];
      if (TAO_debug_level > 2
          && this->message_state_.byte_order != 0 &&
          this->message_state_.byte_order != 1)
        {
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

      if (TAO_debug_level > 2
          && (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
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
  this->message_state_.message_size = this->get_payload_size ();

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

  if (this->message_state_.more_fragments &&
      this->message_state_.giop_version.minor == 2 &&
      this->current_buffer_.length () > TAO_GIOP_MESSAGE_FRAGMENT_HEADER)
    {
      // Fragmented message in GIOP 1.2 should have a fragment header
      // following the GIOP header.  Grab the rd_ptr to get that
      // info.
      buf = this->current_buffer_.rd_ptr ();
      this->message_state_.request_id = this->read_ulong (buf);

      // Move the read pointer to the end of the fragment header
      this->current_buffer_.rd_ptr (TAO_GIOP_MESSAGE_FRAGMENT_HEADER);
    }

  // The GIOP header has been parsed. Set the status to wait for
  // payload
  this->message_status_ = TAO_GIOP_WAITING_FOR_PAYLOAD;

  return 1;
}


int
TAO_GIOP_Message_Handler::parse_magic_bytes (void)
{
  // Grab the read pointer
  char *buf = this->current_buffer_.rd_ptr ();

  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      // For the present...
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) bad header, magic word [%c%c%c%c]\n"),
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
TAO_GIOP_Message_Handler::get_payload_size (void)
{
  // Set the read pointer in <current_buffer_> to point to the size of
  // the payload
  this->current_buffer_.rd_ptr (TAO_GIOP_MESSAGE_SIZE_OFFSET);

  CORBA::ULong x = this->read_ulong (this->current_buffer_.rd_ptr ());

  if ((x + TAO_GIOP_MESSAGE_HEADER_LEN) > this->message_size_)
    {
      // Increase the size of the <current_buffer_>
      this->current_buffer_.size (x + TAO_GIOP_MESSAGE_HEADER_LEN);
      this->message_size_ = x + TAO_GIOP_MESSAGE_HEADER_LEN;
    }

  // Set the read pointer to the end of the GIOP header
  this->current_buffer_.rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN -
                                TAO_GIOP_MESSAGE_SIZE_OFFSET);
  return x;
}

CORBA::ULong
TAO_GIOP_Message_Handler::read_ulong (const char *ptr)
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
TAO_GIOP_Message_Handler::is_message_ready (void)
{
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_PAYLOAD)
    {
      size_t len = this->current_buffer_.length ();
      char *buf = this->current_buffer_.rd_ptr ();
      if (len == this->message_state_.message_size)
        {
          // If the buffer length is equal to the size of the payload we
          // have exactly one message. Check whether we have received
          // only the first part of the fragment.
          this->message_status_ = TAO_GIOP_WAITING_FOR_HEADER;

          buf -= TAO_GIOP_MESSAGE_HEADER_LEN;

          if (TAO_debug_level >= 4)
            this->mesg_base_->dump_msg (
                "Recv msg",
                ACE_reinterpret_cast (u_char *,
                                      buf),
                this->message_state_.message_size);

          return this->message_state_.is_complete (this->current_buffer_);
        }
      else if (len > this->message_state_.message_size)
        {
          // If the length is greater we have received some X messages
          // and a part of X + 1  messages (probably) with X varying
          // from  1 to N.
          this->message_status_ = TAO_GIOP_MULTIPLE_MESSAGES;

          buf -= TAO_GIOP_MESSAGE_HEADER_LEN;

          if (TAO_debug_level >= 4)
            this->mesg_base_->dump_msg (
              "Recv msg",
              ACE_reinterpret_cast (u_char *,
                                    buf),
              this->message_state_.message_size);

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

          // Set the read and write pointers again for the current buffer
          this->current_buffer_.rd_ptr (rd_pos);
          this->current_buffer_.wr_ptr (rd_pos +
                                        this->message_state_.message_size);

          return this->message_state_.is_complete (this->current_buffer_);
        }
    }

  // Just return allowing the reactor to call us back to get the rest
  // of the info
  return 0;
}


int
TAO_GIOP_Message_Handler::more_messages (void)
{
  if (this->message_status_ == TAO_GIOP_MULTIPLE_MESSAGES)
    {
      size_t len = this->supp_buffer_.length ();

      // @@ Once we start receing fragments, somthing like this would
      // be needed. We will revisit this then.
      /*      if (len > TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          this->current_buffer_.reset ();
          if (len > (TAO_GIOP_MESSAGE_HEADER_LEN +
                     TAO_GIOP_MESSAGE_FRAGMENT_HEADER))
            {
              this->current_buffer_.copy (
                this->supp_buffer_.rd_ptr (),
                TAO_GIOP_MESSAGE_HEADER_LEN +
                TAO_GIOP_MESSAGE_FRAGMENT_HEADER);

              this->supp_buffer_.rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN +
                                         TAO_GIOP_MESSAGE_FRAGMENT_HEADER);
            }
            else*/
      if (len > TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          this->current_buffer_.copy (
                                      this->supp_buffer_.rd_ptr (),
                                      TAO_GIOP_MESSAGE_HEADER_LEN);

          this->supp_buffer_.rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN);

          if (this->parse_header () == -1)
            return -1;

          return this->get_message ();
        }

    }

  return TAO_MESSAGE_BLOCK_COMPLETE;
}


int
TAO_GIOP_Message_Handler::get_message (void)
{
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_PAYLOAD)
    {
      size_t len = this->supp_buffer_.length ();

      if (len == this->message_state_.message_size)
        {
          // If the buffer length is equal to the size of the payload we
          // have exactly one message. Check whether we have received
          // only the first part of the fragment.
          this->message_status_ = TAO_GIOP_WAITING_FOR_HEADER;
          this->current_buffer_.copy (this->supp_buffer_.rd_ptr (),
                                      this->message_state_.message_size);

          char * buf =
            this->current_buffer_.rd_ptr ();

          buf -= TAO_GIOP_MESSAGE_HEADER_LEN;

          if (TAO_debug_level >= 4)
            this->mesg_base_->dump_msg (
                "Recv msg",
                ACE_reinterpret_cast (u_char *,
                                      buf),
                this->message_state_.message_size);

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

          char * buf =
            this->current_buffer_.rd_ptr ();

          buf -= TAO_GIOP_MESSAGE_HEADER_LEN;

          if (TAO_debug_level >= 4)
            this->mesg_base_->dump_msg (
              "Recv msg",
              ACE_reinterpret_cast (u_char *,
                                    buf),
              this->message_state_.message_size);

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

  return TAO_MESSAGE_BLOCK_INCOMPLETE;
}
