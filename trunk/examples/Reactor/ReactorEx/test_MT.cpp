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
//    can update the state of ReactorEx.
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
static int interval = 1;


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
	::concurrent_threads = atoi (get_opt.optarg);
	break;
      case 'h':
	::number_of_handles = atoi (get_opt.optarg);
	break;
      case 'i':
	::interval = atoi (get_opt.optarg);
	break;
      case 's':
	::number_of_handles_to_signal = atoi (get_opt.optarg);
	break;
      default:
	print_usage_and_die ();
	break;
      }
}

// All threads do reactorEx->handle_events ()
static void*
work (void *arg)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ()); 
  // Insert thread into thread_manager
  ACE_ReactorEx *reactorEx = (ACE_ReactorEx *) arg;
  while (1)
    {
      int result = reactorEx->handle_events ();
      if (result == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "handle_events"), 0);	
    }	
}


class Event_Handler : public ACE_Event_Handler
{
public:
  int open (int index,
	    ACE_ReactorEx &reactorEx,
	    ACE_Auto_Event *handles,
	    int number_of_handles);
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  ACE_HANDLE get_handle () const;
private:
  int index_;
  ACE_ReactorEx *reactorEx_;
  ACE_Auto_Event *handles_;
  int number_of_handles_;
  ACE_HANDLE handle_;
};

int
Event_Handler::open (int index,
		     ACE_ReactorEx &reactorEx,
		     ACE_Auto_Event *handles,
		     int number_of_handles)
{
  index_ = index;
  reactorEx_ = &reactorEx;
  handles_ = handles;
  number_of_handles_ = number_of_handles;
  this->handle_ = this->handles_[this->index_].handle ();
  
  // Register self with ReactorEx
  if (this->reactorEx_->register_handler (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\tevent handler %d cannot be registered with ReactorEx\n", 
		       "Event_Handler::open", this->index_), -1);
    
  return 0;
}

int
Event_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  // When signaled, print message, remove self, and add self
  // This will force ReactorEx to update its internal handle tables
  ACE_DEBUG ((LM_DEBUG, "(%t)\thandle_signal() called in handler # %d\n", this->index_));
  if (this->reactorEx_->remove_handler (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\tevent handler %d cannot be unregistered from ReactorEx\n", 
		       "Event_Handler::open", this->index_), -1);
  
  if (this->reactorEx_->register_handler (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\tevent handler %d cannot be registered with ReactorEx\n", 
		       "Event_Handler::open", this->index_), -1);
  
  
  return 0;
}

ACE_HANDLE
Event_Handler::get_handle () const
{
  return this->handle_;
}

int 
main (int argc, char **argv)
{
  ::parse_args (argc, argv);
  ACE_ReactorEx reactorEx;
  ACE_Auto_Event *handles = 0;
  Event_Handler *handlers = 0;
  
  // Create a bunch of Event_Handler and Auto_Events
  ACE_NEW_RETURN (handles, ACE_Auto_Event [::number_of_handles], -1);
  ACE_NEW_RETURN (handlers, Event_Handler [::number_of_handles], -1);
  // Initialize Event_Handlers
  for (int i = 0; i < ::number_of_handles; i++)
    {
      if (handlers[i].open (i, reactorEx, handles, ::number_of_handles) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\tevent handler %d cannot be opened\n", "main", i), -1);
    }
  // Creat worker threads
  if (ACE_Thread::spawn_n (::concurrent_threads, work, &reactorEx) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\tThreads could not be spawned\n", "main"), -1);
  ACE_OS::srand (ACE_OS::time (0L));
  for (;;)
    {
      // Sleep for a while
      ACE_OS::sleep (::interval);
      // Randomly generate events
      for (i = 0; i < ::number_of_handles_to_signal; i++)
	{
	  int index = ACE_OS::rand() % ::number_of_handles;
	  handles[index].signal ();	  
	}
    }
}
