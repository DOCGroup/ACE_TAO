// $Id$

#include "tao/GIOP_Message_State.h"
#include "tao/GIOP_Message_Generator_Parser_Impl.h"
#include "tao/ORB_Core.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
//#include "Transport.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_State.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao, GIOP_Message_State, "$Id$")

TAO_GIOP_Message_State::TAO_GIOP_Message_State (
    TAO_ORB_Core * /*orb_core*/,
    TAO_GIOP_Message_Base *base)
  : base_ (base),
    giop_version_ (TAO_DEF_GIOP_MAJOR,
                   TAO_DEF_GIOP_MINOR),
    byte_order_ (0),
    message_type_ (0),
    message_size_ (0),
    request_id_ (0),
    more_fragments_ (0),
    missing_data_ (0)
{
}


int
TAO_GIOP_Message_State::parse_message_header (ACE_Message_Block &incoming)
{
  if (incoming.length () >= TAO_GIOP_MESSAGE_HEADER_LEN)
    {
      // Parse the GIOP header
      if (this->parse_message_header_i (incoming) == -1)
        return -1;
    }

  return 0;
}

int
TAO_GIOP_Message_State::parse_message_header_i (ACE_Message_Block &incoming)
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
  if (this->get_version_info (buf) == -1)
    return -1;

  // Get the byte order information...
  if (this->get_byte_order_info (buf) == -1)
    return -1;

  // Get the message type
  this->message_type_ = buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];


  // Get the size of the message..
  this->get_payload_size (buf);

  if (this->message_size_ == 0)
    {
      if (this->message_type_ == TAO_GIOP_MESSAGERROR)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "TAO (%P|%t) -"
                          "GIOP_MESSAGE_ERROR received \n"));
            }
          return 0;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - "
                        "Message of  size zero recd. \n"));
          return -1;
        }
    }

  if (this->more_fragments_)
    {
      (void) this->parse_fragment_header (buf,
                                          incoming.length ());
    }

  return 0;
}




int
TAO_GIOP_Message_State::parse_magic_bytes (char *buf)
{
  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - bad header, "
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
TAO_GIOP_Message_State::get_version_info (char *buf)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - Getting version info.. \n"));
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
                      ACE_TEXT ("TAO (%P|%t) - bad version <%d.%d>\n"),
                      incoming_major, incoming_minor));
        }

      return -1;
    }

  // Set the version
  this->giop_version_.minor = incoming_minor;
  this->giop_version_.major = incoming_major;

  return 0;
}

int
TAO_GIOP_Message_State::get_byte_order_info (char *buf)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) -  Getting byte order info.. \n"));
    }

    // Let us be specific that this is for 1.0
  if (this->giop_version_.minor == 0 &&
      this->giop_version_.major == 1)
    {
      this->byte_order_ =
        buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];

      if (this->byte_order_ != 0 &&
          this->byte_order_ != 1)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - invalid byte order <%d>")
                        ACE_TEXT (" for version <1.0>\n"),
                        this->byte_order_));
          return -1;
        }
    }
  else
    {
      // Read the byte ORDER
      this->byte_order_ =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x01);

      // Read the fragment bit
      this->more_fragments_ =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x02);

      if ((buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          if (TAO_debug_level > 2)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - invalid flags for <%d>")
                      ACE_TEXT (" for version <%d %d> \n"),
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET],
                      this->giop_version_.major,
                      this->giop_version_.minor));
          return -1;
        }
    }

  return 0;
}

void
TAO_GIOP_Message_State::get_payload_size (char *rd_ptr)
{
  // Move the read pointer
  rd_ptr += TAO_GIOP_MESSAGE_SIZE_OFFSET;

  this->message_size_ =  this->read_ulong (rd_ptr);
}



int
TAO_GIOP_Message_State::parse_fragment_header (char *buf,
                                               size_t length)
{
  size_t len =
    TAO_GIOP_MESSAGE_FRAGMENT_HEADER + TAO_GIOP_MESSAGE_HEADER_LEN;

  buf += TAO_GIOP_MESSAGE_HEADER_LEN;

  // By this point we are doubly sure that we have a more or less
  // valid GIOP message with a valid major revision number.
  if (this->giop_version_.minor == 2 &&
      this->message_type_ == TAO_GIOP_FRAGMENT &&
      length > len)
    {
      // Fragmented message in GIOP 1.2 should have a fragment header
      // following the GIOP header.  Grab the rd_ptr to get that
      // info.
      this->request_id_ = this->read_ulong (buf);

      // As we parsed the header
      return 1;
    }

  return 0;
}

CORBA::ULong
TAO_GIOP_Message_State::read_ulong (char *rd_ptr)
{
  CORBA::ULong x = 0;

  // We dont need to do this sort of copy. But some compilers (read it
  // as solaris ones) have a problem in deferencing from the
  // reinterpret_cast pointer of the <rd_ptr>, as the <rd_ptr> can be
  // on stack. So let us go ahead with this copying...
  char buf [4];
  buf[0] = *rd_ptr;
  buf[1] = *(rd_ptr + 1);
  buf[2] = *(rd_ptr + 2);
  buf[3] = *(rd_ptr + 3);

#if !defined (ACE_DISABLE_SWAP_ON_READ)
  if (!(this->byte_order_ != ACE_CDR_BYTE_ORDER))
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
