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

#ifndef TAO_Notify_SEQUENCEPUSHCONSUMER_H
#define TAO_Notify_SEQUENCEPUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"
#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/Property.h"
#include "orbsvcs/Notify/Property_T.h"
#include "orbsvcs/Notify/Consumer.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "ace/Null_Condition.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ProxySupplier;
class TAO_Notify_QoSProperties;
class TAO_Notify_Timer;

/**
 * @class TAO_Notify_SequencePushConsumer
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_SequencePushConsumer
  : public TAO_Notify_Consumer
{
public:
  TAO_Notify_SequencePushConsumer (TAO_Notify_ProxySupplier* proxy);

  virtual ~TAO_Notify_SequencePushConsumer ();

  /// Init the Consumer
  void init (CosNotifyComm::SequencePushConsumer_ptr push_consumer ACE_ENV_ARG_DECL);

  /// Add request to a queue if necessary.
  /// for Sequence it's always necessary.
  virtual bool enqueue_if_necessary(
    TAO_Notify_Method_Request_Event * request
    ACE_ENV_ARG_DECL);

  virtual bool dispatch_from_queue (
    Request_Queue & requests,
    ACE_Guard <TAO_SYNCH_MUTEX> & ace_mon);


  /// Push <event> to this consumer.
  virtual void push (const CORBA::Any& event ACE_ENV_ARG_DECL);

  // Push event.
  virtual void push (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL);

  /// Push a batch of events to this consumer.
  virtual void push (const CosNotification::EventBatch& event ACE_ENV_ARG_DECL);

  /// Retrieve the ior of this peer
  virtual ACE_CString get_ior (void) const;

  /// on reconnect we need to move events from the old consumer
  /// to the new one
  virtual void reconnect_from_consumer (TAO_Notify_Consumer* old_consumer
    ACE_ENV_ARG_DECL);

protected:

  /// The Consumer
  CosNotifyComm::SequencePushConsumer_var push_consumer_;

private:
  /// TAO_Notify_Destroy_Callback methods.
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SEQUENCEPUSHCONSUMER_H */
