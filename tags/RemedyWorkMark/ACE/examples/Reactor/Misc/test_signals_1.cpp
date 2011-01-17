// $Id$

// This simple program illustrates the difference between handling
// signals via the Reactor (which doesn't cause the event loop to
// terminate) and signals that aren't handled via the Reactor (which
// do).

#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"



// Number of times to allow signal to execute until we quit.
static size_t signal_count = 10;

static void
my_signal_function (int sig)
{
  ACE_DEBUG ((LM_DEBUG,
              "Executed non-ACE signal handler for signal %S\n",
              sig));
}

class My_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_signal (int sig,
                             siginfo_t *,
                             ucontext_t *)
  {
    // @@ Note that this code is not portable to all OS platforms
    // since it uses print statements within signal handler context.
    ACE_DEBUG ((LM_DEBUG,
                "Executed ACE signal handler for signal %S, count = %d\n",
                sig,
                signal_count));
    --signal_count;

    if (signal_count == 0)
      ACE_Reactor::end_event_loop ();

    return 0;
  }

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG,
                "%s\n",
                (const char *) arg));
    return 0;
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // First you need a handler for the timeout.
  My_Handler my_handler;

  // This is the timeout period in seconds.
  ACE_Time_Value period (ACE_DEFAULT_TIMEOUT);

  if (argc > 1)
    period.set (ACE_OS::atoi (argv[1]), 0);

  // Set up the periodic interval timer.
  if (ACE_Reactor::instance ()->schedule_timer
      (&my_handler,
       "hello",
       period,
       period) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p\n",
                       "schedule_timer"),
                      -1);

  // Set up an ACE signal handler.

  if (ACE_Reactor::instance ()->register_handler
      (SIGINT,
       &my_handler) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p\n",
                       "register_handler"),
                      -1);

  // Set up a non-ACE signal handler.  When this goes off, the Reactor
  // should return from its <run_event_loop> method.
  ACE_Sig_Action sig ((ACE_SignalHandler) my_signal_function,
                      SIGQUIT);
  ACE_UNUSED_ARG (sig);

  ACE_DEBUG ((LM_DEBUG,
              "starting event loop that runs until you've typed ^C a total of 10 times or ^\\ once.\n"));

  // This call executes the reactor events until we're finished.
  int result = ACE_Reactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              "result = %d\n",
              result));

  // Make sure to remove my_handler before exiting main() since
  // otherwise weird things can happen...
  if (ACE_Reactor::instance ()->cancel_timer (&my_handler) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p\n",
                       "cancel_timer"),
                      -1);
  return 0;
}
