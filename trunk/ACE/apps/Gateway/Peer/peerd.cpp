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

#include "ace/OS_NS_unistd.h"
#include "Peer.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (ACE_OS::access (ACE_DEFAULT_SVC_CONF, F_OK) != 0)
    {
      // Use static linking.
      ACE_Service_Object_Ptr sp = ACE_SVC_INVOKE (Peer_Factory);

      if (sp->init (argc - 1, argv + 1) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("init")),
                          1);

      // Run forever, performing the configured services until we are
      // shut down by a SIGINT/SIGQUIT signal.

      ACE_Reactor::instance ()->run_reactor_event_loop ();

      // Destructor of <ACE_Service_Object_Ptr> automagically call
      // <fini>.
    }
  else
    {
      if (ACE_Service_Config::open (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("open")),
                          1);
      else // Use dynamic linking.

        // Run forever, performing the configured services until we
        // are shut down by a signal (e.g., SIGINT or SIGQUIT).

        ACE_Reactor::instance ()->run_reactor_event_loop ();
    }
  return 0;
}
