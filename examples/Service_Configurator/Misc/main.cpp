// $Id$

#include "ace/Service_Config.h"
#include "Timer_Service.h"

// Create an object that will insert the <Timer_Service> into the list
// of statically linked services that the <ACE_Service_Config> will
// process at run-time.
ACE_STATIC_SVC_REQUIRE (Timer_Service_1)

int
main (int argc, char *argv[])
{
  char *l_argv[6];

  // Manufacture a "fake" svc.conf entry to demonstrate the -S option
  // that allows us to pass these entries via the "command-line"
  // rather than the svc.conf file.
  l_argv[0] = argv[0];
  l_argv[1] = "-S";
  l_argv[2] = "static Timer_Service_1 \"timer 1 10 $TRACE\"";
  l_argv[3] = "-S";
  l_argv[4] = "dynamic Timer_Service_2 Service_Object * ./Timer:_make_Timer_Service_2() \"timer 2 10 $TRACE\"";
  l_argv[5] = 0;

  if (ACE_Service_Config::open (5, l_argv) == -1 && errno != ENOENT)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      1);

  // Run forever, performing the configured services until we receive
  // a SIGINT.

  ACE_Reactor::run_event_loop ();
  return 0;
}
