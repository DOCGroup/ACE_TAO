#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
// @(#)Handle_L_Stream.cpp	1.1	10/18/96

#include "Handle_L_Stream.h"

/* Static variables. */

const char *Handle_L_Stream::DEFAULT_RENDEZVOUS = "/tmp/foo_stream";
char *Handle_L_Stream::login_name = 0;

#if !defined (__ACE_INLINE__)
#include "Handle_L_Stream.i"
#endif /* __ACE_INLINE__ */

Handle_L_Stream local_stream;
ACE_Service_Object_Type ls (&local_stream, "Local_Stream");
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
