// $Id$

// The main test driver for the dynamically configured server.

#include "ace/Service_Config.h"

int
main (int argc, char *argv[])
{
  ACE_Service_Config server_test;

  if (server_test.open (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server_test.open"), -1);

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  // Register a signal handler.
  ACE_Reactor::instance ()->register_handler (SIGINT, &sa);

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  while (ACE_Reactor::event_loop_done() == 0)
    ACE_Reactor::run_event_loop ();

  return 0;
}
