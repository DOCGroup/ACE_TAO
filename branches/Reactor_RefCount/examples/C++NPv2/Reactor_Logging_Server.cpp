/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "Reactor_Logging_Server.h"
#include "Logging_Acceptor_Ex.h"
#include "ace/Log_Msg.h"

typedef Reactor_Logging_Server<Logging_Acceptor_Ex>
        Server_Logging_Daemon;

int main (int argc, char *argv[])
{
  ACE_Reactor reactor;
  Server_Logging_Daemon *server;
  // Ignore argv[0]...
  --argc; ++argv;
  ACE_NEW_RETURN (server,
                  Server_Logging_Daemon (argc, argv, &reactor),
                  1);

  if (reactor.run_reactor_event_loop () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n",
                       "run_reactor_event_loop()"), 1);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Reactor_Logging_Server<Logging_Acceptor_Ex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Reactor_Logging_Server<Logging_Acceptor_Ex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
