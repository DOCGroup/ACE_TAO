// $Id$

#include "ace/LSOCK_Acceptor.h"                             
#include "ace/LSOCK_Stream.h"
#include "ace/UNIX_Addr.h"                                       

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

// ACE_LSOCK Server

int 
main (int argc, char *argv[])
{                                                                
  char *rendezvous = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;
  // Create a server.
  ACE_OS::unlink (rendezvous);
  ACE_UNIX_Addr addr (rendezvous);
  ACE_LSOCK_Acceptor peer_acceptor (addr);
  ACE_LSOCK_Stream new_stream;
  
  // Performs the iterative server activities.
  
  for (;;) 
    {
      char buf[BUFSIZ];
      ACE_HANDLE handle;
      
      // Create a new ACE_SOCK_Stream endpoint.
      if (peer_acceptor.accept (new_stream) == -1)
        ACE_OS::perror ("accept");                                       
      
      // Read data from client (correctly handles incomplete reads due
      // to flow control).
      
      if (new_stream.recv_handle (handle) == -1)
	ACE_OS::perror ("recv_handle"), ACE_OS::exit (1);
      
      ACE_OS::puts ("----------------------------------------");
      
      for (int n; (n = ACE_OS::read (handle, buf, sizeof buf)) > 0; )
	ACE_OS::write (ACE_STDOUT, buf, n);
      
      ACE_OS::puts ("----------------------------------------");
      
      if (new_stream.send ("yow", 3) == -1)
	ACE_OS::perror ("send"), ACE_OS::exit (1);
      
      // Close new endpoint (listening endpoint stays open).
      if (new_stream.close () == -1) 
        ACE_OS::perror ("close");
    }
  /* NOTREACHED */
  return 0;
}
#else
int
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform doesn't not support UNIX domain sockets\n"), -1);
}
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
