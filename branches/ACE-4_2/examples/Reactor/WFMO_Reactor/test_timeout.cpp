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
//    This example application shows how to write ReactorEx and
//    Proactor event loops that handle events for some fixed amount of
//    time.
//
// = AUTHOR
//    Tim Harrison
// 
// ============================================================================

#include "ace/ReactorEx.h"
#include "ace/Proactor.h"
#include "ace/Service_Config.h"
#include "ace/OS.h"

class Timeout_Handler : public ACE_Event_Handler
// = TITLE
//     Generic timeout handler.
{
public:
  Timeout_Handler (void)
    : count_ (0) {}

  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg)
    // Print out when timeouts occur.
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "%d timeout occurred for %s.\n", 
		  ++count_,
		  (char *) arg));
      return 0;
    }

private:
  int count_;
};

int
main (void)
{
  Timeout_Handler handler;

  // Register a 2 second timer.
  ACE_Time_Value foo_tv (2);
  ACE_Service_Config::reactorEx ()->schedule_timer (&handler,
						    (void *) "Foo",
						    ACE_Time_Value::zero,
						    foo_tv);
  // Register a 3 second timer.
  ACE_Time_Value bar_tv (3);
  ACE_Service_Config::reactorEx ()->schedule_timer (&handler,
						    (void *) "Bar",
						    ACE_Time_Value::zero,
						    bar_tv);

  // Handle events for 12 seconds.
  ACE_Time_Value run_time (12);
  if (ACE_Service_Config::run_reactorEx_event_loop (run_time) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);
						    
  return 0;
}
