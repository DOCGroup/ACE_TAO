// $Id$

#include "tao/GIOP_Message_Reactive_Handler.h"
#include "tao/GIOP_Message_Generator_Parser_Impl.h"
#include "tao/ORB_Core.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "Transport.h"

#if 0
#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Reactive_Handler.inl"
#endif /* __ACE_INLINE__ */

#endif /*if 0*/

ACE_RCSID(tao, GIOP_Message_Reactive_Handler, "$Id$")

TAO_GIOP_Message_Reactive_Handler::TAO_GIOP_Message_Reactive_Handler (
    TAO_ORB_Core * /*orb_core*/,
    TAO_GIOP_Message_Base * /*base*/)
{

}


int
TAO_GIOP_Message_Reactive_Handler::parse_message_header (ACE_Message_Block &incoming,
                                                         TAO_GIOP_Message_State &state)
{
  // @@Bala:  Need to make a check whether we are waiting for the
  // header...
  if (incoming.length () > TAO_GIOP_MESSAGE_HEADER_LEN)
    {
      // Parse the GIOP header
      if (this->parse_message_header_i (incoming, state) == -1)
        return -1;
    }

  return 0;
}

int
TAO_GIOP_Message_Reactive_Handler::parse_message_header_i (ACE_Message_Block &incoming,
                                                           TAO_GIOP_Message_State &state)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - parsing header\n"));
    }

  // Grab the rd_ptr_ from the message block..
  char *buf = incoming.rd_ptr ();

  // Parse the magic bytes first
  if (this->parse_magic_bytes (buf) == -1)
    {
      return -1;
    }

  // Get the version information
  if (this->get_version_info (buf, state) == -1)
    return -1;

  // Get the byte order information...
  if (this->get_byte_order_info (buf, state) == -1)
    return -1;

  // Get the message type
  state.message_type = buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];

  // Get the payload size
  this->get_payload_size (buf, state);

  // Parse the
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
    }
      return 1;



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

 return 0;
}

int
TAO_GIOP_Message_Reactive_Handler::get_version_info (char *buf,
                                                     TAO_GIOP_Message_State &state)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Getting version info.. \n"));
    }

  // We have a GIOP message on hand. Get its revision numbers
  CORBA::Octet incoming_major =
    buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  CORBA::Octet incoming_minor =
    buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  // Check the revision information
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
  state.giop_version.minor = incoming_minor;
  state.giop_version.major = incoming_major;

  return 0;
}

int
TAO_GIOP_Message_Reactive_Handler::get_byte_order_info (char *buf,
                                                        TAO_GIOP_Message_State &message_state)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Getting byte order info.. \n"));
    }

    // Let us be specific that this is for 1.0
  if (message_state.giop_version.minor == 0 &&
      message_state.giop_version.major == 1)
    {
      message_state_.byte_order =
        buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];

      if (message_state.byte_order != 0 &&
          message_state.byte_order != 1)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) invalid byte order <%d>")
                        ACE_TEXT (" for version <1.0>\n"),
                        message_state.byte_order));
          return -1;
        }
    }
  else
    {
      // Read the byte ORDER
      message_state.byte_order     =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x01);

      // Read the fragment bit
      message_state.more_fragments =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x02);

      if ((buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          if (TAO_debug_level > 2)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) invalid flags for <%d>")
                      ACE_TEXT (" for version <%d %d> \n"),
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET],
                      message_state_.giop_version.major,
                      message_state_.giop_version.minor));
          return -1;
        }
    }

  return 0;
}

CORBA::ULong
TAO_GIOP_Message_Reactive_Handler::get_payload_size (char *rd_ptr,
                                                     TAO_GIOP_Message_State &message_state)
{
  // Move the read pointer
  rd_ptr += TAO_GIOP_MESSAGE_SIZE_OFFSET;

  CORBA::ULong x = 0;

  size_t msg_size = 4;

  char *buf = ACE_ptr_align_binary (rd_ptr,
                                    msg_size);

#if !defined (ACE_DISABLE_SWAP_ON_READ)
  if (!(state.byte_order != ACE_CDR_BYTE_ORDER))
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

  message_state.message_size =  x;
}
