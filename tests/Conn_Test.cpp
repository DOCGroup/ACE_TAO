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
#include "ace/Hash_Map_Manager.h"
#include "test_config.h"

// ****************************************

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //      This class is the product created by both <ACE_Connector>
  //      and <ACE_Acceptor> objects.
{
public:
  Svc_Handler (ACE_Thread_Manager * = 0);
  // Do-nothing constructor.

  virtual int open (void *);
  // Initialization hook.

  void send_data (void);
  // Send data to server.

  void recv_data (void);
  // Recv data from client.

  int close (u_long = 0);
  // Shutdown the <Svc_Handler>.

  void in_use (int);
  // Set the use flag (i.e., 1 means "in use", 0 means "not in use").

  int in_use (void);
  // Returns the current use flag.

private:
  int in_use_;
  // Are we currently in use?
};

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
  
  for (char c = 'a'; c <= 'z'; c++)
    if (this->peer ().send_n (&c, 1) == -1) 
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "send_n"));
}

void
Svc_Handler::recv_data (void)
{
  ACE_SOCK_Stream &new_stream = this->peer ();

  ACE_Handle_Set handle_set;
  handle_set.set_bit (new_stream.get_handle ());

  char t = 'a';

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
	      ACE_ASSERT (t == c);

	      // We need to guard against cached connections, which
	      // will send multiple sequences of letters from 'a' ->
	      // 'z' through the same connection.
	      if (t == 'z')
		t = 'a';
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

class Hash_Addr : public ACE_INET_Addr
  // = TITLE
  //     This class adapts an <ACE_INET_Addr> to the signature expected
  //     by the <ACE_Hash_Map_Manager>.
  //
  // = DESCRIPTION
  //     The current hash function is very simple -- it just maps the
  //     6 byte IP addr and port # into a 4 byte quantity.  There must
  //     be a better algorithm for doing this.
{
public:
  Hash_Addr (const ACE_INET_Addr & = (const ACE_INET_Addr &) ACE_Addr::sap_any,
	     Svc_Handler *sh = 0);
  // Constructor precomputes the hash value.

  size_t hash (void) const;
  // Returns the hash value.

  int operator != (const Hash_Addr &);
  // Overload the inequality operator...

private:
  size_t hash_value_;
  // Stores the precomputed hash value.

  Svc_Handler *svc_handler_;
  // Pointer to our associated <Svc_Handler>, which is used to 
  // detect "busy" <Svc_Handler>s, so we can skip over them.
};

// ****************************************

int
Hash_Addr::operator != (const Hash_Addr &rhs)
{
  if (this->svc_handler_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "host = %s and port = %d is not in use\n",
		  this->get_host_name (), 
		  this->get_port_number ()));
      // We need this test here in order to satisfy the
      // <ACE_Hash_Map_Manager>'s use of a sentinel node.  Note that
      // this will never be a real match since <svc_handler_> == 0,
      // which signifies that we've never allocated a <Svc_Handler>
      // for this address.
      return !(*this == rhs);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "host = %s and port = %d is %sin use (%d)\n",
		  this->get_host_name (), 
		  this->get_port_number (),
		  this->svc_handler_->in_use () ? "" : "not ",
		  this->svc_handler_));
      // This function returns "true" (i.e., the addresses don't
      // match) if either the associated <Svc_Handler> is in use or
      // the two addresses aren't equal.  Note that we can't use *this
      // != rhs since that would trigger a recursive call!
      return this->svc_handler_->in_use () || !(*this == rhs);
    }
}

Hash_Addr::Hash_Addr (const ACE_INET_Addr &addr,
		      Svc_Handler *sh)
  : ACE_INET_Addr (addr),
    svc_handler_ (sh)
{
  // This is a simple hash function for now -- it converts 6 bytes
  // into 4 bytes via ``wrap-around arithmetic.''  I'm sure there's a
  // better one.

  this->hash_value_ = this->get_ip_address () + this->get_port_number ();
}

size_t 
Hash_Addr::hash (void) const
{
  return this->hash_value_;
}

// ****************************************

class Creation_Strategy : public ACE_Creation_Strategy<Svc_Handler>
  // = TITLE
  //     Implements a no-op creation strategy in order to defer the
  //     decision until the <Caching_Connect_Strategy>.
{
public:
  virtual int make_svc_handler (Svc_Handler *&) { return 0; }
  // This is a no-op.
};

class Caching_Connect_Strategy : public ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>
  // = TITLE
  //     Implements a connection strategy that maintains a cache of connections.
  //
  // = DESCRIPTION
  //     All the interesting stuff in this test goes in here...
{
private:
  // = Factory method.
  virtual int connect_svc_handler (Svc_Handler *&sh,
				   const ACE_INET_Addr &remote_addr,
				   ACE_Time_Value *timeout,
				   const ACE_INET_Addr &local_addr,
				   int reuse_addr,
				   int flags,
				   int perms);
  // Checks to see if there is already a <Svc_Handler> in the cache
  // corresponding to the <remote_addr>.  If so, we return this
  // pointer.  Otherwise, we establish the connection, put it into the
  // cache, and return the <Svc_Handler> pointer.

  ACE_Hash_Map_Manager<Hash_Addr, Svc_Handler *, ACE_Null_Mutex> connection_cache_;
  // This cache maps <Hash_Addr>s to <Svc_Handler *>s that are already
  // connected.
};

int 
Caching_Connect_Strategy::connect_svc_handler (Svc_Handler *&sh,
					       const ACE_INET_Addr &remote_addr,
					       ACE_Time_Value *timeout,
					       const ACE_INET_Addr &local_addr,
					       int reuse_addr,
					       int flags,
					       int perms)
{
  Hash_Addr search_addr (remote_addr);

  // Try to find the address in the cache.  Only if we don't find it
  // do we create a new <Svc_Handler> and connect it with the server.

  if (connection_cache_.find (search_addr, sh) == -1)
    {
      ACE_NEW_RETURN (sh, Svc_Handler, -1);

      // Actively establish the connection.
      if (ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>::connect_svc_handler
	  (sh, remote_addr, timeout, local_addr, reuse_addr, flags, perms) == -1)
	return -1;

      // Insert the new Svc_Handler into the cache.      
      else
	{
	  Hash_Addr server_addr (remote_addr, sh);

	  if (this->connection_cache_.bind (server_addr, sh) == -1)
	    return -1;
	}
    }

  // Mark this as being "in use" so that we can't match it in <find>.
  sh->in_use (1);
  return 0;
}

// ****************************************

typedef ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;
typedef ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR> CONNECTOR;
typedef ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR> STRAT_CONNECTOR;

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
  if (con.connect (svc_handler[0], server_addr) == -1
      || con.connect (svc_handler[1], server_addr) == -1)
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
#if (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
  // Insert thread into thr_mgr
  ACE_Thread_Control thread_control (ACE_Service_Config::thr_mgr ());  
  ACE_NEW_THREAD;
#endif /* (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS) */

  ACE_INET_Addr *remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr server_addr (remote_addr->get_port_number (), "localhost");
  CONNECTOR connector;

  Creation_Strategy creation_strategy;
  // Configure the Strategy Connector with a strategy that caches
  // connection.
  Caching_Connect_Strategy caching_connect_strategy;

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
#if (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
  // Insert thread into thr_mgr
  ACE_Thread_Control thread_control (ACE_Service_Config::thr_mgr ());  
  ACE_NEW_THREAD;
#endif /* (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS) */

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

// Spawn off the client and server process/thread and run the tests.

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
	  /* NOTREACHED */
	case 0:
	  server ((void *) &acceptor);
	  break;
	  /* NOTREACHED */
	default:
	  client (&server_addr);
	  ACE_OS::wait ();
	  break;
	  /* NOTREACHED */
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

#if defined (ACE_MT_SAFE)
typedef ACE_RW_Mutex RW_MUTEX;
#else
typedef ACE_Null_Mutex RW_MUTEX;
#endif /* ACE_MT_SAFE */

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Concurrency_Strategy<Svc_Handler>;
template class ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Creation_Strategy<Svc_Handler>;
template class ACE_Hash_Map_Entry<Hash_Addr, Svc_Handler *>;
template class ACE_Hash_Map_Manager<Hash_Addr, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Message_Queue<ACE_NULL_SYNCH>;
template class ACE_Module<ACE_NULL_SYNCH>;
template class ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Svc_Tuple<Svc_Handler>;
template class ACE_TSS<ACE_Dynamic>;
template class ACE_Task<ACE_NULL_SYNCH>;
template class ACE_Thru_Task<ACE_NULL_SYNCH>;
template class ACE_Guard<RW_MUTEX>;
template class ACE_Map_Iterator<int, ACE_Svc_Tuple<Svc_Handler> *, RW_MUTEX>;
template class ACE_Map_Manager<int, ACE_Svc_Tuple<Svc_Handler> *, RW_MUTEX>;
template class ACE_Read_Guard<RW_MUTEX>;
template class ACE_Write_Guard<RW_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
