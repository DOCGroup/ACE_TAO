// $Id: test_timeout.cpp

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_multiple_loops.cpp
//
// = DESCRIPTION
//
//    This example application shows how to write programs that
//    combine the Proactor and ReactorEx event loops
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Proactor.h"
#include "ace/Synch.h"
#include "ace/Task.h"

class Timeout_Handler : public ACE_Handler, public ACE_Event_Handler
  // = TITLE
  //     Generic timeout handler.
{
public:
  Timeout_Handler (void) 
    { 
    }

  virtual void handle_time_out (const ACE_Time_Value &tv,
			       const void *arg)
    // Print out when timeouts occur.
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) %d timeout occurred for %s @ %d.\n", 
		  ++count_,
		  (char *) arg,
		  tv.sec ()));

      // Since there is only one thread that can do the timeouts in
      // ReactorEx, lets keep the handle_timeout short for that
      // thread.
      if (ACE_OS::strcmp ((char *) arg, "Proactor") == 0)
	// Sleep for a while
	ACE_OS::sleep (4);
    }
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg)
    {
      this->handle_time_out (tv, arg);
      return 0;
    }
  
private:
  ACE_Atomic_Op <ACE_Thread_Mutex, int> count_;
};

class Worker : public ACE_Task <ACE_NULL_SYNCH>
{
public:
  int svc (void)
  {
    // Handle events for 13 seconds.
    ACE_Time_Value run_time (13);

    // Try to become the owner
    ACE_ReactorEx::instance ()->owner (ACE_Thread::self ());

    if (ACE_ReactorEx::run_event_loop (run_time) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "Worker::svc"), -1);
    else
      ACE_DEBUG ((LM_DEBUG, "(%t) work complete\n"));

    return 0;
  }
};

int
main (void)
{
  Timeout_Handler handler;
  ACE_Proactor proactor (0, 0, 1);

  ACE_ReactorEx::instance ()->register_handler (&proactor);
  
  // Register a 2 second timer.
  ACE_Time_Value foo_tv (2);
  if (proactor.schedule_timer (handler,
			       (void *) "Proactor",
			       ACE_Time_Value::zero,
			       foo_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  // Register a 3 second timer.
  ACE_Time_Value bar_tv (3);
  if (ACE_ReactorEx::instance ()->schedule_timer (&handler,
							(void *) "ReactorEx",
							ACE_Time_Value::zero,
							bar_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  Worker worker;

  if (worker.activate (THR_NEW_LWP, 10) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);
  
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
