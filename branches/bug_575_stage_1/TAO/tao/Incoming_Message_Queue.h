// -*- C++ -*-

//=============================================================================
/**
 *  @file Incoming_Queued_Message.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_INCOMING_MESSAGE_QUEUE_H
#define TAO_INCOMING_MESSAGE_QUEUE_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Forward declarations
class ACE_Data_Block;
class TAO_ORB_Core;
class TAO_Queued_Data;
class TAO_Transport;
enum TAO_Pluggable_Message_Type;

/**
 * @class TAO_Incoming_Message_Queue
 * //@@Bala: Documentation..
 */

class TAO_Export TAO_Incoming_Message_Queue
{
public:
  /// Ctor.
  TAO_Incoming_Message_Queue (TAO_ORB_Core *orb_core);

  /// Dtor.
  ~TAO_Incoming_Message_Queue (void);


  /// @@Bala:Docu
  /*  int add_message (ACE_Message_Block *block,
                   ssize_t missing_data,
                   CORBA::Octet byte_order);*/

  void copy_message (ACE_Message_Block &block);

  CORBA::ULong queue_length (void);

  int is_tail_complete (void);

  int is_head_complete (void);

  size_t missing_data (void) const;
  void missing_data (size_t data);


  TAO_Queued_Data *dequeue_head (void);
  TAO_Queued_Data *dequeue_tail (void);



  int enqueue_tail (TAO_Queued_Data *nd);

private:

  friend class TAO_Transport;

  /// @@Bala:Docu
  TAO_Queued_Data *get_node (void);

private:
  ///
  TAO_Queued_Data *queued_data_;

  /// @@Bala:Docu
  CORBA::ULong size_;

  TAO_ORB_Core *orb_core_;
};

class TAO_Export TAO_Queued_Data
{
public:
  TAO_Queued_Data (void);

  ~TAO_Queued_Data (void);

  static TAO_Queued_Data* get_queued_data (void);

  /// The actual message queue
  ACE_Message_Block *msg_block_;

  CORBA::Long missing_data_;

  CORBA::Octet byte_order_;

  CORBA::Octet major_version_;

  CORBA::Octet minor_version_;

  TAO_Pluggable_Message_Type msg_type_;

  TAO_Queued_Data *next_;
};


#if defined (__ACE_INLINE__)
# include "Incoming_Message_Queue.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_INCOMING_MESSAGE_QUEUE_H*/
