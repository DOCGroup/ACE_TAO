// $Id$

// ============================================================================
//
// = LIBRARY
//    gateway
//
// = FILENAME
//    peerd.h
//
// = DESCRIPTION
//    Driver for the peer daemon (peerd).  Note that this is
//    completely generic code due to the Service Configurator
//    framework!
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#include "Peer.h"

int
main (int argc, char *argv[])
{
  if (ACE_Service_Config::open (argc, argv) == -1)
    {
      if (errno != ENOENT)
	ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "open"),
                          1);
      else // Use static linking.
	{
	  ACE_Service_Object_Ptr sp = ACE_SVC_INVOKE (Peer_Factory);
	  
	  if (sp->init (argc - 1, argv + 1) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "init"),
                              1);
	  
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
