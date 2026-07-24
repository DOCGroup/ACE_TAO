
//=============================================================================
/**
 *  @file    Proactor_Timer_Test.cpp
 *
 *    This is a simple test that illustrates the timer mechanism of
 *    the <ACE_Proactor>.  Scheduling timers, handling expired timers and
 *    cancelling scheduled timers are all exercised in this test.
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author and Miljenko Norsic <Miljenko.Norsic@etk.ericsson.se>
 */
//=============================================================================

#include "test_config.h"
#include "ace/Trace.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls or io_uring.

#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_string.h"
#include "ace/Proactor.h"
#include "ace/High_Res_Timer.h"
#include "ace/Asynch_IO.h"
#include "ace/Timer_Heap.h"
#include "ace/Auto_Ptr.h"
#include "Proactor_Test_Backend.h"

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
 * correctly. This class is scheduled with a repeating timer and cancels
 * itself after two expirations. The test then continues dispatching events
 * long enough to verify that canceling the repeat prevents future
 * expirations.
 */
class Repeat_Timer_Handler : public ACE_Handler
{
public:
  static const int REPEAT_INTERVAL = 2;

  Repeat_Timer_Handler (void)
    : expirations_ (0)
    , cancel_result_ (0)
    , failed_ (false)
  {
  }

  // Handle the timeout.
  virtual void handle_time_out (const ACE_Time_Value &tv, const void *arg);

  int expirations (void) const;
  bool failed (void) const;
  int cancel_result (void) const;

private:
  int expirations_;
  int cancel_result_;
  bool failed_;
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

void
Repeat_Timer_Handler::handle_time_out (const ACE_Time_Value &, const void *)
{
  // Let the first one go.
  if (++this->expirations_ == 1)
    return;

  if (this->expirations_ == 2)
    {
      this->cancel_result_ = this->proactor ()->cancel_timer (*this);
      if (this->cancel_result_ != 1)
        {
          this->failed_ = true;
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Repeater cancel timer: %d; should be 1\n"),
                      this->cancel_result_));
        }
    }
  else
    {
      this->failed_ = true;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repeater expiration #%d; should get only 2\n"),
                  this->expirations_));
    }
  return;
}

int
Repeat_Timer_Handler::expirations (void) const
{
  return this->expirations_;
}

bool
Repeat_Timer_Handler::failed (void) const
{
  return this->failed_;
}

int
Repeat_Timer_Handler::cancel_result (void) const
{
  return this->cancel_result_;
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

static int
test_cancel_repeat_timer (void)
{
  Repeat_Timer_Handler handler;
  ACE_Time_Value timeout (Repeat_Timer_Handler::REPEAT_INTERVAL);
  long t_id = ACE_Proactor::instance ()->schedule_repeating_timer
    (handler, 0, timeout);
  if (t_id == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("schedule_repeating_timer")));
      return -1;
    }

  ACE_Time_Value deadline = ACE_OS::gettimeofday ()
    + ACE_Time_Value (3 * Repeat_Timer_Handler::REPEAT_INTERVAL);
  while (handler.expirations () < 2 && ACE_OS::gettimeofday () < deadline)
    {
      ACE_Time_Value wait_time (0, 10000);
      if (ACE_Proactor::instance ()->handle_events (wait_time) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("handle_events")));
          ACE_Proactor::instance ()->cancel_timer (handler);
          return -1;
        }
    }

  if (handler.expirations () != 2 || handler.cancel_result () != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Repeater expired %d times, cancel result %d; ")
                  ACE_TEXT ("expected 2 expirations and cancel result 1\n"),
                  handler.expirations (),
                  handler.cancel_result ()));
      ACE_Proactor::instance ()->cancel_timer (handler);
      return -1;
    }

  deadline = ACE_OS::gettimeofday ()
    + ACE_Time_Value (Repeat_Timer_Handler::REPEAT_INTERVAL + 1);
  while (!handler.failed () && ACE_OS::gettimeofday () < deadline)
    {
      ACE_Time_Value wait_time (0, 10000);
      if (ACE_Proactor::instance ()->handle_events (wait_time) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("handle_events")));
          return -1;
        }
    }

  if (handler.failed () || handler.expirations () != 2)
    return -1;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Repeater expired twice; correct\n")));
  return 0;
}


// If any command line arg is given (other than -t u), run the test with
// high res timer queue. Else run it normally.
int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_Proactor *proactor = 0;
  Proactor_Test_Backend::Type backend = Proactor_Test_Backend::BACKEND_DEFAULT;

  ACE_START_TEST (ACE_TEXT ("Proactor_Timer_Test"));

  // Determine whether to run with high-res timer queue.  Ignore the
  // backend selection arguments so they don't accidentally trigger
  // this branch.
  bool use_hires = false;
  for (int i = 1; i < argc; ++i)
    {
      if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-t")) == 0)
        {
          if (i + 1 >= argc
              || Proactor_Test_Backend::parse_type (argv[i + 1], backend) != 0)
            {
              Proactor_Test_Backend::print_type_usage (argv[0]);
              ACE_END_TEST;
              return -1;
            }
          ++i; // skip the type argument
          continue;
        }
      use_hires = true;
    }

  if (Proactor_Test_Backend::create_proactor (backend, 128, proactor, true) != 0)
    {
      ACE_END_TEST;
      return -1;
    }

  if (use_hires)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Running with high-res timer queue\n")));
      ACE_Proactor *r = ACE_Proactor::instance ();

      (void) ACE_High_Res_Timer::global_scale_factor ();

      // Change the source of time in the Proactor to the
      // high-resolution  timer.  Why does this test require such
      // precision for a 1 second timer is beyond me ...  I think it
      // is a cut&paste error.
      //
      // The use of auto_ptr<> is optional, ACE uses dangerous memory
      // management idioms everywhere, I thought I could demonstrate how
      // to do it right in at least one test.  Notice the lack of
      // ACE_NEW_RETURN, that monstrosity has no business in proper C++
      // code ...
      typedef ACE_Timer_Heap_T<ACE_Handler*,ACE_Proactor_Handle_Timeout_Upcall,ACE_SYNCH_RECURSIVE_MUTEX,ACE_FPointer_Time_Policy> Timer_Queue;

      ACE_Auto_Ptr<Timer_Queue> tq(new Timer_Queue);
      // ... notice how the policy is in the derived timer queue type.
      // The abstract timer queue does not have a time policy ...
      tq->set_time_policy(&ACE_High_Res_Timer::gettimeofday_hr);
      // ... and then the timer queue is replaced.  Strangely, the
      // Proactor does *not* copy the timers, it just deletes the
      // existing timer queue ....
      r->timer_queue(tq.get());
      // ... the Proactor has assumed ownership, release the
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

  int status = 0;
  if (test_cancel_repeat_timer () != 0)
    status = -1;

#if defined (ACE_WIN32)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Skipping ACE_Proactor::close_singleton() on Windows test shutdown\n")));
#else
  ACE_Proactor::close_singleton ();
#endif
  ACE_END_TEST;
  return status;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Timer_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_Timer_Test will not be run.\n")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
