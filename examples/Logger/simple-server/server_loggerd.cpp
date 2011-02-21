// $Id$

// This server daemon collects, formats, and displays logging
// information forwarded from client daemons running on other hosts in
// the network.
//
// In addition, it also illustrates how the ACE_Reactor framework is
// used.

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/Signal.h"

#include "Logging_Acceptor.h"
#include "Reactor_Singleton.h"



static sig_atomic_t finished = 0;

extern "C" void
handler (int)
{
  finished = 1;
}

// It doesn't get anymore const than this....
static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  Logging_Acceptor *peer_acceptor;
  ACE_NEW_RETURN (peer_acceptor,
                  Logging_Acceptor,
                  1);

  ACE_INET_Addr addr (PORT);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:"));

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'p':
        addr.set (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      default:
        break;
      }

  if (peer_acceptor->open (addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      -1);
  else if (REACTOR::instance ()->register_handler (peer_acceptor,
                                                   ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("registering service with ACE_Reactor\n")),
                      -1);

  // Run forever, performing the logging service.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting up server logging daemon\n")));

  while (!finished)
     REACTOR::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) shutting down server logging daemon\n")));
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> *
  ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
