/*
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
  ~Logging_Acceptor () override = default;

public:
  using PEER_ADDR = ACE_INET_Addr;

  // Simple constructor.
  explicit Logging_Acceptor (ACE_Reactor *r = ACE_Reactor::instance ())
    : ACE_Event_Handler (r) {};

  Logging_Acceptor (const Logging_Acceptor &) = delete;
  Logging_Acceptor (Logging_Acceptor &&) = delete;
  Logging_Acceptor &operator= (const Logging_Acceptor &) = delete;
  Logging_Acceptor &operator= (Logging_Acceptor &&) = delete;

  // Initialization method.
  virtual int open (const ACE_INET_Addr &local_addr);

  // Called by a reactor when there's a new connection to accept.
  int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE) override;

  // Called when this object is destroyed, e.g., when it's
  // removed from a reactor.
  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = 0) override;

  // Return the passive-mode socket's I/O handle.
  ACE_HANDLE get_handle () const override
    { return acceptor_.get_handle (); };

  // Returns a reference to the underlying <acceptor_>.
  ACE_SOCK_Acceptor &acceptor () { return acceptor_; };
};

#endif /* _LOGGING_ACCEPTOR_H */
