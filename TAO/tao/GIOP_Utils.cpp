//$Id$
#include "tao/debug.h"
#include "tao/GIOP_Utils.h"
#include "tao/ORB_Core.h"


#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Utils.i"
#endif /* __ACE_INLINE__ */



TAO_GIOP_Message_State::TAO_GIOP_Message_State (TAO_ORB_Core* orb_core)
  : byte_order (TAO_ENCAP_BYTE_ORDER),
    more_fragments (0),
    message_type (TAO_GIOP_MESSAGERROR),
    message_size (0),
    current_offset (0),
    cdr (orb_core->create_input_cdr_data_block (ACE_CDR::DEFAULT_BUFSIZE),
         TAO_ENCAP_BYTE_ORDER,
         orb_core),
    fragments_begin (0),
    fragments_end (0)
{
  //giop_version.major = TAO_DEF_GIOP_MAJOR;
  //giop_version.minor = TAO_DEF_GIOP_MINOR;
}

int
TAO_GIOP_Message_State::is_complete ()
{
  if (this->message_size != this->current_offset)
    return 0;

  if (this->more_fragments)
    {
      // This is only one fragment of the complete Request....
      ACE_Message_Block* current =
        this->cdr.steal_contents ();
      if (this->fragments_begin == 0)
        {
          this->first_fragment_byte_order = this->byte_order;
          this->first_fragment_giop_version = this->giop_version;
          this->first_fragment_message_type = this->message_type;
          this->fragments_end = this->fragments_begin = current;
          this->reset ();
          return 0;
        }

      return this->append_fragment (current);
    }

  if (this->fragments_begin != 0)
    {
      // This is the last message, but we must defragment before
      // sending

      ACE_Message_Block* current =
        this->cdr.steal_contents ();
      if (this->append_fragment (current) == -1)
        return -1;

      // Copy the entire chain into the input CDR.....
      this->cdr.reset (this->fragments_begin,
                       this->first_fragment_byte_order);
      ACE_Message_Block::release (this->fragments_begin);
      this->fragments_begin = 0;
      this->fragments_end = 0;
      
      this->byte_order = this->first_fragment_byte_order;
      this->giop_version = this->first_fragment_giop_version;
      this->message_type = this->first_fragment_message_type;

      /*FALLTHROUGH*/
    }
  // else
  // {
  // This message has no more fragments, and there where no fragments
  // before it, just return... notice that this->cdr has the right
  // contents.
  // }

  return 1;
}

int
TAO_GIOP_Message_State::append_fragment (ACE_Message_Block* current)
{
  this->fragments_end->cont (current);
  this->fragments_end = this->fragments_end->cont ();

  if (this->first_fragment_byte_order != this->byte_order
      || this->first_fragment_giop_version.major != this->giop_version.major
      || this->first_fragment_giop_version.minor != this->giop_version.minor)
    {
      // Yes, print it out in all debug levels!
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) incompatible fragments:\n"
                  "   Different GIOP versions or byte order\n"));
      this->reset ();
      return -1;
    }
  this->reset ();
  return 0;
}


void
TAO_GIOP_Utils::dump_msg (const char */*label*/,
                         const u_char */*ptr*/,
                         size_t /*len*/)
{
  if (TAO_debug_level >= 5)
    {
      // I will have to print out all the relevant debug messages!!
      // Let me not wory about that now. I will get back to that at a
      // later date!!
    }
}

CORBA::Boolean
TAO_GIOP_Utils::start_message (const TAO_GIOP_Version &version,
                               TAO_Pluggable_Message_Type t,
                               TAO_OutputCDR &msg)
{
  static CORBA::Octet magic[] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
  };

  static int magic_size = sizeof (magic)/sizeof (magic[0]);
  msg.write_octet_array (magic, magic_size);

  msg.write_octet (version.major);
  msg.write_octet (version.minor);
  msg.write_octet (TAO_ENCAP_BYTE_ORDER);
  
  CORBA::Octet type = 0;
  
  // Conversion from the Pluggable Message types to the GIOP types. 
  switch (t)
    {
    case (TAO_MESSAGE_REQUEST):
      type = TAO_GIOP_REQUEST;
      break;
    case (TAO_MESSAGE_REPLY):
      type = TAO_GIOP_REPLY;
      break;
    case (TAO_MESSAGE_CANCELREQUEST):
      type = TAO_GIOP_CANCELREQUEST;
      break;
    case (TAO_MESSAGE_LOCATEREQUEST):
      type = TAO_GIOP_LOCATEREQUEST;
      break;
    case (TAO_MESSAGE_LOCATEREPLY):
      type = TAO_GIOP_LOCATEREPLY;
      break;
    case (TAO_MESSAGE_CLOSECONNECTION):
      type = TAO_GIOP_CLOSECONNECTION;
      break;
    case (TAO_MESSAGE_MESSAGERROR):
      type = TAO_GIOP_MESSAGERROR;
      break;
    case (TAO_MESSAGE_FRAGMENT):
      type = TAO_GIOP_FRAGMENT;
      break;
    }
  
  msg.write_octet ((CORBA::Octet) type);

  // Write a dummy <size> later it is set to the right value...
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}
