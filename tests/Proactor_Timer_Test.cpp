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

ACE_RCSID (tests,
           Proactor_Timer_Test,
           "$Id$")

#if ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls.

#include "ace/Timer_Queue.h"
#include "ace/Proactor.h"
#include "ace/High_Res_Timer.h"
#include "ace/Asynch_IO.h"

static int    done = 0;
static size_t count = 0;
static int    odd = 0;

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

Time_Handler::Time_Handler (void)
  : timer_id_ (-1)
{
  // Nothing
}

void
Time_Handler::handle_time_out (const ACE_Time_Value &, const void *arg)
{
  size_t current_count = *(ACE_reinterpret_cast (const size_t *, arg));
  if (current_count != count)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Expected timer %d, not %d\n"),
                count,
                current_count));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%@] Timer id %d with count #%d|%d expired.\n"),
              this,
              this->timer_id (),
              count,
              current_count));

  if (current_count == (ACE_MAX_TIMERS - 1))
    done = 1;
  else if (count == ACE_MAX_TIMERS - 1)
    {
      done = 1;
      return;
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
  long t_id[ACE_MAX_TIMERS];
  size_t which[ACE_MAX_TIMERS];
  long secs = 0;
  size_t i = 0;
  for ( ; i < ACE_MAX_TIMERS; i++, secs++)
    {
      which[i] = i;
      t_id[i] =
        ACE_Proactor::instance ()->schedule_timer
            (rt[i], &which[i], ACE_Time_Value (2 * secs + 1));
      if (t_id[i] == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("schedule_timer")));
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
  long t_id[ACE_MAX_TIMERS];
  size_t which[ACE_MAX_TIMERS];

  done = 0;
  count = 0;
  long secs = 0;
  size_t i = 0;
  for ( ; i < ACE_MAX_TIMERS; i++, secs++)
    {
      which[i] = i;
      t_id[i] =
        ACE_Proactor::instance ()->schedule_timer
          (rt[0], &which[i], ACE_Time_Value (2 * secs + 1));
      if (t_id[i] == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("schedule_timer")));
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
  long t_id[ACE_MAX_TIMERS];
  size_t which[ACE_MAX_TIMERS];

  done = 0;
  count = 1;
  odd = 1;
  size_t i = 0;
  long secs = 0;
  for ( ; i < ACE_MAX_TIMERS; i++, secs++)
    {
      which[i] = i;
      t_id[i] = ACE_Proactor::instance ()->schedule_timer
        (rt[i], &which[i], ACE_Time_Value (2 * secs + 1));
      if (t_id[i] == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("schedule_timer")));
      rt[i].timer_id (t_id[i]);
    }

  for (i = 0; i < ACE_MAX_TIMERS; i++)
    // Cancel handlers with odd numbered timer ids.
    if (ACE_ODD (rt[i].timer_id ()))
      {
        if (ACE_Proactor::instance ()->cancel_timer (rt[i].timer_id ()) == -1)
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("cancel_timer")));
      }

  while (!done)
    ACE_Proactor::instance ()->handle_events ();
}

// If any command line arg is given, run the test with high res timer
// queue. Else run it normally.
int
run_main (int argc, ACE_TCHAR *[])
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

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Timer_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_Timer_Test will not be run.")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS */
