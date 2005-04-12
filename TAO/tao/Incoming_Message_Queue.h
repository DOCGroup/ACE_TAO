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

/// Forward declarations
class ACE_Allocator;
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

  /// Copy message from @a block to the tail of the queue. The size
  /// of message that is copied to the tail node is returned. The
  /// number of bytes copied depends on the amount of bytes needed to
  /// make the tail node consistent.
  size_t copy_tail (ACE_Message_Block &block);

  /// Return the length of the queue..
  CORBA::ULong queue_length (void);

  /*!
    @name Node Inspection Predicates

    \brief These methods allow inspection of head and tail nodes for "completeness".

    These methods check to see whether the node on the head or tail is
    "complete" and ready for further processing.  See each method's
    documentation for its definition of "complete".
  */
  //@{
  /*!
    "complete" == the GIOP message at the tail is not missing any data (it may be a complete GIOP Fragment, though)

    \retval -1 queue is empty
    \retval 0  tail is not "complete"
    \retval 1  tail is "complete"
  */
  int is_tail_complete (void);

  /*!

    "complete" == the GIOP message at the head is not missing any data
    AND, if it's the first message in a series of GIOP fragments, all
    the fragments have been received, parsed, and placed into the
    queue

    \retval -1 if queue is empty
    \retval 0  if head is not "complete"
    \retval 1  if head is "complete"
   */
  int is_head_complete (void);
  //@}

  /*!
    \brief Check to see if the message at the tail (complete or incomplete) is a GIOP Fragment.
   */
  int is_tail_fragmented (void);

  /// Return the size of data that is missing in tail of the queue.
  size_t missing_data_tail (void) const;

  /// Find the first fragment that matches the GIOP version
  TAO_Queued_Data *find_fragment_chain (CORBA::Octet major,
                                        CORBA::Octet minor) const;

  /// Find the first fragment that matches the request id
  TAO_Queued_Data *find_fragment_chain (CORBA::ULong request_id) const;

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

/************************************************************************/

/**
 * @class TAO_Queued_Data
 *
 * @brief Represents a node in the queue of incoming messages.
 *
 * This class contains necessary information about a message that is
 * stored in the queue. Such a node can be used by the incoming thread
 * from the reactor to dequeue and process the message by sending it
 * to the higher layers of the ORB.
 *
 * The ACE_Message_Block contained within this class may contain a chain
 * of message blocks (usually when GIOP fragments are involved).  In that
 * case consolidate () needs to be called prior to being sent to higher
 * layers of the ORB when the GIOP fragment chain is complete.
 */

class TAO_Export TAO_Queued_Data
{
public:
  /// Default Constructor
  TAO_Queued_Data (ACE_Allocator *alloc = 0);

  /// Constructor.
  TAO_Queued_Data (ACE_Message_Block *mb, ACE_Allocator *alloc = 0);

  /// Copy constructor.
  TAO_Queued_Data (const TAO_Queued_Data &qd);

  /// Creation of a node in the queue.
  static TAO_Queued_Data* make_queued_data (ACE_Allocator *alloc = 0);

  /// Deletion of a node from the queue.
  static void release (TAO_Queued_Data *qd);

  /// Duplicate ourselves. This creates a copy of ourselves on the
  /// heap and returns a pointer to the duplicated node.
  static TAO_Queued_Data* duplicate (TAO_Queued_Data &qd);

  /// Consolidate this fragments chained message blocks into one.
  void consolidate (void);

public:
  /// The message block that contains the message.
  ACE_Message_Block *msg_block_;

  /*!
    @name Missing Data details

    The \a missing_data_ member contains the number of bytes of
    data missing from \a msg_block_.
   */
  //@{
  /*! Data missing in the above message that hasn't been read or processed yet. */
  CORBA::Long missing_data_;
  //@}

  /// The byte order of the message that is stored in the node.
  CORBA::Octet byte_order_;

  /// Many protocols like GIOP have a major and minor version
  /// information that would be needed to read and decipher the
  /// message.
  CORBA::Octet major_version_;
  CORBA::Octet minor_version_;

  /// Some messages can be fragmented by the protocol (this is an ORB
  /// level fragmentation on top of the TCP/IP fragmentation. This
  /// member indicates whether the message that we have recd. and
  /// queue already has more fragments that is missing..
  CORBA::Octet more_fragments_;

  /// The fragment request id
  CORBA::ULong request_id_;

  /// The message type of the message
  TAO_Pluggable_Message_Type msg_type_;

  /// Pounter to the next element in the queue.
  TAO_Queued_Data *next_;

private:
  /// Replace the datablock with a one allocated on the heap or
  /// allocator
  static void replace_data_block (ACE_Message_Block &mb);

private:

  /// The allocator used to allocate this class.
  ACE_Allocator *allocator_;
};


#if defined (__ACE_INLINE__)
# include "Incoming_Message_Queue.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_INCOMING_MESSAGE_QUEUE_H*/
