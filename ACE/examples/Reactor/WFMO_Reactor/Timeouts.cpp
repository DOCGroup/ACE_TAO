//=============================================================================
/**
 *  @file    Timeouts.cpp
 *
 *  This example application shows how to write Reactor event
 *  loops that handle events for some fixed amount of time.
 *
 *  Run this example (without arguments) to see the timers
 *  expire. The order should be:
 *
 *  foo, bar, foo, bar, foo, foo, bar, foo, bar, foo
 *
 *  @author Tim Harrison Irfan Pyarali
 */
//=============================================================================


#include "ace/Reactor.h"
#include "ace/Service_Config.h"
#include "ace/OS_main.h"


/**
 * @class Timeout_Handler
 *
 * @brief Generic timeout handler.
 */
class Timeout_Handler : public ACE_Event_Handler
{
public:
  Timeout_Handler () = default;

  Timeout_Handler (Timeout_Handler const &) = delete;
  Timeout_Handler (Timeout_Handler &&) = delete;
  Timeout_Handler &operator= (Timeout_Handler const &) = delete;
  Timeout_Handler &operator= (Timeout_Handler &&) = delete;

  /// Print out when timeouts occur.
  int handle_timeout (ACE_Time_Value const &tv,
                      void const *arg) override
  {
    ACE_UNUSED_ARG (tv);
    ACE_DEBUG ((LM_DEBUG,
                "%d timeout occurred for %s.\n",
                ++count_,
                static_cast<char const *> (arg)));
    return 0;
  }

private:
  int count_ {};
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Timeout_Handler handler;

  // Register a 3 second timer.
  ACE_Time_Value bar_tv (3);
  static char bar[] = "Bar";
  ACE_Reactor::instance ()->schedule_timer (&handler,
                                            bar,
                                            bar_tv,
                                            bar_tv);

  // Register a 2 second timer.
  ACE_Time_Value foo_tv (2);
  static char foo[] = "Foo";
  ACE_Reactor::instance ()->schedule_timer (&handler,
                                            foo,
                                            foo_tv,
                                            foo_tv);
  // Handle events for 12 seconds.
  ACE_Time_Value run_time (12);
  if (ACE_Reactor::run_event_loop (run_time) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);

  ACE_Reactor::instance ()->cancel_timer (&handler);

  return 0;
}
