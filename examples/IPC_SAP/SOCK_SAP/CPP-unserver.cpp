// ACE_LSOCK Server.
// $Id$

#include "ace/LSOCK_Acceptor.h"                             
#include "ace/LSOCK_Stream.h"
#include "ace/UNIX_Addr.h"                                       

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

int 
main (int argc, char *argv[])
{                                                                
  char *rendezvous = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;

  // Create a server address.
  ACE_UNIX_Addr server_addr (rendezvous);

  ACE_LSOCK_Acceptor peer_acceptor;

  ACE_OS::unlink (rendezvous);

  // Create a server.

  if (peer_acceptor.open (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);

  // Keep these guys out here to prevent excessive constructor
  // calls...
  ACE_LSOCK_Stream new_stream;                                   
  ACE_UNIX_Addr    cli_addr;

  ACE_DEBUG ((LM_DEBUG, "starting server %s\n", 
	     server_addr.get_path_name ()));

  // Performs the iterative server activities.

  for (;;) 
    {
      char buf[BUFSIZ];                                          
      ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);
                                                                     
      // Create a new ACE_SOCK_Stream endpoint (note automatic restart
      // if errno == EINTR).

      if (peer_acceptor.accept (new_stream, &cli_addr, &timeout) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", "accept"));
	  continue;
	}          
                                                                 
      ACE_DEBUG ((LM_DEBUG, "client %s\n", 
		 cli_addr.get_path_name ()));

      // Read data from client (terminate on error).

      for (int r_bytes; 
	   (r_bytes = new_stream.recv (buf, sizeof buf)) > 0; )
        if (ACE_OS::write (ACE_STDOUT, buf, r_bytes) != r_bytes)
	  ACE_ERROR ((LM_ERROR, "%p\n", "ACE::send_n"));

      if (new_stream.send_n ("", 1) != 1)
	ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));

      // Close new endpoint (listening endpoint stays open).
      if (new_stream.close () == -1) 
	ACE_ERROR ((LM_ERROR, "%p\n", "close"));
    }

  /* NOTREACHED */
  return 0;
}
#else
int main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, 
		     "this platform does not support UNIX-domain sockets\n"), -1);
}
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
