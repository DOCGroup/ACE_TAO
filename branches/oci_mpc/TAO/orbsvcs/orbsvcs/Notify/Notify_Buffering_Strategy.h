/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Buffering_Strategy.h
 *
 *  $Id$
 *
 * A Strategy class that buffers command objects in a message queue.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_NOTIFY_BUFFERING_STRATEGY_H
#define TAO_NOTIFY_BUFFERING_STRATEGY_H

#include "ace/pre.h"
#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_AdminProperties.h"
#include "orbsvcs/TimeBaseC.h"

#include "ace/Message_Queue.h"
#include "ace/Synch.h"

class TAO_Notify_Command;

/**
 * @class TAO_Notify_Buffering_Strategy
 *
 * @brief TAO_Notify_Buffering_Strategy
 *
 * This buffering strategy buffers and discards events according to the
 * policies specified in the CosNotification.idl
 */
class TAO_Notify_Export TAO_Notify_Buffering_Strategy
{
public:
  TAO_Notify_Buffering_Strategy (TAO_Notify_Signal_Property_Long* const queue_length);

  virtual ~TAO_Notify_Buffering_Strategy ();

  /// Execute this strategy.
  virtual void execute (ACE_Message_Queue<ACE_SYNCH>* msg_queue, TAO_Notify_Command *mb ACE_ENV_ARG_DECL, ACE_Time_Value *tv);

  /// Set the max. queue length.
  void max_queue_length (CORBA::Long max_queue_length);

  /// Set the max. events per consumer.
  void max_events_per_consumer (CORBA::Long max_events_per_consumer);

  /// Set the order policy.
  void order_policy (CORBA::Short order_policy);

  /// Set the discard policy.
  void discard_policy (CORBA::Short discard_policy);

  /// Set the blocking timeout.
  void blocking_timeout (TimeBase::TimeT timeout);

protected:
  /// The overall queue length.
  TAO_Notify_Signal_Property_Long* const queue_length_;

  /// Discarding policies kicks in when the <queue_length_> exceeds <max_queue_length_>
  CORBA::Long max_queue_length_;

  /// Discarding policies kicks in when the <queue_length_> exceeds <max_events_per_consumer_>
  CORBA::Long max_events_per_consumer_;

  /// Order of events in internal buffers.
  CORBA::Short order_policy_;

  /// Policy to discard when buffers are full.
  CORBA::Short discard_policy_;

  /// Should we use the max queue or max events.
  CORBA::Boolean use_max_queue_;

  /// The blocking timeout will be used in place of discarding
  /// if use_blocking_timeout_ is set.  This is a TAO specific extension.
  TimeBase::TimeT blocking_timeout_;
  CORBA::Boolean use_blocking_timeout_;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_BUFFERING_STRATEGY_H */
