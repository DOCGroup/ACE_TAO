// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    SOCK_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the ACE_SOCK_Acceptor and
//     ACE_SOCK_Connector classes. The test forks two processes or
//     spawns two threads (depending upon the platform) and then
//     executes client and server allowing them to connect and
//     exchange data.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================

#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Service_Config.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"                             
#include "ace/Handle_Set.h"
#include "test_config.h"

static void *
client (void *arg)
{
#if (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
  // Insert thread into thr_mgr
  ACE_Thread_Control thread_control (ACE_Service_Config::thr_mgr ());  
  ACE_NEW_THREAD;
#endif /* (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS) */

  ACE_INET_Addr *remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr server_addr (remote_addr->get_port_number (), "localhost");
  ACE_SOCK_Stream cli_stream;
  ACE_SOCK_Connector con;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting non-blocking connect\n"));
  // Initiate timed, non-blocking connection with server.
  
  // Attempt a non-blocking connect to the server, reusing the local
  // addr if necessary.
  if (con.connect (cli_stream, server_addr,
		   (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
    {
      if (errno != EWOULDBLOCK)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting timed connect\n"));

      // Check if non-blocking connection is in progress, 
      // and wait up to ACE_DEFAULT_TIMEOUT seconds for it to complete.
      ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);

      if (con.complete (cli_stream, &server_addr, &tv) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "connection failed"), 0);
      else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) connected to %s\n", 
		    server_addr.get_host_name ()));
    }

  if (cli_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "disable"));    

  // Send data to server (correctly handles "incomplete writes").
  
  for (char c = 'a'; c <= 'z'; c++)
    if (cli_stream.send_n (&c, 1) == -1) 
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));

  // Explicitly close the writer-side of the connection.
  if (cli_stream.close_writer () == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close_writer"));

  char buf[1];

  // Wait for handshake with server.
  if (cli_stream.recv_n (buf, 1) != 1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "recv_n"));

  // Close the connection completely.
  if (cli_stream.close () == -1) 
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));

  return 0;
}

static void *
server (void *arg)
{
#if (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
  // Insert thread into thr_mgr
  ACE_Thread_Control thread_control (ACE_Service_Config::thr_mgr ());  
  ACE_NEW_THREAD;
#endif /* (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS) */

  ACE_SOCK_Acceptor *peer_acceptor = (ACE_SOCK_Acceptor *) arg;

  if (peer_acceptor->enable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "enable"));

  // Keep these objects out here to prevent excessive constructor
  // calls...
  ACE_SOCK_Stream new_stream;                                   
  ACE_INET_Addr cli_addr;
  ACE_Handle_Set handle_set;
  const ACE_Time_Value def_timeout(ACE_DEFAULT_TIMEOUT);
  ACE_Time_Value tv (def_timeout);

  // Performs the iterative server activities.

  for (;;) 
    {
      char buf[BUFSIZ];                                     
      char t = 'a';
      
      handle_set.reset ();
      handle_set.set_bit (peer_acceptor->get_handle ());
      
      int result = ACE_OS::select (int (peer_acceptor->get_handle ()) + 1,
				   handle_set,
				   0, 0, &tv);
      ACE_ASSERT (tv == def_timeout);

      if (result == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "select"), 0);
      else if (result == 0)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%P|%t) select timed out, shutting down\n"));
	  return 0;
	}

      // Create a new ACE_SOCK_Stream endpoint (note automatic restart
      // if errno == EINTR).
      
      while ((result = peer_acceptor->accept (new_stream, &cli_addr)) != -1)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client %s connected from %d\n", 
		      cli_addr.get_host_name (), cli_addr.get_port_number ()));
	  
	  // Enable non-blocking I/O.
	  if (new_stream.enable (ACE_NONBLOCK) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "enable"), 0);
	  
	  handle_set.reset ();
	  handle_set.set_bit (new_stream.get_handle ());
	  
	  // Read data from client (terminate on error).
	  
	  for (ssize_t r_bytes; ;)
	    {
	      if (ACE_OS::select (int (new_stream.get_handle ()) + 1,
				  handle_set,
				  0, 0, 0) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "select"), 0);
	      
	      while ((r_bytes = new_stream.recv_n (buf, 1)) > 0)
		{
		  ACE_ASSERT (t == buf[0]);
		  t++;
		}
	      
	      if (r_bytes == 0)
		{
		  ACE_DEBUG ((LM_DEBUG, 
			      "(%P|%t) reached end of input, connection closed by client\n"));

		  // Handshake back with client.
		  if (new_stream.send_n ("", 1) != 1)
		    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));

		  // Close endpoint.
		  if (new_stream.close () == -1) 
		    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
		  return 0;
		}
	      else if (r_bytes == -1)
		{
		  if (errno == EWOULDBLOCK)
		    ACE_DEBUG ((LM_DEBUG, "(%P|%t) no input available, going back to reading\n"));
		  else
		    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "recv_n"), 0);
		}
	    }
	}

      if (result == -1)
	{
	  if (errno == EWOULDBLOCK)
	    ACE_DEBUG ((LM_DEBUG, "(%P|%t) no connections available, going back to accepting\n"));
	  else
	    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "accept"));
	}
    }
  return 0;
}

static void
spawn (void)
{
  // Acceptor
  ACE_SOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr;

  // Bind listener to any port and then find out what the port was.
  if (peer_acceptor.open (ACE_Addr::sap_any) == -1
      || peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "open"));
  else 
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting server at port %d\n",
		  server_addr.get_port_number ()));

#if !defined (ACE_WIN32) && !defined (VXWORKS)
      switch (ACE_OS::fork ())
	{
	case -1:
	  ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "fork failed"));
	  exit (-1);
	case 0: 
	  ACE_LOG_MSG->sync ("child");
	  client (&server_addr);
	default:
	  server ((void *) &peer_acceptor);
	  ACE_OS::wait ();
	}
#elif defined (ACE_HAS_THREADS)
      if (ACE_Service_Config::thr_mgr ()->spawn 
	  (ACE_THR_FUNC (server), (void *) &peer_acceptor, THR_NEW_LWP | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

      if (ACE_Service_Config::thr_mgr ()->spawn 
	  (ACE_THR_FUNC (client), (void *) &server_addr, THR_NEW_LWP | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

      // Wait for the threads to exit.
      ACE_Service_Config::thr_mgr ()->wait ();
#else
      ACE_ERROR ((LM_ERROR, "(%P|%t) only one thread may be run in a process on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */	
    }
}

int
main (int, char *[])
{
  ACE_START_TEST ("SOCK_Test");

  spawn ();

  ACE_END_TEST;
  return 0;
}
