/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace/RMCast
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_RECEIVER_MODULE_H
#define ACE_RMCAST_RECEIVER_MODULE_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Module.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Keep track of the receiver module.
/**
 * A receiver proxy must reject invalid messages and communicate with
 * the sender to join and leave the multicast group.
 *
 * This module performs all the task related to 
 *
 */
class ACE_RMCast_Export ACE_RMCast_Receiver_Module : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Receiver_Module (void);

  //! Destructor
  virtual ~ACE_RMCast_Receiver_Module (void);

  virtual int data (ACE_RMCast::Data &);
  virtual int poll (ACE_RMCast::Poll &);
  virtual int ack_join (ACE_RMCast::Ack_Join &);
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);

  /// Simple enum used to describe the receiver state transitions
  /**
   * Receivers go through several states before they can fully accept
   * messages, the following comments describe those states, as well as
   * the possible transitions
   * This configuration is pesimistic, any invalid message is cause
   * enough to reclaim all the resources.  This partially addresses
   * situations where either accidentally or intentionally a sender is
   * multicasting packets to the wrong group.

   <CODE>
              NON_EXISTENT JOINING      JOINED       LEAVING<BR>
   ----------------------------------------------------------------<BR>
  <BR>
   DATA       JOINING      JOINING      JOINED       LEAVING<BR>
              Send/Join    Send/Join    Recv/Data    Send/Leave<BR>
  <BR>
   POLL       JOINING      JOINING      JOINED       LEAVING<BR>
              Send/Join    Send/Join    Send/Ack     Send/Leave<BR>
  <BR>
   ACK_JOIN   JOINING      JOINED       JOINED       LEAVING<BR>
              Send/Join    Receive Msg  Receive Msg  Send/Leave<BR>
  <BR>
   ACK_LEAVE  NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT<BR>
              Noop         Destroy      Destroy      Destroy<BR>
  <BR>
   ACK        NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT<BR>
              Noop         Destroy      Destroy      Destroy<BR>
  <BR>
   JOIN       NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT<BR>
              Noop         Destroy      Destroy      Destroy<BR>
  <BR>
   LEAVE      NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT<BR>
              Noop         Destroy      Destroy      Destroy<BR>
  <BR>
  </CODE>
  */
  enum Receiver_State
  {
    RS_NON_EXISTENT,
    RS_JOINING,
    RS_JOINED,
    RS_LEAVING
  };

private:
  /// Enter the leaving state, prepare for termination
  void leaving (ACE_RMCast_Proxy *proxy);

private:
  /// The current state of the receiver
  int state_;
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Receiver_Module.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_RECEIVER_MODULE_H */
