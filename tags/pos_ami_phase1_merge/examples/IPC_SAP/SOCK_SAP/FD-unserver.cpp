// $Id$

#include "ace/LSOCK_Acceptor.h"                             
#include "ace/LSOCK_Stream.h"
#include "ace/UNIX_Addr.h"                                       

ACE_RCSID(SOCK_SAP, FD_unserver, "$Id$")

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

// ACE_LSOCK Server

void
handle_client (ACE_LSOCK_Stream &stream)
{
  char buf[BUFSIZ];
  ACE_HANDLE handle;

  // Retrieve the socket descriptor passed from the client.

  if (stream.recv_handle (handle) == -1)
    ACE_ERROR ((LM_ERROR, "%p", "recv_handle"));
      
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ----------------------------------------\n"));
      
  // Read data from client (correctly handles incomplete reads due to
  // flow control).
      
  for (ssize_t n; 
       (n = ACE_OS::read (handle, buf, sizeof buf)) > 0; 
       )
    ACE_DEBUG ((LM_DEBUG, "%*s", n, buf));
      
  ACE_OS::sprintf (buf, "%ld", ACE_OS::getpid ());
  
  ACE_DEBUG ((LM_DEBUG, "(%s, %d) ----------------------------------------\n", buf, ACE_OS::strlen (buf)));
      
  // Tell the client to shut down.
  if (stream.send_n (buf, ACE_OS::strlen (buf)) == -1)
    ACE_ERROR ((LM_ERROR, "%p", "send"));
      
  // Close new endpoint (listening endpoint stays open).
  if (stream.close () == -1) 
    ACE_ERROR ((LM_ERROR, "%p", "close"));
}

int 
main (int argc, char *argv[])
{                                                                
  const char *rendezvous = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;
  // Create a server.
  ACE_OS::unlink (rendezvous);
  ACE_UNIX_Addr addr (rendezvous);
  ACE_LSOCK_Acceptor peer_acceptor (addr);
  ACE_LSOCK_Stream stream;
  
  // Performs the concurrent server activities.
  
  for (;;) 
    {
      // Create a new ACE_SOCK_Stream endpoint.
      if (peer_acceptor.accept (stream) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "accept"), -1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) accepted new connection\n"));
      
#if defined (VXWORKS)
      handle_client (stream);
#else
      switch (ACE_OS::fork (argv[0]))
	{
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "fork"), -1);
	  /* NOTREACHED */
	case 0:
	  ACE_LOG_MSG->sync (argv[0]);
	  handle_client (stream);
	  ACE_OS::exit (0);
	  /* NOTREACHED */
	default:
	  stream.close ();
	}
#endif /* VXWORKS */
    }
  /* NOTREACHED */
  return 0;
}
#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform doesn't not support UNIX domain sockets\n"), -1);
}
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
