// This simple program illustrates the difference between handling
// signals via the Reactor (which doesn't cause the event loop to
// terminate) and signals that aren't handled via the Reactor (which
// do).

#include "ace/Service_Config.h"

// Number of times to allow signal to execute until we quit.
static size_t count = 10;

static void 
my_signal_function (int sig)
{
  ACE_DEBUG ((LM_DEBUG, "Executed non-ACE signal handler for sig %S\n", sig));
}

class My_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_signal (int sig,
			     siginfo_t *,
			     ucontext_t *)
  {
    ACE_DEBUG ((LM_DEBUG, "Executed ACE signal handler for sig %S, count = %d\n", 
		sig, count));

    count--;

    if (count == 0)
      ACE_Service_Config::end_reactor_event_loop ();

    return 0;
  }

  virtual int handle_timeout (const ACE_Time_Value &,
			      const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG, "%s\n", (const char *) arg));
    return 0;
  }
};

int 
main (int, char *argv[])
{
  // The following sets up a service configurator - mostly used to get
  // a reactor for the process.
  ACE_Service_Config my_config (argv[0]);

  // ----- First you need a handler for the timeout -----
  My_Handler my_handler;

  // ----- Now set up the timer -----

  // This is the timeout period in seconds.
  ACE_Time_Value period (2);

  if (my_config.reactor ()->schedule_timer (&my_handler, 
					    "hello", period, period) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "schedule_timer"), -1);

  // ----- Set up an ACE signal handler -----

  if (my_config.reactor ()->register_handler (SIGINT, &my_handler) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "register_handler"), -1);

  // ----- Set up a non-ACE signal handler -----
  ACE_Sig_Action sig ((ACE_SignalHandler) my_signal_function, SIGQUIT);

  // This just executes the reactor events until my_handler tells us
  // we are finished.
  ACE_DEBUG ((LM_DEBUG, "starting event loop that runs until you've typed ^C a total of 10 times or ^\\ once.\n"));

  while (my_config.reactor_event_loop_done () == 0)
    my_config.run_reactor_event_loop ();

  return 0;
}
