// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "ace/Thread.h"
#include "ace/Signal.h"
#include "ace/Truncate.h"



#if defined (ACE_HAS_THREADS)
#define MAX_ITERATIONS 10000

class Thread_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   Illustrate how the ACE_Reactor's thread-safe event notification
  //   mechanism works.
  //
  // = DESCRIPTION
  //   Handle timeouts in the main thread via the ACE_Reactor and I/O
  //   events in a separate thread.  Just before the separate I/O
  //   thread exits it notifies the ACE_Reactor in the main thread
  //   using the ACE_Reactor's notification mechanism.
public:
  Thread_Handler (long delay,
                  long interval,
                  size_t n_threads,
                  size_t max_iterations);
  // Constructor.

  Thread_Handler (size_t id,
                  size_t max_iterations);

  ~Thread_Handler (void);
  // Destructor.

  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0);
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

  size_t iterations_;

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

Thread_Handler::Thread_Handler (size_t id,
                                size_t max_iterations)
  : id_ (id),
    iterations_ (max_iterations)
{
}

Thread_Handler::~Thread_Handler (void)
{
  // Cleanup resources so that we don't crash and burn when shutdown.
  ACE_Event_Handler::remove_stdin_handler (ACE_Reactor::instance (),
                                           ACE_Thread_Manager::instance ());
  ACE_Reactor::instance ()->cancel_timer (this);
}

Thread_Handler::Thread_Handler (
        long delay,
        long interval,
        size_t n_threads,
        size_t max_iterations)
    : iterations_ (max_iterations)
{
  ACE_Sig_Set sig_set;

  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGINT);

  delay_.set (delay);
  interval_.set (interval);
  this->id_ = 0;

  if (ACE_Event_Handler::register_stdin_handler (this,
                                                 ACE_Reactor::instance (),
                                                 ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "register_stdin_handler"));
  else if (ACE_Reactor::instance ()->register_handler (sig_set,
                                                       this) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n",
                "register_handler"));
  else if (ACE_Reactor::instance ()->schedule_timer
           (this,
            0,
            Thread_Handler::delay_,
            Thread_Handler::interval_) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n",
                "schedule_timer"));

  // Set up this thread's signal mask to block all the signal in the
  // <sig_set>, which is inherited by the threads it spawns.
  ACE_Sig_Guard guard (&sig_set);

  // Create N new threads of control Thread_Handlers.

  for (size_t i = 0; i < n_threads; i++)
    {
      Thread_Handler *th;

      ACE_NEW (th,
               Thread_Handler (i + 1,
                               this->iterations_));

      if (ACE_Thread::spawn (reinterpret_cast<ACE_THR_FUNC> (&Thread_Handler::svc_run),
                             reinterpret_cast<void *> (th),
                             THR_NEW_LWP | THR_DETACHED) != 0)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "ACE_Thread::spawn"));
    }

  // The destructor of <guard> unblocks the signal set so that only
  // this thread receives them!
}

int
Thread_Handler::notify (ACE_Time_Value *timeout)
{
  // Just do something to test the ACE_Reactor's multi-thread
  // capabilities...

  if (ACE_Reactor::instance ()->notify
      (this,
       ACE_Event_Handler::EXCEPT_MASK,
       timeout) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) %p\n",
                       "notification::notify:exception"),
                      -1);
  else if (ACE_Reactor::instance ()->notify
            (this,
            ACE_Event_Handler::WRITE_MASK,
            timeout) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) %p\n",
                       "notification::notify:write"),
                      -1);
  return 0;
}

// Test stdin handling that uses <select> to demultiplex HANDLEs.
// Input is only handled by the main thread.

int
Thread_Handler::handle_input (ACE_HANDLE handle)
{
  char buf[BUFSIZ];
  ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

  if (n > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "input to (%t) %*s",
                  n,
                  buf));

      ACE_DEBUG ((LM_DEBUG,
                  "%d more input to kill\n",
                  this->iterations_));

      // Only wait up to 10 milliseconds to notify the Reactor.
      ACE_Time_Value timeout (0,
                              10 * 1000);

      if (this->notify (&timeout) == -1)
        ACE_ERROR ((LM_DEBUG,
                    "(%t), %p\n",
                    "notification::handle_input:notify"));
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
  ACE_Time_Value sleep_timeout (Thread_Handler::interval_.sec () / 2);

  for (int i = ACE_Utils::truncate_cast<int> (this->iterations_);
       i > 0;
       --i)
    {
      if (this->shutdown_ != 0)
        break;

      // Block for delay_.secs () / 2, then notify the Reactor.
      ACE_OS::sleep (sleep_timeout);

      // Wait up to 10 milliseconds to notify the Reactor.
      ACE_Time_Value timeout (0,
                              10 * 1000);
      if (this->notify (&timeout) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "notify"));
    }

  ACE_Reactor::instance ()->remove_handler (this,
                                            ALL_EVENTS_MASK);
  ACE_DEBUG ((LM_DEBUG,
              "(%t) exiting svc\n"));
  return 0;
}

// Test signal handling.

int
Thread_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  // @@ Note that this code is not portable to all OS platforms since
  // it uses print statements within signal handler context.
  ACE_DEBUG ((LM_DEBUG,
              "(%t) received signal %S\n",
              signum));

  switch (signum)
    {
    case SIGINT:
      // This is coded thusly to avoid problems if SIGQUIT is a legit
      // value but is not a preprocessor macro.
#if !defined (SIGQUIT) || (SIGQUIT != 0)
    case SIGQUIT:
#endif
      ACE_ERROR ((LM_ERROR,
                  "(%t) ******************** shutting down %n on signal %S\n",
                  signum));
      this->shutdown_ = 1;
      ACE_Reactor::end_event_loop();
    }
  return 0;
}

int
Thread_Handler::handle_timeout (const ACE_Time_Value &time, const void *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) received timeout at (%u, %u), iterations = %d\n",
              time.sec (),
              time.usec (),
              this->iterations_));

  if (--this->iterations_ <= 0
      || Thread_Handler::interval_.sec () == 0)
    ACE_Reactor::end_event_loop ();

  return 0;
}

// Called by the ACE_Reactor when it receives a notification.

int
Thread_Handler::handle_exception (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) exception to id %d, iteration = %d\n",
              this->id_,
              this->iterations_));
  return 0;
}

// Called by the ACE_Reactor when it receives a notification.

int
Thread_Handler::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) output to id %d, iteration = %d\n",
              this->id_,
              // This decrement must come last since
              // <handle_exception> is called before <handle_output>!
              this->iterations_--));
  return 0;
}

// "Shim" function that integrates C thread API with C++.

void *
Thread_Handler::svc_run (void *eh)
{
  Thread_Handler *this_handler =
    reinterpret_cast<Thread_Handler *> (eh);

  if (this_handler->svc () == 0)
    return 0;
  else
    return reinterpret_cast<void *> (-1);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  if (argc < 4)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("usage: %s delay interval n_threads [iterations]\n"),
                  argv[0]));
      ACE_OS::exit (1);
    }

  int delay = ACE_OS::atoi (argv[1]);
  int interval = ACE_OS::atoi (argv[2]);
  size_t n_threads = ACE_OS::atoi (argv[3]);
  size_t max_iterations = argc > 4 ? ACE_OS::atoi (argv[4]) : MAX_ITERATIONS;

  Thread_Handler thr_handler (delay,
                              interval,
                              n_threads,
                              max_iterations);

  ACE_Reactor::instance ()->run_reactor_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("exiting from main\n")));
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads must be supported to run this application\n"), -1);
}
#endif /* ACE_HAS_THREADS */
