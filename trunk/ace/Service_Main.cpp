// Service_Main.cpp
// $Id$

/* This is an example of a canonical Service Configurator daemon's
   main() function.  Note how this driver file is completely generic
   and may be used to configure almost any type of network daemon. */

#define ACE_BUILD_DLL
#include "ace/Service_Config.h"

sig_atomic_t finished = 0;

static void
handler (int)
{
  ACE_TRACE ("handler");
  finished = 1;
}

int
sc_main (int argc, char *argv[])
{
  ACE_TRACE ("sc_main");
  ACE_Service_Config daemon;

  ACE_OS::signal (SIGINT, ACE_SignalHandler (handler));

  if (daemon.open (argc, argv) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "open", 1));

  /* Run forever, performing the configured services. */

  while (!finished)
    daemon.run_reactor_event_loop ();

  return 0;
}
