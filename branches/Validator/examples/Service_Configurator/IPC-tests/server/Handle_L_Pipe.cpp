// $Id$

#include "Handle_L_Pipe.h"

ACE_RCSID(server, Handle_L_Pipe, "$Id$")

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

// Uppercase N bytes of S.

char *
Handle_L_Pipe::upper_case (char s[], int n)
{
  while (--n >= 0)
    if (islower (s[n]))
      s[n] = toupper (s[n]);

  return s;
} 

int
Handle_L_Pipe::handle_input (ACE_HANDLE)
{
  ACE_LSOCK_Stream new_local_stream;
  int n;
  ACE_HANDLE fd1 = ACE_INVALID_HANDLE;
  ACE_HANDLE fd2 = ACE_INVALID_HANDLE;
  char buf[BUFSIZ];

  if (this->accept (new_local_stream) == -1)
    return -1;

  if (new_local_stream.recv_handle (fd1) == -1 
      || new_local_stream.recv_handle (fd2) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO,
                "received file descriptors %d and %d\n",
                fd1,
                fd2));

  if ((n = ACE_OS::read (fd1, buf, sizeof buf)) == -1)
    return -1;
  else if (ACE_OS::write (fd2, this->upper_case (buf, n), n) == -1)
    return -1;
  if (ACE_OS::close (fd1) == -1 
      || ACE_OS::close (fd2) == -1)
    return -1;
  if (new_local_stream.close () == -1)
    return -1;

  return 0;
}

const char *Handle_L_Pipe::DEFAULT_RENDEZVOUS = "/tmp/foo_pipe";

#if !defined (__ACE_INLINE__)
#include "Handle_L_Pipe.i"
#endif /* __ACE_INLINE__ */

Handle_L_Pipe local_pipe;
ACE_Service_Object_Type lp (&local_pipe, "Local_Pipe");

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
