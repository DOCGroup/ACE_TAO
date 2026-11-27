#include "Handle_L_CODgram.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_TCHAR const *Handle_L_CODgram::DEFAULT_RENDEZVOUS =
  ACE_TEXT ("/tmp/foo_codgram");

#if !defined (__ACE_INLINE__)
#include "Handle_L_CODgram.inl"
#endif /* __ACE_INLINE__ */

extern "C"
{
  ACE_Svc_Export Handle_L_CODgram local_codgram;
}
ACE_Service_Object_Type lc (&local_codgram, ACE_TEXT ("Local_CODgram"));

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
