// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Proctor_Timer_Test.cpp
//
// = DESCRIPTION
//      This is a simple test that illustrates the timer mechanism of
//      the <ACE_Proactor>.  Scheduling timers, handling expired timers and
//      cancelling scheduled timers are all exercised in this test. 
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu>, 
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>, and
//    Miljenko Norsic <Miljenko.Norsic@etk.ericsson.se>
//
// ============================================================================

#include "test_config.h"
#include "ace/Timer_Queue.h"
#include "ace/Proactor.h"
#include "ace/High_Res_Timer.h"
#include "ace/Asynch_IO.h"

static int done = 0;
static int count = 0;
static int odd = 0;

class Time_Handler : public ACE_Handler
{
public:
  Time_Handler ();
  // Default constructor

  virtual void handle_time_out (const ACE_Time_Value &tv, const void *arg);
  // Handle the timeout.

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

void
Time_Handler::handle_time_out (const ACE_Time_Value &tv, const void *arg)
{
  long current_count = ACE_reinterpret_cast (long, arg);
  if (current_count >= 0)
    ACE_ASSERT (current_count == count);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%x] Timer id %d with count #%d|%d timed out at %d!\n"),
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
      return;
    }
  else if (current_count == -1)
    {
      int result = ACE_Proactor::instance ()->cancel_timer (this->timer_id ());
      result = ACE_Proactor::instance ()->schedule_timer(*this,
                                                         (const void *) -1,
                                                         ACE_Time_Value(count + 1));
      ACE_ASSERT (result != -1);
      this->timer_id(result);
    }
  count += (1 + odd);
  return;
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
test_registering_all_handlers (void)
{
  ACE_Trace t (ACE_TEXT ("test_registering_all_handler"), 
               __LINE__, 
               ACE_TEXT_CHAR_TO_TCHAR (__FILE__));
  Time_Handler rt[ACE_MAX_TIMERS];
  int t_id[ACE_MAX_TIMERS];

  for (size_t i = 0; i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] =
        ACE_Proactor::instance ()->schedule_timer (rt[i], 
                                                  (const void *) i, 
                                                  ACE_Time_Value (2 * i + 1));
      ACE_ASSERT (t_id[i] != -1);
      rt[i].timer_id (t_id[i]);
    }

  while (!done)
    ACE_Proactor::instance ()->handle_events ();
}

static void
test_registering_one_handler (void)
{
  ACE_Trace t (ACE_TEXT ("test_registering_one_handler"), 
               __LINE__, 
               ACE_TEXT_CHAR_TO_TCHAR (__FILE__));
  Time_Handler rt[ACE_MAX_TIMERS];
  int t_id[ACE_MAX_TIMERS];

  done = 0;
  count = 0;

  for (size_t i = 0; (u_long) i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] =
        ACE_Proactor::instance ()->schedule_timer (rt[0],
                                                  (const void *) i,
                                                  ACE_Time_Value (2 * i + 1));
      ACE_ASSERT (t_id[i] != -1);
    }

  while (!done)
    ACE_Proactor::instance ()->handle_events ();
}

static void
test_canceling_odd_timers (void)
{
  ACE_Trace t (ACE_TEXT ("test_canceling_odd_timers"), 
               __LINE__, 
               ACE_TEXT_CHAR_TO_TCHAR (__FILE__));
  Time_Handler rt[ACE_MAX_TIMERS];
  int t_id[ACE_MAX_TIMERS];

  done = 0;
  count = 1;
  odd = 1;

  for (size_t i = 0; (u_long) i < ACE_MAX_TIMERS; i++)
    {
      t_id[i] = ACE_Proactor::instance ()->schedule_timer (rt[i],
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
          ACE_Proactor::instance ()->cancel_timer (rt[j].timer_id ());
        ACE_ASSERT (result != -1);
      }

  while (!done)
    ACE_Proactor::instance ()->handle_events ();
}

// If any command line arg is given, run the test with high res timer
// queue. Else run it normally.
int
main (int argc, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Timer_Test"));

  if (argc > 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Running with high-res timer queue\n")));
      ACE_Proactor *r = ACE_Proactor::instance ();
      (void) ACE_High_Res_Timer::global_scale_factor ();
      r->timer_queue ()->gettimeofday (&ACE_High_Res_Timer::gettimeofday_hr);
    }

  // Register all different handlers, i.e., one per timer.
  test_registering_all_handlers ();

  // Now try multiple timers for ONE event handler (should produce the
  // same result).
  test_registering_one_handler ();

  // Try canceling handlers with odd numbered timer ids.
  test_canceling_odd_timers ();

  ACE_END_TEST;
  return 0;
}
