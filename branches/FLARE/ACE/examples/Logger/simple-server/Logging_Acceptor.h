/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Logging_Acceptor.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef _CLIENT_ACCEPTOR_H
#define _CLIENT_ACCEPTOR_H

#include "ace/SOCK_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

class Logging_Acceptor : public ACE_Event_Handler
  // = TITLE
  //     Handle connection requests from remote client clients.
  //
  // = DESCRIPTION
  //     Accepts client connection requests, creates Logging_Handler's
  //     to process them, and registers these Handlers with the
  //     ACE_Reactor Singleton.
{
friend class Logging_Handler;
public:
  Logging_Acceptor (void);
  // Constructor.

  int open (const ACE_INET_Addr &a);
  // Initialization.

private:
  // = Demuxing hooks.
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  virtual ACE_HANDLE get_handle (void) const;

  ~Logging_Acceptor (void);
  // By making this private we ensure that the <Logging_Acceptor> is
  // allocated dynamically.

  ACE_SOCK_Acceptor peer_acceptor_;
  // Passive connection acceptor factory.
};

#endif /* _CLIENT_ACCEPTOR_H */
