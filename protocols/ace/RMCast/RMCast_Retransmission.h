/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Retransmission.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_RETRANSMISSION_H
#define ACE_RMCAST_RETRANSMISSION_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Module.h"
#include "ace/RMCast/RMCast_Copy_On_Write.h"
#include "ace/Utils/Templates/RB_Tree.h"
#include "ace/Threads/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Store messages for retransmission in reliable configurations
/**
 * Reliable configurations of the RMCast framework need to store
 * messages on the sender side to resend them if one or more clients
 * do not receive them successfully.
 */
class ACE_RMCast_Export ACE_RMCast_Retransmission : public ACE_RMCast_Module
{
public:
  // = Initialization and termination methods.
  /// Constructor
  ACE_RMCast_Retransmission (void);

  /// Destructor
  virtual ~ACE_RMCast_Retransmission (void);

  /// Use a Red-Black Tree to keep the queue of messages
  typedef ACE_RB_Tree<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex> Collection;
  typedef ACE_RB_Tree_Iterator<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex> Collection_Iterator;

  /// The messages are stored in the Copy_On_Write wrapper to provide
  /// an efficient, but thread safe interface.
  typedef ACE_RMCast_Copy_On_Write<ACE_UINT32,ACE_RMCast::Data,Collection,Collection_Iterator> Messages;

  /// Resend messages
  /**
   * Resends all the messages up to \param max_sequence_number
   * Returns the number of messages sent, or -1 if there where any
   * errors.
   */
  int resend (ACE_UINT32 max_sequence_number);

  /// Resend all messages
  /**
   * Resends all the messages currently in the queue.
   */
  int resend_all (void);

  /// Return 0 if there is no pending data to send
  int has_data (void);

  /// Cleanup all the stored messages
  virtual int close (void);

  /// Pass the message downstream, but also save it in the
  /// retransmission queue
  /**
   * Sequence number are assigned by the ACE_RMCast_Fragmentation
   * class, consequently this class first passes the message
   * downstream, to obtain the sequence number and then stores the
   * message for later retransmission.
   */
  virtual int data (ACE_RMCast::Data &data);

  /// Process an Ack message from the remote receivers.
  /**
   * Normally this Ack message will be a summary of all the Ack
   * messages received by the ACE_RMCast_Membership class
   */
  virtual int ack (ACE_RMCast::Ack &);

  /// Detect when new members join the group and Ack_Join them
  /**
   * When a new receiver joins the group this module sends an Ack_Join
   * message with the next sequence number that the receiver should
   * expect.
   * The sequence number is obtained from the current list of cached
   * messages.
   */
  virtual int join (ACE_RMCast::Join &);

  /// A receiver is leaving
  /**
   * Normally the ACE_RMCast_Membership module could do this, but,
   * because this module processes the Join messages, it seems more
   * natural to process the Leave messages too.
   */
  virtual int leave (ACE_RMCast::Leave &);

protected:

  /// The retransmission buffer
  Messages messages_;
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Retransmission.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_RETRANSMISSION_H */
