/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ATM_Acceptor.h
//
// = AUTHOR
//    Joe Hoffert
//
// ============================================================================

#ifndef ACE_ATM_ACCEPTOR_H
#define ACE_ATM_ACCEPTOR_H
#include "ace/pre.h"

#include "ace/Time_Value.h"
#include "ace/ATM_Stream.h"
#include "ace/ATM_Params.h"
#include "ace/ATM_QoS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_ATM)

#if defined (ACE_HAS_FORE_ATM_WS2)
#include "SOCK_Acceptor.h"
typedef ACE_SOCK_Acceptor ATM_Acceptor;
#elif defined (ACE_HAS_FORE_ATM_XTI)
#include "TLI_Acceptor.h"
typedef ACE_TLI_Acceptor ATM_Acceptor;
#endif

class ACE_Export ACE_ATM_Acceptor
{
  // = TITLE
  //     Defines the member functions for ACE_ATM_Acceptor abstraction.
  //
  // = DESCRIPTION
  //     This class wraps up the ACE_SOCK_Acceptor and ACE_TLI_Acceptor
  //     to make the mechanism for the ATM protocol transparent.

public:
  // = Initialization and termination methods.
  ACE_ATM_Acceptor (void);
  // Default constructor.

  ~ACE_ATM_Acceptor ();

  ACE_ATM_Acceptor (const ACE_Addr &remote_sap,
                    int backlog = ACE_DEFAULT_BACKLOG,
                    ACE_ATM_Params params = ACE_ATM_Params());
  // Initiate a passive mode connection.

  ACE_HANDLE open (const ACE_Addr &remote_sap,
                   int backlog = ACE_DEFAULT_BACKLOG,
                   ACE_ATM_Params params = ACE_ATM_Params());
  // Initiate a passive mode socket.

  int close (void);
  // Close down the acceptor and release resources.

  // = Passive connection acceptance method.

  int accept (ACE_ATM_Stream &new_sap,
              ACE_Addr *remote_addr = 0,
              ACE_Time_Value *timeout = 0,
              int restart = 1,
              int reset_new_handle = 0,
              ACE_ATM_Params params = ACE_ATM_Params(),
              ACE_ATM_QoS qos = ACE_ATM_QoS());

  // Accept a new data transfer connection.  A <timeout> of 0 means
  // block forever, a <timeout> of {0, 0} means poll.  <restart> == 1
  // means "restart if interrupted."

  int get_local_addr( ACE_ATM_Addr &local_addr );
  // Get the local address currently listening on

  // = Meta-type info
  typedef ACE_ATM_Addr PEER_ADDR;
  typedef ACE_ATM_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ATM_Acceptor acceptor_;
};

#if defined (__ACE_INLINE__)
#include "ace/ATM_Acceptor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ATM */
#include "ace/post.h"
#endif /* ACE_ATM_ACCEPTOR_H */

