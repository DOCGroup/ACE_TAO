//$Id$
#include "GIOP_Message.h"

#if defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */

TAO_GIOP_Message_Factory::TAO_GIOP_Message_Factory (void)
{
}

TAO_GIOP_Message_Factory::~TAO_GIOP_Message_Factory (void)
{
}

CORBA::Boolean
TAO_GIOP_Message_Factory::start_message (const TAO_GIOP_Version &version,
                                        TAO_GIOP_Message_Factory::Message_Type t,
                                        TAO_OutputCDR &msg)
{
  msg.reset ();

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
  msg.write_octet ((CORBA::Octet) type);

  // Write a dummy <size> later it is set to the right value...
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}
