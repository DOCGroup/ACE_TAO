// This example tests the non-blocking features of the
// ACE_SOCK_Acceptor and ACE_SOCK_Stream classes.
// $Id$


#include "ace/SOCK_Acceptor.h"                             
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Handle_Set.h"

// ACE SOCK_SAP server. 

int 
main (int argc, char *argv[])
{                                                                
  u_short port = argc > 1 
    ? ACE_OS::atoi (argv[1]) 
    : ACE_DEFAULT_SERVER_PORT;
  ACE_Time_Value timeout (argc > 2 
			  ? ACE_OS::atoi (argv[2]) 
			  : ACE_DEFAULT_TIMEOUT);
  int sleep_time = argc > 3 ? ACE_OS::atoi (argv[3]) : 0;
    
  ACE_SOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr (port);

  // Create a server, reuse the address.
  if (peer_acceptor.open (server_addr, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);
  // Set the peer acceptor into non-blocking mode.
  else if (peer_acceptor.enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enable"), 1);
  else if (peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_local_addr"), 1);

  ACE_DEBUG ((LM_DEBUG, "starting server at port %d\n",
	      server_addr.get_port_number ()));

  // Keep these objects out here to prevent excessive constructor
  // calls within the loop.
  ACE_SOCK_Stream new_stream;                                   
  ACE_INET_Addr cli_addr;
  ACE_Handle_Set handle_set;

  // Performs the iterative server activities.

  for (;;) 
    {
      char buf[BUFSIZ];                                     
                                                                     
      handle_set.reset ();
      handle_set.set_bit (peer_acceptor.get_handle ());

      int result = ACE_OS::select (int (peer_acceptor.get_handle ()) + 1,
				   handle_set,
				   0, 0, &timeout);
      if (result == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "select"), -1);
      else if (result == 0)
	ACE_DEBUG ((LM_DEBUG, "select timed out\n"));
      else
	{
	  // Create a new ACE_SOCK_Stream endpoint (note automatic restart
	  // if errno == EINTR). 
      
	  while ((result = peer_acceptor.accept (new_stream, &cli_addr)) != -1)
	    {
	      ACE_DEBUG ((LM_DEBUG, "client %s connected from %d\n", 
			  cli_addr.get_host_name (), cli_addr.get_port_number ()));
      
	      // Enable non-blocking I/O.
	      if (new_stream.enable (ACE_NONBLOCK) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enable"), -1);
	  
	      handle_set.reset ();
	      handle_set.set_bit (new_stream.get_handle ());
	  
	      // Read data from client (terminate on error).
	  
	      for (ssize_t r_bytes;;)
		{
		  // Wait to read until there's something from the client.
		  if (ACE_OS::select (int (new_stream.get_handle ()) + 1,
				      handle_set,
				      0, 0, timeout) == -1)
		    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "select"), -1);
	      
		  // Keep reading until the client shuts down.
		  for (;;)
		    {
		      // Sleep for some amount of time in order to
		      // test client flow control.
		      ACE_OS::sleep (sleep_time);

		      r_bytes = new_stream.recv (buf, sizeof buf, 0, &timeout);

		      if (r_bytes <= 0)
			{
			  if (errno == ETIME)
			    ACE_ERROR ((LM_ERROR, "%p\n", "ACE::recv"));
			  break;
			}
		      else if (ACE::write_n (ACE_STDOUT, buf, r_bytes) != r_bytes)
			ACE_ERROR ((LM_ERROR, "%p\n", "ACE::send_n"));
		    }

		  if (r_bytes == 0)
		    {
		      ACE_DEBUG ((LM_DEBUG, 
				  "reached end of input, connection closed by client\n"));

		      // Send handshake back to client to unblock it.
		      if (new_stream.send_n ("", 1) != 1)
			ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));
		      break;
		    }
		  else if (r_bytes == -1)
		    {
		      if (errno == EWOULDBLOCK || errno == ETIME)
			ACE_DEBUG ((LM_DEBUG, 
				    "no input available, going back to reading\n"));
		      else
			ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), -1);
		    }
		}

	      // Close new endpoint (listening endpoint stays open).
	      if (new_stream.close () == -1) 
		ACE_ERROR ((LM_ERROR, "%p\n", "close"));
	    }

	  if (result == -1)
	    {
	      if (errno == EWOULDBLOCK)
		ACE_DEBUG ((LM_DEBUG, 
			    "no connections available, going back to accepting\n"));
	      else
		ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE::write"), -1);
	    }
	}
    }
  /* NOTREACHED */
  return 0;
}
