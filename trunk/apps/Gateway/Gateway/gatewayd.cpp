// $Id$

// Main driver program for the Gateway.  This file is completely
// generic code due to the ACE Service Configurator framework!

#include "ace/Service_Config.h"
#include "Gateway.h"

int
main (int argc, char *argv[])
{
  if (ACE_Service_Config::open (argc, argv) == -1)
    {
      if (errno != ENOENT)
	ACE_ERROR ((LM_ERROR,
                    "%p\n%a",
                    "open",
                    1));
      else // Use static linking.
	{
	  ACE_Service_Object_Ptr sp = ACE_SVC_INVOKE (Gateway);

	  if (sp->init (argc - 1, argv + 1) == -1)
	    ACE_ERROR ((LM_ERROR,
                        "%p\n%a",
                        "init",
                        1));
	  
	  // Run forever, performing the configured services until we
	  // are shut down by a SIGINT/SIGQUIT signal.

	  ACE_Reactor::run_event_loop ();

	  // Destructor of <ACE_Service_Object_Ptr> automagically call
	  // <fini>.
	}
    }
  else // Use dynamic linking.

    // Run forever, performing the configured services until we are
    // shut down by a signal (e.g., SIGINT or SIGQUIT).

    ACE_Reactor::run_event_loop ();

  return 0;
}
