/* -*- C++ -*- */
/**
 *  @file PushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PUSHCONSUMER_H
#define TAO_NS_PUSHCONSUMER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Consumer.h"

/**
 * @class TAO_NS_PushConsumer
 *
 * @brief Wrapper for the PushConsumer that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_NS_PushConsumer : public TAO_NS_Consumer
{
public:
  /// Constuctor
  TAO_NS_PushConsumer (TAO_NS_ProxySupplier* proxy);

  /// Destructor
  ~TAO_NS_PushConsumer ();

  /// Init
  void init (CosEventComm::PushConsumer_ptr push_consumer ACE_ENV_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods.
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
  CosEventComm::PushConsumer_var push_consumer_;
};

#if defined (__ACE_INLINE__)
#include "PushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PUSHCONSUMER_H */
