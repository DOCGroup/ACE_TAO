// $Id: test_timeout.cpp

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_timeout.cpp
//
// = DESCRIPTION
//
//    This example application shows how to write event loops that
//    handle events for some fixed amount of time. Note that any
//    thread in the Proactor thread pool can call back the handler
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Proactor.h"
#include "ace/Synch.h"
#include "ace/Task.h"

class Timeout_Handler : public ACE_Handler
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
      // Sleep for a while
      ACE_OS::sleep (4);
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

    if (ACE_Proactor::run_event_loop(run_time) == -1)
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

  // Register a 2 second timer.
  ACE_Time_Value foo_tv (2);
  if (ACE_Proactor::instance ()->schedule_timer (handler,
						       (void *) "Foo",
						       ACE_Time_Value::zero,
						       foo_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  // Register a 3 second timer.
  ACE_Time_Value bar_tv (3);
  if (ACE_Proactor::instance ()->schedule_timer (handler,
						       (void *) "Bar",
						       ACE_Time_Value::zero,
						       bar_tv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_timer"), -1);

  Worker worker;

  if (worker.activate (THR_NEW_LWP, 10) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
