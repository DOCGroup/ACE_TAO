/**
 * Changing the interval and canceling
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

#if !defined (ACE_LACKS_UNIX_SIGNALS)

// Listing 1 code/ch07
class SignalHandler : public ACE_Event_Handler
{
public:
  SignalHandler (long timerId, int currentInterval)
    : timerId_ (timerId),
      currentInterval_ (currentInterval)
  {
  }

  SignalHandler (SignalHandler const &) = delete;
  SignalHandler (SignalHandler &&) = delete;
  SignalHandler &operator= (SignalHandler const &) = delete;
  SignalHandler &operator= (SignalHandler &&) = delete;

  int handle_signal (int sig,
                     siginfo_t * = nullptr,
                     ucontext_t * = nullptr) override
  {
    if (sig == SIGINT)
      {
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("Resetting interval of timer ")
                    ACE_TEXT ("%d to %d\n"),
                    this->timerId_,
                    ++this->currentInterval_));
        ACE_Time_Value const newInterval {this->currentInterval_};
        ACE_Reactor::instance ()->
          reset_timer_interval (this->timerId_, newInterval);
      }
    else if (sig == SIGTSTP)
      {
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("Canceling timer %d\n"),
                    this->timerId_));
        ACE_Reactor::instance ()->cancel_timer (this->timerId_);
      }

    return 0;
  }

private:
  long const timerId_;
  int currentInterval_;
};
// Listing 1

#endif /* ACE_LACKS_UNIX_SIGNALS */


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

#if !defined (ACE_LACKS_UNIX_SIGNALS)

  // Listing 3 code/ch07
  SignalHandler mutateTimer {timerId, 5};
  ACE_Reactor::instance ()->register_handler (SIGINT,
                                              &mutateTimer);
  ACE_Reactor::instance ()->register_handler (SIGTSTP,
                                              &mutateTimer);
  // Listing 3

#else
   ACE_UNUSED_ARG (timerId);
#endif /* ACE_LACKS_UNIX_SIGNALS */

  ACE_Reactor::instance ()->run_reactor_event_loop ();
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  ACE_Sig_Set signals;
  signals.sig_add (SIGINT);
  signals.sig_add (SIGTSTP);
  ACE_Reactor::instance ()->remove_handler (signals);
#endif /* ACE_LACKS_UNIX_SIGNALS */
  ACE_Reactor::instance ()->cancel_timer (timerId);
  return 0;
}
