// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Reactor_Timer_Test.cpp
//
// = DESCRIPTION
//      This is a simple test that illustrates the timer mechanism of
//      the reactor. Scheduling timers, handling expired timers and
//      cancelling scheduled timers are all tested in this test. No
//      command line arguments are needed to run the test.
//
// = AUTHOR
//    Prashant Jain and Douglas C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"

ACE_RCSID(tests, Reactor_Timer_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static int done = 0;
static int count = 0;
static int odd = 0;

class Time_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
    {
      long current_count = long (arg);
      ACE_ASSERT (current_count == count);

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("%d: Timer #%d timed out at %d!\n"),
                  count,
                  current_count,
                  tv.sec ()));
      count += (1 + odd);

      if (long (current_count) == long (ACE_MAX_TIMERS - 1))
        done = 1;
      return 0;
    }
};

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Reactor_Timer_Test"));

  // We put the <Time_Handler> first so that it'll still be alive when
  // the <reactor> is closed down.
  Time_Handler rt[ACE_MAX_TIMERS];

  ACE_Reactor reactor;

  int t_id[ACE_MAX_TIMERS];

  size_t i;
  
  for (i = 0; i < ACE_MAX_TIMERS; i++)
    t_id[i] = reactor.schedule_timer (&rt[i], 
				      (const void *) i, 
				      ACE_Time_Value (2 * i + 1));
  while (!done)
    reactor.handle_events ();

  done = 0;
  count = 0;

  // Now try multiple timers for ONE event handler (should produce the
  // same result).
  for (i = 0; (u_long) i < ACE_MAX_TIMERS; i++)
    t_id[i] = reactor.schedule_timer (&rt[0],
                                      (const void *) i,
                                      ACE_Time_Value (2 * i + 1));
  while (!done)
    reactor.handle_events ();

  done = 0;
  count = 1;
  odd = 1;

  for (i = 0; (u_long) i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] = reactor.schedule_timer (&rt[0],
                                        (const void *) i,
                                        ACE_Time_Value (2 * i + 1));

      // Cancel even numbered timers.
      if (ACE_EVEN (i))
        reactor.cancel_timer (t_id[i]);
    }

  while (!done)
    reactor.handle_events ();

  ACE_END_TEST;
  return 0;
}
