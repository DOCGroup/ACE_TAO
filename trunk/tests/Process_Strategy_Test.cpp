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
//     <ACE_File_Lock> classes.  The <ACE_Strategy_Acceptor> uses the
//     <ACE_Process_Strategy>, which forks a process-per-connection
//     and runs as a concurrent server.  This concurrent server
//     queries and increments a "counting value" in a file.
//
//     To execute this test program you can simply start the server
//     process and connect to it via telnet.  If you type "inc" and
//     "read" in the telnet window you'll exercise the functionality
//     of the server.
//
// = AUTHOR
//    Doug Schmidt and Kevin Boyle <kboyle@sanwafp.com>
// 
// ============================================================================

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Strategies_T.h"
#include "ace/Service_Config.h"
#include "test_config.h"

#if !defined (ACE_LACKS_EXEC)

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
  Counting_Service (ACE_Thread_Manager * = 0, ACE_File_Lock * = 0);
  // Constructor.

  virtual int open (void *v);
  // Hook that is used to initialize the service (called by the
  // <ACE_Strategy_Acceptor::handle_input>).

protected:
  // = Operations corresponding to requests from the client.
  virtual int read (void);
  // Execute the read operation on the file.

  virtual int inc (void);
  // Execute the increment operation on the file.

  // = Hooks called by <Reactor> and <Strategy_Acceptor>.

  virtual int handle_input (ACE_HANDLE p);
  // Hook called by the <Reactor> when data arrives from the client.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Close down the server and exit the process.

private:
  ACE_File_Lock *file_lock_;
  // Lock for the counting file.
};

typedef ACE_Strategy_Acceptor <Counting_Service, ACE_SOCK_ACCEPTOR> ACCEPTOR;

class Counting_Acceptor : public ACCEPTOR
  // = TITLE
  //     Factory that accepts the connection and creates the
  //     <Counting_Service> handler.  This illustrates both the
  //     Strategy pattern as a means to configure the
  //     <ACE_Strategy_Acceptor> (i.e., via the
  //     <ACE_Process_Strategy>), as well as the Template Method
  //     pattern (via the <make_svc_handler> method).
{
  friend class Counting_Service;
public:
  Counting_Acceptor (LPCTSTR filename);
  // Constructor.

  virtual int make_svc_handler (Counting_Service *&);
  // Factory Method that makes a new <Counting_Service> in response to
  // a connection from a client.

  virtual int open (const ACE_INET_Addr &,
		    ACE_Reactor * = ACE_Service_Config::reactor ());
  // Initialize the Acceptor activation strategy.

  virtual int handle_signal (int, siginfo_t *, ucontext_t *);
  // Catch the SIGCHLD signal and reap the exiting child processes.

private:
  ACE_File_Lock file_lock_;
  // Lock for the counting file.

  ACE_Process_Strategy<Counting_Service> proc_strategy_;
  // Activation strategy that forks a new process for each client
  // connection.
};

// Factory Method that creates a new <Counting_Service>.

int
Counting_Acceptor::make_svc_handler (Counting_Service *&svc)
{
  ACE_NEW_RETURN (svc, Counting_Service (0, &this->file_lock_), -1);
  return 0;
}

// Reap child processes.

int
Counting_Acceptor::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) signal %S\n", signum));
  
  pid_t pid;

  while ((pid = ACE_OS::waitpid (-1, 0, WNOHANG)) > 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) reaping child %d\n", pid));

  return 0;
}

Counting_Acceptor::Counting_Acceptor (LPCTSTR filename)
  : file_lock_ (filename, 
		O_RDWR | O_CREAT, 
		ACE_DEFAULT_FILE_PERMS)
{
  // Note that this object lives beyond the lifetime of the Acceptor.

  ACE_DEBUG ((LM_DEBUG, 
	      "(%P|%t) opening %s on handle %d.\n",
	      filename, this->file_lock_.get_handle ()));

  int count = 0;

  // Store the initial value of the count in the file.
  if (ACE_OS::write (this->file_lock_.get_handle (),
		     (const void *) &count,
		     sizeof count) != sizeof count)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "write"));
}

int
Counting_Acceptor::open (const ACE_INET_Addr &addr,
			 ACE_Reactor *reactor)
{
  // Initialize the Concurrency strategy.
  if (this->proc_strategy_.open (1, this, reactor) == -1)
    return -1;

  // Open the <Strategy_Acceptor>.
  else if (ACCEPTOR::open (addr,
		      reactor,
		      0,
		      0,
		      &this->proc_strategy_) == -1)
    return -1;

  // Register to handle <SIGCHLD> when a child exits.
  else if (reactor->register_handler (SIGCHLD, this) == -1)
    return -1;
  else
    {
      ACE_INET_Addr my_addr;

      // Find out what port we're really connected to.
      if (this->acceptor ().get_local_addr (my_addr) == -1)
	return -1;
      else
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) port = %d, handle = %d\n", 
		    my_addr.get_port_number (),
		    this->get_handle ()));
      return 0;
    }
}

Counting_Service::Counting_Service (ACE_Thread_Manager *, 
				    ACE_File_Lock *file_lock)
  : file_lock_ (file_lock)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) creating the Counting_Service\n"));
}

// Read the current value from the shared file and return it to the
// client.

int
Counting_Service::read (void)
{
  ACE_READ_GUARD_RETURN (ACE_File_Lock, ace_mon, *this->file_lock_, -1);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) reading on handle %d.\n",
	      this->file_lock_->get_handle ()));

  int count;
  if (ACE_OS::pread (this->file_lock_->get_handle (), 
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
  ACE_WRITE_GUARD_RETURN (ACE_File_Lock, ace_mon, *this->file_lock_, -1);

  ACE_DEBUG ((LM_DEBUG, 
	      "(%P|%t) incrementing on handle %d.\n",
	      this->file_lock_->get_handle ()));

  int count;
  if (ACE_OS::pread (this->file_lock_->get_handle (),
		     (void *) &count,
		     sizeof count,
		     0) != sizeof count)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "read"), -1);

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) incrementing count from %d to %d\n",
	      count, count + 1));
  count++;

  if (ACE_OS::pwrite (this->file_lock_->get_handle (), 
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

  ssize_t bytes = this->peer ().recv (buf, sizeof buf);

  if (bytes <= 0 || buf[0] == EOF)
    return -1;
  else
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%P|%t) input on %d is %*s\n",
		  this->peer ().get_handle (), bytes, buf));

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
Counting_Service::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%P|%t) opening service\n"));

  // Register the new handle with the reactor and look for new input
  // in the handle_input routine.

  if (ACE_Service_Config::reactor ()->register_handler 
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "(%P|%t) register_handler"), -1);

  // We need to rerun the event loop here since we ended up here due
  // to being fork'd and we can't just return to our context since
  // it's in the wrong location in the process.
  ACE_Service_Config::run_reactor_event_loop ();

  /* NOTREACHED */
  return 0;
}

// Hook called when the child is going to exit.

int 
Counting_Service::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%P|%t) About to exit from the child\n"));

  // Exit the child.
  ACE_OS::exit (0);
  return 0;
}
#endif /* !ACE_LACKS_EXEC */

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Process_Stratey_Test");

#if !defined (ACE_LACKS_EXEC)
  ACE_Service_Config svc_conf;

  // Initialize the counting file.
  Counting_Acceptor counting_acceptor (__TEXT ("counting_file"));
  
  u_short port = ACE_DEFAULT_SERVER_PORT;

  if (argc > 1)
    port = ACE_OS::atoi (argv[1]);

  ACE_INET_Addr addr (port);

  // Open the Acceptor.
  if (counting_acceptor.open (addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);

  // Run the event loop.

  ACE_Service_Config::run_reactor_event_loop ();

#else
  ACE_ERROR ((LM_ERROR, "fork() not supported on this platform\n"));
#endif /* !ACE_LACKS_EXEC */
  ACE_END_TEST;
  return 0;
}


#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Accept_Strategy<Counting_Service, ACE_SOCK_ACCEPTOR>;
template class ACE_Acceptor<Counting_Service, ACE_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<Counting_Service>;
template class ACE_Concurrency_Strategy<Counting_Service>;
template class ACE_Guard<ACE_File_Lock>;
template class ACE_Message_Queue<ACE_NULL_SYNCH>;
template class ACE_Module<ACE_NULL_SYNCH>;
template class ACE_Process_Strategy<Counting_Service>;
template class ACE_Read_Guard<ACE_File_Lock>;
template class ACE_Scheduling_Strategy<Counting_Service>;
template class ACE_Strategy_Acceptor<Counting_Service, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_TSS<ACE_Dynamic>;
template class ACE_Task<ACE_NULL_SYNCH>;
template class ACE_Thru_Task<ACE_NULL_SYNCH>;
template class ACE_Write_Guard<ACE_File_Lock>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
