#include "ace/Log_Msg.h"
// $Id$

#include "ace/TLI_Acceptor.h"                             
#include "ace/INET_Addr.h"                                       

#if defined (ACE_HAS_TLI)
/* ACE_TLI Server */

int 
main (int argc, char *argv[])
{                                                                
  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;
  ACE_Time_Value timeout (argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_TIMEOUT);

  /* Create a server address. */
  ACE_INET_Addr addr (port); 

  /* Create a server, reuse the addr. */
  ACE_TLI_Acceptor peer_acceptor;

  if (peer_acceptor.open (addr, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  ACE_TLI_Stream new_stream;                                   

  ACE_DEBUG ((LM_DEBUG, "starting server at port %d\n", 
	     addr.get_port_number ()));

  /* Performs the iterative server activities */

  for (;;) 
    {
      char buf[BUFSIZ];                                          
                                                                     
      /* Create a new ACE_TLI_Stream endpoint (note automatic restart
	 if errno == EINTR) */
      if (peer_acceptor.accept (new_stream, 
				&addr, &timeout) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", "accept"));
	  continue;
	}          
                                                                 
      ACE_DEBUG ((LM_DEBUG, "client %s connected from %d\n", 
		 addr.get_host_name (), addr.get_port_number ()));

      // Read data from client (terminate on error).

      for (int r_bytes; 
	   (r_bytes = new_stream.recv (buf, sizeof buf)) > 0; )
        if (ACE_OS::write (ACE_STDOUT, buf, r_bytes) != r_bytes)
	  ACE_ERROR ((LM_ERROR, "%p\n", "ACE::send_n"));

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
  ACE_ERROR_RETURN ((LM_ERROR, "your platform must support ACE_TLI\n"), 1);
}
#endif /* ACE_HAS_TLI */
