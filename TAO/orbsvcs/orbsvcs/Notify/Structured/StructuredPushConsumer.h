/* -*- C++ -*- */
/**
 *  @file StructuredPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_STRUCTUREDPUSHCONSUMER_H
#define TAO_Notify_STRUCTUREDPUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "../notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Consumer.h"

class TAO_Notify_ProxySupplier;

/**
 * @class TAO_Notify_StructuredPushConsumer
 *
 * @brief Wrapper for the StructuredPushConsumer that connect to the EventChannel.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredPushConsumer : public TAO_Notify_Consumer
{
public:
  /// Constuctor
  TAO_Notify_StructuredPushConsumer (TAO_Notify_ProxySupplier* proxy);

  /// Init the Consumer
  void init (CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL);

  /// Destructor
  ~TAO_Notify_StructuredPushConsumer ();

  /// Release
  virtual void release (void);

  /// Push <event> to this consumer.
//  virtual void push_i (const TAO_Notify_Event* event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push (const CORBA::Any& event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL);

  /// Push a batch of events to this consumer.
  virtual void push (const CosNotification::EventBatch& event ACE_ENV_ARG_DECL);

  /// Retrieve the ior of this peer
  virtual bool get_ior (ACE_CString & iorstr) const;

  /// on reconnect we need to move events from the old consumer
  /// to the new one
  virtual void reconnect_from_consumer (
    TAO_Notify_Consumer* old_consumer
    ACE_ENV_ARG_DECL);


protected:
  /// The Consumer
  CosNotifyComm::StructuredPushConsumer_var push_consumer_;
};

#if defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_STRUCTUREDPUSHCONSUMER_H */
