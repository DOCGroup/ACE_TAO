// Main driver program for the Gateway.  This file is completely
// $Id$

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
	  ACE_Service_Object *so = ACE_SVC_INVOKE (Gateway);

	  if (so->init (argc - 1, argv + 1) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "init", 1));
	}
    }

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  daemon.run_reactor_event_loop ();

  return 0;
}
