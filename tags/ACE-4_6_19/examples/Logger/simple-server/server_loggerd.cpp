// $Id$

// This server daemon collects, formats, and displays logging
// information forwarded from client daemons running on other hosts in
// the network.
//
// In addition, it also illustrates how the ACE_Reactor framework is
// used.

#include "ace/Get_Opt.h"

#include "Logging_Acceptor.h"
#include "Reactor_Singleton.h"

ACE_RCSID(simple_server, server_loggerd, "$Id$")

static sig_atomic_t finished = 0;

extern "C" void
handler (int)
{
  finished = 1;
}

// It doesn't get anymore const than this....
static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int
main (int argc, char *argv[])
{
  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  Logging_Acceptor *peer_acceptor;
  ACE_NEW_RETURN (peer_acceptor,
                  Logging_Acceptor,
                  1);

  ACE_INET_Addr addr (PORT);

  ACE_Get_Opt get_opt (argc, argv, "p:");

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'p':
	 addr.set (ACE_OS::atoi (get_opt.optarg));
	 break;
       default:
	 break;
       }

  if (peer_acceptor->open (addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  else if (REACTOR::instance ()->register_handler
	   (peer_acceptor,
	    ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"),
		       -1);

  // Run forever, performing the logging service.

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up server logging daemon\n"));

  while (!finished)
     REACTOR::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down server logging daemon\n"));
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

