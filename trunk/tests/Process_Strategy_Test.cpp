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

#include "ace/Acceptor.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Strategies_T.h"
#include "ace/Service_Config.h"
#include "ace/Singleton.h"
#include "test_config.h"

typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

class Counting_Service : public SVC_HANDLER
  // = TITLE
  //     Reads and increments the count in a shared file.
  //
  // = DESCRIPTION
  //     Objects of this class execute in a separate process as a
  //     result of the <ACE_Strategy_Acceptor> and
  //     <ACE_Process_Strategy>.
{
public:
  Counting_Service (ACE_Thread_Manager * = 0);
  // Constructor.

  virtual int open (void *v);
  // Hook that is used to initialize the service (called by the
  // <ACE_Strategy_Acceptor::handle_input> Template Method).

protected:
  // = Methods invoked via "pointer to method" table entry.
  
  virtual int svc (void);
  // Handle the THREAD case.

  // = Operations corresponding to requests from the client.
  int read (void);
  // Execute the read operation on the file.

  int inc (void);
  // Execute the increment operation on the file.

  // = Hooks called by <Reactor> and <Strategy_Acceptor>.

  virtual int handle_input (ACE_HANDLE p = ACE_INVALID_HANDLE);
  // Hook called by the <Reactor> when data arrives from the client.
};

// Define a <Strategy_Acceptor> that's parameterized by the
// <Counting_Service>.

typedef ACE_Strategy_Acceptor <Counting_Service, ACE_SOCK_ACCEPTOR> 
	ACCEPTOR;

class Options : public ACE_Event_Handler
  // = TITLE
  //     Maintains the options for this program.
{
public:
  Options (void);
  // Constructor.

  int parse_args (int argc, char *argv[]);

  enum Concurrency_Type
  {
    PROCESS,
    REACTIVE,
    THREAD
  };

  // = Get/set concurrency type.
  Concurrency_Type concurrency_type (void);
  void concurrency_type (Concurrency_Type);

  ACE_File_Lock &file_lock (void);
  // Returns the file lock.

  const char *filename (void);
  // Returns the filename.

  ACE_Concurrency_Strategy <Counting_Service> *concurrency_strategy (void);
  // Returns the concurrency strategy.

  virtual int handle_signal (int, siginfo_t *, ucontext_t *);
  // Catch the SIGCHLD signal and reap the exiting child processes.

private:
  Concurrency_Type concurrency_type_;
  // Concurrency strategy that we're running.

  ACE_File_Lock file_lock_;
  // Lock for the counting file.

  ACE_Concurrency_Strategy<Counting_Service> *concurrency_strategy_;
  // Activation strategy that either forks a new process or spawns a
  // new thread for each client connection.

  const char *filename_;
  // Name of the counting file.
};

// Create an options Singleton.
typedef ACE_Singleton<Options, ACE_Null_Mutex> OPTIONS;

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
#if !defined (ACE_LACKS_EXEC)
    concurrency_type_ (PROCESS)
#elif defined (ACE_HAS_THREADS)
    concurrency_type_ (THREAD)
#else
    concurrency_type_ (REACTIVE)
#endif /* !ACE_LACKS_EXEC */
{
}

int
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "p:c:f:");

  this->filename_ = ACE_TEMP_FILE_NAME;

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'c':
	if (ACE_OS::strcmp (get_opt.optarg, "REACTIVE") == 0)
	  OPTIONS::instance ()->concurrency_type (Options::REACTIVE);
#if !defined (ACE_LACKS_EXEC)
	else if (ACE_OS::strcmp (get_opt.optarg, "PROCESS") == 0)
	  OPTIONS::instance ()->concurrency_type (Options::PROCESS);
#endif /* !ACE_LACKS_EXEC */
#if defined (ACE_HAS_THREADS)
	else if (ACE_OS::strcmp (get_opt.optarg, "THREAD") == 0)
	  OPTIONS::instance ()->concurrency_type (Options::THREAD);
#endif /* ACE_HAS_THREADS */
	break;
      case 'f':
	this->filename_ = get_opt.optarg;
	break;
      default:
	ACE_DEBUG ((LM_DEBUG, 
		    "usage: %n [-f (filename)] [-c (concurrency strategy)]\n%a", 1));
	/* NOTREACHED */
      }

  // Initialize the file lock.
  if (this->file_lock_.open (this->filename_, 
			     O_RDWR | O_CREAT, 
			     ACE_DEFAULT_FILE_PERMS) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  // Note that this object lives beyond the lifetime of the Acceptor.

  ACE_DEBUG ((LM_DEBUG, 
	      "(%P|%t) opening %s on handle %d.\n",
	      this->filename_, this->file_lock_.get_handle ()));

  int count = 0;

  // Store the initial value of the count in the file.
  if (ACE_OS::write (this->file_lock_.get_handle (),
		     (const void *) &count,
		     sizeof count) != sizeof count)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "write"));

  // Initialize the Concurrency strategy.
  switch (this->concurrency_type_)
    {
    case Options::PROCESS:
#if !defined (ACE_LACKS_EXEC)
      ACE_NEW_RETURN (this->concurrency_strategy_, 
		      ACE_Process_Strategy<Counting_Service> 
		      (1, this, ACE_Service_Config::reactor ()),
		      -1);
      break;
#else
      ACE_ASSERT (!"PROCESS invalid on this platform");
#endif /* !defined (ACE_LACKS_EXEC) */
    case Options::THREAD:
#if defined (ACE_HAS_THREADS)
      ACE_NEW_RETURN (this->concurrency_strategy_, 
		      ACE_Thread_Strategy<Counting_Service> 
		      (ACE_Service_Config::thr_mgr (),
		       THR_NEW_LWP, 1),
		      -1);
      break;
#else
      ACE_ASSERT (!"THREAD invalid on this platform");
#endif /* !ACE_HAS_THREADS */
    case Options::REACTIVE:
      // Settle for the purely Reactive strategy.
      ACE_NEW_RETURN (this->concurrency_strategy_, 
		      ACE_Reactive_Strategy<Counting_Service> 
		      (ACE_Service_Config::reactor ()),
		      -1);
      break;
    }

#if !defined (ACE_WIN32) || !defined (VXWORKS)
  // Register to handle <SIGCHLD> when a child exits.
  if (ACE_Service_Config::reactor ()->register_handler (SIGCHLD, this) == -1)
    return -1;
#endif /* !defined (ACE_WIN32) || !defined (VXWORKS) */
  if (ACE_Service_Config::reactor ()->register_handler (SIGINT, this) == -1)
    return -1;
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

// Reap child processes.

int
Options::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) signal %S\n", signum));

  switch (signum)
    {
    case SIGCHLD:
      pid_t pid;

      while ((pid = ACE_OS::waitpid (-1, 0, WNOHANG)) > 0)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) reaping child %d\n", pid));
      break;

    case SIGINT:
      ACE_Service_Config::end_reactor_event_loop ();
      break;
    }

  return 0;
}

Counting_Service::Counting_Service (ACE_Thread_Manager *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) creating the Counting_Service\n"));
}

// Read the current value from the shared file and return it to the
// client.

int
Counting_Service::read (void)
{
  ACE_READ_GUARD_RETURN (ACE_File_Lock, ace_mon, OPTIONS::instance ()->file_lock (), -1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) reading on handle %d.\n",
	      OPTIONS::instance ()->file_lock ().get_handle ()));

  int count;
  if (ACE_OS::pread (OPTIONS::instance ()->file_lock ().get_handle (), 
		     (void *) &count,
		     sizeof count,
		     0) != sizeof count)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "read"), -1);

  char buf[BUFSIZ];

  int n = ACE_OS::sprintf (buf, "count = %d\n", count);

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) count = %d\n",
	      count));

  if (this->peer ().send_n (buf, n) != n)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "send_n"), -1);
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
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "read"), -1);

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) incrementing count from %d to %d\n",
	      count, count + 1));
  count++;

  if (ACE_OS::pwrite (OPTIONS::instance ()->file_lock ().get_handle (), 
		      (const void *) &count,
		      sizeof count,
		      0) != sizeof count)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "write"), -1);
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
  ssize_t bytes = this->peer ().recv ((void *) &len, sizeof len);

  if (bytes <= 0)
    return -1;

  bytes = this->peer ().recv (buf, len);

  if (bytes <= 0 || (int) buf[0] == EOF)
    return -1;
  else
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%P|%t) %d bytes of input on %d is %*s\n",
		  len, this->peer ().get_handle (), bytes, buf));

      // Read and return the current value in the file.
      if (ACE_OS::strncmp (buf, "read", 4) == 0)
	return this->read ();
      // Increment the current value in the file.
      else if (ACE_OS::strncmp (buf, "inc", 3) == 0)
	return this->inc ();
      else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) no match...\n"));

      return 0;
    }
}

int
Counting_Service::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling thread\n"));

  while (this->handle_input () >= 0)
    continue;

  return 0;
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
      // We need to rerun the event loop here since we ended up here due
      // to being fork'd and we can't just return to our context since
      // it's in the wrong location in the process.
      while (this->handle_input () >= 0)
	continue;

      ACE_DEBUG ((LM_DEBUG, 
		  "(%P|%t) About to exit from the child\n"));

      // Exit the child.
      ACE_OS::exit (0);
    }
  return 0;
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

  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connector;

  char buf[BUFSIZ];
  char *command;
  size_t command_len;
  size_t i;

  for (i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client iteration %d\n", i));

      if (connector.connect (stream, server_addr) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 0);

      command = "inc";
      command_len = ACE_OS::strlen (command);

      if (stream.send (4,
		       &command_len, sizeof command_len,
		       command, command_len) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 0);

      command = "read";
      command_len = ACE_OS::strlen (command);

      if (stream.send (4,
		       &command_len, sizeof command_len,
		       command, command_len) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 0);
      else if (stream.recv (buf, sizeof buf) <= 0)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), 0);

      //      ACE_DEBUG ((LM_DEBUG, 
      //		  "(%P|%t) client iteration %d, buf = %s\n", 
      //		  i, buf));

      if (stream.close () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 0);
    }

  command = "read";
  command_len = ACE_OS::strlen (command);
  int bytes_read = 0;

  if (connector.connect (stream, server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 0);
  else if (stream.send (4,
			&command_len, sizeof command_len,
			command, command_len) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), 0);
  else if ((bytes_read = stream.recv (buf, sizeof buf)) <= 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv"), 0);
  else
    {
      // Null terminate buf to avoid an uninitialized memory read in the
      // call to ACE_OS::strrchr ().
      buf [bytes_read] = '\0';

      size_t count = ACE_OS::atoi (ACE_OS::strrchr (buf, ' '));

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) count = %d\n", count));

      // Make sure that the count is correct.
      ACE_ASSERT (count == ACE_MAX_ITERATIONS);
    }

  if (stream.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), 0);

  return 0;
}

// Performs the server activities.

static void *
server (void *)
{
#if (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS)
  // Insert thread into thr_mgr
  ACE_Thread_Control thread_control (ACE_Service_Config::thr_mgr ());  
  ACE_NEW_THREAD;
#endif /* (defined (ACE_WIN32) || defined (VXWORKS)) && defined (ACE_HAS_THREADS) */

  ACE_Time_Value timeout (3);

  // Run the main event loop, but only wait for up to 3 seconds (this
  // is used to shutdown the server.
  ACE_Service_Config::run_reactor_event_loop (timeout);

  // Remove the filename.
  ACE_OS::unlink (OPTIONS::instance ()->filename ());

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Process_Strategy_Test");

  ACE_Service_Config svc_conf;

  if (OPTIONS::instance ()->parse_args (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "parse_args"), -1);

  ACCEPTOR acceptor;

  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open ((const ACE_INET_Addr &) ACE_Addr::sap_any,
		     ACE_Service_Config::reactor (),
		     0,
		     0,
		     OPTIONS::instance ()->concurrency_strategy ()) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting server at port %d\n",
		  server_addr.get_port_number ()));

#if !defined (ACE_WIN32) && !defined (VXWORKS)
      pid_t pid = ACE_OS::fork ("child");

      switch (pid)
	{
	case -1:
	  ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n%a", "fork failed"));
	  exit (-1);
	  /* NOTREACHED */
	case 0:
	  server (0);
	  break;
	  /* NOTREACHED */
	default:
	  client (&server_addr);
	  // Shutdown the server process.
	  ACE_OS::kill (pid, SIGINT);
	  ACE_OS::wait ();
	  break;
	  /* NOTREACHED */
	}
#elif defined (ACE_HAS_THREADS)
      if (ACE_Service_Config::thr_mgr ()->spawn 
	  (ACE_THR_FUNC (server), 
	   (void *) 0,
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

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
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
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
