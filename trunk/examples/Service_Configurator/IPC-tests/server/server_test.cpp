// $Id$

// The main test driver for the dynamically configured server.

#include "ace/Service_Config.h"

int
main (int argc, char *argv[])
{
  if (ACE_Service_Config::open (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Service_Config::open"),
                      -1);

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive signals so we can shut down
  // gracefully.
  if (ACE_Reactor::instance ()->register_handler (sig_set, &sa) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n"),
                      -1);

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  ACE_Reactor::run_event_loop ();

  return 0;
}
