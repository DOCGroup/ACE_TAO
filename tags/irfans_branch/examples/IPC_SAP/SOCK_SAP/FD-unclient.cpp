// $Id$

#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"                              
                                                        
#if defined (ACE_HAS_MSG) && !defined (ACE_HAS_UNIX_DOMAIN_SOCKETS)
// ACE_LSOCK Client.
                                                        
int 
main (int argc, char *argv[])                       
{                                                       
  const char *file_name = argc > 1 ? argv[1] : "./local_data";
  const char *rendezvous = argc > 2 ? argv[2] : ACE_DEFAULT_RENDEZVOUS;

  ACE_LSOCK_Stream cli_stream;
  ACE_UNIX_Addr addr (rendezvous);

  // Establish the connection with server.
  ACE_LSOCK_Connector connector;

  if (connector.connect (cli_stream, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "connect"), -1);

  ACE_HANDLE handle = ACE_OS::open (file_name, O_RDONLY);

  if (handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "open"), -1);

  // Send handle to server (correctly handles incomplete writes).
  if (cli_stream.send_handle (handle) == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "send"), -1);

  char buf[BUFSIZ];
  ssize_t n = cli_stream.recv (buf, sizeof buf);

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "recv"), -1);
  else if (n == 0)
    ACE_DEBUG ((LM_DEBUG, "server shutdown (bug in kernel?)\n"));
  else    
    ACE_DEBUG ((LM_DEBUG, "server %*s shutdown\n", n, buf));

  // Explicitly close the connection.
  if (cli_stream.close () == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "close"), -1);

  return 0;
}                                                       
#else
int main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support sendmsg/recvmsg to run this test\n"), -1);
}
#endif /* ACE_HAS_MSG */
