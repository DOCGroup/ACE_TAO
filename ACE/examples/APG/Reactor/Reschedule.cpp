/**
 * Changing the interval
 */

#include "ace/OS_NS_time.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Signal.h"

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
                ACE_TEXT ("handle_timeout: %C"),
                ACE_OS::ctime (&epoch)));
    return 0;
  }
};

// Listing 1 code/ch07
class SigintHandler : public ACE_Event_Handler
{
public:
  SigintHandler (long timerId, int currentInterval)
    : timerId_ (timerId),
      currentInterval_ (currentInterval)
  {
  }

  SigintHandler (SigintHandler const &) = delete;
  SigintHandler (SigintHandler &&) = delete;
  SigintHandler &operator= (SigintHandler const &) = delete;
  SigintHandler &operator= (SigintHandler &&) = delete;

  int handle_signal (int, siginfo_t * = nullptr, ucontext_t * = nullptr) override
  {
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Resetting interval of timer ")
                ACE_TEXT ("%d to %d\n"),
                this->timerId_,
                ++this->currentInterval_));
    ACE_Time_Value const newInterval {this->currentInterval_};
    ACE_Reactor::instance()->
      reset_timer_interval (this->timerId_, newInterval);
    return 0;
  }

private:
  long const timerId_;
  int currentInterval_;
};
// Listing 1

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Time_Value initialDelay {3};
  ACE_Time_Value interval {5};

  // Listing 2 code/ch07
  MyTimerHandler handler {};

  long const timerId =
    ACE_Reactor::instance ()->schedule_timer (&handler,
                                              nullptr,
                                              initialDelay,
                                              interval);
  // Listing 2

  // Listing 3 code/ch07
  SigintHandler handleSigint {timerId, 5};
  ACE_Reactor::instance ()->register_handler (SIGINT,
                                              &handleSigint);
  // Listing 3

  ACE_Reactor::instance ()->run_reactor_event_loop ();
  ACE_Reactor::instance ()->cancel_timer (timerId);
  ACE_Sig_Set signals;
  signals.sig_add (SIGINT);
  ACE_Reactor::instance ()->remove_handler (signals);
  return 0;
}
