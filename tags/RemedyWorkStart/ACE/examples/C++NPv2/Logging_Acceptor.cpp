/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "Logging_Acceptor.h"
#include "Logging_Event_Handler.h"


int Logging_Acceptor::open (const ACE_INET_Addr &local_addr) {
  if (acceptor_.open (local_addr) == -1) return -1;
  return reactor ()->register_handler
           (this, ACE_Event_Handler::ACCEPT_MASK);
}

int Logging_Acceptor::handle_input (ACE_HANDLE) {
  Logging_Event_Handler *peer_handler = 0;
  ACE_NEW_RETURN (peer_handler,
                  Logging_Event_Handler (reactor ()),
                  -1);

  if (acceptor_.accept (peer_handler->peer ()) == -1) {
    delete peer_handler;
    return -1;
  } else if (peer_handler->open () == -1) {
    peer_handler->handle_close ();
    return -1;
  }
  return 0;
}

int Logging_Acceptor::handle_close (ACE_HANDLE,
                                    ACE_Reactor_Mask) {
  acceptor_.close ();
  delete this;
  return 0;
}
