/**
 * $Id$
 *
 * Scheduling timers with the Reactor
 */

#include "ace/OS_NS_time.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

// Listing 1 code/ch07
class MyTimerHandler : public ACE_Event_Handler
{
public:
  int handle_timeout (const ACE_Time_Value &current_time,
                      const void * = 0)
  {
    time_t epoch = ((timespec_t)current_time).tv_sec;

    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("handle_timeout: %s\n"),
                ACE_OS::ctime (&epoch)));

    return 0;
  }
};
// Listing 1

// Create a SIGINT handler so that we can exit
// the program politely
class SigintHandler : public ACE_Event_Handler
{
public:
  int handle_signal (int signum, siginfo_t * = 0,
                     ucontext_t * = 0)
  {
    if (signum == SIGINT)
      {
        ACE_Reactor::instance ()->end_reactor_event_loop ();
      }
    return 0;
  }
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Listing 2 code/ch07
  MyTimerHandler * timer = new MyTimerHandler ();
  ACE_Time_Value initialDelay (3);
  ACE_Time_Value interval (5);
  ACE_Reactor::instance()->schedule_timer (timer,
                                           0,
                                           initialDelay,
                                           interval);
  // Listing 2

  // Exclude 1
  SigintHandler * handleExit = new SigintHandler ();
  ACE_Reactor::instance()->register_handler (SIGINT,
                                             handleExit);
  // Exclude 1
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}
