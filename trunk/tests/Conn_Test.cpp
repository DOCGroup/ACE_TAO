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
#include "ace/LOCK_SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/Strategies.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "Conn_Test.h"

// Default number of clients/servers.
static int n_servers = 4;
static int n_clients = 10;
static int n_client_iterations = 2;

Svc_Handler::Svc_Handler (ACE_Thread_Manager *)
{
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
Svc_Handler::recycle (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) recycling Svc_Handler %d with handle %d\n",
              this, this->peer ().get_handle ()));
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
ACE_Hash_Addr<ACE_INET_Addr>::hash_i (const ACE_INET_Addr &addr) const
{
  return addr.get_ip_address () + addr.get_port_number ();
}

// ****************************************

// The following works around bugs with some operating systems, which
// don't allow multiple threads/process to call accept() on the same
// listen-mode port/socket.
#if defined (ACE_HAS_THREAD_SAFE_ACCEPT)
typedef ACE_Null_Mutex ACCEPTOR_LOCKING;
#elif (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
typedef ACE_Thread_Mutex ACCEPTOR_LOCKING;
#else
typedef ACE_Process_Mutex ACCEPTOR_LOCKING;
#endif /* ACE_HAS_THREAD_SAFE_ACCEPT */

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define LOCK_SOCK_ACCEPTOR ACE_LOCK_SOCK_Acceptor<ACCEPTOR_LOCKING>
#else
#define LOCK_SOCK_ACCEPTOR ACE_LOCK_SOCK_Acceptor<ACCEPTOR_LOCKING>, ACE_INET_Addr
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

typedef ACE_Oneshot_Acceptor<Svc_Handler,
                             LOCK_SOCK_ACCEPTOR>
        ACCEPTOR;
typedef ACE_Connector<Svc_Handler,
                      ACE_SOCK_CONNECTOR> 
        CONNECTOR;
typedef ACE_Strategy_Connector<Svc_Handler,
                               ACE_SOCK_CONNECTOR> 
        STRAT_CONNECTOR;
typedef ACE_NOOP_Creation_Strategy<Svc_Handler> 
        NULL_CREATION_STRATEGY;
typedef ACE_NOOP_Concurrency_Strategy<Svc_Handler> 
        NULL_ACTIVATION_STRATEGY;
typedef ACE_Cached_Connect_Strategy<Svc_Handler,
                                    ACE_SOCK_CONNECTOR,
                                    ACE_SYNCH_RW_MUTEX> 
        CACHED_CONNECT_STRATEGY;

// ****************************************

static void
timed_blocking_connect (CONNECTOR &con,
                        const ACE_INET_Addr &server_addr)
{
  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler;
  ACE_NEW (svc_handler, Svc_Handler);

  // Perform a timed-blocking connect to the server (this should
  // connect quickly since we're in the same address space or same
  // host).
  if (con.connect (svc_handler, server_addr, options) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));
  else
    {
      // Send the data to the server.
      svc_handler->send_data ();

      // Close the connection completely.
      if (svc_handler->close (1) == -1)
        ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
    }
}

static void
blocking_connect (CONNECTOR &con,
                  const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler;
  ACE_NEW (svc_handler, Svc_Handler);

  // Perform a blocking connect to the server.
  if (con.connect (svc_handler, server_addr) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));
  else
    {
      // Send the data to the server.
      svc_handler->send_data ();

      // Close the connection completely.
      if (svc_handler->close (1) == -1)
        ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
    }
}

// This function runs the more sophisticated tests involving the
// Caching_Connect_Strategy.

static void
cached_connect (STRAT_CONNECTOR &con,
                const ACE_INET_Addr &server_addr)
{
  for (int i = 0; i < n_client_iterations; i++)
    {
      Svc_Handler *svc_handler = 0;

      // Perform a blocking connect to the server using the Strategy
      // Connector with a connection caching strategy.  Since we are
      // connecting to the same <server_addr> these calls will return the
      // same dynamically allocated <Svc_Handler> for each <connect>.
      if (con.connect (svc_handler, server_addr) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "connection failed"));
          return;
        }

      // Send the data to the server.
      svc_handler->send_data ();

      // Svc_Handler is now idle, so mark it as such and let the cache
      // recycle it in another thread.
      svc_handler->idle (1);

      // Rest for a second to give another thread a chance to reuse the
      // connection.
      ACE_OS::sleep (1);
    }
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
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) **** starting timed-blocking connect\n"));
  timed_blocking_connect (*info->connector_,
                          *info->server_addr_);

#if defined (ACE_HAS_THREADS)
  // Wait for other threads to join us.
  info->barrier_->wait ();
#endif /* ACE_HAS_THREADS */

  // Run the blocking test.
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) **** starting blocking connect\n"));
  blocking_connect (*info->connector_,
                    *info->server_addr_);

#if defined (ACE_HAS_THREADS)
  // Wait for other threads to join us.
  info->barrier_->wait ();
#endif /* ACE_HAS_THREADS */

  // Run the cached blocking test.
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) **** starting cached blocking connect\n"));
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
  NULL_ACTIVATION_STRATEGY activation_strategy;
  // Configure the Strategy Connector with a strategy that caches
  // connection.
  CACHED_CONNECT_STRATEGY caching_connect_strategy;

  STRAT_CONNECTOR strat_connector (0,
                                   &creation_strategy,
                                   &caching_connect_strategy,
                                   &activation_strategy);
  Client_Info info;
  info.server_addr_ = &server_addr;
  info.connector_ = &connector;
  info.strat_connector_ = &strat_connector;

#if defined (ACE_HAS_THREADS)
  int n_threads = n_clients;
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
  const ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);
  ACE_UNUSED_ARG (options);

  Svc_Handler *svc_handler;
  ACE_NEW_RETURN (svc_handler, Svc_Handler, 0);

  // Keep looping until we timeout on accept() or fail.

  for (;;)
    {
      // Create a new <Svc_Handler> to consume the data.

      int result = acceptor->accept (svc_handler,
                                     &cli_addr
// Timing out is the only way for threads to stop accepting, since we
// don't have signals
#if defined (ACE_WIN32) || defined (VXWORKS)
                                     , options
#endif /* ACE_WIN32 || VXWORKS */
                                     );

      if (result == -1)
        {
          svc_handler->close ();
          ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n",
                             "accept failed, shutting down"),
                            0);
        }
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client %s connected from %d\n",
                  cli_addr.get_host_name (),
                  cli_addr.get_port_number ()));

      svc_handler->recv_data ();
    }

  if (svc_handler->close () == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));

  return 0;
}

#if !defined (ACE_WIN32) && !defined (VXWORKS)
static void
handler (int signum)
{
  // Print the signal number and exit.
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %S\n", signum));
  ACE_OS::exit (0);
}

// Spawn threads.

static int
spawn_processes (ACCEPTOR *acceptor,
                 ACE_INET_Addr *server_addr)
{
  pid_t *children_ptr;
  ACE_NEW_RETURN (children_ptr, pid_t[n_servers], -1);
  ACE_Auto_Basic_Array_Ptr<pid_t> children (children_ptr);
  int i;

  // Spawn off a number of server processes all of which will listen
  // on the same port number for clients to connect.
  for (i = 0; i < n_servers; i++)
    {
      pid_t pid = ACE_OS::fork ("child");
      switch (pid)
        {
        case -1:
          ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "fork failed"));
          ACE_OS::exit (-1);
          /* NOTREACHED */
        case 0:             // In the child.
          {
            // Register a signal handler to close down the child.
            ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGTERM);
            ACE_UNUSED_ARG (sa);

            server ((void *) acceptor);
            return 0;
            /* NOTREACHED */
          }
        default:          // In the parent.
          children[i] = pid;
          break;
        }
    }

  client ((void *) server_addr);

  for (i = 0; i < n_servers; i++)
    // Shutdown the servers.
    if (ACE_OS::kill (children[i], SIGTERM) == -1)
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p for %d\n", children[i]));

  pid_t child;

  do
    {
      child = ACE_OS::wait ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) reaping %d\n", child));
    }
  while (child != -1);
  
  // Remove the lock so we don't have process semaphores lying around.
  return acceptor->acceptor ().lock ().remove ();
}
#endif /* ! ACE_WIN32 ! VXWORKS */

#if (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
// Spawn threads and run the client and server.

static void
spawn_threads (ACCEPTOR *acceptor,
               ACE_INET_Addr *server_addr)
{
  if (ACE_Thread_Manager::instance ()->spawn_n
      (n_servers,
       ACE_THR_FUNC (server),
       (void *) acceptor,
       THR_NEW_LWP) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (client),
       (void *) server_addr,
       THR_NEW_LWP) == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "thread create failed"));

  // Wait for the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
}
#endif /* (ACE_WIN32 || VXWORKS) && ACE_HAS_THREADS */

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Conn_Test");

  ACE_Get_Opt getopt (argc, argv, "c:i:s:", 1);
  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'c':
        n_clients = atoi (getopt.optarg);
        break;
      case 'i':
        n_client_iterations = atoi (getopt.optarg);
        break;
      case 's':
        n_servers = atoi (getopt.optarg);
        break;
      }

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
      if (spawn_processes (&acceptor, &server_addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%P|%r) %p\n", "spawn_processes"), 1);
#elif defined (ACE_HAS_THREADS)
      spawn_threads (&acceptor, &server_addr);
#else
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) only one thread may be run in a process on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */
    }

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Addr<ACE_INET_Addr>;
template class ACE_NOOP_Creation_Strategy<Svc_Handler>;
template class ACE_Concurrency_Strategy<Svc_Handler>;
template class ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Creation_Strategy<Svc_Handler>;
template class ACE_Hash_Map_Entry<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *>;
template class ACE_Hash_Map_Iterator<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_LOCK_SOCK_Acceptor<ACCEPTOR_LOCKING> SOCK_ACCEPTOR;
template class ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_NOOP_Concurrency_Strategy<Svc_Handler>;
template class ACE_Recycling_Strategy<Svc_Handler>;
template class ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Svc_Tuple<Svc_Handler>;
template class ACE_Auto_Basic_Array_Ptr<pid_t>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Addr<ACE_INET_Addr>
#pragma instantiate ACE_NOOP_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Concurrency_Strategy<Svc_Handler>
#pragma instantiate ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Hash_Map_Entry<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr>, Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_LOCK_SOCK_Acceptor<ACCEPTOR_LOCKING> SOCK_ACCEPTOR;
#pragma instantiate ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate template class ACE_NOOP_Concurrency_Strategy<Svc_Handler>
#pragma instantiate template class ACE_Recycling_Strategy<Svc_Handler>
#pragma instantiate ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Svc_Tuple<Svc_Handler>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<pid_t>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
