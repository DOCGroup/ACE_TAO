// $Id$

#include "ace/Service_Config.h"
#include "Timer_Service.h"

// Create an object that will insert the Timer_Service into the list
// of statically linked services that the ACE_Service_Config will
// process at run-time.
ACE_STATIC_SVC_REQUIRE(Timer_Service)

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  if (daemon.open (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 1);

  // Run forever, performing the configured services until we receive
  // a SIGINT.

  daemon.run_reactor_event_loop ();
  return 0;
}
