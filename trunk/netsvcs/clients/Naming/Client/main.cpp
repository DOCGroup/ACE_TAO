// Test the client-side of the ACE Name Server...
// $Id$


#include "ace/Service_Config.h"
#include "ace/Naming_Context.h"
#include "Client_Test.h"

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
	  char *l_argv[3];
	  l_argv[0] = argv[0];
	  l_argv[1] = "-p 10011";
	  l_argv[2] = 0;
	  ACE_Service_Object *so = ACE_SVC_INVOKE (ACE_Naming_Context);

	  if (so->init (2, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "ACE_Naming_Context", 1));

	  so = ACE_SVC_INVOKE (Client_Test);

	  if (so->init (0, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "Client_Test", 1));
	}
    }

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  ACE_Service_Config::run_reactor_event_loop ();

  return 0;
}
