// -*- C++ -*-

//=============================================================================
/**
 *  @file Queued_Data.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_QUEUED_DATA_H
#define TAO_QUEUED_DATA_H

#include /**/ "ace/pre.h"

#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Message_State.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Allocator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  static TAO_Queued_Data* make_queued_data (
                                    ACE_Allocator *message_buffer_alloc = 0,
                                    ACE_Allocator *input_cdr_alloc = 0,
                                    ACE_Data_Block *db = 0);

  /// Deletion of a node from the queue.
  static void release (TAO_Queued_Data *qd);

  /// Duplicate ourselves. This creates a copy of ourselves on the
  /// heap and returns a pointer to the duplicated node.
  static TAO_Queued_Data* duplicate (TAO_Queued_Data &qd);

  /// Consolidate this fragments chained message blocks into one.
  /// @return -1 if consolidation failed, eg out or memory, otherwise 0
  int consolidate (void);

  /// Get missing data
  size_t missing_data (void) const;

  /// Set missing data
  void missing_data (size_t data);

  /// Get the GIOP version
  TAO_GIOP_Message_Version const &giop_version (void) const;

  /// Get byte_order
  CORBA::Octet byte_order (void) const;

  /// Get more fragments
  CORBA::Boolean more_fragments (void) const;

  /// Get message type
  GIOP::MsgType msg_type (void) const;

  /// Get next
  TAO_Queued_Data *next (void) const;

  /// Set next
  void next (TAO_Queued_Data* qd);

  /// Get message block
  ACE_Message_Block *msg_block (void) const;

  /// Set message block
  void msg_block (ACE_Message_Block *mb);

  /// Set the state
  void state (const TAO_GIOP_Message_State& state);

  /// Get the state
  const TAO_GIOP_Message_State& state (void) const;

private:

  /// The message block that contains the message.
  ACE_Message_Block *msg_block_;

  /*!
    @name Missing Data details

    The \a missing_data_ member contains the number of bytes of
    data missing from \a msg_block_.
   */
  //@{
  /*! Data missing in the above message that hasn't been read or processed yet,
      the value TAO_MISSING_DATA_UNDEFINED indicates it hasn't been processed yet,
      otherwise greater or equal zero. */
  size_t missing_data_;
  //@}

  /// State of this queued data
  TAO_GIOP_Message_State state_;

  /// Pounter to the next element in the queue.
  TAO_Queued_Data *next_;

  /// Replace the datablock with a one allocated on the heap or
  /// allocator
  static void replace_data_block (ACE_Message_Block &mb);

  /// The allocator used to allocate this class.
  ACE_Allocator *allocator_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Queued_Data.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_QUEUED_DATA_H*/
