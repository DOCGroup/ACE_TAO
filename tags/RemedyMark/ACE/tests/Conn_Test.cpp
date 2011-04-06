
//=============================================================================
/**
 *  @file    Conn_Test.cpp
 *
 *  $Id$
 *
 *   This is a test of the <ACE_Acceptor> and <ACE_Connector>
 *   classes. The test forks processes or spawns threads (depending
 *   upon the platform) and then executes client and server allowing
 *   them to connect and exchange data.  The test also illustrates
 *   how the <ACE_Strategy_Connector> works by showing how you can
 *   cache connections on the client.
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Chris Cleeland <cleeland@cs.wustl.edu>
 *  @author and Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/SOCK_Connector.h"
#include "ace/LOCK_SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/Process_Mutex.h"
#include "ace/Signal.h"
#include "Conn_Test.h"
#include "ace/Barrier.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"



static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

// This test doesn't work well using fork() on MacOS X.  So we
// will force it to use threads instead.
#if defined (__APPLE__)
#  define ACE_LACKS_FORK
#endif /* __APPLE__ */

// The following works around bugs with some operating systems, which
// don't allow multiple threads/process to call accept() on the same
// listen-mode port/socket.  Also, note that since timed accept is
// implemented using select(), and we use timed accepts with threads,
// we need a real lock when using timed accepts even if the OS has
// thread-safe accept.
//
#if defined (ACE_LACKS_FORK)
#  if defined (ACE_HAS_THREADS)
#    include "ace/Thread_Mutex.h"
     typedef ACE_Thread_Mutex ACCEPTOR_LOCKING;
#  else
#    include "ace/Null_Mutex.h"
     typedef ACE_Null_Mutex ACCEPTOR_LOCKING;
#  endif /* ACE_HAS_THREADS */
#else
#  if defined (ACE_HAS_THREAD_SAFE_ACCEPT)
#    include "ace/Null_Mutex.h"
     typedef ACE_Null_Mutex ACCEPTOR_LOCKING;
#  else
#    include "ace/Process_Mutex.h"
     typedef ACE_Process_Mutex ACCEPTOR_LOCKING;
#  endif /* ACE_HAS_THREAD_SAFE_ACCEPT */
#endif /* ACE_LACKS_FORK */

typedef ACE_Oneshot_Acceptor<Svc_Handler,
                             ACE_LOCK_SOCK_Acceptor<ACCEPTOR_LOCKING> >
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
                                    ACE_SYNCH_MUTEX>
        CACHED_CONNECT_STRATEGY;

#define CACHED_CONNECT_STRATEGY ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_MUTEX>
#define REFCOUNTED_HASH_RECYCLABLE_ADDR ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>

// Default number of clients/servers.
#if defined (ACE_HAS_PHARLAP)
// PharLap is, by default, resource contrained. Test for something that works
// on the default configuration.
static int n_servers = 2;
static int n_clients = 4;
#else
static int n_servers = 5;
static int n_clients = 5;
#endif /* ACE_HAS_PHARLAP */

static int n_client_iterations = 3;

Svc_Handler::Svc_Handler (ACE_Thread_Manager *)
{
}

int
Svc_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) opening Svc_Handler %@ with handle %d\n"),
              this,
              this->peer ().get_handle ()));
  // Enable non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("enable")),
                      -1);
  return 0;
}

int
Svc_Handler::recycle (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) recycling Svc_Handler %@ with handle %d\n"),
              this,
              this->peer ().get_handle ()));
  return 0;
}

void
Svc_Handler::send_data (void)
{
  // Send data to server.

  for (const char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (this->peer ().send_n (c, 1) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("send_n")));
}

void
Svc_Handler::recv_data (void)
{
  ACE_SOCK_Stream &new_stream = this->peer ();

  ACE_Handle_Set handle_set;
  handle_set.set_bit (new_stream.get_handle ());

  const char *t = ACE_ALPHABET;

  // Read data from client (terminate on error).

  for (ssize_t r_bytes; ;)
    {
      // Since we're in non-blocking mode we need to use <select> to
      // avoid busy waiting.
#if defined (ACE_WIN64)
      int select_width = 0;
#else
      int select_width = int (new_stream.get_handle ()) + 1;
#endif /* ACE_WIN64 */
      if (ACE_OS::select (select_width, handle_set) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("select")));
      else
        {
          char c;

          while ((r_bytes = new_stream.recv (&c, 1)) > 0)
            {
              ACE_TEST_ASSERT (*t == c);

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
                          ACE_TEXT ("(%P|%t) reached end of input, connection closed by client\n")));

              // Close endpoint handle (but don't close <this> yet
              // since we're going to recycle it for the next
              // iteration).
              if (new_stream.close () == -1)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) %p\n"),
                            ACE_TEXT ("close")));
              break;
            }
          else if (r_bytes == -1)
            {
              if (errno == EWOULDBLOCK)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) no input available, going back to reading\n")));
              else
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) %p\n"),
                            ACE_TEXT ("recv_n")));
            }
        }
    }
}

int
Svc_Handler::close (u_long side)
{
  // Only run this protocol if we're the write-side (i.e., "1").
  if (side == 1 && this->peer ().close () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("close_writer")));
  // Trigger the shutdown.
  return this->handle_close ();
}

int
Svc_Handler::idle (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) idling Svc_Handler %@ with handle %d\n"),
              this,
              this->peer ().get_handle ()));
  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::idle (flags);
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

#if !defined (ACE_LACKS_FORK) || defined (ACE_HAS_THREADS)

static void
timed_blocking_connect (CONNECTOR &con,
                        const ACE_INET_Addr &server_addr)
{
  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler;
  ACE_NEW (svc_handler,
           Svc_Handler);

  // Perform a timed-blocking connect to the server (this should
  // connect quickly since we're in the same address space or same
  // host).
  if (con.connect (svc_handler,
                   server_addr,
                   options) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("connection failed")));
  else
    {
      // Send the data to the server.
      svc_handler->send_data ();

      // Close the connection completely.
      if (svc_handler->close (1) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("close")));
    }
}

static void
blocking_connect (CONNECTOR &con,
                  const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler;
  ACE_NEW (svc_handler,
           Svc_Handler);

  // Perform a blocking connect to the server.
  if (con.connect (svc_handler,
                   server_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("connection failed")));
  else
    {
      // Send the data to the server.
      svc_handler->send_data ();

      // Close the connection completely.
      if (svc_handler->close (1) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("close")));
    }
}

// This function runs the more sophisticated tests involving the
// Caching_Connect_Strategy.

static void
cached_connect (STRAT_CONNECTOR &con,
                const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler = 0;

  for (int i = 0; i < n_client_iterations; i++)
    {
      // Perform a blocking connect to the server using the Strategy
      // Connector with a connection caching strategy.  Since we are
      // connecting to the same <server_addr> these calls will return
      // the same dynamically allocated <Svc_Handler> for each
      // <connect>.
      if (con.connect (svc_handler,
                       server_addr) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p\n"),
                      ACE_TEXT ("connection failed")));
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

static void *
client_connections (void *arg)
{
  Client_Info *info = (Client_Info *) arg;

  // Run the timed-blocking test.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) **** starting timed-blocking connect\n")));
  timed_blocking_connect (*info->connector_,
                          *info->server_addr_);

#if defined (ACE_HAS_THREADS)
  // Wait for other threads to join us.
  info->barrier_->wait ();
#endif /* ACE_HAS_THREADS */

  // Run the blocking test.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) **** starting blocking connect\n")));
  blocking_connect (*info->connector_,
                    *info->server_addr_);

#if defined (ACE_HAS_THREADS)
  // Wait for other threads to join us.
  info->barrier_->wait ();
#endif /* ACE_HAS_THREADS */

  // Run the cached blocking test.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) **** starting cached blocking connect\n")));
  cached_connect (*info->strat_connector_,
                  *info->server_addr_);
  return 0;
}

// Execute the client tests.

static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = reinterpret_cast<ACE_INET_Addr *> (arg);
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
  ACE_Barrier thread_barrier (n_threads);
  info.barrier_ = &thread_barrier;

  ACE_Thread_Manager client_manager;

  if (client_manager.spawn_n
      (n_threads,
       (ACE_THR_FUNC) client_connections,
       (void *) &info,
       THR_NEW_LWP) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n%a"),
                ACE_TEXT ("client thread spawn failed"),
                1));

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
#if defined (VXWORKS)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) server stack size is %u\n"),
              ACE_OS::thr_min_stack ()));
#endif /* VXWORKS */

  ACCEPTOR *acceptor = (ACCEPTOR *) arg;
  ACE_INET_Addr cli_addr;
  ACE_TCHAR peer_host[MAXHOSTNAMELEN];
  const ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler;
  ACE_NEW_RETURN (svc_handler,
                  Svc_Handler,
                  0);

  // Keep looping until we timeout on <accept> or fail.

  for (;;)
    {
      // Create a new <Svc_Handler> to consume the data.

#if defined (ACE_LACKS_FORK)
      int result = acceptor->accept (svc_handler,
                                     &cli_addr,
                                     options);
#else  /* ! ACE_LACKS_FORK */
      int result = acceptor->accept (svc_handler,
                                     &cli_addr);
      ACE_UNUSED_ARG (options);
#endif /* ! ACE_LACKS_FORK */

      // Timing out is the only way for threads to stop accepting
      // since we don't have signals.

      if (result == -1)
        {
          // svc_handler->close (); The ACE_Onsehot_Acceptor closed it.

          if (errno == ETIMEDOUT)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("accept timed out\n")));
              return 0;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) %p\n"),
                               ACE_TEXT ("accept failed, shutting down")),
                              0);
        }
      // Use this rather than get_host_name() to properly adjust to the
      // charset width in use.
      cli_addr.get_host_name (peer_host, MAXHOSTNAMELEN);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) client %s connected from %d\n"),
                  peer_host,
                  cli_addr.get_port_number ()));

      svc_handler->recv_data ();
    }

  ACE_NOTREACHED (return 0);
}

#endif /* !ACE_LACKS_FORK || ACE_HAS_THREADS */

#if !defined (ACE_LACKS_FORK)
static void
handler (int /* signum */)
{
  // No printout here, to be safe.  Signal handlers must not acquire
  // locks, etc.  It's not even safe to call ACE_OS::exit ()!
  ACE_OS::exit (0);
}

// Spawn threads.

static int
spawn_processes (ACCEPTOR *acceptor,
                 ACE_INET_Addr *server_addr)
{
  pid_t *children_ptr = 0;
  ACE_NEW_RETURN (children_ptr,
                  pid_t[n_servers],
                  -1);
  ACE_Auto_Basic_Array_Ptr<pid_t> children (children_ptr);
  int i;

  // Spawn off a number of server processes all of which will listen
  // on the same port number for clients to connect.
  for (i = 0; i < n_servers; i++)
    {
      pid_t pid = ACE_OS::fork (ACE_TEXT ("child"));
      switch (pid)
        {
        case -1:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p\n%a"),
                      ACE_TEXT ("fork failed"),
                      1));
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
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p for %d\n"),
                  ACE_TEXT ("kill"), children[i]));

  pid_t child;

  do
    {
      child = ACE_OS::wait ();
      if (child != -1)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) reaping %d\n"),
                    child));
    }
  while (child != -1);

  // Remove the lock so we don't have process semaphores lying around.
  return acceptor->acceptor ().lock ().remove ();
}
#endif /* ! ACE_LACKS_FORK */

#if defined (ACE_LACKS_FORK) && defined (ACE_HAS_THREADS)
// Spawn threads and run the client and server.

static
int
spawn_threads (ACCEPTOR *acceptor,
               ACE_INET_Addr *server_addr)
{
  int status = 0;

#if 0
  // Assign thread (VxWorks task) names to test that feature.
  ACE_hthread_t *server_name = 0;
  ACE_NEW_RETURN (server_name,
                  ACE_hthread_t[n_servers],
                  -1);

  // And test ability to provide stacks.
  size_t *stack_size = 0;
  ACE_NEW_RETURN (stack_size,
                  size_t[n_servers],
                  -1);
  char **stack;
  ACE_NEW_RETURN (stack,
                  char *[n_servers],
                  -1);
  int i;

  for (i = 0; i < n_servers; ++i)
    {
      ACE_NEW_RETURN (server_name[i], ACE_TCHAR[32], -1);
      ACE_OS::sprintf (server_name[i],
                       ACE_TEXT ("server%u"),
                       i);
      stack_size[i] = 40000;
      ACE_NEW_RETURN (stack[i], char[stack_size[i]], -1);

      // Initialize the stack for checkStack.
      ACE_OS::memset (stack[i], 0xEE, stack_size[i]);
    }

  ACE_TCHAR *client_name = ACE_TEXT ("Conn client");
#endif /* ACE_HAS_VXTHREADS */

  if (ACE_Thread_Manager::instance ()->spawn_n
      (
       n_servers,
       (ACE_THR_FUNC) server,
       (void *) acceptor,
       THR_NEW_LWP
       , ACE_DEFAULT_THREAD_PRIORITY
       , -1
       , 0
#if 0
       , server_name
#if 0 /* Don't support setting of stack, because it doesn't seem to work. */
       , (void **) stack
#else
       , 0
#endif /* 0 */
       , stack_size
#endif /* ACE_HAS_VXTHREADS */
       ) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n%a"),
                ACE_TEXT ("server thread create failed"),
                1));

  if (ACE_Thread_Manager::instance ()->spawn
      ((ACE_THR_FUNC) client,
       (void *) server_addr,
       THR_NEW_LWP,
       0
#if 0
       , &client_name
#endif /* ACE_HAS_VXTHREADS */
       ) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n%a"),
                ACE_TEXT ("client thread create failed"),
                1));

  // Wait for the threads to exit.
  // But, wait for a limited time because sometimes the test hangs on Irix.
  ACE_Time_Value const max_wait (200 /* seconds */);
  ACE_Time_Value const wait_time (ACE_OS::gettimeofday () + max_wait);
  if (ACE_Thread_Manager::instance ()->wait (&wait_time) == -1)
    {
      if (errno == ETIME)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("maximum wait time of %d msec exceeded\n"),
                               max_wait.msec ()));
      else
        ACE_OS::perror (ACE_TEXT ("wait"));

      status = -1;
    }

#if 0
  for (i = 0; i < n_servers; ++i)
    {
      delete [] server_name[i];
      delete [] stack[i];
    }
  delete [] server_name;
  delete [] stack;
  delete [] stack_size;
#endif /* ACE_HAS_VXTHREADS */

  return status;
}
#endif /* ! ACE_LACKS_FORK && ACE_HAS_THREADS */

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Conn_Test"));
  int status = 0;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("c:i:s:"));
  for (int c; (c = getopt ()) != -1; )
    //FUZZ: enable check_for_lack_ACE_OS
    switch (c)
      {
      case 'c':
        n_clients = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'i':
        n_client_iterations = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 's':
        n_servers = ACE_OS::atoi (getopt.opt_arg ());
        break;
      }

  // Acceptor
  ACCEPTOR acceptor;
  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("open")));
      ACE_TEST_ASSERT (0);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) starting server at port %d\n"),
                  server_addr.get_port_number ()));

#if !defined (ACE_LACKS_FORK)
      if (spawn_processes (&acceptor,
                           &server_addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("spawn_processes")),
                          1);
#elif defined (ACE_HAS_THREADS)
      status = spawn_threads (&acceptor, &server_addr);
#else  /* ACE_LACKS_FORK && ! ACE_HAS_THREADS */
      ACE_ERROR ((LM_INFO,
                  ACE_TEXT ("(%P|%t) ")
                  ACE_TEXT ("only one thread may be run")
                  ACE_TEXT (" in a process on this platform")));
#endif /* ACE_LACKS_FORK && ! ACE_HAS_THREADS */
    }

  ACE_END_TEST;
  return status;
}
