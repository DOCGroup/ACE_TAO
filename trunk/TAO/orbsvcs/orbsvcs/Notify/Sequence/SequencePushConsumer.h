/* -*- C++ -*- */
/**
 *  @file SequencePushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_SEQUENCEPUSHCONSUMER_H
#define TAO_NS_SEQUENCEPUSHCONSUMER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"
#include "../Event.h"
#include "../Property.h"
#include "../Property_T.h"
#include "../Consumer.h"
#include "EventBatch.h"

class TAO_NS_ProxySupplier;
class TAO_NS_QoSProperties;

/**
 * @class TAO_NS_SequencePushConsumer
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_SequencePushConsumer : public ACE_Event_Handler, public TAO_NS_Consumer
{
public:
  /// Constuctor
  TAO_NS_SequencePushConsumer (TAO_NS_ProxySupplier* proxy);

  /// Destructor
  ~TAO_NS_SequencePushConsumer ();

  /// Init the Consumer
  void init (CosNotifyComm::SequencePushConsumer_ptr push_consumer ACE_ENV_ARG_DECL);

  /// Shutdown the consumer
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods.
  virtual void release (void);

  /// Push <event> to this consumer.
  void push_i (const TAO_NS_Event_var& event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push (const CORBA::Any& event ACE_ENV_ARG_DECL);

  // Push event.
  virtual void push (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  // virtual void push (const CosNotification::EventBatch& event ACE_ENV_ARG_DECL);

  /// Push Collection.
  void push (const TAO_NS_Event_Collection event_collection ACE_ENV_ARG_DECL);

  /// Override, Peer::qos_changed
  virtual void qos_changed (const TAO_NS_QoSProperties& qos_properties);

protected:
  /// When the pacing interval is used, handle_timeout () is called by
  /// the reactor.
  virtual int handle_timeout (const ACE_Time_Value& current_time,
                              const void* act = 0);

  /// Schedule timer
  void schedule_timer (void);

  /// The Pacing Interval
  TAO_NS_Property_Time pacing_interval_;

  /// Timer Id.
  long timer_id_;

  /// The Consumer
  CosNotifyComm::SequencePushConsumer_var push_consumer_;

  /// EventBatch
  TAO_NS_EventBatch event_batch_;
};

#if defined (__ACE_INLINE__)
#include "SequencePushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_SEQUENCEPUSHCONSUMER_H */
