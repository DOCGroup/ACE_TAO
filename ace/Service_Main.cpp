// Service_Main.cpp
// $Id$

// This is an example of a canonical Service Configurator daemon's
// main() function.  Note how this driver file is completely generic
// and may be used to configure almost any type of network daemon.

#define ACE_BUILD_DLL
#include "ace/Service_Config.h"
#include "ace/Reactor.h"

int
sc_main (int argc, char *argv[])
{
  ACE_TRACE ("sc_main");
  ACE_Service_Config daemon;

  if (daemon.open (argc, argv) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "open", 1));

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  // Register a signal handler.
  ACE_Reactor::instance()->register_handler (SIGINT, &sa);

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  ACE_Reactor::run_event_loop ();

  return 0;
}
