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

#include "ace/ReactorEX.h"
#include "ace/Get_Opt.h"

static int concurrent_threads = 1;
static int number_of_handles = ACE_ReactorEx::DEFAULT_SIZE;
static int number_of_handles_to_signal = 1;
static int interval = 2;

// Explain usage and exit.
static void 
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "usage: %n [-t (# of threads)] [-h (# of handlers)] [-i (# time interval between signals)] [-s (# of handles to signal)] \n"));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opt (argc, argv, "t:h:s:i:");
  int c; 
  
  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 't':
	concurrent_threads = atoi (get_opt.optarg);
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

class Task_Handler : public ACE_Task<ACE_NULL_SYNCH>, public ACE_Auto_Event
{
public:
  Task_Handler (void);
  // Constructor.

  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Handle events being signaled by the main thread.

  int svc (void);
  // Task event loop.

  ACE_HANDLE get_handle (void) const;
  // Our event handle.
};

// All threads do reactorEx->handle_events ()

int
Task_Handler::svc (void)
{
  // Run the event loop.
  for (;;)
    if (ACE_Service_Config::reactorEx ()->handle_events () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "handle_events"), 0);	
}

Task_Handler::Task_Handler (void)
{
  if (ACE_Service_Config::reactorEx ()->register_handler (this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\t cannot register with ReactorEx\n", 
		"Task_Handler::Task_Handler"));
  // Make us an active object.
  else if (this->activate () == -1)
    ACE_ERROR ((LM_ERROR, "%p\t cannot activate task\n", 
		"activate"));
}

int
Task_Handler::handle_signal (int signum, siginfo_t *siginfo, ucontext_t *)
{
  // When signaled, print message, remove self, and add self
  // This will force ReactorEx to update its internal handle tables
  ACE_DEBUG ((LM_DEBUG, "(%t) handle_signal() called\n"));

  ACE_ASSERT (siginfo->si_handle_ == this->handle ());

  if (ACE_Service_Config::reactorEx ()->remove_handler (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "(%t) %p\tTask cannot be unregistered from ReactorEx\n", 
		       "Task_Handler::handle_signal"), -1);
  
  if (ACE_Service_Config::reactorEx ()->register_handler (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "(%t) %p\tTask cannot be registered with ReactorEx\n", 
		       "Task_Handler::handle_signal"), -1);
  return 0;
}

ACE_HANDLE
Task_Handler::get_handle (void) const
{
  return this->handle ();
}

int 
main (int argc, char **argv)
{
  parse_args (argc, argv);
  Task_Handler *handlers = 0;
  
  // Create an array Task_Handlers.
  ACE_NEW_RETURN (handlers, Task_Handler [number_of_handles], -1);

  ACE_OS::srand (ACE_OS::time (0L));

  for (;;)
    {
      // Sleep for a while
      ACE_OS::sleep (interval);

      // Randomly generate events
      ACE_DEBUG ((LM_DEBUG, "***************************************\n"));		
      ACE_DEBUG ((LM_DEBUG, "(%t -- main thread) signaling %d events\n", 
		  number_of_handles_to_signal));		
      ACE_DEBUG ((LM_DEBUG, "***************************************\n"));		

      for (i = 0; i < number_of_handles_to_signal; i++)
	// Randomly select a handle to signal.
	handlers[ACE_OS::rand() % number_of_handles].signal ();	  
    }

  return 0;
}
