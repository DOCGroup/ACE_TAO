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

#ifndef ACE_RMCAST_REORDERING_H
#define ACE_RMCAST_REORDERING_H
#include /**/ "ace/pre.h"

#include "RMCast_Module.h"
#include "ace/RB_Tree.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Proxy;

/// Pass messages up in sent order
/**
 * Some applications require receivers to process messages in the same
 * order that messages are sent.  This module buffers out of order
 * messages and only delivers a message if:
 * - All the previous messages have been delivered.
 * - The sender sends a notification that previous messages will not
 *   be resent.
 *
 * The module also sends the Ack feedback to the sender.
 *
 * NOTE:  This is not the same as causal or total ordering, that could
 * be implemented someday, but requires a lot more than what we have
 * right now.
 *
 */
class ACE_RMCast_Export ACE_RMCast_Reordering : public ACE_RMCast_Module
{
public:
  /// Constructor
  ACE_RMCast_Reordering (void);

  /// Destructor
  virtual ~ACE_RMCast_Reordering (void);

  /// Use a Red-Black Tree to keep the queue of messages
  //@{
  typedef ACE_RB_Tree<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex> Messages;
  typedef ACE_RB_Tree_Iterator<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex> Messages_Iterator;
  //@}

  /// Remove messages still pending
  virtual int close (void);

  /// Process a Data message.
  /**
   * Process a Data message, sending the right Ack message back.
   * The message is passed up only if it is in order.
   */
  virtual int data (ACE_RMCast::Data &);

  /// During the join process the server informs us of the next
  /// expected message
  virtual int ack_join (ACE_RMCast::Ack_Join &);

private:
  /// Push any messages that are pending in the queue
  int push_queued_messages (void);

protected:
  /// The reordering buffer
  Messages messages_;

  /// The smallest value of \param next_expected for all the proxies
  ACE_UINT32 next_expected_;

  /// The highest value of \param highest_received for all the proxies
  ACE_UINT32 highest_received_;

  /// Synchronization
  ACE_SYNCH_MUTEX mutex_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Reordering.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_RMCAST_REORDERING_H */
