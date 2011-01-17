// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    MT_Reactor_Timer_Test.cpp
//
// = DESCRIPTION
//      This is a simple test that illustrates the timer mechanism of
//      the reactor scheduling timers, handling expired timers and
//      cancelling scheduled timers from multiple threads.  No command
//      line arguments are needed to run the test.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "test_config.h"
#include "MT_Reactor_Timer_Test.h"
#include "ace/ACE.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_THREADS)

// This test exercises the setting and cancelling of timers from a
// thread other than the one the reactor is running in.  It sets up an
// initial set of timers (3, 4, 5 seconds) from the main thread.  When
// the second thread starts, it cancels the 3 second timer and sets a
// 2-second timer and an already-expired timer, which should be the
// first to fire.  It then sleeps for 3 seconds (letting the 2 second
// timer fire, and if things are slow, the 4 second timer will also
// fire.  Then it sets 2 more timers at 10 and 12 seconds and cancels
// the original 5 second timer.  Then returns, ending the thread.  The
// destructor for Time_Handler insures that everything happened
// correctly.

Time_Handler::Time_Handler (void)
{
  for (int i = 0;
       i < Time_Handler::TIMER_SLOTS;
       this->timer_id_[i++] = Time_Handler::TIMER_NOTSET)
    continue;

  this->prev_timer_ = -1;
}

// Set up initial timer conditions.  Timers set up at 3, 4, and 5
// seconds.  The one at 3 seconds will get cancelled when the thread
// starts.

void
Time_Handler::setup (void)
{
  ACE_Reactor *r = ACE_Reactor::instance ();

  this->timer_id_[2] = r->schedule_timer (this,
                                          (const void *) 2,
                                          ACE_Time_Value (3));
  this->timer_id_[3] = r->schedule_timer (this,
                                          (const void *) 3,
                                          ACE_Time_Value (4));
  this->timer_id_[4] = r->schedule_timer (this,
                                          (const void *) 4,
                                          ACE_Time_Value (5));
  return;
}

int
Time_Handler::verify_results (void)
{
  ACE_TEST_ASSERT (this->timer_id_[0] == Time_Handler::TIMER_FIRED);
  ACE_TEST_ASSERT (this->timer_id_[1] == Time_Handler::TIMER_FIRED);
  ACE_TEST_ASSERT (this->timer_id_[2] == Time_Handler::TIMER_CANCELLED);
  ACE_TEST_ASSERT (this->timer_id_[3] == Time_Handler::TIMER_FIRED);
  ACE_TEST_ASSERT (this->timer_id_[4] == Time_Handler::TIMER_CANCELLED);
  ACE_TEST_ASSERT (this->timer_id_[5] == Time_Handler::TIMER_FIRED);
  ACE_TEST_ASSERT (this->timer_id_[6] == Time_Handler::TIMER_FIRED);

  for (int i = 7; i < Time_Handler::TIMER_SLOTS; i++)
    ACE_TEST_ASSERT (this->timer_id_[i] == Time_Handler::TIMER_NOTSET);

  return 0;
}

int
Time_Handler::svc (void)
{
  ACE_Reactor *r = ACE_Reactor::instance ();

  ACE_TEST_ASSERT (r->cancel_timer (this->timer_id_[2]) == 1);
  this->timer_id_[2] = Time_Handler::TIMER_CANCELLED;

  this->timer_id_[1] = r->schedule_timer(this,
                                         (const void *) 1,
                                         ACE_Time_Value (2));
  // This one may get the callback before we return, so serialize.
  this->lock_.acquire ();
  this->timer_id_[0] = r->schedule_timer(this,
                                         (const void *) 0,
                                         ACE_Time_Value (0, -5));
  this->lock_.release ();
  ACE_OS::sleep(3);

  this->timer_id_[5] = r->schedule_timer(this,
                                         (const void *)5,
                                         ACE_Time_Value (10));
  this->timer_id_[6] = r->schedule_timer(this,
                                         (const void *)6,
                                         ACE_Time_Value (12));

  ACE_TEST_ASSERT (r->cancel_timer (this->timer_id_[4]) == 1);
  this->timer_id_[4] = Time_Handler::TIMER_CANCELLED;

  return 0;
}

int
Time_Handler::handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
{
  long time_tag = static_cast <long> (reinterpret_cast <size_t> (arg));
  ACE_UNUSED_ARG(tv);

  ACE_GUARD_RETURN (ACE_Thread_Mutex, id_lock, this->lock_, 0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%T (%t): Timer #%d (id #%d) expired\n"),
              time_tag,
              this->timer_id_[time_tag]));

  ACE_TEST_ASSERT (time_tag > this->prev_timer_);
  ACE_TEST_ASSERT (this->timer_id_[time_tag] != Time_Handler::TIMER_NOTSET);
  ACE_TEST_ASSERT (this->timer_id_[time_tag] != Time_Handler::TIMER_CANCELLED);
  ACE_TEST_ASSERT (this->timer_id_[time_tag] != Time_Handler::TIMER_FIRED);
  this->timer_id_[time_tag] = Time_Handler::TIMER_FIRED;
  this->prev_timer_ = time_tag;

  return 0;
}

#endif /* ACE_HAS_THREADS */

Dispatch_Count_Handler::Dispatch_Count_Handler (void)
{

  ACE_Reactor *r = ACE_Reactor::instance ();

  this->input_seen_ = this->notify_seen_ = 0;
  this->timers_fired_ = 0;

  // Initialize the pipe.
  if (this->pipe_.open () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_Pipe::open")));
  // Register the "read" end of the pipe.
  else if (r->register_handler (this->pipe_.read_handle (),
                                this,
                                ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("register_handler")));
  // Put something in our pipe and smoke it... ;-)
  else if (ACE::send (this->pipe_.write_handle (),
                      "z",
                      1) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("send")));
  // Call notify to prime the pump for this, as well.
  else if (r->notify (this) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("notify")));
}

int
Dispatch_Count_Handler::handle_close (ACE_HANDLE h,
                                      ACE_Reactor_Mask m)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%T (%t): handle_close\n")));

  ACE_TEST_ASSERT (h == this->pipe_.read_handle ()
              && m == ACE_Event_Handler::READ_MASK);

  return 0;
}

int
Dispatch_Count_Handler::handle_input (ACE_HANDLE h)
{
  char c;

  ACE_TEST_ASSERT (this->input_seen_ == 0);
  this->input_seen_ = 1;

  if (ACE::recv (h, &c, 1) != 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("recv")),
                      -1);

  ACE_TEST_ASSERT (c == 'z');
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%T (%t): handle_input\n")));
  // Trigger the <handle_close> hook.
  return -1;
}

int
Dispatch_Count_Handler::handle_exception (ACE_HANDLE h)
{
  ACE_UNUSED_ARG (h);

  ACE_TEST_ASSERT (this->notify_seen_ == 0);
  this->notify_seen_ = 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%T (%t): handle_exception\n")));
  return 0;
}

int
Dispatch_Count_Handler::handle_timeout (const ACE_Time_Value &tv,
                                        const void *arg)
{
  ACE_UNUSED_ARG (tv);

  ++this->timers_fired_;

  long value = static_cast <long> (reinterpret_cast <size_t> (arg));

  // This case just tests to make sure the Reactor is counting timer
  // expiration correctly.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%T (%t): expiration %d\n"),
              value));
  return 0;
}

int
Dispatch_Count_Handler::verify_results (void)
{
  int result = 0;

  if (this->input_seen_ != 1)
    {
      ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("input_seen_ is not 1 but %d\n"),
                 input_seen_));
    result = -1;
  }

  if (this->notify_seen_ != 1)
    {
      ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("notify_seen_ is not 1 but %d\n"),
                 notify_seen_));
    result = -1;
  }

  if (this->timers_fired_ != ACE_MAX_TIMERS)
    {
      ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("timers fired not equal max timers: %d != %d\n"),
                 this->timers_fired_,
                 ACE_MAX_TIMERS));
    result = -1;
  }

  return result;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MT_Reactor_Timer_Test"));

  int status = 0;
  int test_result = 0;

  ACE_Reactor *r = ACE_Reactor::instance ();

  Dispatch_Count_Handler callback;

  for (int i = ACE_MAX_TIMERS; i > 0; i--)
    // Schedule a timeout to expire immediately.
    if (r->schedule_timer (&callback,
                           reinterpret_cast <const void *> (static_cast <size_t> (i)),
                           ACE_Time_Value (0)) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("schedule_timer")),
                        1);

  ACE_Time_Value no_waiting (0);
  size_t events = 0;

  while (1)
    {
      int result = r->handle_events (no_waiting);

      // Timeout.
      if (result == 0)
        break;

      // Make sure there were no errors.
      ACE_TEST_ASSERT (result != -1);

      events += result;
    }

  // All <ACE_MAX_TIMERS> + 2 I/O dispatches (one for <handle_input>
  // and the other for <handle_exception>) should be counted in
  // events.
  if (events < ACE_MAX_TIMERS + 2)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("expected %d events, got %d instead\n"),
                  ACE_MAX_TIMERS + 2,
                  events));
    }

  status = callback.verify_results ();
  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Dispatch counting test failed.\n")));
      test_result = 1;
    }

#if defined (ACE_HAS_THREADS)

  Time_Handler other_thread;
  ACE_Time_Value time_limit (30);

  // Set up initial set of timers.
  other_thread.setup ();

  other_thread.activate (THR_NEW_LWP | THR_JOINABLE);
  status = ACE_Reactor::instance()->run_reactor_event_loop (time_limit);
  // Should have returned only because the time limit is up...
  ACE_TEST_ASSERT (status != -1);
  ACE_TEST_ASSERT (time_limit.sec () == 0);

  status = other_thread.wait ();
  if (status == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p, errno is %d\n"),
                  "wait ()",
                  ACE_ERRNO_GET));
      ACE_TEST_ASSERT (status != -1);
    }

  status = other_thread.verify_results ();
  if (status != 0)
    test_result = 1;

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return test_result;
}
