// $Id$

#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"                              
                                                        
#if defined (ACE_HAS_MSG) && !defined (ACE_HAS_UNIX_DOMAIN_SOCKETS)
// ACE_LSOCK Client.
                                                        
int 
main (int argc, char *argv[])                       
{                                                       
  char *file_name = argc > 1 ? argv[1] : "./local_data";
  char *rendezvous = argc > 2 ? argv[2] : ACE_DEFAULT_RENDEZVOUS;
  ACE_HANDLE handle;
  int n;
  char buf[BUFSIZ];

  ACE_LSOCK_Stream cli_stream;
  ACE_UNIX_Addr addr (rendezvous);

  // Establish the connection with server.
  ACE_LSOCK_Connector connector;

  if (connector.connect (cli_stream, addr) == -1)
    ACE_OS::perror ("connect"), ACE_OS::exit (1);

  if ((handle = ACE_OS::open (file_name, O_RDONLY)) == -1)
    ACE_OS::perror ("open"), ACE_OS::exit (1);

  // Send handle to server (correctly handles incomplete writes).
  if (cli_stream.send_handle (handle) == -1) 
    ACE_OS::perror ("send"), ACE_OS::exit (1);                          

  if ((n = cli_stream.recv_n (buf, sizeof buf)) == -1)
    ACE_OS::perror ("recv"), ACE_OS::exit (1);
  else
    ACE_OS::write (ACE_STDOUT, buf, n);

  // Explicitly close the connection.
  if (cli_stream.close () == -1) 
    ACE_OS::perror ("close"), ACE_OS::exit (1);

  return 0;
}                                                       
#else
int main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support sendmsg/recvmsg to run this test\n"), -1);
}
#endif /* ACE_HAS_MSG */
