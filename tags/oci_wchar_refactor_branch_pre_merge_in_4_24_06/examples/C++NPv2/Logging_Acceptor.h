/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _LOGGING_ACCEPTOR_H
#define _LOGGING_ACCEPTOR_H

#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Record.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "Logging_Handler.h"

class Logging_Acceptor : public ACE_Event_Handler
{
protected:
  // Factory that connects <ACE_SOCK_Stream>s passively.
  ACE_SOCK_Acceptor acceptor_;

protected:
  virtual ~Logging_Acceptor () {}; // No-op destructor.

public:
  typedef ACE_INET_Addr PEER_ADDR;

  // Simple constructor.
  Logging_Acceptor (ACE_Reactor *r = ACE_Reactor::instance ())
    : ACE_Event_Handler (r) {};

  // Initialization method.
  virtual int open (const ACE_INET_Addr &local_addr);

  // Called by a reactor when there's a new connection to accept.
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);

  // Called when this object is destroyed, e.g., when it's
  // removed from a reactor.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = 0);

  // Return the passive-mode socket's I/O handle.
  virtual ACE_HANDLE get_handle () const
    { return acceptor_.get_handle (); };

  // Returns a reference to the underlying <acceptor_>.
  ACE_SOCK_Acceptor &acceptor () { return acceptor_; };
};

#endif /* _LOGGING_ACCEPTOR_H */
