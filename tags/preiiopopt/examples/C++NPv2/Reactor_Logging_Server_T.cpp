/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

// Implementations for Reactor_Logging_Server<>

template <class ACCEPTOR>
Reactor_Logging_Server<ACCEPTOR>::Reactor_Logging_Server
  (int argc, char *argv[], ACE_Reactor *reactor)
  : ACCEPTOR (reactor) {
  u_short logger_port = argc > 0 ? atoi (argv[0]) : 0;
  ACE_TYPENAME ACCEPTOR::PEER_ADDR server_addr;
  int result;

  if (logger_port != 0)
    result = server_addr.set (logger_port,
                              (ACE_UINT32) INADDR_ANY);
  else
    result = server_addr.set ("ace_logger",
                              (ACE_UINT32) INADDR_ANY);
  if (result != -1)
    result = ACCEPTOR::open (server_addr);
  if (result == -1) reactor->end_reactor_event_loop ();
}
