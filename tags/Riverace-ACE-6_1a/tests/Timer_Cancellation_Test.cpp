
//=============================================================================
/**
 *  @file    Timer_Cancellation_Test.cpp
 *
 *  $Id$
 *
 *  A test to ensure the timer cancellation works correctly.
 *
 *
 *  @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Task.h"



#if defined (ACE_HAS_THREADS)

class Deadlock : public ACE_Task_Base
{
public:

  int svc (void);

  int handle_timeout (const ACE_Time_Value &current_time,
                      const void *act);
};

int
Deadlock::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Deadlock starts accessing Reactor and Timer Queue....\n")));

  this->reactor ()->schedule_timer (this,
                                    0,
                                    ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Deadlock completes accessing Reactor and Timer Queue....\n")));

  return 0;
}

int
Deadlock::handle_timeout (const ACE_Time_Value &,
                          const void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Deadlock timeout\n")));

  return 0;
}

class Event_Handler : public ACE_Event_Handler
{
public:

  Event_Handler (Deadlock &deadlock);

  int handle_timeout (const ACE_Time_Value &current_time,
                      const void *act);

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask);

  Deadlock &deadlock_;
};

Event_Handler::Event_Handler (Deadlock &deadlock)
  : deadlock_ (deadlock)
{
}

int
Event_Handler::handle_timeout (const ACE_Time_Value &,
                               const void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Event_Handler timeout\n")));

  return -1;
}

int
Event_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Event_Handler closed\n")));

  // Activate Deadlock.
  this->deadlock_.activate ();

  // Give Deadlock a chance to deadlock... ;-)
  ACE_OS::sleep (1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Event Handler starts accessing Reactor....\n")));

  // This is a superfluous call to the Reactor to acquire its lock.
  this->reactor ()->max_notify_iterations ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Event Handler completes accessing Reactor....\n")));

  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Timer_Cancellation_Test"));

  ACE_Reactor reactor (new ACE_TP_Reactor,
                       1);

  Deadlock deadlock;
  deadlock.reactor (&reactor);

  Event_Handler handler (deadlock);

  // Scheduler a timer to kick things off.
  reactor.schedule_timer (&handler,
                          0,
                          ACE_Time_Value (1));

  // Run the event loop for a while.
  ACE_Time_Value timeout (4);
  reactor.run_reactor_event_loop (timeout);

  ACE_END_TEST;

  return 0;
}

#else /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Timer_Cancellation_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;

  return 0;
}

#endif /* ACE_HAS_THREADS */
