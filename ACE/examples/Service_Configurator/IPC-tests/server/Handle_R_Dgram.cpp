#include "Handle_R_Dgram.h"

unsigned short Handle_R_Dgram::DEFAULT_PORT = ACE_DEFAULT_SERVER_PORT;

#if !defined (__ACE_INLINE__)
#include "Handle_R_Dgram.inl"
#endif /* __ACE_INLINE__ */

extern "C"
{
  ACE_Svc_Export Handle_R_Dgram remote_dgram;
}
ACE_Service_Object_Type rd (&remote_dgram, ACE_TEXT ("Remote_Dgram"));
