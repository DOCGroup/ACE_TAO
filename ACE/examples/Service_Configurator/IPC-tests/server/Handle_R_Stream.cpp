#include "Handle_R_Stream.h"

// Static variables.
u_short Handle_R_Stream::DEFAULT_PORT = ACE_DEFAULT_SERVER_PORT;
char *Handle_R_Stream::login_name  = 0;
char Handle_R_Stream::login[ACE_MAX_USERID];

#if !defined (__ACE_INLINE__)
#include "Handle_R_Stream.inl"
#endif /* __ACE_INLINE__ */

Handle_R_Stream remote_stream;
ACE_Service_Object_Type rs (&remote_stream, ACE_TEXT("Remote_Stream"));
