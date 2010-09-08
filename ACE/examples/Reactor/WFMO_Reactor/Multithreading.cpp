// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Multithreading.cpp
//
// = DESCRIPTION
//
//    This application tests multiple threads simultaneously calling
//    Reactor::handle_events(). It also shows how different threads
//    can update the state of Reactor by registering and removing
//    Event_Handlers.
//
//    Note that this test will only work with WFMO_Reactor
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#include "ace/OS_main.h"

#if defined (ACE_WIN32)

#include "ace/Task.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_time.h"



static int concurrent_threads = 1;
static int number_of_handles = static_cast<int> (ACE_Reactor::instance ()->size ());
static int number_of_handles_to_signal = 1;
static int interval = 2;
static int iterations = 10;

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "usage: \n\t"
              "[-t (# of threads - default 1)] \n\t"
              "[-h (# of handlers) - default 62] \n\t"
              "[-i (# time interval between signals) - default 2] \n\t"
              "[-s (# of handles to signal) - default 1] \n\t"
              "[-e (# of iterations) - default 10] \n\t"));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("t:h:s:i:e:"));
  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 't':
        concurrent_threads = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'e':
        iterations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'h':
        number_of_handles = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'i':
        interval = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 's':
        number_of_handles_to_signal = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      default:
        print_usage_and_die ();
        break;
      }
}

class Task_Handler : public ACE_Task<ACE_NULL_SYNCH>
{
public:
  Task_Handler (size_t number_of_handles,
                size_t concurrent_threads);
  // Constructor.

  ~Task_Handler (void);
  // Destructor.

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  // Called when object is removed from the ACE_Reactor

  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Handle events being signaled by the main thread.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
  // Called when timer expires.

  int svc (void);
  // Task event loop.

  //FUZZ: disable check_for_lack_ACE_OS
  int signal (size_t index);
  // Signal an event.
  //FUZZ: enable check_for_lack_ACE_OS

private:
  ACE_Auto_Event *events_;
};

// All threads do reactor->handle_events ()
int
Task_Handler::svc (void)
{
  // Try to become the owner
  ACE_Reactor::instance ()->owner (ACE_Thread::self ());
  // Run the event loop.
  return ACE_Reactor::run_event_loop ();
}

Task_Handler::Task_Handler (size_t number_of_handles,
                            size_t concurrent_threads)
{
  ACE_NEW (this->events_, ACE_Auto_Event [number_of_handles]);

  for (size_t i = 0; i < number_of_handles; ++i)
    if (ACE_Reactor::instance ()->register_handler (this,
                                                    this->events_[i].handle ()) == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\t cannot register handle %d with Reactor\n",
                  "Task_Handler::Task_Handler",
                  i));

  // Make us an active object.
  if (this->activate (THR_NEW_LWP,
                      static_cast<int> (concurrent_threads)) == -1)
    ACE_ERROR ((LM_ERROR, "%p\t cannot activate task\n",
                "activate"));
}

Task_Handler::~Task_Handler (void)
{
  this->reactor (0);
  delete [] this->events_;
}


int
Task_Handler::handle_signal (int, siginfo_t *siginfo, ucontext_t *)
{
  // When signaled, print message, remove self, and add self
  // This will force Reactor to update its internal handle tables

  ACE_DEBUG ((LM_DEBUG,
              "(%t) calls handle_signal for handle %d\n",
              siginfo->si_handle_));

  if (ACE_Reactor::instance ()->remove_handler (siginfo->si_handle_,
                                                ACE_Event_Handler::DONT_CALL) == -1)
    return -1;
  // ACE_ERROR_RETURN ((LM_ERROR,
  //                   "(%t) %p\tTask cannot be unregistered from Reactor: handle value = %d\n",
  //                   "Task_Handler::handle_signal",
  //                   siginfo->si_handle_), -1);

  if (ACE_Reactor::instance ()->register_handler (this,
                                                  siginfo->si_handle_) == -1)
    return -1;
  // ACE_ERROR_RETURN ((LM_ERROR,
  //                   "(%t) %p\tTask cannot be registered with Reactor: handle value = %d\n",
  //                   "Task_Handler::handle_signal",
  //                   siginfo->si_handle_), -1);
  return 0;
}

int
Task_Handler::handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_close() called: handle value = %d\n",
              handle));
  return 0;
}

int
Task_Handler::handle_timeout (const ACE_Time_Value &,
                              const void *arg)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_timeout() called: iteration value = %d\n",
              size_t (arg)));
  return 0;
}

int
Task_Handler::signal (size_t index)
{
  return this->events_[index].signal ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  parse_args (argc, argv);
  Task_Handler task (number_of_handles,
                     concurrent_threads);

  ACE_OS::srand ((u_int) ACE_OS::time (0L));

  for (int i = 1; i <= iterations; i++)
    {
      // Sleep for a while
      ACE_OS::sleep (interval);

      // Randomly generate events
      ACE_DEBUG ((LM_DEBUG, "********************************************************\n"));
      ACE_DEBUG ((LM_DEBUG, "(%t -- main thread) signaling %d events : iteration = %d\n",
                  number_of_handles_to_signal,
                  i));
      ACE_DEBUG ((LM_DEBUG, "********************************************************\n"));

      // Setup a timer for the task
      if (ACE_Reactor::instance ()->schedule_timer (&task,
                                                    (void *)((size_t)i),
                                                    ACE_Time_Value::zero) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

      for (int i = 0; i < number_of_handles_to_signal; i++)
        // Randomly select a handle to signal.
        task.signal (ACE_OS::rand() % number_of_handles);
    }

  // Sleep for a while
  ACE_OS::sleep (interval);

  // End the Reactor event loop
  ACE_Reactor::end_event_loop ();

  // Wait for all threads to exit
  ACE_Thread_Manager::instance ()->wait ();

  // Close the Reactor singleton before exiting this function.
  // If we wait for the Object Manager to do this, it will be too
  // late since Task_Handler instance would have disappeared.
  ACE_Reactor::close_singleton ();

  return 0;
}
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 */
