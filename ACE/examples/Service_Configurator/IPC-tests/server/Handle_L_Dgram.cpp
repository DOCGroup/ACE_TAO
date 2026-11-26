#include "Handle_L_Dgram.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_TCHAR const *Handle_L_Dgram::DEFAULT_RENDEZVOUS =
  ACE_TEXT ("/tmp/foo_dgram");

#if !defined (__ACE_INLINE__)
#include "Handle_L_Dgram.inl"
#endif /* __ACE_INLINE__ */

extern "C"
{
  ACE_Svc_Export Handle_L_Dgram local_dgram;
}
ACE_Service_Object_Type ld (&local_dgram, ACE_TEXT ("Local_Dgram"));
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
