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

ACE_RCSID(tests, MT_Reactor_Timer_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

static ACE_Reactor *the_reactor;

Time_Handler::Time_Handler (void)
{
  for (int i = 0;
       i < Time_Handler::TIMER_SLOTS;
       this->timer_id_[i++] = -1)
    continue;
}

// Set up initial timer conditions.

void 
Time_Handler::setup (void)
{
  this->timer_id_[1] = the_reactor->schedule_timer 
    (this,
     (const void *) 1,
     ACE_Time_Value (5));

  ACE_ASSERT (this->timer_id_[1] != -1);
}

// In the secondary thread, set a heartbeat timer to go off every
// second.  The heartbeat checks the status of things to be sure
// they're being set and expired correctly.

int 
Time_Handler::svc (void)
{
  ACE_Time_Value backstop (10);

  this->timer_id_[2] = the_reactor->schedule_timer 
    (this,
     (const void *) 2,
     ACE_Time_Value (3));

  ACE_ASSERT (this->timer_id_[2] != -1);

  this->my_reactor_.owner (ACE_OS::thr_self ());

  long result = this->my_reactor_.schedule_timer 
    (this,
     (const void *) 0,
     ACE_Time_Value (1),
     ACE_Time_Value (1)) != -1;
  ACE_ASSERT (result != -1);
  
  for (;;)
    {
      result = this->my_reactor_.handle_events (backstop);
      ACE_ASSERT (result != -1);
      if (result == 0)
        break;
    }

  ACE_ASSERT (result != -1);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("%T (%t): leaving svc thread\n")));              
  return 0;
}

int 
Time_Handler::handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
{
  long time_tag = ACE_reinterpret_cast (long, arg);
  ACE_UNUSED_ARG(tv);

  if (time_tag == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("%T (%t): heartbeat from main thread\n")));
      return 0;
    }
  if (time_tag == 0) 
    {	// Heartbeat.
      int i;

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("%T (%t): heartbeat\n")));

      // See if all of the timers have fired.  If so, leave the
      // thread's reactor loop, which will exit the thread and end the
      // test.

      for (i = 0; i < Time_Handler::TIMER_SLOTS; i++) 
        if (this->timer_id_[i] != -1)
          break;

      // All timers should be gone.
      if (i == Time_Handler::TIMER_SLOTS)
        {	
          // Cancel heartbeat.
          i = this->my_reactor_.cancel_timer (this);
          ACE_ASSERT (i == 1);

          i = the_reactor->cancel_timer (this);
          // Shouldn't be any.
          ACE_ASSERT (i == 0);

          this->my_reactor_.end_event_loop ();
        }

      return 0;
    }

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("%T (%t): Timer #%d (id #%d) expired\n"),
              time_tag,
              this->timer_id_[time_tag]));

  ACE_ASSERT (this->timer_id_[time_tag] != -1);
  this->timer_id_[time_tag] = -1;

  return 0;
}

#endif /* ACE_HAS_THREADS */

Dispatch_Count_Handler::Dispatch_Count_Handler (void)
{
  // Initialize the pipe.
  if (this->pipe_.open () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_Pipe::open")));
  // Register the "read" end of the pipe.
  else if (the_reactor->register_handler (this->pipe_.read_handle (),
                                          this,
                                          ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("register_handler")));
  // Put something in our pipe and smoke it... ;-)
  else if (ACE_OS::write (this->pipe_.write_handle (),
                          "z",
                          1) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("write")));
  // Call notify to prime the pump for this, as well.
  else if (the_reactor->notify (this) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("notify")));
}

int
Dispatch_Count_Handler::handle_close (ACE_HANDLE h,
                                      ACE_Reactor_Mask m)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("%T (%t): handle_close\n")));

  ACE_ASSERT (h == this->pipe_.read_handle ()
              && m == ACE_Event_Handler::READ_MASK);
  
  if (the_reactor->remove_handler (this->pipe_.read_handle (),
                                   ACE_Event_Handler::READ_MASK 
                                   | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("remove_handler")),
                      -1);
  return 0;
}

int 
Dispatch_Count_Handler::handle_input (ACE_HANDLE h)
{
  char c;

  if (ACE_OS::read (h, &c, 1) != 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("read")),
                      -1);

  ACE_ASSERT (c == 'z');
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("%T (%t): handle_input\n")));
  // Trigger the <handle_close> hook.
  return -1;
}

int 
Dispatch_Count_Handler::handle_exception (ACE_HANDLE h)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("%T (%t): handle_exception\n")));
  return 0;
}

int 
Dispatch_Count_Handler::handle_timeout (const ACE_Time_Value &tv,
                                        const void *arg)
{
  long value = ACE_reinterpret_cast (long, arg);
  
  // This case just tests to make sure the Reactor is counting timer
  // expiration correctly.
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("%T (%t): expiration %d\n"),
              value));
  return 0;
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("MT_Reactor_Timer_Test"));

  the_reactor = ACE_Reactor::instance ();

  Dispatch_Count_Handler callback;

  for (int i = ACE_MAX_TIMERS; i > 0; i--)
    // Schedule a timeout to expire immediately.
    if (the_reactor->schedule_timer (&callback,
                                     (const void *) i,
                                     ACE_Time_Value (0)) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("%p\n"),
                         ASYS_TEXT ("schedule_timer")),
                        1);
  
  int result = the_reactor->handle_events ();
  // All <ACE_MAX_TIMERS> should be counted in the return value + 2
  // I/O dispatches (one for <handle_input> and the other for
  // <handle_exception>).
  ACE_ASSERT (result == ACE_MAX_TIMERS + 2);

#if defined (ACE_HAS_THREADS)
  Time_Handler other_thread;

  // Set up initial set of timers.
  other_thread.setup ();

  // Schedule a timeout to expire in 5 seconds.
  result = the_reactor->schedule_timer (&other_thread,
                                        (const void *) -1,
                                        ACE_Time_Value (5));
  ACE_ASSERT (result != -1);

  result = other_thread.activate (THR_NEW_LWP | THR_JOINABLE);
  ACE_ASSERT (result != -1);

  // Note that we can only call run_event_loop() on the Singleton
  // thread!
  result = the_reactor->run_event_loop ();
  ACE_ASSERT (result != -1);

  result = other_thread.wait ();
  ACE_ASSERT (result != -1);
#else
  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}
