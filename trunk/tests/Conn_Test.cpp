// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Conn_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the Acceptor and Connector
//     classes. The test forks two processes or spawns two threads
//     (depending upon the platform) and then executes client and
//     server allowing them to connect and exchange data.
//
// = AUTHOR
//    Doug Schmidt
// 
// ============================================================================

#include "ace/OS.h"
#include "ace/Thread.h"
#include "ace/Service_Config.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Connector.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "test_config.h"

class Svc_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  virtual int open (void *);
};

int
Svc_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) opening Svc_Handler\n"));
  return 0;
}

typedef ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;
typedef ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR> CONNECTOR;

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
  CONNECTOR con;
  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler;
  ACE_NEW_RETURN (svc_handler, Svc_Handler, 0);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting non-blocking connect\n"));
  // Initiate timed, non-blocking connection with server.
  
  // Attempt a non-blocking connect to the server.
  if (con.connect (svc_handler, server_addr, options) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // Send data to server (correctly handles "incomplete writes").
  
  for (char c = 'a'; c <= 'z'; c++)
    if (svc_handler->peer ().send_n (&c, 1) == -1) 
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));

  // Explicitly close the writer-side of the connection.
  if (svc_handler->peer ().close_writer () == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close_writer"));

  char buf[1];

  // Wait for handshake with server.
  if (svc_handler->peer ().recv_n (buf, 1) != 1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "recv_n"));

  // Close the connection completely.
  if (svc_handler->close () == -1) 
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

  ACCEPTOR *acceptor = (ACCEPTOR *) arg;

  // Keep these objects out here to prevent excessive constructor
  // calls...
  ACE_INET_Addr cli_addr;
  const ACE_Time_Value def_timeout (ACE_DEFAULT_TIMEOUT);
  ACE_Time_Value tv (def_timeout);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler;
  ACE_NEW_RETURN (svc_handler, Svc_Handler, 0);

  // Performs the iterative server activities.

  for (;;) 
    {
      char buf[BUFSIZ];                                     
      char t = 'a';
      int result;
      ACE_Handle_Set handle_set;

      // Create a new ACE_SOCK_Stream endpoint (note automatic restart
      // if errno == EINTR).
      
      while ((result = acceptor->accept (svc_handler, 
					 &cli_addr,
					 options)) != -1)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client %s connected from %d\n", 
		      cli_addr.get_host_name (), 
		      cli_addr.get_port_number ()));
	  
	  // Enable non-blocking I/O.
	  if (svc_handler->peer ().enable (ACE_NONBLOCK) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, 
			       "(%P|%t) %p\n", "enable"), 0);
	  
	  ACE_SOCK_Stream &new_stream = svc_handler->peer ();

	  handle_set.reset ();
	  handle_set.set_bit (new_stream.get_handle ());
	  
	  // Read data from client (terminate on error).
	  
	  for (ssize_t r_bytes; ;)
	    {
	      if (ACE_OS::select (int (new_stream.get_handle ()) + 1,
				  handle_set,
				  0, 0, 0) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, 
				   "(%P|%t) %p\n", "select"), 0);
	      
	      while ((r_bytes = new_stream.recv (buf, 1)) > 0)
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
		  if (svc_handler->close () == -1) 
		    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
		  return 0;
		}
	      else if (r_bytes == -1)
		{
		  if (errno == EWOULDBLOCK)
		    ACE_DEBUG ((LM_DEBUG, 
				"(%P|%t) no input available, going back to reading\n"));
		  else
		    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "recv_n"), 0);
		}
	    }
	}

      if (result == -1)
	{
	  if (errno == ETIMEDOUT)
	    ACE_ERROR_RETURN ((LM_DEBUG, 
			       "(%P|%t) accept timedout, shutting down\n"), 0);
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
  ACCEPTOR acceptor;

  // Create a server address.
  ACE_INET_Addr server_addr;

  // Bind listener to any port and then find out what the port was.
  if (acceptor.open ((const ACE_INET_Addr &) ACE_Addr::sap_any) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "open"));
  else 
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting server at port %d\n",
		  server_addr.get_port_number ()));

#if !defined (ACE_WIN32) && !defined (VXWORKS)
      switch (ACE_OS::fork ("child"))
	{
	case -1:
	  ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "fork failed"));
	  exit (-1);
	case 0: 
	  client (&server_addr);
	default:
	  server ((void *) &acceptor);
	  ACE_OS::wait ();
	}
#elif defined (ACE_HAS_THREADS)
      if (ACE_Service_Config::thr_mgr ()->spawn 
	  (ACE_THR_FUNC (server), 
	   (void *) &acceptor, 
	   THR_NEW_LWP | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

      if (ACE_Service_Config::thr_mgr ()->spawn 
	  (ACE_THR_FUNC (client), 
	   (void *) &server_addr, 
	   THR_NEW_LWP | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

      // Wait for the threads to exit.
      ACE_Service_Config::thr_mgr ()->wait ();
#else
      ACE_ERROR ((LM_ERROR, 
      "(%P|%t) only one thread may be run in a process on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */	
    }
}

int
main (int, char *[])
{
  ACE_START_TEST ("Conn_Test");

  spawn ();

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Message_Queue<ACE_NULL_SYNCH>;
template class ACE_Svc_Tuple<Svc_Handler>;
template class ACE_Map_Manager<int, ACE_Svc_Tuple<Svc_Handler> *, ACE_RW_Mutex>;
template class ACE_Map_Iterator<int, ACE_Svc_Tuple<Svc_Handler> *, ACE_RW_Mutex>;
template class ACE_Write_Guard<ACE_RW_Mutex>;
template class ACE_Read_Guard<ACE_RW_Mutex>;
template class ACE_Guard<ACE_RW_Mutex>;
template class ACE_Concurrency_Strategy<Svc_Handler>;
template class ACE_TSS<ACE_Dynamic>;
template class ACE_Task<ACE_Null_Mutex, ACE_Null_Condition_Mutex>;
template class ACE_Module<ACE_Null_Mutex, ACE_Null_Condition_Mutex>;
template class ACE_Thru_Task<ACE_Null_Mutex, ACE_Null_Condition_Mutex>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
