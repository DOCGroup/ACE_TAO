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
//     This is a test of the <ACE_Acceptor> and <ACE_Connector>
//     classes. The test forks processes or spawns threads (depending
//     upon the platform) and then executes client and server allowing
//     them to connect and exchange data.  The test also illustrates
//     how the <ACE_Strategy_Connector> works by showing how you can
//     cache connections on the client.
//
// = AUTHOR
//    Doug Schmidt, Chris Cleeland, and Irfan Pyarali
//
// ============================================================================

#include "test_config.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/Strategies.h"
#include "Conn_Test.h"

#if defined (DEC_CXX)

int
main (int, char *[])
{
  ACE_START_TEST ("Conn_Test");

  // Conn_Test doesn't build on DEC_CXX 5.5, and doesn't run properly
  // (it runs indefinitely) on DEC_CXX 6.0.
  ACE_ERROR ((LM_ERROR, "Conn_Test not supported on this platform\n"));

  ACE_END_TEST;
  return 0;
}

#else

// Maximum number of concurrent servers.
static const int MAX_SERVERS = 4;

// Maximum number of concurrent clients.
static const int MAX_CLIENTS = 10;

Svc_Handler::Svc_Handler (ACE_Thread_Manager *)
  : in_use_ (0)
{
}

int
Svc_Handler::in_use (void)
{
  return this->in_use_;
}

void
Svc_Handler::in_use (int use)
{
  this->in_use_ = use;
}

int
Svc_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) opening Svc_Handler %d with handle %d\n",
	      this, this->peer ().get_handle ()));
  // Enable non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "enable"), -1);

  return 0;
}

int
Svc_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  // @@ Fix me!  We cannot currently call destroy: we are being
  // cached...
  return 0;
}

void
Svc_Handler::send_data (void)
{
  // Send data to server.

  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (this->peer ().send_n (c, 1) == -1)
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));
}

void
Svc_Handler::recv_data (void)
{
  ACE_SOCK_Stream &new_stream = this->peer ();

  ACE_Handle_Set handle_set;
  handle_set.set_bit (new_stream.get_handle ());

  char *t = ACE_ALPHABET;

  // Read data from client (terminate on error).

  for (ssize_t r_bytes; ;)
    {
      // Since we're in non-blocking mode we need to use <select> to
      // avoid busy waiting.
      if (ACE_OS::select (int (new_stream.get_handle ()) + 1,
			  handle_set,
			  0, 0, 0) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "select"));
      else
	{
	  char c;

	  while ((r_bytes = new_stream.recv (&c, 1)) > 0)
	    {
	      ACE_ASSERT (*t == c);

	      // We need to guard against cached connections, which
	      // will send multiple sequences of letters from 'a' ->
	      // 'z' through the same connection.
	      if (*t == 'z')
		t = ACE_ALPHABET;
	      else
		t++;
	    }

	  if (r_bytes == 0)
	    {
	      ACE_DEBUG ((LM_DEBUG,
			  "(%P|%t) reached end of input, connection closed by client\n"));

	      // Close endpoint handle (but don't close <this> yet
	      // since we're going to recycle it for the next
	      // iteration).
	      if (new_stream.close () == -1)
		ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
	      break;
	    }
	  else if (r_bytes == -1)
	    {
	      if (errno == EWOULDBLOCK)
		ACE_DEBUG ((LM_DEBUG,
			    "(%P|%t) no input available, going back to reading\n"));
	      else
		ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "recv_n"));
	    }
	}
    }
}

int
Svc_Handler::close (u_long side)
{
  // Only run this protocol if we're the write-side (i.e., "1").
  if (side == 1 && this->peer ().close () == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close_writer"));

  // Trigger the shutdown.
  return this->handle_close ();
}

// ****************************************

// Template specializations!

size_t
ACE_Hash_Addr<ACE_INET_Addr, Svc_Handler>::hash_i (const ACE_INET_Addr &addr) const
{
  return addr.get_ip_address () + addr.get_port_number ();
}

int
ACE_Hash_Addr<ACE_INET_Addr, Svc_Handler>::compare_i (const ACE_INET_Addr &a1,
						      const ACE_INET_Addr &a2) const
{
  return a1 != a2;
}

// ****************************************

typedef ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;
typedef ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR> CONNECTOR;
typedef ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR> STRAT_CONNECTOR;
typedef ACE_NOOP_Creation_Strategy<Svc_Handler> NULL_CREATION_STRATEGY;
typedef ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_Null_Mutex> CACHED_CONNECT_STRATEGY;

// ****************************************

static void
timed_blocking_connect (CONNECTOR &con,
			const ACE_INET_Addr &server_addr)
{
  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler;
  ACE_NEW (svc_handler, Svc_Handler);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) **** starting timed-blocking connect\n"));

  // Perform a timed-blocking connect to the server (this should
  // connect quickly since we're in the same address space or same
  // host).
  if (con.connect (svc_handler, server_addr, options) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));
  else
    // Send the data to the server.
    svc_handler->send_data ();

  // Close the connection completely.
  if (svc_handler->close (1) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
}

static void
blocking_connect (CONNECTOR &con,
		  const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler;
  ACE_NEW (svc_handler, Svc_Handler);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) **** starting blocking connect\n"));

  // Perform a blocking connect to the server.
  if (con.connect (svc_handler, server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // Send the data to the server.
  svc_handler->send_data ();

  // Close the connection completely.
  if (svc_handler->close (1) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
}

typedef ACE_Hash_Addr<ACE_INET_Addr, Svc_Handler> EXT_ID;
typedef Svc_Handler *INT_ID;
typedef ACE_Hash_Map_Entry<EXT_ID, INT_ID> MAP_ENTRY;

// This function runs the more sophisticated tests involving the
// Caching_Connect_Strategy.

static void
cached_connect (STRAT_CONNECTOR &con,
		const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler = 0;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) **** starting cached blocking connect\n"));

  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.  Since we are
  // connecting to the same <server_addr> these calls will return the
  // same dynamically allocated <Svc_Handler> for each <connect>.
  if (con.connect (svc_handler, server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // Send the data to the server.
  svc_handler->send_data ();

  // Mark this as being available.
  svc_handler->in_use (0);
  svc_handler = 0;

  // Try to reconnect.
  if (con.connect (svc_handler, server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // Send the data to the server.
  svc_handler->send_data ();

  // Close the connection completely.
  if (svc_handler->close (1) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
}

struct Client_Info 
  // = TITLE
  //   Information passed to the client so it can communicate with the
  //   server.
{
  ACE_INET_Addr *server_addr_;
  // Address of the server to connect with.

  CONNECTOR *connector_;
  // Connection factory.

  STRAT_CONNECTOR *strat_connector_;
  // Strategy for connecting.

#if defined (ACE_HAS_THREADS)
  ACE_Barrier *barrier_;
  // Performs barrier synchronization.
#endif /* ACE_HAS_THREADS */
};

static void *
client_connections (void *arg)
{
  Client_Info *info = (Client_Info *) arg;

  // Run the timed-blocking test.
  timed_blocking_connect (*info->connector_,
			  *info->server_addr_);

#if defined (ACE_HAS_THREADS)
  // Wait for other threads to join us.
  info->barrier_->wait ();
#endif /* ACE_HAS_THREADS */

  // Run the blocking test.
  blocking_connect (*info->connector_,
		    *info->server_addr_);

#if defined (ACE_HAS_THREADS)
  // Wait for other threads to join us.
  info->barrier_->wait ();
#endif /* ACE_HAS_THREADS */

  // Run the cached blocking test.
  cached_connect (*info->strat_connector_,
		  *info->server_addr_);

  return 0;
}

// Execute the client tests.

static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr server_addr (remote_addr->get_port_number (),
			     ACE_DEFAULT_SERVER_HOST);
  CONNECTOR connector;

  NULL_CREATION_STRATEGY creation_strategy;
  // Configure the Strategy Connector with a strategy that caches
  // connection.
  CACHED_CONNECT_STRATEGY caching_connect_strategy;

  STRAT_CONNECTOR strat_connector (0,
				   &creation_strategy,
				   &caching_connect_strategy);
  Client_Info info;
  info.server_addr_ = &server_addr;
  info.connector_ = &connector;
  info.strat_connector_ = &strat_connector;

#if defined (ACE_HAS_THREADS)
  int n_threads = MAX_CLIENTS;
  ACE_Barrier barrier (n_threads);
  info.barrier_ = &barrier;

  ACE_Thread_Manager client_manager;

  if (client_manager.spawn_n 
      (n_threads,
       ACE_THR_FUNC (client_connections),
       (void *) &info,
       THR_NEW_LWP) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));
  
  // Wait for the threads to exit.
  client_manager.wait ();

#else  /* ACE_HAS_THREADS */
  client_connections (&info);
#endif /* ACE_HAS_THREADS */
  return 0;
}

// Performs the iterative server activities.

static void *
server (void *arg)
{
  ACCEPTOR *acceptor = (ACCEPTOR *) arg;
  ACE_INET_Addr cli_addr;

  Svc_Handler *svc_handler;
  ACE_NEW_RETURN (svc_handler, Svc_Handler, 0);

  // Keep looping until we timeout on accept() or fail.

  for (;;)
    {
      // Create a new <Svc_Handler> to consume the data.

      int result = acceptor->accept (svc_handler, &cli_addr);

      if (result == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", 
			   "accept failed, shutting down"),
			  0);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client %s connected from %d\n",
		  cli_addr.get_host_name (),
		  cli_addr.get_port_number ()));

      svc_handler->recv_data ();
    }

  if (svc_handler->close () == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));

  return 0;
}

static void
handler (int signum)
{
  // Print the signal number and exit.
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %S\n", signum));
  ACE_OS::_exit (0);
}

// Spawn threads.

static void
spawn_processes (ACCEPTOR *acceptor, 
		 ACE_INET_Addr *server_addr)
{
  // Spawn off a number of server processes all of which will listen
  // on the same port number for clients to connect.
  for (int i = 0; i < MAX_SERVERS; i++)
    switch (ACE_OS::fork ("child"))
      {
      case -1:
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "fork failed"));
	ACE_OS::_exit (-1);
	/* NOTREACHED */
      case 0:
	{
	  // Register a signal handler to close down the child.
	  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGHUP);
	  ACE_UNUSED_ARG (sa);

	  // In the child.
	  server ((void *) acceptor);
	  return;
	  /* NOTREACHED */
	}
      default:
	{
	  // Ignore SIGHUP in the parent.
	  ACE_Sig_Action sa ((ACE_SignalHandler) SIG_IGN, SIGHUP);
	  ACE_UNUSED_ARG (sa);

	  // In the parent.
	  break;
	}
      }

  client ((void *) server_addr);

  // Shutdown the servers.
  ACE_OS::kill (0, SIGHUP);

  pid_t child;

  do
    {
      child = ACE_OS::wait ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) reaping %d\n", child));
    }
  while (child != -1);
}

// Spawn threads and run the client and server.

static void
spawn_threads (ACCEPTOR *acceptor,
	       ACE_INET_Addr *server_addr)
{
#if defined (ACE_HAS_THREAD_SAFE_ACCEPT)
  // The OS allows multiple threads to block in accept().
  if (ACE_Thread_Manager::instance ()->spawn_n
      (MAX_SERVERS,
       ACE_THR_FUNC (server),
       (void *) &acceptor,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));
#else
  // The OS only allow one thread to block in accept().
  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (server),
       (void *) &acceptor,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));
#endif /* ACE_HAS_THREAD_SAFE_ACCEPT */

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (client),
       (void *) &server_addr,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

  // Wait for the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
}

int
main (int, char *[])
{
  ACE_START_TEST ("Conn_Test");

  // Acceptor
  ACCEPTOR acceptor;
  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open ((const ACE_INET_Addr &) ACE_Addr::sap_any) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "open"));
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting server at port %d\n",
		  server_addr.get_port_number ()));

#if !defined (ACE_WIN32) && !defined (VXWORKS)
      spawn_processes (&acceptor, &server_addr);
#elif defined (ACE_HAS_THREADS)
      spawn_thread (&acceptor, &server_addr);
#else
      ACE_ERROR ((LM_ERROR,
		  "(%P|%t) only one thread may be run in a process on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */
    }

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_Null_Mutex>;
template class ACE_Hash_Addr<ACE_INET_Addr, Svc_Handler>;
template class ACE_NOOP_Creation_Strategy<Svc_Handler>;
template class ACE_Concurrency_Strategy<Svc_Handler>;
template class ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Creation_Strategy<Svc_Handler>;
template class ACE_Hash_Map_Entry<ACE_Hash_Addr<ACE_INET_Addr,Svc_Handler>, Svc_Handler *>;
template class ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr,Svc_Handler>, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Svc_Tuple<Svc_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Addr<ACE_INET_Addr, Svc_Handler>
#pragma instantiate ACE_NOOP_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Concurrency_Strategy<Svc_Handler>
#pragma instantiate ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Hash_Map_Entry<ACE_Hash_Addr<ACE_INET_Addr,Svc_Handler>, Svc_Handler *>
#pragma instantiate ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr,Svc_Handler>, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Svc_Tuple<Svc_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* DEC_CXX */
