// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_timeouts.cpp
//
// = DESCRIPTION
//
//    This example application shows how to write Reactor event
//    loops that handle events for some fixed amount of time.
//
//    Run this example (without arguments) to see the timers
//    expire. The order should be:
//
//    foo, bar, foo, bar, foo, foo, bar, foo, bar, foo
//
// = AUTHOR
//    Tim Harrison
//    Irfan Pyarali
//
// ============================================================================

#include "ace/Reactor.h"
#include "ace/Service_Config.h"
#include "ace/OS.h"

ACE_RCSID(WFMO_Reactor, test_timeouts, "$Id$")

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
main (int, char *[])
{
  Timeout_Handler handler;

  // Register a 3 second timer.
  ACE_Time_Value bar_tv (3);
  ACE_Reactor::instance ()->schedule_timer (&handler,
                                            (void *) "Bar",
                                            bar_tv,
                                            bar_tv);

  // Register a 2 second timer.
  ACE_Time_Value foo_tv (2);
  ACE_Reactor::instance ()->schedule_timer (&handler,
                                            (void *) "Foo",
                                            foo_tv,
                                            foo_tv);
  // Handle events for 12 seconds.
  ACE_Time_Value run_time (12);
  if (ACE_Reactor::run_event_loop(run_time) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);

  return 0;
}
