/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_Buffering_Strategy.h
//
// = DESCRIPTION
//   A Strategy class that buffers command objects in a message queue.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_BUFFERING_STRATEGY_H
#define TAO_NOTIFY_BUFFERING_STRATEGY_H

#include "ace/pre.h"
#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_AdminProperties.h"

#include "ace/Message_Queue.h"
#include "ace/Synch.h"

class TAO_Notify_Command;

class TAO_Notify_Export TAO_Notify_Buffering_Strategy
{
  // = TITLE
  //   TAO_Notify_Buffering_Strategy
  //
  // = DESCRIPTION
  //   This buffering strategy buffers and discards events according to the
  //   policies specified in the CosNotification.idl
  //
public:
  TAO_Notify_Buffering_Strategy (TAO_Notify_Signal_Property_Long* const queue_length);

  virtual ~TAO_Notify_Buffering_Strategy ();

  virtual void execute (ACE_Message_Queue<ACE_SYNCH>* msg_queue, TAO_Notify_Command *mb TAO_ENV_ARG_DECL, ACE_Time_Value *tv);
  // Execute this strategy.

  void max_queue_length (CORBA::Long max_queue_length);
  // Set the max. queue length.

  void max_events_per_consumer (CORBA::Long max_events_per_consumer);
  // Set the max. events per consumer.

  void order_policy (CORBA::Short order_policy);
  // Set the order policy.

  void discard_policy (CORBA::Short discard_policy);
  // Set the discard policy.

  void blocking_timeout (TimeBase::TimeT timeout);
  // Set the blocking timeout.

protected:
  TAO_Notify_Signal_Property_Long* const queue_length_;
  // The overall queue length.

  CORBA::Long max_queue_length_;
  // Discarding policies kicks in when the <queue_length_> exceeds <max_queue_length_>

  CORBA::Long max_events_per_consumer_;
  // Discarding policies kicks in when the <queue_length_> exceeds <max_events_per_consumer_>

  CORBA::Short order_policy_;
  // Order of events in internal buffers.

  CORBA::Short discard_policy_;
  // Policy to discard when buffers are full.

  CORBA::Boolean use_max_queue_;
  // Should we use the max queue or max events.

  TimeBase::TimeT blocking_timeout_;
  CORBA::Boolean use_blocking_timeout_;
  // The blocking timeout will be used in place of discarding
  // if use_blocking_timeout_ is set.  This is a TAO specific extension.
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_BUFFERING_STRATEGY_H */
