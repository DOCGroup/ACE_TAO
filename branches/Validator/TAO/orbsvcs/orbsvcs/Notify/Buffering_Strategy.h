/* -*- C++ -*- */
/**
 *  @file Buffering_Strategy.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_BUFFERING_STRATEGY_H
#define TAO_NS_BUFFERING_STRATEGY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_NS_AdminProperties;

#include "orbsvcs/TimeBaseC.h"
#include "Property.h"
#include "Property_T.h"
#include "ace/Message_Queue.h"

class ACE_Activation_Queue;
class TAO_NS_AdminProperties;
class TAO_NS_Method_Request;
class TAO_NS_QoSProperties;

/**
 * @class TAO_NS_Buffering_Strategy
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_Buffering_Strategy
{
public:
  /// Constuctor
  TAO_NS_Buffering_Strategy (ACE_Message_Queue<ACE_SYNCH>& msg_queue, TAO_NS_AdminProperties& admin_properties);

  /// Destructor
  ~TAO_NS_Buffering_Strategy ();

  /// Execute this strategy.
  void execute (TAO_NS_Method_Request& method_request, ACE_Time_Value *tv);

  /// Update QoS Properties.
  void update_qos_properties (const TAO_NS_QoSProperties& qos_properties);

protected:
  /// Message Queue;
  ACE_Message_Queue<ACE_SYNCH>& msg_queue_;

  /// The overall queue length.
  TAO_NS_Signal_Property_Long& queue_length_;

  /// Discarding policies kicks in when the <queue_length_> exceeds <max_queue_length_>
  const TAO_NS_Property_Long& max_queue_length_;

  /// Discarding policies kicks in when the <queue_length_> exceeds <max_events_per_consumer_>
  TAO_NS_Property_Long max_events_per_consumer_;

  /// Order of events in internal buffers.
  TAO_NS_Property_Short order_policy_;

  /// Policy to discard when buffers are full.
  TAO_NS_Property_Short discard_policy_;

  /// Should we use the max queue or max events.
  CORBA::Boolean use_max_queue_;

  /// The blocking timeout will be used in place of discarding
  /// if use_blocking_timeout_ is set.  This is a TAO specific extension.
  TAO_NS_Property_Time blocking_timeout_;
};

#if defined (__ACE_INLINE__)
#include "Buffering_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_BUFFERING_STRATEGY_H */
