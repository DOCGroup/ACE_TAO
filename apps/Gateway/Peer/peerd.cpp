// $Id$

// Driver for the peer daemon (peerd).  Note that this is completely
// generic code due to the Service Configurator framework!

#include "ace/Service_Config.h"
#include "Peer.h"

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

	  ACE_Service_Object *so = _make_Peer_Acceptor ();

	  
	  if (so->init (3, l_argv) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n%a", "init", 1));
	}
    }

  // Create an adapter to end the event loop.
  ACE_Sig_Adapter sa ((ACE_Sig_Handler_Ex) ACE_Service_Config::end_reactor_event_loop);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT so we can shut
  // down gracefully via signals.
  ACE_Service_Config::reactor ()->register_handler (sig_set, &sa);

  // Run forever, performing the configured services until we are shut
  // down by a SIGINT/SIGQUIT signal.

  daemon.run_reactor_event_loop ();

  return 0;
}
