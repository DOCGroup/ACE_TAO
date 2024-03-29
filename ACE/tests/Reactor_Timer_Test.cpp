//=============================================================================
/**
 *  @file    Reactor_Timer_Test.cpp
 *
 *    This is a simple test that illustrates the timer mechanism of
 *    the reactor.  Scheduling timers, resetting timer intervals,
 *    handling expired timers and cancelling scheduled timers are
 *    all exercised in this test.
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu> and Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#include "test_config.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include "ace/High_Res_Timer.h"
#include "ace/Trace.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Log_Msg.h"
#include "ace/Timer_Heap.h"
#include <memory>

static int done = 0;
static int the_count = 0;
static int odd = 0;

class Time_Handler : public ACE_Event_Handler
{
public:
  /// Default constructor
  Time_Handler ();

  /// Handle the timeout.
  int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg) override;

  /// Called when <Time_Handler> is removed.
  int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask) override;

  /// Return our timer id.
  long timer_id () const;

  /// Set our timer id;
  void timer_id (long);

private:
  /// Stores the id of this timer.
  long timer_id_;
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
              ACE_TEXT ("[%x] handle = %d, close_mask = %d, timer id = %d\n"),
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
  long current_count = static_cast<long> (reinterpret_cast<size_t> (arg));
  if (current_count >= 0)
    ACE_TEST_ASSERT (current_count == the_count);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%x] Timer id %d with count #%d|%d timed out at %d!\n"),
              this,
              this->timer_id (),
              the_count,
              current_count,
              tv.sec ()));

  if (current_count == long (ACE_MAX_TIMERS - 1))
    done = 1;
  else if (the_count == long (ACE_MAX_TIMERS - 1))
    {
      done = 1;
      return -1;
    }
  else if (current_count == -1)
    {
      int result = ACE_Reactor::instance ()->reset_timer_interval (this->timer_id (),
                                                                   std::chrono::seconds {the_count + 1});
      if (result == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Error resetting timer interval\n")));
    }
  the_count += (1 + odd);
  return 0;
}

long
Time_Handler::timer_id () const
{
  return this->timer_id_;
}

void
Time_Handler::timer_id (long t)
{
  this->timer_id_ = t;
}

static void
test_registering_all_handlers ()
{
  ACE_Trace t (ACE_TEXT ("test_registering_all_handler"),
               __LINE__,
               ACE_TEXT_CHAR_TO_TCHAR (__FILE__));
  Time_Handler rt[ACE_MAX_TIMERS];
  long t_id[ACE_MAX_TIMERS];

  for (size_t i = 0; i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] =
        ACE_Reactor::instance ()->schedule_timer (&rt[i],
                                                  (const void *) i,
                                                  std::chrono::seconds {2 * i + 1});
      ACE_TEST_ASSERT (t_id[i] != -1);
      rt[i].timer_id (t_id[i]);
    }

  while (!done)
    ACE_Reactor::instance ()->handle_events ();
}

static void
test_registering_one_handler ()
{
  ACE_Trace t (ACE_TEXT ("test_registering_one_handler"),
               __LINE__,
               ACE_TEXT_CHAR_TO_TCHAR (__FILE__));
  Time_Handler rt[ACE_MAX_TIMERS];
  long t_id[ACE_MAX_TIMERS];

  done = 0;
  the_count = 0;

  for (size_t i = 0; i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] =
        ACE_Reactor::instance ()->schedule_timer (&rt[0],
                                                  (const void *) i,
                                                  ACE_Time_Value (2 * i + 1));
      ACE_TEST_ASSERT (t_id[i] != -1);
    }

  while (!done)
    ACE_Reactor::instance ()->handle_events ();
}

static void
test_canceling_odd_timers ()
{
  ACE_Trace t (ACE_TEXT ("test_canceling_odd_timers"),
               __LINE__,
               ACE_TEXT_CHAR_TO_TCHAR (__FILE__));
  Time_Handler rt[ACE_MAX_TIMERS];
  long t_id[ACE_MAX_TIMERS];

  done = 0;
  the_count = 1;
  odd = 1;

  for (size_t i = 0; i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] = ACE_Reactor::instance ()->schedule_timer (&rt[i],
                                                          (const void *) i,
                                                          ACE_Time_Value (2 * i + 1));
      ACE_TEST_ASSERT (t_id[i] != -1);
      rt[i].timer_id (t_id[i]);
    }

  for (size_t j = 0; (u_long) j < ACE_MAX_TIMERS; j++)
    // Cancel handlers with odd numbered timer ids.
    if (ACE_ODD (rt[j].timer_id ()))
      {
        int result =
          ACE_Reactor::instance ()->cancel_timer (rt[j].timer_id ());
        if (result == -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Error cancelling timer\n")));
      }

  while (!done)
    ACE_Reactor::instance ()->handle_events ();
}

static void
test_resetting_timer_intervals ()
{
  ACE_Trace t (ACE_TEXT ("test_resetting_timer_intervals"),
               __LINE__,
               ACE_TEXT_CHAR_TO_TCHAR (__FILE__));
  Time_Handler rt;
  long t_id;

  done = 0;
  the_count = 0;
  odd = 0;

  t_id =
    ACE_Reactor::instance ()->schedule_timer
    (&rt,
     (const void *) -1,
     ACE_Time_Value (1),
     // Start off by making this an interval timer.
     ACE_Time_Value (1));

  ACE_TEST_ASSERT (t_id != -1);
  rt.timer_id (t_id);

  while (!done)
    ACE_Reactor::instance ()->handle_events ();
}

// If any command line arg is given, run the test with high res timer
// queue. Else run it normally.
int
run_main (int argc, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Timer_Test"));

  if (argc > 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Running with high-res timer queue\n")));
      ACE_Reactor *r = ACE_Reactor::instance ();

      (void) ACE_High_Res_Timer::global_scale_factor ();

      // Change the source of time in the Reactor to the
      // high-resolution  timer.  Why does this test require such
      // precision for a 1 second timer is beyond me ...  I think it
      // is a cut&paste error.
      //
      // The use of auto_ptr<> is optional, ACE uses dangerous memory
      // management idioms everywhere, I thought I could demonstrate how
      // to do it right in at least one test.  Notice the lack of
      // ACE_NEW_RETURN, that monstrosity has no business in proper C++
      // code ...
      std::unique_ptr<ACE_Timer_Heap_Variable_Time_Source> tq(
          new ACE_Timer_Heap_Variable_Time_Source);
      // ... notice how the policy is in the derived timer queue type.
      // The abstract timer queue does not have a time policy ...
      tq->set_time_policy(&ACE_High_Res_Timer::gettimeofday_hr);
      // ... and then the timer queue is replaced.  Strangely, the
      // Reactor does *not* copy the timers, it just deletes the
      // existing timer queue ....
      r->timer_queue(tq.get());
      // ... the Reactor has assumed ownership, release the
      // auto_ptr<> ...
      tq.release();
    }

  // Register all different handlers, i.e., one per timer.
  test_registering_all_handlers ();

  // Now try multiple timers for ONE event handler (should produce the
  // same result).
  test_registering_one_handler ();

  // Try canceling handlers with odd numbered timer ids.
  test_canceling_odd_timers ();

  // Make sure <reset_timer_inverval> works.
  test_resetting_timer_intervals ();

  ACE_END_TEST;
  return 0;
}
