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
class TAO_GIOP_Message_State;
class TAO_Queued_Data;

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
  int add_message (const ACE_Message_Block &block,
                   const TAO_GIOP_Message_State &state);

  const ACE_Data_Block *current_message (void) const;
  const CORBA::Octet current_byte_order (void) const;

  CORBA::ULong queue_length (void);

  int complete_message (void);


  ACE_Data_Block *get_current_message (CORBA::Octet &byte_order);

private:

  /// @@Bala:Docu
  class TAO_Export TAO_Queued_Data
  {
  public:
    TAO_Queued_Data (void);

    /// The actual message queue
    ACE_Data_Block *data_block_;

    CORBA::ULong missing_data_;

    CORBA::Octet byte_order_;

    TAO_Queued_Data *next_;
  };

  TAO_Queued_Data* get_node (void);
  int add_node (TAO_Queued_Data *nd);

private:
  ///
  TAO_Queued_Data *queued_data_;

  /// @@Bala:Docu
  CORBA::ULong size_;

  TAO_ORB_Core *orb_core_;
};


#if defined (__ACE_INLINE__)
# include "Incoming_Message_Queue.inl"
#endif /* __ACE_INLINE__ */

#endif /*TAO_INCOMING_MESSAGE_QUEUE_H*/
