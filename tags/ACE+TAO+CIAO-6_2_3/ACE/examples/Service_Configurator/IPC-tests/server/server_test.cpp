// $Id$

// The main test driver for the dynamically configured server.

#include "ace/OS_NS_unistd.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"
#include "ace/Sig_Adapter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (ACE_Service_Config::open (argc,
                                argv,
                                ACE_DEFAULT_LOGGER_KEY,
                                0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ACE_Service_Config::open")),
                      -1);

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive signals so we can shut down
  // gracefully.
  if (ACE_Reactor::instance ()->register_handler (sig_set,
                                                  &sa) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"), ACE_TEXT ("register_handler")),
                      -1);

  // This makes the README demo even easier (for sighup).
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("pid = %d\n"),
              ACE_OS::getpid ()));

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  // We use this version of the event loop so that reconfigurations
  // are triggered properly.
  ACE_Reactor::run_event_loop ();

  return 0;
}
