
//=============================================================================
/**
 *  @file    Proactor_Timer_Test.cpp
 *
 *  $Id$
 *
 *    This is a simple test that illustrates the timer mechanism of
 *    the <ACE_Proactor>.  Scheduling timers, handling expired timers and
 *    cancelling scheduled timers are all exercised in this test.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author and Miljenko Norsic <Miljenko.Norsic@etk.ericsson.se>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Trace.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls.

#include "ace/OS_NS_unistd.h"
#include "ace/Timer_Queue.h"
#include "ace/Proactor.h"
#include "ace/High_Res_Timer.h"
#include "ace/Asynch_IO.h"

static int    done = 0;
static size_t counter = 0;
static int    odd = 0;

class Time_Handler : public ACE_Handler
{
public:
  /// Default constructor
  Time_Handler ();

  /// Handle the timeout.
  virtual void handle_time_out (const ACE_Time_Value &tv, const void *arg);

  /// Return our timer id.
  long timer_id (void) const;

  /// Set our timer id;
  void timer_id (long);

private:
  /// Stores the id of this timer.
  long timer_id_;
};

/*
 * Need a variant of this that will track if a repeating timer is working
 * correctly. This class should be scheduled with a repeating timer that
 * repeats on a specified number of seconds. This class will let two
 * expirations happen then wait in handle_time_out() longer than the repeat
 * time to cause at least one timer expiration to be queued up while we're
 * waiting; then cancel the timer.
 */
class Repeat_Timer_Handler : public ACE_Handler
{
public:
  static const int REPEAT_INTERVAL = 2;

  // Constructor arg tells how many seconds we intend to do the repeat with.
  // The internals will use this to tell how long to wait in order to cause
  // a timer expiration to be missed and queued up.
  Repeat_Timer_Handler (const int repeat_time = REPEAT_INTERVAL)
    : repeat_secs_ (repeat_time), expirations_ (0) {};

  ~Repeat_Timer_Handler ();

  // Handle the timeout.
  virtual void handle_time_out (const ACE_Time_Value &tv, const void *arg);

private:
  int repeat_secs_;
  int expirations_;
};


Time_Handler::Time_Handler (void)
  : timer_id_ (-1)
{
  // Nothing
}

void
Time_Handler::handle_time_out (const ACE_Time_Value &, const void *arg)
{
  size_t current_counter = *(reinterpret_cast<const size_t *> (arg));
  if (current_counter != counter)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Expected timer %d, not %d\n"),
                counter,
                current_counter));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%@] Timer id %d with counter #%d|%d expired.\n"),
              this,
              this->timer_id (),
              counter,
              current_counter));

  if (current_counter == (ACE_MAX_TIMERS - 1))
    done = 1;
  else if (counter == ACE_MAX_TIMERS - 1)
    {
      done = 1;
      return;
    }

  counter += (1 + odd);
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

Repeat_Timer_Handler::~Repeat_Timer_Handler ()
{
  if (this->expirations_ == 2)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Repeater expired twice; correct\n")));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Repeater expired %d times; should be 2\n"),
                this->expirations_));
}

void
Repeat_Timer_Handler::handle_time_out (const ACE_Time_Value &, const void *)
{
  // Let the first one go.
  if (++this->expirations_ == 1)
    return;

  if (this->expirations_ == 2)
    {
      ACE_OS::sleep (this->repeat_secs_ + 1);
      int canceled = this->proactor ()->cancel_timer (*this);
      if (canceled != 1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Repeater cancel timer: %d; should be 1\n"),
                      canceled));
        }
      delete this;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repeater expiration #%d; should get only 2\n"),
                  this->expirations_));
    }
  return;
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
  counter = 0;
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
  counter = 1;
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

static void
test_cancel_repeat_timer (void)
{
  Repeat_Timer_Handler *handler = new Repeat_Timer_Handler;
  ACE_Time_Value timeout (Repeat_Timer_Handler::REPEAT_INTERVAL);
  long t_id = ACE_Proactor::instance ()->schedule_repeating_timer
    (*handler, 0, timeout);
  if (t_id == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("schedule_repeating_timer")));
      delete handler;
      return;
    }

  ACE_Time_Value test_timer (4 * Repeat_Timer_Handler::REPEAT_INTERVAL);
  if (-1 == ACE_Proactor::instance ()->proactor_run_event_loop (test_timer))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("proactor loop fail")));

  // handler should be deleted by its own handle_time_out().
  return;
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

  test_cancel_repeat_timer ();

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

#endif  /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
