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
//      the reactor.  Scheduling timers, resetting timer intervals,
//      handling expired timers and cancelling scheduled timers are
//      all exercised in this test. 
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu> and Douglas C. Schmidt
//    <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"

ACE_RCSID(tests, Reactor_Timer_Test, "$Id$")

static int done = 0;
static int count = 0;
static int odd = 0;

class Time_Handler : public ACE_Event_Handler
{
public:
  Time_Handler ();
  // Default constructor

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);
  // Handle the timeout.

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  // Called when <Time_Handler> is removed.

  long timer_id (void) const;
  // Return our timer id.

  void timer_id (long);
  // Set our timer id;

private:
  long timer_id_;
  // Stores the id of this timer.
};

Time_Handler::Time_Handler ()
: timer_id_ (-1)
{
  // Nothing
}

int 
Time_Handler::handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("[%x] handle = %d, close_mask = %d, timer id = %d\n"),
              this,
              handle,
              close_mask,
              this->timer_id ()));
  return 0;
}

int 
Time_Handler::handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
{
  long current_count = ACE_reinterpret_cast (long, arg);
  if (current_count >= 0)
    ACE_ASSERT (current_count == count);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("[%x] Timer id %d with count #%d|%d timed out at %d!\n"),
              this,
              this->timer_id (),
              count,
              current_count,
              tv.sec ()));

  if (current_count == long (ACE_MAX_TIMERS - 1))
    done = 1;
  else if (count == ACE_MAX_TIMERS - 1)
    {
      done = 1;
      return -1;
    }
  else if (current_count == -1)
    {
      int result =
        this->reactor ()->reset_timer_interval (this->timer_id (),
                                                ACE_Time_Value (count + 1));
      ACE_ASSERT (result != -1);
    }
  count += (1 + odd);
  return 0;
}

long
Time_Handler::timer_id (void) const
{
  return this->timer_id_;
}

void
Time_Handler::timer_id (long t)
{
  this->timer_id_ = t;
}

static void
test_registering_all_handlers (ACE_Reactor *reactor)
{
  ACE_Trace t ("test_registering_all_handler", __LINE__, __FILE__);
  Time_Handler rt[ACE_MAX_TIMERS];
  int t_id[ACE_MAX_TIMERS];

  for (size_t i = 0; i < ACE_MAX_TIMERS; i++)
    {
      rt[i].reactor (reactor);
      t_id[i] =
        reactor->schedule_timer (&rt[i], 
                                 (const void *) i, 
                                 ACE_Time_Value (2 * i + 1));
      ACE_ASSERT (t_id[i] != -1);
      rt[i].timer_id (t_id[i]);
    }

  while (!done)
    reactor->handle_events ();
}

static void
test_registering_one_handler (ACE_Reactor *reactor)
{
  ACE_Trace t ("test_registering_one_handler", __LINE__, __FILE__);
  Time_Handler rt[ACE_MAX_TIMERS];
  int t_id[ACE_MAX_TIMERS];

  done = 0;
  count = 0;

  rt[0].reactor (reactor);
  for (size_t i = 0; (u_long) i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] =
        reactor->schedule_timer (&rt[0],
                                 (const void *) i,
                                 ACE_Time_Value (2 * i + 1));
      ACE_ASSERT (t_id[i] != -1);
    }

  while (!done)
    reactor->handle_events ();
}

static void
test_canceling_odd_timers (ACE_Reactor *reactor)
{
  ACE_Trace t ("test_canceling_odd_timers", __LINE__, __FILE__);
  Time_Handler rt[ACE_MAX_TIMERS];
  int t_id[ACE_MAX_TIMERS];

  done = 0;
  count = 1;
  odd = 1;

  for (size_t i = 0; (u_long) i < ACE_MAX_TIMERS; i++)
    {
      rt[i].reactor (reactor);
      t_id[i] = reactor->schedule_timer (&rt[i],
                                         (const void *) i,
                                         ACE_Time_Value (2 * i + 1));
      ACE_ASSERT (t_id[i] != -1);
      rt[i].timer_id (t_id[i]);
    }

  for (size_t j = 0; (u_long) j < ACE_MAX_TIMERS; j++)
    // Cancel handlers with odd numbered timer ids.
    if (ACE_ODD (rt[j].timer_id ()))
      {
        int result = 
          reactor->cancel_timer (rt[j].timer_id ());
        ACE_ASSERT (result != -1);
      }

  while (!done)
    reactor->handle_events ();
}

static void
test_resetting_timer_intervals (ACE_Reactor *reactor)
{
  ACE_Trace t ("test_resetting_timer_intervals", __LINE__, __FILE__);
  Time_Handler rt;
  int t_id;

  done = 0;
  count = 0;
  odd = 0;

  rt.reactor (reactor);
  t_id =
    reactor->schedule_timer (&rt, 
                             (const void *) -1,
                             ACE_Time_Value (1),
                             // Start off by making this an interval timer.
                             ACE_Time_Value (1));

  ACE_ASSERT (t_id != -1);
  rt.timer_id (t_id);

  while (!done)
    reactor->handle_events ();
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Reactor_Timer_Test"));

  ACE_DEBUG ((LM_DEBUG, "Testing with a non-singleton reactor\n"));

  ACE_Reactor other_reactor;
  test_registering_all_handlers (&other_reactor);
  test_registering_one_handler (&other_reactor);
  test_canceling_odd_timers (&other_reactor);
  test_resetting_timer_intervals (&other_reactor);

  // Register all different handlers, i.e., one per timer.
  ACE_Reactor *reactor =
    ACE_Reactor::instance ();
  test_registering_all_handlers (reactor);

  // Now try multiple timers for ONE event handler (should produce the
  // same result).
  test_registering_one_handler (reactor);

  // Try canceling handlers with odd numbered timer ids.
  test_canceling_odd_timers (reactor);

  // Make sure <reset_timer_inverval> works.
  test_resetting_timer_intervals (reactor);


  ACE_END_TEST;
  return 0;
}
