/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Logging_Acceptor.h
 *
 *  @author Doug Schmidt
 */
//=============================================================================

#ifndef _CLIENT_ACCEPTOR_H
#define _CLIENT_ACCEPTOR_H

#include "ace/SOCK_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

/**
 * @class Logging_Acceptor
 *
 * @brief Handle connection requests from remote client clients.
 *
 * Accepts client connection requests, creates Logging_Handler's
 * to process them, and registers these Handlers with the
 * ACE_Reactor Singleton.
 */
class Logging_Acceptor : public ACE_Event_Handler
{
friend class Logging_Handler;
public:
  /// Constructor.
  Logging_Acceptor () = default;

  /// Initialization.
  int open (const ACE_INET_Addr &a);

private:
  // = Demuxing hooks.
  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;
  ACE_HANDLE get_handle () const override;

  /// By making this private we ensure that the <Logging_Acceptor> is
  /// allocated dynamically.
  ~Logging_Acceptor () = default;

  /// Passive connection acceptor factory.
  ACE_SOCK_Acceptor peer_acceptor_;
};

#endif /* _CLIENT_ACCEPTOR_H */
