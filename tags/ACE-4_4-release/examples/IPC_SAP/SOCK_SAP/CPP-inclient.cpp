// $Id$

// This tests the non-blocking features of the ACE_SOCK_Connector
// class.

#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
                                                        
// ACE SOCK_SAP client.
                                                        
int main (int argc, char *argv[])                       
{                                                       
  const char *host = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_HOST;
  u_short r_port = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_SERVER_PORT;
  ACE_Time_Value timeout (argc > 3 ? ACE_OS::atoi (argv[3]) : ACE_DEFAULT_TIMEOUT);
  char buf[BUFSIZ];

  ACE_SOCK_Stream cli_stream;
  ACE_INET_Addr remote_addr (r_port, host);

  ACE_SOCK_Connector con;
                                                        
  // Attempt a non-blocking connect to the server, reusing the local
  // addr if necessary.  Initiate blocking connection with server.
  ACE_DEBUG ((LM_DEBUG, "starting connect\n"));

  if (con.connect (cli_stream, remote_addr) == -1)
    {
      // Initiate timed, non-blocking connection with server.
      ACE_DEBUG ((LM_DEBUG, "starting non-blocking connect\n"));

      if (con.connect (cli_stream, remote_addr, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
	{
	  if (errno != EWOULDBLOCK)
	    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connection failed"), 1);

	  ACE_DEBUG ((LM_DEBUG, "starting timed connect\n"));

	  // Check if non-blocking connection is in progress, 
	  // and wait up to timeout seconds for it to complete.
	  
	  if (con.complete (cli_stream, &remote_addr, &timeout) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "complete failed"), 1);
	  else
	    ACE_DEBUG ((LM_DEBUG, "connected to %s\n", remote_addr.get_host_name ()));
	}
    }
  else
    ACE_DEBUG ((LM_DEBUG, "connected to %s\n", remote_addr.get_host_name ()));    

  if (cli_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "disable"), 1);    

  // Send data to server (correctly handles "incomplete writes").
  
  for (ssize_t r_bytes;
       (r_bytes = ACE_OS::read (ACE_STDIN, buf, sizeof buf)) > 0; )
    if (ACE_OS::strcmp (buf, "quit\n") == 0)
      break;
    else if (cli_stream.send (buf, r_bytes, 0, &timeout) == -1)
      {
	if (errno == ETIME)
	  ACE_DEBUG ((LM_DEBUG, "%p\n", "send_n"));
	else
	  // Breakout if we didn't fail due to a timeout.
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send_n"), -1);
      }

    // Explicitly close the writer-side of the connection.
  if (cli_stream.close_writer () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close_writer"), 1);

  // Wait for handshake with server. 
  if (cli_stream.recv_n (buf, 1) != 1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv_n"), 1);    

  // Close the connection completely. 
  if (cli_stream.close () == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 1);

  return 0;
}                                                       
