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

#if defined (ACE_HAS_THREADS)

static ACE_Reactor *	TheReactor;


Time_Handler::Time_Handler()
{
    int i;
    for (i = 0; i < TIMER_SLOTS; timer_id[i++] = -1)
      ;
}


/*
** Set up initial timer conditions.
*/
void Time_Handler::setup(void)
{

  timer_id[1] = TheReactor->schedule_timer(this,
					   (const void *)1,
					   ACE_Time_Value(5));
  return;

}


/*
** In the secondary thread, set a heartbeat timer to go off every second.
** The heartbeat checks the status of things to be sure they're being
** set and expired correctly.
*/
int Time_Handler::svc(void)
{

ACE_Time_Value	backstop(30);

  timer_id[2] = TheReactor->schedule_timer(this,
					   (const void *)2,
					   ACE_Time_Value(3));
  my_reactor.owner(ACE_OS::thr_self());
  my_reactor.schedule_timer(this, (const void *)0,
			    ACE_Time_Value(1), ACE_Time_Value(1));
  my_reactor.run_event_loop(backstop);
  return 0;

}


int Time_Handler::handle_timeout (const ACE_Time_Value &tv,
				  const void *arg)
{

long time_tag = long (arg);
ACE_UNUSED_ARG(tv);

    if (time_tag == 0) {	/* Heartbeat */
      int i;
      ACE_DEBUG((LM_DEBUG, "%T (%t): heartbeat\n"));
      /*
      ** See if all of the timers have fired.  If so, leave the thread's
      ** reactor loop which will exit the thread and end the test.
      */
      for (i = 0; i < TIMER_SLOTS; i++) {
	if (timer_id[i] != -1)
	  break;
      }
      if (i == TIMER_SLOTS) {	/* All timers should be gone */
	ACE_ASSERT(my_reactor.cancel_timer(this) == 1);	/* Cancel heartbeat */
	ACE_ASSERT(TheReactor->cancel_timer(this) == 0); /* Shouldn't be any */
	my_reactor.end_event_loop();
      }
      return 0;
    }

    ACE_DEBUG((LM_DEBUG, "%T (%t): Timer #%d (id #%d) expired\n",
	       time_tag, timer_id[time_tag]));
    ACE_ASSERT (timer_id[time_tag] != -1);
    timer_id[time_tag] = -1;

    return 0;

}

#endif /* ACE_HAS_THREADS */


int
main (int, char *[])
{
  ACE_START_TEST ("MT_Reactor_Timer_Test");

#if defined (ACE_HAS_THREADS)

  Time_Handler	other_thread;

  TheReactor = ACE_Reactor::instance();

  other_thread.setup();		/* Set up initial set of timers */

  TheReactor->schedule_timer(&other_thread,
			     (const void *)1,
			     ACE_Time_Value(5));

  other_thread.activate(THR_NEW_LWP | THR_JOINABLE);
  TheReactor->run_event_loop();
  other_thread.wait();

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}
