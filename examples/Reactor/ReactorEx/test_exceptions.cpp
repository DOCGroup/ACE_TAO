// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_exceptions.cpp
//
// = DESCRIPTION
//
//    This test application tests the state of ReactorEx when
//    exceptions occurs when executing user callbacks. 
//
//    The thread count in ReactorEx is used to ensure that state of
//    ReactorEx is not fouled up when exceptions occur in user code.
//    This example also shows how to write event loops that survive
//    user exceptions
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/ReactorEx.h"

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler ()
    : event_ (1)
  {
    ACE_DEBUG ((LM_DEBUG, "Event_Handler created\n"));
  }
  ~Event_Handler ()
  {
    ACE_DEBUG ((LM_DEBUG, "Event_Handler destroyed\n"));
  }
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0)
  {
    char *cause_exception = 0;
    char a = *cause_exception;
    return 0;
  }
  ACE_HANDLE get_handle (void) const
  {
    return this->event_.handle ();
  }
private:
  ACE_Manual_Event event_;
};

class ACE_ReactorEx_Test
{
public:
  static void doit ()
  {
    for (int i = 1; i <= 10; i++)
      {
	ACE_DEBUG ((LM_DEBUG, "Active threads in ReactorEx (before handle_events) = %d\n", 
		    ACE_ReactorEx::instance ()->active_threads_));
	ACE_SEH_TRY 
	  {
	    ACE_ReactorEx::instance ()->handle_events ();
	  }
	ACE_SEH_EXCEPT (EXCEPTION_EXECUTE_HANDLER)
	  {
	    ACE_DEBUG ((LM_DEBUG, "Exception occurred\n"));
	  }
	ACE_DEBUG ((LM_DEBUG, "Active threads in ReactorEx (after handle_events) = %d\n", 
		    ACE_ReactorEx::instance ()->active_threads_));
      }
  }
};

int
main (int, char *[])
{
  Event_Handler handler;
  ACE_ReactorEx::instance ()->register_handler (&handler);
  ACE_ReactorEx_Test::doit ();
  return 0;
}

