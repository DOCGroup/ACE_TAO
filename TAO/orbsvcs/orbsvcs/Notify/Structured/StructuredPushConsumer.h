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

#ifndef TAO_NS_STRUCTUREDPUSHCONSUMER_H
#define TAO_NS_STRUCTUREDPUSHCONSUMER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Consumer.h"

class TAO_NS_ProxySupplier;

/**
 * @class TAO_NS_StructuredPushConsumer
 *
 * @brief Wrapper for the StructuredPushConsumer that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_NS_StructuredPushConsumer : public TAO_NS_Consumer
{
public:
  /// Constuctor
  TAO_NS_StructuredPushConsumer (TAO_NS_ProxySupplier* proxy);

  /// Init the Consumer
  void init (CosNotifyComm::StructuredPushConsumer_ptr push_consumer ACE_ENV_ARG_DECL);

  /// Destructor
  ~TAO_NS_StructuredPushConsumer ();

  /// Release
  virtual void release (void);

  /// Push <event> to this consumer.
  virtual void push_i (const TAO_NS_Event* event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push_i (const TAO_NS_Event_var& event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push (const CORBA::Any& event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL);

protected:
  /// The Consumer
  CosNotifyComm::StructuredPushConsumer_var push_consumer_;
};

#if defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPUSHCONSUMER_H */
