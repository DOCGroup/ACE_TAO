// $Id$

#include "ace/Service_Config.h"
#include "ace/Thread.h"
#include "ace/Synch_T.h"

#if defined (ACE_HAS_THREADS)
#if defined (CHORUS)
// Chorus does not have signal, so we'll stop after a number of rounds.
#define MAX_ITERATIONS 3
#else
#define MAX_ITERATIONS 10000
#endif

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
  Thread_Handler (int delay, int interval, size_t n_threads);
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

  virtual int notify (ACE_Time_Value *tv = 0);
  // Perform notifications.

  virtual int svc (void);
  // Handle I/O events in a separate threads.

private:
  static void *svc_run (void *);
  // Glues C++ to C thread library functions.

  size_t id_;
  // ID passed in by Thread_Handler constructor.

  int iterations_;

  static sig_atomic_t shutdown_;
  // Shutting down.

  // = Timing variables.
  // Delay factor for timer-driven I/O.
  static ACE_Time_Value delay_;

  // Interval factor for Event_Handler timer.
  static ACE_Time_Value interval_;
};

// Shutdown flag.
sig_atomic_t Thread_Handler::shutdown_ = 0;

// Delay factor for timer-driven I/O.
ACE_Time_Value Thread_Handler::delay_;

// Interval factor for Event_Handler timer.
ACE_Time_Value Thread_Handler::interval_;


Thread_Handler::Thread_Handler (int delay, 
				int interval,
				size_t n_threads)
    : iterations_(MAX_ITERATIONS)
{
  delay_.set (delay);
  interval_.set (interval);

#if !defined(CHORUS)
  ACE_Sig_Set sig_set;
  
  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGINT);
#endif

  this->id_ = 0;

  if (ACE::register_stdin_handler (this,
				   ACE_Service_Config::reactor (),
				   ACE_Service_Config::thr_mgr ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));

#if !defined(CHORUS)
  else if (ACE_Service_Config::reactor ()->register_handler (sig_set, this) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "register_handler"));
#endif

  else if (ACE_Service_Config::reactor ()->schedule_timer 
      (this, 0, Thread_Handler::delay_, Thread_Handler::interval_) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "schedule_timer"));
   
  // Set up this thread's signal mask, which is inherited by the
  // threads it spawns.

#if !defined(CHORUS)
  ACE_Thread::sigsetmask (SIG_BLOCK, sig_set);
#endif

  // Create N new threads of control Thread_Handlers.

  for (size_t i = 0; i < n_threads; i++)
    if (ACE_Thread::spawn (&Thread_Handler::svc_run, 
			   new Thread_Handler (i + 1),
			   THR_NEW_LWP | THR_DETACHED) != 0)
      ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Thread::spawn"));

  // Unblock signal set so that only this thread receives them!
#if !defined(CHORUS)
  ACE_Thread::sigsetmask (SIG_UNBLOCK, sig_set);
#endif
}

int
Thread_Handler::notify (ACE_Time_Value *timeout)
{
  // Just do something to test the ACE_Reactor's multi-thread
  // capabilities...

  if (ACE_Service_Config::reactor ()->notify 
      (this, ACE_Event_Handler::EXCEPT_MASK, timeout) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		       "notification::notify:exception"), -1);
  else if (ACE_Service_Config::reactor ()->notify 
	   (this, ACE_Event_Handler::WRITE_MASK, timeout) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		       "notification::notify:write"), -1);

  return 0;
}

// Test stdin handling (can use select to demultiplex HANDLEs)
// Input is only handled by the main thread
int
Thread_Handler::handle_input (ACE_HANDLE handle)
{
  char buf[BUFSIZ];
  ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

  if (n > 0)
  {
      ACE_DEBUG ((LM_DEBUG, "input to (%t) %*s", 
		  n, buf));

      if (--iterations_ <= 0)
      {
	  // would like to put this in handle_timeout(), but chorus
	  // clock_gettime() does not seem to work in my version!
	  ACE_Service_Config::end_reactor_event_loop();
      }
      else
      {
	  ACE_DEBUG ((LM_DEBUG, "%d more input to kill\n", 
		      iterations_));
	  // Only wait up to 10 milliseconds to notify the Reactor.
	  ACE_Time_Value timeout (0, 10 * 1000);
	  
	  if (this->notify (&timeout) == -1)
	      ACE_ERROR ((LM_DEBUG, "(%t), %p\n", 
			  "notification::handle_input:notify"));
      }
      return 0;
  }
  else
      return -1;
}

// Perform a task that will test the ACE_Reactor's multi-threading
// capabilities in separate threads.

int
Thread_Handler::svc (void)
{
  iterations_ = MAX_ITERATIONS;

  while (this->shutdown_ == 0 && --iterations_ > 0 )
    {
      if (Thread_Handler::delay_.sec () > 0)
	// Block for delay_.secs () / 2, then notify the Reactor.
	ACE_OS::sleep (Thread_Handler::delay_.sec () / 2);

      // Only wait up to 10 milliseconds to notify the Reactor.
      ACE_Time_Value timeout (0, 10 * 1000);

      if (notify (&timeout) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "notify"));
    }

  ACE_Service_Config::reactor()->remove_handler(this, ALL_EVENTS_MASK);
  ACE_DEBUG ((LM_DEBUG, "(%t) exiting svc\n"));
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
      this->shutdown_ = 1;
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
	      "(%t) exception to id %d\n", 
	      this->id_));
  return 0;
}

// Called by the ACE_Reactor when it receives a notification.

int
Thread_Handler::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) output to id %d\n", 
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
