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
#include "ace/pre.h"

#include "Pluggable_Messaging.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Forward declarations
class ACE_Data_Block;
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

  /// Copy message from <block> to the tail of the queue. The size
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

    \return -1 queue is empty
    \return 0  tail is not "complete"
    \return 1  tail is "complete"
  */
  int is_tail_complete (void);

  /*!

    "complete" == the GIOP message at the head is not missing any data
    AND, if it's the first message in a series of GIOP fragments, all
    the fragments have been received, parsed, and placed into the
    queue

    \return -1 if queue is empty
    \return 0  if head is not "complete"
    \return 1  if head is "complete"
   */
  int is_head_complete (void);
  //@}

  /*!
    \brief Check to see if the message at the tail (complete or incomplete) is a GIOP Fragment.
   */
  int is_tail_fragmented (void);

  /// Return the size of data that is missing in tail of the queue.
  size_t missing_data_tail (void) const;
  ///  void missing_data (size_t data);

private:

  friend class TAO_Transport;

#if 0
  /// Make a node for the queue.
  TAO_Queued_Data *get_node (void);
#endif

private:
  /// A linked listof messages that await processing
  TAO_Queued_Data *queued_data_;

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
 */

class TAO_Export TAO_Queued_Data
{
protected:
  /// Default Constructor
  TAO_Queued_Data (ACE_Allocator *alloc = 0);

  /// Constructor.
  TAO_Queued_Data (ACE_Message_Block *mb, ACE_Allocator *alloc = 0);

public:
  /// Copy constructor.
  TAO_Queued_Data (const TAO_Queued_Data &qd);

  /*!
    \name Factory Methods

    These methods manufacture instances of TAO_Queued_Data and return
    them.  These instances should be removed via TAO_Queued_Data::release.

    Instances are initialized from data in the ACE_Message_Block,
    interpreted according to rules defined in the
    TAO_Pluggable_Messaging object.

    The manufactured instance adopts the message block \em without
    duplicating it; therefore, the caller must duplicate or orphan the
    message block.  The caller also must insure that the message block
    can be released via ACE_Message_Block::release, and that its life
    endures beyond the calling scope.

    For the purposes of TAO_Queued_Data, a completed message is a
    completely received message as defined by the messaging protocol
    object.  For GIOP, that means that the number of bytes specified
    in the general GIOP header have been completely received.  It
    specifically DOES NOT mean that all \em fragments have been
    received.  Fragment reassembly is another matter altogether.
  */
  //@{
  /*!
   \brief Make and return an instance of TAO_Queued_Data suitable for use as an uncompleted message.
  */
  static TAO_Queued_Data* make_uncompleted_message (ACE_Message_Block *mb,
                                                    TAO_Pluggable_Messaging &msging_obj,
                                                    ACE_Allocator *alloc = 0);
  /*!
   \brief Make and return an instance of TAO_Queued_Data suitable for use as a completed message.
  */
  // THIS IMPLEMENTATION DOESN'T WORK THE SAME AS ITS USAGE!
  // WE CAN'T JUST ADOPT mb, BECAUSE IT MAY CONTAIN MORE THAN
  // ONE PROTOCOL MESSAGE.  WE THEREFORE NEED TO CLONE IT.  THIS
  // MEANS UPDATING THE DOCUMENTATION, AND IT ALSO MEANS THAT IT
  // BEHAVES DIFFERENTLY FROM make_uncompleted_message.
  static TAO_Queued_Data* make_completed_message (ACE_Message_Block &mb,
                                                  TAO_Pluggable_Messaging &msging_obj,
                                                  ACE_Allocator *alloc = 0);
  /*!
    \brief Creation and deletion of a node in the queue.
    \todo Maybe this should be private?
   */
private:
  static TAO_Queued_Data* make_queued_data (ACE_Allocator *alloc = 0);
public:
  //@}

  static void release (TAO_Queued_Data *qd);
  void release (void);

  /// Duplicate ourselves. This creates a copy of ourselves on the
  /// heap and returns a pointer to the duplicated node.
  static TAO_Queued_Data* duplicate (TAO_Queued_Data &qd);

public:
  /// The message block that contains the message.
  ACE_Message_Block *msg_block_;

  /*!
    @name Missing Data details

    The \a missing_data_bytes_ member contains the number of bytes of
    data missing from \a msg_block_.  However, there can be two places
    where data is missing: header and payload.  We cannot know how
    much data is missing from the payload until we have a complete
    header.  Fortunately, headers are a fixed length, so we can know
    how much we're missing from the header.

    We use \param current_state_ to indicate which portion of the message
    \param missing_data_bytes_ refers to, as well as the general state of
    the message.
   */
  //@{
  /*!
    Describes the meaning given to the number stored in \a missing_data_bytes_.
   */
  enum Queued_Data_State
  {
    INVALID = -1,               //!< The queued data is in an invalid/uninitialized state, and no data should be trusted.
    COMPLETED = 0,              //!< Message is complete; \a missing_data_ should be zero.
    WAITING_TO_COMPLETE_HEADER, //!< Value in \a missing_data_ indicates part of header is missing.
    WAITING_TO_COMPLETE_PAYLOAD //!< Value in \a missing_data_ indicates part of payload is missing.
  };

  /*!
    Indicates the current state of the message, including hints at
    how to interpret the value stored in \a missing_data_bytes_.
  */
  Queued_Data_State current_state_;

  /*! Data missing in the above message that hasn't been read or processed yet. */
  CORBA::ULong missing_data_bytes_;
  //@}

  /*! The byte order of the message that is stored in the node. */
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

#include "ace/post.h"
#endif /*TAO_INCOMING_MESSAGE_QUEUE_H*/
