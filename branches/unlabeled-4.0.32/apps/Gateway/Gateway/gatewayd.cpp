// Main driver program for the Gateway.  This file is completely
// @(#)gatewayd.cpp	1.1	10/18/96

// generic code due to the ACE Service Configurator framework!

#include "ace/Service_Config.h"
#include "Gateway.h"

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  if (daemon.open (argc, argv) == -1)
    {
      if (errno != ENOENT)
	ACE_ERROR ((LM_ERROR, "%p\n%a", "open", 1));
      else // Use static binding.
	{
	  static char *l_argv[3] = { "-d" };
	  ACE_Service_Object *so = ACE_SVC_INVOKE (ACE_Gateway);

	  if (so->init (1, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "init", 1));
	}
    }

  // Run forever, performing the configured services until we are shut
  // down by a signal.

  ACE_Service_Config::run_reactor_event_loop ();

  return 0;
}
