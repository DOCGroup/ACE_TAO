/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Reactor_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <ACE_CLASSIX_Select_Reactor> class.
//     based on $ACE_ROOT/tests/MT_Reactor_Timer_Test.cpp
//     
// = AUTHOR
//    Wei Chiang
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Select_Reactor.h"

#include "CLASSIX_test_config.h"
#include "CLASSIX_Reactor_Test.h"

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

  while (!ACE_Reactor::event_loop_done())
  {
      int result = this->my_reactor_.handle_events (backstop);
      if (result == -1)
         break;
      ACE_Thread::yield();
  }

  ACE_DEBUG((LM_DEBUG, "(%T(%t) heartbeat's event loop ended\n"));

  return 0;
}

int 
Time_Handler::handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
{
  long time_tag = long (arg);
  ACE_UNUSED_ARG(tv);

  if (time_tag == 0) 
    {   // Heartbeat.
      int i;

      ACE_DEBUG ((LM_DEBUG,
                  "%T (%t): heartbeat...\n"));
      // See if all of the timers have fired.  If so, leave the thread's
      // reactor loop which will exit the thread and end the test.

      for (i = 0; i < Time_Handler::TIMER_SLOTS; i++) 
        if (this->timer_id_[i] != -1)
          break;

      if (i == Time_Handler::TIMER_SLOTS)
        {       // All timers should be gone.

          // Cancel heartbeat.
          ACE_ASSERT (this->my_reactor_.cancel_timer (this) == 1);

          // Shouldn't be any.
          ACE_ASSERT (the_reactor->cancel_timer (this) == 0); 
          this->my_reactor_.end_event_loop ();
        }
      ACE_DEBUG ((LM_DEBUG,
                  "%T (%t): ...heartbeat\n"));
      return 0;
    }

  ACE_DEBUG ((LM_DEBUG,
              "%T (%t): Timer #%d (id #%d) expired\n",
              time_tag,
              this->timer_id_[time_tag]));

  ACE_ASSERT (this->timer_id_[time_tag] != -1);
  this->timer_id_[time_tag] = -1;

  return 0;
}


//===============================================================//

int
main (int, char *[])
{
  ACE_START_TEST ("Reactor_Test");

  // CLASSIX environment initialization
  ACE_CLASSIX_OS classix;
  the_reactor = ACE_Reactor::instance();
  Time_Handler other_thread;


  // Set up initial set of timers.
  other_thread.setup ();

  other_thread.activate (THR_NEW_LWP | THR_JOINABLE);
  the_reactor->run_event_loop ();
  ACE_DEBUG((LM_DEBUG, "(%t) main thread's event loop ended\n"));
  other_thread.wait ();


  ACE_END_TEST;
  return 0;
}

