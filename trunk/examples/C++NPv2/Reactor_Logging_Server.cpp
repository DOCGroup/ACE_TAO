/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "Reactor_Logging_Server.h"
#include "Logging_Acceptor_Ex.h"


template <class ACCEPTOR>
Reactor_Logging_Server<ACCEPTOR>::Reactor_Logging_Server
  (int argc, char *argv[], ACE_Reactor *reactor)
  : ACCEPTOR (reactor) {
  u_short logger_port = argc > 1 ? atoi (argv[1]) : 0;
  ACE_TYPENAME ACCEPTOR::PEER_ADDR server_addr;
  int result;

  if (logger_port != 0)
    result = server_addr.set (logger_port, INADDR_ANY);
  else
    result = server_addr.set ("ace_logger", INADDR_ANY);
  if (result != -1)
    result = ACCEPTOR::open (server_addr);
  if (result == -1) reactor->end_reactor_event_loop ();
}

typedef Reactor_Logging_Server<Logging_Acceptor_Ex>
        Server_Logging_Daemon;

int main (int argc, char *argv[])
{
  ACE_Reactor reactor;
  Server_Logging_Daemon *server;
  ACE_NEW_RETURN (server,
                  Server_Logging_Daemon (argc, argv, &reactor),
                  1);

  if (reactor.run_reactor_event_loop () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n",
                       "run_reactor_event_loop()"), 1);
  return 0;
}
