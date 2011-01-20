// -*- C++ -*-

//=============================================================================
/**
 *  @file Incoming_Message_Queue.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_INCOMING_MESSAGE_QUEUE_H
#define TAO_INCOMING_MESSAGE_QUEUE_H

#include /**/ "ace/pre.h"

#include "tao/Pluggable_Messaging_Utils.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Queued_Data;
class TAO_Transport;

/**
 * @class TAO_Incoming_Message_Queue
 *
 * @brief A queue of the messages in the incoming data path.
 *
 * Please read the documentation in the TAO_Transport class to find
 * out more about the design of the incoming data path.
 *
 * Under certain conditions TAO may have to maintain a queue
 * per-connection. This queue is drained by the pluggable
 * protocols framework, normally under control of the ACE_Reactor, but
 * other configurations are conceivable.
 *
 * The memory that is allocated for holding the messages comes from
 * the global pool for the following reasons
 *
 * - the thread that reads a part of the message would not be the same
 *   thread that reads and fills the rest of the message
 * - the thread that actually processes the message can be totally
 *   different.
 *
 */
class TAO_Export TAO_Incoming_Message_Queue
{
public:

  /// Constructor.
  TAO_Incoming_Message_Queue (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_Incoming_Message_Queue (void);

  /// Adding and deleting a node from the queue.
  TAO_Queued_Data *dequeue_head (void);
  TAO_Queued_Data *dequeue_tail (void);
  int enqueue_tail (TAO_Queued_Data *nd);

  /// Return the length of the queue..
  CORBA::ULong queue_length (void) const;

private:

  friend class TAO_Transport;

private:
  /*!
    \brief A circular linked list of messages awaiting processing.

    \a last_message_added_ points to the most recent message added to
    the list.  The earliest addition can be easily accessed via
    \a last_message_added_->next_.
   */
  TAO_Queued_Data *last_added_;

  /// The size of the queue
  CORBA::ULong size_;

  /// Copy of our ORB Core
  TAO_ORB_Core *orb_core_;
};

/// Constant value indicating that the correct value is unknown yet,
/// probably parsing the header failed due to insufficient data in buffer.
const size_t TAO_MISSING_DATA_UNDEFINED = ~((size_t) 0); // MAX_SIZE_T

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Incoming_Message_Queue.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_INCOMING_MESSAGE_QUEUE_H*/
