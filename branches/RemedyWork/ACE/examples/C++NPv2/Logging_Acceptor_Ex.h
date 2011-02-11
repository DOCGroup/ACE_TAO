/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _LOGGING_ACCEPTOR_EX_H
#define _LOGGING_ACCEPTOR_EX_H

#include "ace/INET_Addr.h"
#include "ace/Reactor.h"

#include "Logging_Acceptor.h"
#include "Logging_Event_Handler_Ex.h"

class Logging_Acceptor_Ex : public Logging_Acceptor
{
public:
  typedef ACE_INET_Addr PEER_ADDR;

  // Simple constructor to pass ACE_Reactor to base class.
  Logging_Acceptor_Ex (ACE_Reactor *r = ACE_Reactor::instance ())
    : Logging_Acceptor (r) {}

  int handle_input (ACE_HANDLE) {
    Logging_Event_Handler_Ex *peer_handler = 0;
    ACE_NEW_RETURN (peer_handler,
                    Logging_Event_Handler_Ex (reactor ()),
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
};

#endif /* _LOGGING_ACCEPTOR_EX_H */
