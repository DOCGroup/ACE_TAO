#include "tao/GIOP_Message_Handler.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Generator_Parser_Impl.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Handler.inl"
#endif /* __ACE_INLINE__ */





ACE_RCSID(tao, GIOP_Message_Handler, "$Id$")



TAO_GIOP_Message_Handler::
    TAO_GIOP_Message_Handler (TAO_ORB_Core * orb_core)
      : message_status_ (TAO_GIOP_WAITING_FOR_HEADER),
        message_size_ (ACE_CDR::DEFAULT_BUFSIZE),
        current_buffer_ (message_size_),
        message_state_ (orb_core)
{
}


int
TAO_GIOP_Message_Handler::read_parse_message (TAO_Transport *transport)
{
  // Read the message from the transport
  ssize_t n = transport->read (this->current_buffer_.wr_ptr (),
                               this->message_size_);

  if (n == -1)
    {
      if (errno == EWOULDBLOCK)
        return 0;
     else if (errno == ECONNRESET)
       {
         // @@ Is this OK??

         // We got a connection reset (TCP RSET) from the other side,
         // i.e., they didn't initiate a proper shutdown.
         //
         // Make it look like things are OK to the upper layer.
         errno = 0;
         return 0;
       }
      return -1;
    }
  // @@ What are the other error handling here??
  else if (n == 0)
    {
      return -1;
    }

  // Now we have a succesful read. First adjust the read pointer
  this->current_buffer_.wr_ptr (n);

  // Check what message are we waiting for and take suitable action
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_HEADER)
    {
      if (this->current_buffer_.length () >=
          TAO_GIOP_MESSAGE_HEADER_LEN)
        {
          if (this->parse_header () == -1)
            return -1;
        }
    }

  /*if (this->message_status_ == TAO_GIOP_WAITING_FOR_PAYLOAD)
    {
      // If the length of the buffer is greater than the size of the
      // message that we received then process that message. If not
      // just return allowing the reactor to call us back.
      if (this->current_buffer_.length () < (this->message_status_.message_size +
                                             TAO_GIOP_MESSAGE_HEADER_LEN))
        return 0;
      else
        {
          // We have payloads that we need to process
          if (this->parse_payload () == -1)
            return -1;
        }
        }*/

  return 0;
}

int
TAO_GIOP_Message_Handler::parse_header (void)
{
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
  // length then set the length of the message block to that size
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

  // The GIOP header has been parsed. Set the status to wait for payload
  this->message_status_ = TAO_GIOP_WAITING_FOR_PAYLOAD;

  return 0;
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

  // No. of bytes occupied by the message size in the header.
  size_t msg_size = 4;

  char *buf = ACE_ptr_align_binary (this->current_buffer_.rd_ptr (),
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

  if ((x + TAO_GIOP_MESSAGE_HEADER_LEN) > this->message_size_)
    {
      // Increase the size of the <current_buffer_>
      this->current_buffer_.size (x + TAO_GIOP_MESSAGE_HEADER_LEN);
    }

  // Set the read pointer to the end of the GIOP header
  this->current_buffer_.rd_ptr (TAO_GIOP_MESSAGE_HEADER_LEN -
                                TAO_GIOP_MESSAGE_SIZE_OFFSET);
  return x;
}

int
TAO_GIOP_Message_Handler::is_message_ready (void)
{
  if (this->message_status_ == TAO_GIOP_WAITING_FOR_PAYLOAD)
    {
      // If the length of the buffer is greater than the size of the
      // message that we received then process that message. If not
      // just return allowing the reactor to call us back.
      if (this->current_buffer_.length () <
          (this->message_state_.message_size))

        {
          return 0;
        }
    }

  this->message_status_ = TAO_GIOP_WAITING_FOR_HEADER;

  // We have atleast one message for processing
  return 1;
}
/*int
TAO_GIOP_Message_Handler::parse_payload (void)
{
  if (this->current_buffer_.length () ==
      (this->message_state_.message_size + TAO_GIOP_MESSAGE_HEADER_LEN))
    {
      // We have exactly one message in the buffer

        // Reset our input CDR stream
      this->message_state_.cdr.reset_byte_order
        (this->message_state_.byte_order);
    }
}
*/
