/* Driver for the peer daemon (peerd).  Note that this
// $Id$

   is completely generic code due to the Service Configurator
   framework! */

#include "ace/Service_Config.h"
#include "Gateway_Handler.h"

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
	  static char *l_argv[3] = { "-d", "-p", "10002" };

	  ACE_Service_Object *so = _alloc_peerd ();

	  
	  if (so->init (3, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "init", 1));
	}
    }

  /* Run forever, performing the configured services (until SIGINT/SIGQUIT occurs) */

  daemon.run_reactor_event_loop ();

  return 0;
}
