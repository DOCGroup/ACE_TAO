#include "ace/Log_Msg.h"
// @(#)notification.cpp	1.1	10/18/96

#include "ace/Service_Config.h"
#include "ace/Thread.h"

#if defined (ACE_HAS_THREADS)

class Thread_Handler : public ACE_Event_Handler
  // = TITLE
  //   Illustrate how the ACE_Reactor's thread-safe event notification 
  //   mechanism works.
  //     
  // = DESCRIPTION
  //   Handle timeouts in the main thread via the ACE_Reactor and I/O
  //   events in a separate thread.  Just before the separate I/O thread
  //   exits it notifies the ACE_Reactor in the main thread using the
  //   ACE_Reactor's notification mechanism. 
{
public:
  Thread_Handler (int delay, int interval, int n_threads);
  Thread_Handler (size_t id): id_ (id) {}

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Handle signals.

  virtual int handle_exception (ACE_HANDLE);
  // Print data from main thread.

  virtual int handle_output (ACE_HANDLE);
  // Print data from main thread.

  virtual int handle_timeout (const ACE_Time_Value &, 
			      const void *);
  // Handle timeout events in the main thread.

  virtual int handle_input (ACE_HANDLE);
  // General notification messages to the Reactor.

  virtual int notify (void);
  // Perform notifications.

  virtual int svc (void);
  // Handle I/O events in a separate threads.

private:
  static void *svc_run (void *);
  // Glues C++ to C thread library functions.

  size_t id_;
  // ID passed in by Thread_Handler constructor.

  // = Timing variables.
  // Delay factor for timer-driven I/O.
  static ACE_Time_Value delay_;

  // Interval factor for Event_Handler timer.
  static ACE_Time_Value interval_;
};

// Delay factor for timer-driven I/O.
ACE_Time_Value Thread_Handler::delay_;

// Interval factor for Event_Handler timer.
ACE_Time_Value Thread_Handler::interval_;

Thread_Handler::Thread_Handler (int delay, 
				int interval,
				int n_threads)
{
  delay_.set (delay);
  interval_.set (interval);

  ACE_Sig_Set sig_set;
  
  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGINT);

  this->id_ = 0;

  if (ACE::register_stdin_handler (this,
				   ACE_Service_Config::reactor (),
				   ACE_Service_Config::thr_mgr ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));

  else if (ACE_Service_Config::reactor ()->register_handler (sig_set, this) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "register_handler"));

  else if (ACE_Service_Config::reactor ()->schedule_timer 
      (this, 0, Thread_Handler::delay_, Thread_Handler::interval_) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "schedule_timer"));
   
  // Set up this thread's signal mask, which is inherited by the
  // threads it spawns.

  ACE_Thread::sigsetmask (SIG_BLOCK, sig_set);

  // Create N new threads of control Thread_Handlers.

  for (size_t i = 0; i < n_threads; i++)
    if (ACE_Thread::spawn (&Thread_Handler::svc_run, 
			   new Thread_Handler (i + 1),
			   THR_NEW_LWP | THR_DETACHED) != 0)
      ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Thread::spawn"));

  // Unblock signal set so that only this thread receives them!
  ACE_Thread::sigsetmask (SIG_UNBLOCK, sig_set);
}

// Test stdin handling (can use select to demultiplex HANDLEs)

int
Thread_Handler::handle_input (ACE_HANDLE handle)
{
  char buf[BUFSIZ];
  ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

  if (n > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) %*s", n, buf));
      return this->notify ();
    }
  else
    return -1;
}

int
Thread_Handler::notify (void)
{
  // Just do something to test the ACE_Reactor's multi-thread
  // capabilities...

  if (ACE_Service_Config::reactor ()->notify 
      (this, ACE_Event_Handler::EXCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "notify"), -1);

  else if (ACE_Service_Config::reactor ()->notify 
	   (this, ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "notify"), -1);

  return 0;
}

// Perform a task that will test the ACE_Reactor's multi-threading
// capabilities in separate threads.

int
Thread_Handler::svc (void)
{
  for (;;)
    {
      if (Thread_Handler::delay_.sec () > 0)
	// Block for delay_.secs () / 2, then notify the Reactor.
	ACE_OS::sleep (Thread_Handler::delay_.sec () / 2);

      this->notify ();
    }
  return 0;
}

// Test signal handling.

int 
Thread_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGINT:
    case SIGQUIT:
      ACE_ERROR ((LM_ERROR, 
		  "(%t) ******************** shutting down %n on signal %S\n", 
		  signum));
      ACE_Service_Config::end_reactor_event_loop ();
    }
  return 0;
}

int 
Thread_Handler::handle_timeout (const ACE_Time_Value &time,
				const void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) received timeout at (%u, %u)\n",
	      time.sec (), time.usec ()));
  return 0;
}

// Called by the ACE_Reactor when it receives a notification.

int
Thread_Handler::handle_exception (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) handle_exception received notification from id %d\n", 
	      this->id_));
  return 0;
}

// Called by the ACE_Reactor when it receives a notification.

int
Thread_Handler::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) handle_output received notification from id %d\n", 
	      this->id_));
  return 0;
}

// "Shim" function that integrates C thread API with C++.

void *
Thread_Handler::svc_run (void *eh)
{
  Thread_Handler *this_handler = (Thread_Handler *) eh;

  return this_handler->svc () == 0 ? 0 : (void *) -1;
}

int 
main (int argc, char *argv[])
{
  // Required to initialize the Service Configurator.
  ACE_Service_Config daemon (argv[0]);

  if (argc != 4)
    ACE_ERROR ((LM_ERROR, 
		"usage: %s delay interval n_threads%a\n",
		argv[0], 1));

  int delay = ACE_OS::atoi (argv[1]);
  int interval = ACE_OS::atoi (argv[2]);
  size_t n_threads = ACE_OS::atoi (argv[3]);

  Thread_Handler thr_handler (delay, interval, n_threads);

  ACE_Service_Config::run_reactor_event_loop ();
  ACE_DEBUG ((LM_DEBUG, "exiting from main%a\n", 1));
  return 0;
}
#else
int 
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, 
		     "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
