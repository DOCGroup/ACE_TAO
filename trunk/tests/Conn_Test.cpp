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
//     classes. The test forks two processes or spawns two threads
//     (depending upon the platform) and then executes client and
//     server allowing them to connect and exchange data.  The test
//     also illustrates how the <ACE_Strategy_Connector> works by
//     showing how you can cache connections on the client.
//
// = AUTHOR
//    Doug Schmidt, Chris Cleeland
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

// ****************************************

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
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) opening Svc_Handler %d with handle\n",
	      this, this->peer ().get_handle ()));
  // Enable non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "enable"), -1);

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
      // Since we're in non-blocking mode we need to use <select>
      // to avoid busy waiting.
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

	      // Handshake back with client.
	      if (new_stream.send_n ("", 1) != 1)
		ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));

	      // Close endpoint handle (but don't close <this> yet since
	      // we're going to recycle it for the next iteration).
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
  if (side == 1)
    {
      // Explicitly close the writer-side of the connection.
      if (this->peer ().close_writer () == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close_writer"));

      char c;

      // Wait for handshake with server.
      if (this->peer ().recv_n (&c, 1) != 1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "recv_n"));
    }

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
nonblocking_connect (CONNECTOR &con, const ACE_INET_Addr &server_addr)
{
  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler[2];
  ACE_NEW (svc_handler[0], Svc_Handler);
  ACE_NEW (svc_handler[1], Svc_Handler);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting non-blocking connect\n"));

  // Perform a non-blocking connect to the server (this should connect
  // immediately since we're in the same address space or same host).
  if (con.connect (svc_handler[0], server_addr, options) == -1
      || con.connect (svc_handler[1], server_addr, options) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // These two handles should be different.
  ACE_ASSERT (svc_handler[0]->peer ().get_handle ()
	      != svc_handler[1]->peer ().get_handle ());

  // Send the data to the server.
  svc_handler[0]->send_data ();
  svc_handler[1]->send_data ();

  // Close the connection completely.
  if (svc_handler[0]->close (1) == -1
      || svc_handler[1]->close (1) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
}

static void
blocking_connect (CONNECTOR &con, const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler[2];
  ACE_NEW (svc_handler[0], Svc_Handler);
  ACE_NEW (svc_handler[1], Svc_Handler);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting blocking connect\n"));

  // Perform a blocking connect to the server.
  if (con.connect (svc_handler[0], server_addr) == -1
      || con.connect (svc_handler[1], server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // These two handles should be different.
  ACE_ASSERT (svc_handler[0]->peer ().get_handle ()
	      != svc_handler[1]->peer ().get_handle ());

  // Send the data to the server.
  svc_handler[0]->send_data ();
  svc_handler[1]->send_data ();

  // Close the connection completely.
  if (svc_handler[0]->close (1) == -1
      || svc_handler[1]->close (1) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
}

typedef ACE_Hash_Addr<ACE_INET_Addr, Svc_Handler> EXT_ID;
typedef Svc_Handler *INT_ID;
typedef ACE_Hash_Map_Entry<EXT_ID, INT_ID> MAP_ENTRY;

#if 0
static void
dump_map (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex> &hashmap)
{
  ACE_DEBUG ((LM_DEBUG,
	      "Dumping hash map at 0x%p (cur_size_ = %d, total_size_ = %d, table_ = 0x%p)\n",
	      &hashmap, hashmap.cur_size_, hashmap.total_size_, hashmap.table_));

  for (size_t slot = 0; slot < hashmap.total_size_; slot++)
    {
      if (hashmap.table_[slot] == hashmap.sentinel_)
	continue;

      ACE_DEBUG ((LM_DEBUG, "slot %-4d: ", slot));

      for (MAP_ENTRY *temp = hashmap.table_[slot];
	   temp != hashmap.sentinel_;
	   temp = temp->next_)
	{
	  EXT_ID& key = temp->ext_id_;
	  INT_ID& val = temp->int_id_;

	  ACE_DEBUG ((LM_DEBUG, "(%s,%d,%sin use,0x%p), ",
		      key.get_host_name (),
		      key.get_port_number (),
		      val->in_use() ? "" : "not ", val));
	}

      ACE_DEBUG ((LM_DEBUG, "SENTINEL (0x%p)\n", hashmap.sentinel_));
    }

  ACE_DEBUG ((LM_DEBUG, "end of dump\n"));
}

static void
dump (STRAT_CONNECTOR &con)
{
  CACHED_CONNECT_STRATEGY* csp =
    (CACHED_CONNECT_STRATEGY *) con.connect_strategy_;

  dump_map (csp->connection_cache_);
}
#endif /* 0 */

// This function runs the more sophisticated tests involving the
// Caching_Connect_Strategy.

static void
cached_connect (STRAT_CONNECTOR &con, const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler[2];
  svc_handler[0] = 0;
  svc_handler[1] = 0;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting cached blocking connect\n"));

  // Initiate timed, non-blocking connection with server.

  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.  Since we are
  // connecting to the same <server_addr> these calls will return the
  // same dynamically allocated <Svc_Handler> for each <connect>.
  if (con.connect (svc_handler[0], server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));
  if (con.connect (svc_handler[1], server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // These two handles must be different since we can only have open
  // one <Svc_Handler> busy at a time .
  ACE_ASSERT (svc_handler[0] != svc_handler[1]);

  // Send the data to the server.
  svc_handler[0]->send_data ();
  svc_handler[1]->send_data ();

  // Mark this as being available.
  svc_handler[0]->in_use (0);

  Svc_Handler *sh = svc_handler[0];
  svc_handler[0] = 0;

  // Try to reconnect (this should reacquire the <Svc_Handler> that we
  // just marked as no longer in use).
  if (con.connect (svc_handler[0], server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));

  // These pointers should be identical.
  ACE_ASSERT (sh == svc_handler[0]);

  // Send the data to the server.
  svc_handler[0]->send_data ();
  svc_handler[1]->send_data ();

  // Close the connection completely.
  if (svc_handler[0]->close (1) == -1
      || svc_handler[1]->close (1) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
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

  // Run the non-blocking test.
  nonblocking_connect (connector, server_addr);

  // Run the blocking test.
  blocking_connect (connector, server_addr);

  // Run the cached blocking test.
  cached_connect (strat_connector, server_addr);
  return 0;
}

// Performs the iterative server activities.

static void *
server (void *arg)
{
  ACCEPTOR *acceptor = (ACCEPTOR *) arg;
  ACE_INET_Addr cli_addr;
  const ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler;
  ACE_NEW_RETURN (svc_handler, Svc_Handler, 0);

  // Create a new <Svc_Handler> to consume the data.

  int result;
  while ((result = acceptor->accept (svc_handler,
                                     &cli_addr,
                                     options)) != -1)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client %s connected from %d\n",
		  cli_addr.get_host_name (),
		  cli_addr.get_port_number ()));

      svc_handler->recv_data ();
    }

  if (result == -1)
    {
      if (errno == ETIMEDOUT)
	ACE_ERROR ((LM_DEBUG,
		    "(%P|%t) accept timedout, shutting down\n"));
      else
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "accept"));
    }

  if (svc_handler->close () == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));

  return 0;
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
      switch (ACE_OS::fork ("child"))
	{
	case -1:
	  ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "fork failed"));
	  exit (-1);
	  /* NOTREACHED */
	case 0:
	  server ((void *) &acceptor);
	  break;
	  /* NOTREACHED */
	default:
	  client ((void *) &server_addr);
	  ACE_OS::wait ();
	  break;
	  /* NOTREACHED */
	}
#elif defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn
	  (ACE_THR_FUNC (server),
	   (void *) &acceptor,
	   THR_NEW_LWP | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

      if (ACE_Thread_Manager::instance ()->spawn
	  (ACE_THR_FUNC (client),
	   (void *) &server_addr,
	   THR_NEW_LWP | THR_DETACHED) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();
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

