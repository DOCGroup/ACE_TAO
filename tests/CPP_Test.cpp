// ============================================================================
// $Id$

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    CPP_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the ACE_SOCK_Acceptor and
//     AC_SOCK_Connector classes. The test forks two processes or
//     spawns two threads (depending upon the platform) and then executes
//     client and server allowing them to connect and exchange
//     data. No user input is required as far as command line
//     arguments are concerned.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================

#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"                             
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Handle_Set.h"
#include "test_config.h"

static char *host   = ACE_DEFAULT_SERVER_HOST;
static u_short port = ACE_DEFAULT_SERVER_PORT;
static int timeout  = ACE_DEFAULT_TIMEOUT;
// Global thread manager.
static ACE_Thread_Manager thr_mgr;

static void *
client (void *dummy)
{
#if defined (ACE_WIN32)
  // Insert thread into thr_mgr
  ACE_Thread_Control thread_control (&thr_mgr);  
  ACE_NEW_THREAD;
#endif

  char buf[BUFSIZ];

  ACE_SOCK_Stream cli_stream;
  ACE_INET_Addr   remote_addr (port, host);
  ACE_SOCK_Connector con;

  ACE_DEBUG ((LM_DEBUG, "starting non-blocking connect\n"));
  // Initiate timed, non-blocking connection with server.
  ACE_OS::sleep (3);
  
  // Attempt a non-blocking connect to the server, reusing the local
  // addr if necessary.
  if (con.connect (cli_stream, remote_addr, 
		   (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
    {
      if (errno != EWOULDBLOCK)
	ACE_ERROR ((LM_ERROR, "%p\n", "connection failed"));

      ACE_DEBUG ((LM_DEBUG, "starting timed connect\n"));

      // Check if non-blocking connection is in progress, 
      // and wait up to timeout seconds for it to complete.
      ACE_Time_Value tv (timeout);

      if (con.complete (cli_stream, &remote_addr, &tv) == -1)
	ACE_ERROR ((LM_ERROR, "%p\n", "connection failed"));
      else
	ACE_DEBUG ((LM_DEBUG, "connected to %s\n", remote_addr.get_host_name ()));
    }

  if (cli_stream.disable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "disable"));    

  // Send data to server (correctly handles "incomplete writes").
  
  for (char c = 'a'; c <= 'z'; c++)
    if (cli_stream.send_n (&c, 1) == -1) 
      ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));

  // Explicitly close the writer-side of the connection.
  if (cli_stream.close_writer () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "close_writer"));

  // Wait for handshake with server. 
  if (cli_stream.recv_n (buf, 1) != 1)
    ACE_ERROR ((LM_ERROR, "%p\n", "recv_n"));

  // Close the connection completely. 
  if (cli_stream.close () == -1) 
    ACE_ERROR ((LM_ERROR, "%p\n", "close"));

#if !defined (ACE_WIN32)
  ACE_OS::exit (0);
#endif
  return 0;
}

static void *
server (void *dummy)
{
#if defined (ACE_WIN32)
  // Insert thread into thr_mgr
  ACE_Thread_Control thread_control (&thr_mgr);  
  ACE_NEW_THREAD;
#endif
  ACE_SOCK_Acceptor peer_acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr (port);

  // Create a server, reuse the address.
  if (peer_acceptor.open (server_addr, 1) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));
  else if (peer_acceptor.enable (ACE_NONBLOCK) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "enable"));
  else if (peer_acceptor.get_local_addr (server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "get_local_addr"));

  ACE_DEBUG ((LM_DEBUG, "starting server at port %d\n",
	      server_addr.get_port_number ()));

  // Keep these objects out here to prevent excessive constructor calls...
  ACE_SOCK_Stream new_stream;                                   
  ACE_INET_Addr cli_addr;
  ACE_Handle_Set handle_set;
  ACE_Time_Value tv (timeout);
  // Performs the iterative server activities.

  for (;;) 
    {
      char buf[BUFSIZ];                                     
      int result = 0;
      char t = 'a';
      
      handle_set.reset ();
      handle_set.set_bit (peer_acceptor.get_handle ());
      
      if ((result = ACE_OS::select (int (peer_acceptor.get_handle ()) + 1,
				    handle_set,
				    0, 0, &tv)) == -1)
	ACE_ERROR ((LM_ERROR, "%p\n", "select"));
      else if (result == 0)
	{
	  ACE_DEBUG ((LM_DEBUG, "select timed out\n"));
	  continue;
	}

      // Create a new ACE_SOCK_Stream endpoint (note automatic restart
      // if errno == EINTR). 
      
      while ((result = peer_acceptor.accept (new_stream, &cli_addr)) != -1)
	{
	  ACE_DEBUG ((LM_DEBUG, "client %s connected from %d\n", 
		      cli_addr.get_host_name (), cli_addr.get_port_number ()));
	  
	  // Enable non-blocking I/O.
	  if (new_stream.enable (ACE_NONBLOCK) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n", "enable"));
	  
	  handle_set.reset ();
	  handle_set.set_bit (new_stream.get_handle ());
	  
	  // Read data from client (terminate on error).
	  
	  for (ssize_t r_bytes; ;)
	    {
	      if (ACE_OS::select (int (new_stream.get_handle ()) + 1,
				  handle_set,
				  0, 0, 0) == -1)
		{
		  ACE_ERROR ((LM_ERROR, "%p\n", "select"));
		  ACE_OS::exit (1);
		}
	      
	      while ((r_bytes = new_stream.recv_n (buf, 1)) > 0)
		{
		  ACE_ASSERT (t == buf[0]);
		  t++;
		}
	      
	      if (r_bytes == 0)
		{
		  ACE_DEBUG ((LM_DEBUG, "reached end of input, connection closed by client\n"));
		  return 0;
		}
	      else if (r_bytes == -1)
		{
		  if (errno == EWOULDBLOCK)
		    ACE_DEBUG ((LM_DEBUG, "no input available, going back to reading\n"));
		  else
		    ACE_ERROR ((LM_ERROR, "%p\n", "ACE::write"));
		}
	      
	      if (new_stream.send_n ("", 1) != 1)
		ACE_ERROR ((LM_ERROR, "%p\n", "send_n"));
	    }

	  // Close new endpoint (listening endpoint stays open).
	  if (new_stream.close () == -1) 
	    ACE_ERROR ((LM_ERROR, "%p\n", "close"));
	}

      if (result == -1)
	{
	  if (errno == EWOULDBLOCK)
	    ACE_DEBUG ((LM_DEBUG, "no connections available, going back to accepting\n"));
	  else
	    ACE_ERROR ((LM_ERROR, "%p\n", "ACE::write"));
	}
    }
  return 0;
}

void
spawn ()
{
#if !defined (ACE_WIN32)
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, "%p\n%a", "fork failed"));
      exit (-1);
    case 0: 
      client (0);
    default:
      server (0);
    }
#elif defined (ACE_HAS_THREADS)
  if (thr_mgr.spawn (ACE_THR_FUNC (client),
		     (void *) 0,
		     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));

  if (thr_mgr.spawn (ACE_THR_FUNC (server),
		     (void *) 0,
		     THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));
  thr_mgr.wait ();
#else
  ACE_ERROR ((LM_ERROR, "only one thread may be run in a process on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */	
}

int
main (int, char *argv[])
{
  ACE_START_TEST;
  ACE_DEBUG ((LM_DEBUG, "starting %s test at %u\n", argv[0], ACE_OS::time (0)));

  spawn ();

  ACE_END_TEST;
  return 0;
}

