// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Strategy_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <ACE_Strategy_Acceptor> and
//     <ACE_File_Lock> classes.  The <ACE_Strategy_Acceptor> uses
//     either the <ACE_Process_Strategy> (which forks a
//     process-per-connection and runs as a concurrent server
//     process), the <ACE_Thread_Strategy> (which spawns a
//     thread-per-connection and runs as a concurrent server thread),
//     or <ACE_Reactive_Strategy> (which register the <Svc_Handler>
//     with the <Reactor> and runs in the main thread of control as an
//     iterative server).  This server queries and increments a
//     "counting value" in a file.
//
//     This test program can be run in the following ways:
//
//     # Run the server "reactively" (i.e., iteratively)
//     % Process_Strategy_Test -c REACTIVE
//
//     # Run the server in multi-threads.
//     % Process_Strategy_Test -c THREAD
//
//     # Run the server in multi-processes
//     % Process_Strategy_Test -c PROCESS
//
// = AUTHOR
//    Doug Schmidt and Kevin Boyle <kboyle@sanwafp.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Strategies_T.h"
#include "ace/Singleton.h"

// Counting_Service and Options in here
#include "Process_Strategy_Test.h"

ACE_RCSID(tests, Process_Strategy_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

// Define a <Strategy_Acceptor> that's parameterized by the
// <Counting_Service>.

typedef ACE_Strategy_Acceptor <Counting_Service, ACE_SOCK_ACCEPTOR> ACCEPTOR;

// Create an Options Singleton.
typedef ACE_Singleton<Options, ACE_Null_Mutex> OPTIONS;

// counter for connections
static int connections = 0;

// Use this to show down the process gracefully.
static void
connection_completed (void)
{
  // Increment connection counter.
  connections++;

  // If all connections have been serviced.
  if (connections == ACE_MAX_ITERATIONS + 1)
    // Make sure that the event loop is interrupted.
    ACE_Reactor::instance()->wakeup_all_threads ();
}

// Have all connections been serviced?

static int
done (void)
{
  return connections == ACE_MAX_ITERATIONS + 1;
}

// Constructor
Process_Strategy::Process_Strategy (size_t n_processes,
                                    ACE_Event_Handler *acceptor,
                                    ACE_Reactor *r,
                                    int avoid_zombies)
  : ACE_Process_Strategy<Counting_Service> (n_processes,
                                            acceptor,
                                            r,
                                            avoid_zombies)
{
}

// Destructor.  g++ 2.7.2.3 gets very confused ("Internal compiler
// error") without it.

Process_Strategy::~Process_Strategy (void)
{
}

// Overwrite the process creation method to include connection
// counting.

int
Process_Strategy::activate_svc_handler (Counting_Service *svc_handler,
                                        void *arg)
{
  // Call down to the base class
  int result =
    ACE_Process_Strategy<Counting_Service>::activate_svc_handler (svc_handler,
                                                                  arg);
  // Connection is now complete
  connection_completed ();

  return result;
}

ACE_File_Lock &
Options::file_lock (void)
{
  return this->file_lock_;
}

ACE_Concurrency_Strategy <Counting_Service> *
Options::concurrency_strategy (void)
{
  return this->concurrency_strategy_;
}

const char *
Options::filename (void)
{
  return this->filename_;
}

Options::Options (void)
  :
  // Choose to use processes by default.
#if !defined (ACE_LACKS_FORK)
  concurrency_type_ (PROCESS)
#elif defined (ACE_HAS_THREADS)
  concurrency_type_ (THREAD)
#else
  concurrency_type_ (REACTIVE)
#endif /* !ACE_LACKS_FORK */
{
}

Options::~Options (void)
{
  delete concurrency_strategy_;
  concurrency_strategy_ = 0;
}

int
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "p:c:f:");

  this->filename_ = ACE_TEMP_FILE_NAME_A;

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'c':
        if (ACE_OS::strcmp (get_opt.optarg,
                            "REACTIVE") == 0)
          OPTIONS::instance ()->concurrency_type (Options::REACTIVE);
#if !defined (ACE_LACKS_FORK)
        else if (ACE_OS::strcmp (get_opt.optarg,
                                 "PROCESS") == 0)
          OPTIONS::instance ()->concurrency_type (Options::PROCESS);
#endif /* !ACE_LACKS_FORK */
#if defined (ACE_HAS_THREADS)
        else if (ACE_OS::strcmp (get_opt.optarg,
                                 "THREAD") == 0)
          OPTIONS::instance ()->concurrency_type (Options::THREAD);
#endif /* ACE_HAS_THREADS */
        else
          ACE_DEBUG ((LM_DEBUG,
                      "WARNING: concurrency strategy \"%s\" is not supported\n",
                      get_opt.optarg));
        break;
      case 'f':
        this->filename_ = get_opt.optarg;
        break;
      default:
        ACE_DEBUG ((LM_DEBUG,
                    "usage: %n [-f (filename)] [-c (concurrency strategy)]\n%a", 1));
        /* NOTREACHED */
      }

  // Initialize the file lock.  Note that this object lives beyond the
  // lifetime of the Acceptor.
  if (this->file_lock_.open (ACE_WIDE_STRING (this->filename_),
                             O_RDWR | O_CREAT,
                             ACE_DEFAULT_FILE_PERMS) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) opening %s on handle %d.\n",
              this->filename_,
              this->file_lock_.get_handle ()));

  int count = 0;

  // Store the initial value of the count in the file.
  if (ACE_OS::write (this->file_lock_.get_handle (),
                     (const void *) &count,
                     sizeof count) != sizeof count)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "write"));

  // Initialize the Concurrency strategy.
  switch (this->concurrency_type_)
    {
    case Options::PROCESS:
#if !defined (ACE_LACKS_FORK)
      ACE_NEW_RETURN (this->concurrency_strategy_,
                      Process_Strategy (1,
                                        this,
                                        ACE_Reactor::instance (),
                                        1), // Avoid zombies.
                      -1);
      break;
#else
      ACE_ASSERT ("PROCESS invalid on this platform" == 0);
#endif /* !defined (ACE_LACKS_FORK) */
    case Options::THREAD:
#if defined (ACE_HAS_THREADS)
      ACE_NEW_RETURN (this->concurrency_strategy_,
                      ACE_Thread_Strategy<Counting_Service>
                      (ACE_Thread_Manager::instance (),
                       THR_NEW_LWP,
                       1),
                      -1);
      break;
#else
      ACE_ASSERT (!"THREAD invalid on this platform");
#endif /* !ACE_HAS_THREADS */
    case Options::REACTIVE:
      // Settle for the purely Reactive strategy.
      ACE_NEW_RETURN (this->concurrency_strategy_,
                      ACE_Reactive_Strategy<Counting_Service>
                      (ACE_Reactor::instance ()),
                      -1);
      break;
    }

  return 0;
}

Options::Concurrency_Type
Options::concurrency_type (void)
{
  return this->concurrency_type_;
}

void
Options::concurrency_type (Options::Concurrency_Type cs)
{
  this->concurrency_type_ = cs;
}

Counting_Service::Counting_Service (ACE_Thread_Manager *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) creating the Counting_Service\n"));
}

// Read the current value from the shared file and return it to the
// client.

int
Counting_Service::read (void)
{
  ACE_READ_GUARD_RETURN (ACE_File_Lock, ace_mon, OPTIONS::instance ()->file_lock (), -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) reading on handle %d.\n",
              OPTIONS::instance ()->file_lock ().get_handle ()));

  int count;
  if (ACE_OS::pread (OPTIONS::instance ()->file_lock ().get_handle (),
                     (void *) &count,
                     sizeof count,
                     0) != sizeof count)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "read"),
                      -1);
  char buf[BUFSIZ];

  int n = ACE_OS::sprintf (buf,
                           "count = %d\n",
                           count);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) count = %d\n",
              count));

  if (this->peer ().send_n (buf, n) != n)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "send_n"),
                      -1);
  return 0;
}

// Increment the current value in the shared file by 1.

int
Counting_Service::inc (void)
{
  ACE_WRITE_GUARD_RETURN (ACE_File_Lock, ace_mon, OPTIONS::instance ()->file_lock (), -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) incrementing on handle %d.\n",
              OPTIONS::instance ()->file_lock ().get_handle ()));

  int count;
  if (ACE_OS::pread (OPTIONS::instance ()->file_lock ().get_handle (),
                     (void *) &count,
                     sizeof count,
                     0) != sizeof count)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "read"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) incrementing count from %d to %d\n",
              count,
              count + 1));
  count++;

  if (ACE_OS::pwrite (OPTIONS::instance ()->file_lock ().get_handle (),
                      (const void *) &count,
                      sizeof count,
                      0) != sizeof count)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "write"),
                      -1);
  return 0;
}

// Receive the request from the client and call the appropriate
// operation.

int
Counting_Service::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) reading from peer on %d\n",
              this->peer ().get_handle ()));

  size_t len;
  // Read the PDU length first.
  ssize_t bytes = this->peer ().recv ((void *) &len,
                                      sizeof len);

  if (bytes <= 0)
    return -1;

  bytes = this->peer ().recv (buf, len);

  if (bytes <= 0 || (int) buf[0] == EOF)
    return -1;
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) %d bytes of input on %d is %*s\n",
                  bytes,
                  this->peer ().get_handle (),
                  bytes,
                  buf));

      // Read and return the current value in the file.
      if (ACE_OS::strncmp (buf,
                           "read",
                           4) == 0)
        return this->read ();
      // Increment the current value in the file.
      else if (ACE_OS::strncmp (buf, "inc", 3) == 0)
        return this->inc ();
      else
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) no match...\n"));
      return 0;
    }
}

int
Counting_Service::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) handling thread\n"));

  while (this->handle_input () >= 0)
    continue;

  return 0;
}

int
Counting_Service::handle_close (ACE_HANDLE,
                                ACE_Reactor_Mask)
{
  // Done with another connection.
  connection_completed ();

  // Call down to base class
  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::handle_close ();
}

// This method is called back by the <Acceptor> once the client has
// connected and the process is forked or spawned.

int
Counting_Service::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) opening service\n"));

  if (OPTIONS::instance ()->concurrency_type () == Options::PROCESS)
    {
      // We need to rerun the event loop here since we ended up here
      // due to being fork'd and we can't just return to our context
      // since it's in the wrong location in the process.
      while (this->handle_input () >= 0)
        continue;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) About to exit from the child\n"));

      // Exit the child.
      ACE_OS::exit (0);
    }
  else if (OPTIONS::instance ()->concurrency_type () == Options::THREAD)
    // We need to set this to 0 so that our <shutdown> method doesn't
    // try to deregister <this> from the Reactor.
    this->reactor (0);
  return 0;
}

// Execute the client tests.

static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = (ACE_INET_Addr *) arg;
  ACE_INET_Addr server_addr (remote_addr->get_port_number (),
                             ACE_DEFAULT_SERVER_HOST);
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connector;

  char buf[BUFSIZ];
  char *command;
  size_t command_len;
  size_t i;

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) client iteration %d\n",
                  i));
      if (connector.connect (stream, server_addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "open"),
                          0);
      command = "inc";
      command_len = ACE_OS::strlen (command);

      if (stream.send (4,
                       &command_len, sizeof command_len,
                       command, command_len) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "send"),
                          0);
      command = "read";
      command_len = ACE_OS::strlen (command);

      if (stream.send (4,
                       &command_len, sizeof command_len,
                       command, command_len) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "send"),
                          0);
      else if (stream.recv (buf, sizeof buf) <= 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "recv"),
                          0);

      //      ACE_DEBUG ((LM_DEBUG,
      //                  "(%P|%t) client iteration %d, buf = %s\n",
      //                  i, buf));

      if (stream.close () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "close"),
                          0);
    }

  command = "read";
  command_len = ACE_OS::strlen (command);
  int bytes_read = 0;

  if (connector.connect (stream, server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      0);
  else if (stream.send (4,
                        &command_len, sizeof command_len,
                        command, command_len) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "send"),
                      0);
  else if ((bytes_read = stream.recv (buf, sizeof buf)) <= 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "recv"),
                      0);
  else
    {
      // Null terminate buf to avoid an uninitialized memory read in
      // the call to ACE_OS::strrchr ().
      buf [bytes_read] = '\0';

      size_t count = ACE_OS::atoi (ACE_OS::strrchr (buf, ' '));

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) count = %d\n",
                  count));
      // Make sure that the count is correct.
      ACE_ASSERT (count == ACE_MAX_ITERATIONS);
    }

  if (stream.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                      0);

  // Remove the filename.
  ACE_OS::unlink (OPTIONS::instance ()->filename ());

  return 0;
}

// Performs the server activities.

static void *
server (void *)
{
  int result = 0;
  ACE_Reactor::instance ()->owner (ACE_Thread::self ());

  while (!done () && result != -1)
    // Run the main event loop.
    result = ACE_Reactor::instance ()->handle_events ();

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Process_Strategy_Test");

  if (OPTIONS::instance ()->parse_args (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "parse_args"),
                      -1);

  ACCEPTOR acceptor;

  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  // Note that this implicitly creates the Reactor singleton.
  if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &),
                     ACE_Reactor::instance(),
                     0,
                     0,
                     OPTIONS::instance ()->concurrency_strategy ()) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) starting server at port %d\n",
                  server_addr.get_port_number ()));

#if !defined (ACE_LACKS_FORK)
      // We're running the client and serve as separate processes.
      pid_t pid = ACE::fork ("child", 
                             1); // Avoid zombies.

      switch (pid)
        {
        case -1:
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) %p\n%a",
                      "fork failed"));
          exit (-1);
          /* NOTREACHED */
        case 0:
          server (0);
          break;
          /* NOTREACHED */
        default:
          client (&server_addr);
          break;
          /* NOTREACHED */
        }
#elif defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (server),
           (void *) 0,
           THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n%a",
                    "thread create failed"));

      if (ACE_Thread_Manager::instance ()->spawn
          (ACE_THR_FUNC (client),
           (void *) &server_addr,
           THR_NEW_LWP | THR_DETACHED) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n%a",
                    "thread create failed"));

      // Wait for the threads to exit.
      ACE_Thread_Manager::instance ()->wait ();
#else
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) only one thread may be run in a process on this platform\n%a",
                  1));
#endif /* ACE_HAS_THREADS */
    }

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Accept_Strategy<Counting_Service, ACE_SOCK_ACCEPTOR>;
template class ACE_Acceptor<Counting_Service, ACE_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<Counting_Service>;
template class ACE_Concurrency_Strategy<Counting_Service>;
template class ACE_Guard<ACE_File_Lock>;
template class ACE_Singleton<Options, ACE_Null_Mutex>;
template class ACE_Process_Strategy<Counting_Service>;
template class ACE_Thread_Strategy<Counting_Service>;
template class ACE_Reactive_Strategy<Counting_Service>;
template class ACE_Read_Guard<ACE_File_Lock>;
template class ACE_Scheduling_Strategy<Counting_Service>;
template class ACE_Strategy_Acceptor<Counting_Service, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Write_Guard<ACE_File_Lock>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Accept_Strategy<Counting_Service, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Acceptor<Counting_Service, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<Counting_Service>
#pragma instantiate ACE_Concurrency_Strategy<Counting_Service>
#pragma instantiate ACE_Guard<ACE_File_Lock>
#pragma instantiate ACE_Singleton<Options, ACE_Null_Mutex>
#pragma instantiate ACE_Process_Strategy<Counting_Service>
#pragma instantiate ACE_Thread_Strategy<Counting_Service>
#pragma instantiate ACE_Reactive_Strategy<Counting_Service>
#pragma instantiate ACE_Read_Guard<ACE_File_Lock>
#pragma instantiate ACE_Scheduling_Strategy<Counting_Service>
#pragma instantiate ACE_Strategy_Acceptor<Counting_Service, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Write_Guard<ACE_File_Lock>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
