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
//      cancelling scheduled timers from multiple threads.
//      No command line arguments are needed to run the test.
//
// = AUTHOR
//    Steve Huston
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
  this->timer_id_[1] = the_reactor->schedule_timer (this,
                                                    (const void *) 1,
                                                    ACE_Time_Value (5));
}


// In the secondary thread, set a heartbeat timer to go off every
// second.  The heartbeat checks the status of things to be sure
// they're being set and expired correctly.

int 
Time_Handler::svc (void)
{
  ACE_Time_Value backstop (30);

  this->timer_id_[2] = the_reactor->schedule_timer(this,
                                                   (const void *) 2,
                                                   ACE_Time_Value (3));
  this->my_reactor_.owner (ACE_OS::thr_self ());
  this->my_reactor_.schedule_timer (this, (const void *) 0,
                                    ACE_Time_Value (1),
                                    ACE_Time_Value (1));
  this->my_reactor_.run_event_loop (backstop);
  return 0;
}

int 
Time_Handler::handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
{
  long time_tag = long (arg);
  ACE_UNUSED_ARG(tv);

  if (time_tag == 0) 
    {	// Heartbeat.
      int i;

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("%T (%t): heartbeat\n")));
      // See if all of the timers have fired.  If so, leave the thread's
      // reactor loop which will exit the thread and end the test.

      for (i = 0; i < Time_Handler::TIMER_SLOTS; i++) 
        if (this->timer_id_[i] != -1)
          break;

      if (i == Time_Handler::TIMER_SLOTS)
        {	// All timers should be gone.

          // Cancel heartbeat.
          ACE_ASSERT (this->my_reactor_.cancel_timer (this) == 1);	

          // Shouldn't be any.
          ACE_ASSERT (the_reactor->cancel_timer (this) == 0); 
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


int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("MT_Reactor_Timer_Test"));

#if defined (ACE_HAS_THREADS)

  Time_Handler other_thread;

  the_reactor = ACE_Reactor::instance ();

  // Set up initial set of timers.
  other_thread.setup ();

  the_reactor->schedule_timer (&other_thread,
                               (const void *) 1,
                               ACE_Time_Value (5));

  other_thread.activate (THR_NEW_LWP | THR_JOINABLE);
  the_reactor->run_event_loop ();
  other_thread.wait ();
#else
  ACE_ERROR ((LM_ERROR, ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}
