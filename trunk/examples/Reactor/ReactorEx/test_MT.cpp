// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_MT.cpp
//
// = DESCRIPTION
//    This application tests multiple threads simultaneously calling
//    ReactorEx->handle_events(). It also shows how different threads
//    can update the state of ReactorEx by registering and removing
//    Event_Handlers. 
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Task.h"
#include "ace/ReactorEx.h"
#include "ace/Get_Opt.h"

static int concurrent_threads = 1;
static int number_of_handles = ACE_ReactorEx::DEFAULT_SIZE;
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
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opt (argc, argv, "t:h:s:i:e:");
  int c; 
  
  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 't':
	concurrent_threads = atoi (get_opt.optarg);
	break;
      case 'e':
	iterations = atoi (get_opt.optarg);
	break;
      case 'h':
	number_of_handles = atoi (get_opt.optarg);
	break;
      case 'i':
	interval = atoi (get_opt.optarg);
	break;
      case 's':
	number_of_handles_to_signal = atoi (get_opt.optarg);
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

  int signal (size_t index);
  // Signal an event.

private:
  ACE_Auto_Event *events_;
};

// All threads do reactorEx->handle_events ()
int
Task_Handler::svc (void)
{
  // Try to become the owner
  ACE_Service_Config::reactorEx ()->owner (ACE_Thread::self ());
  // Run the event loop.
  return ACE_Service_Config::run_reactorEx_event_loop ();
}

Task_Handler::Task_Handler (size_t number_of_handles,
			    size_t concurrent_threads)
{
  ACE_NEW (this->events_, ACE_Auto_Event [number_of_handles]);

  for (size_t i = 0; i < number_of_handles; i++)
    {
      if (ACE_Service_Config::reactorEx ()->register_handler (this,
							      this->events_[i].handle ()) == -1)
	ACE_ERROR ((LM_ERROR, "%p\t cannot register handle %d with ReactorEx\n", 
		    "Task_Handler::Task_Handler", i));      
    }
  // Make us an active object.
  if (this->activate (THR_NEW_LWP, 
		      concurrent_threads) == -1)
    ACE_ERROR ((LM_ERROR, "%p\t cannot activate task\n", 
		"activate"));
}

Task_Handler::~Task_Handler (void)
{
  delete [] this->events_;
}


int
Task_Handler::handle_signal (int signum, siginfo_t *siginfo, ucontext_t *)
{
  // When signaled, print message, remove self, and add self
  // This will force ReactorEx to update its internal handle tables
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_signal() called: handle value = %d\n", 
	      siginfo->si_handle_));

  if (ACE_Service_Config::reactorEx ()->remove_handler (siginfo->si_handle_,
							ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "(%t) %p\tTask cannot be unregistered from ReactorEx: handle value = %d\n", 
		       "Task_Handler::handle_signal",
		       siginfo->si_handle_), -1);
  
  if (ACE_Service_Config::reactorEx ()->register_handler (this,
							  siginfo->si_handle_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "(%t) %p\tTask cannot be registered with ReactorEx: handle value = %d\n",  
		       "Task_Handler::handle_signal",
		       siginfo->si_handle_), -1);
  return 0;
}

int 
Task_Handler::handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_close() called: handle value = %d\n", 
	      handle));
  return 0;
}

int 
Task_Handler::handle_timeout (const ACE_Time_Value &tv,
			      const void *arg)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_timeout() called: iteration value = %d\n", 
	      int (arg)));
  return 0;
}

int 
Task_Handler::signal (size_t index)
{
  return this->events_[index].signal ();
}

int 
main (int argc, char **argv)
{
  parse_args (argc, argv);
  Task_Handler task (number_of_handles,
		     concurrent_threads);
  
  ACE_OS::srand (ACE_OS::time (0L));

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
      ACE_Service_Config::reactorEx ()->schedule_timer (&task,
							(void *) i,
							0);  

      for (int i = 0; i < number_of_handles_to_signal; i++)
	{
	  // Randomly select a handle to signal.
	  task.signal (ACE_OS::rand() % number_of_handles);	  
	}
    }

  // Sleep for a while
  ACE_OS::sleep (interval);
  // Close ReactorEx
  ACE_Service_Config::reactorEx ()->close ();
  // Wait for all threads to exit 
  ACE_Service_Config::thr_mgr ()->wait ();
  // Delete dynamic resources
  ACE_Service_Config::close_singletons ();

  return 0;
}
