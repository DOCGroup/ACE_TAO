/**
 * Scheduling timers with the Reactor
 */

#include "ace/OS_NS_time.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Signal.h"

// Listing 1 code/ch07
class MyTimerHandler : public ACE_Event_Handler
{
public:
  MyTimerHandler () = default;
  MyTimerHandler (MyTimerHandler const &) = delete;
  MyTimerHandler (MyTimerHandler &&) = delete;
  MyTimerHandler &operator= (MyTimerHandler const &) = delete;
  MyTimerHandler &operator= (MyTimerHandler &&) = delete;

  int handle_timeout (ACE_Time_Value const &current_time,
                      void const * = nullptr) override
  {
    time_t const epoch = static_cast<timespec_t> (current_time).tv_sec;

    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("handle_timeout: %C\n"),
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
  SigintHandler () = default;
  SigintHandler (SigintHandler const &) = delete;
  SigintHandler (SigintHandler &&) = delete;
  SigintHandler &operator= (SigintHandler const &) = delete;
  SigintHandler &operator= (SigintHandler &&) = delete;

  int handle_signal (int signum,
                     siginfo_t * = nullptr,
                     ucontext_t * = nullptr) override
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
  MyTimerHandler timer {};
  ACE_Time_Value const initialDelay {3};
  ACE_Time_Value const interval {5};
  long const timer_id = ACE_Reactor::instance ()->schedule_timer (
    &timer, nullptr, initialDelay, interval);
  // Listing 2

  // Exclude 1
  SigintHandler handleExit {};
  ACE_Reactor::instance()->register_handler (SIGINT,
                                             &handleExit);
  // Exclude 1
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  ACE_Reactor::instance ()->cancel_timer (timer_id);
  ACE_Sig_Set signals;
  signals.sig_add (SIGINT);
  ACE_Reactor::instance ()->remove_handler (signals);
  return 0;
}
