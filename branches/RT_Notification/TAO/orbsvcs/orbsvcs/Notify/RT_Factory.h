/* -*- C++ -*- */
/**
 *  @file RT_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RT_FACTORY_H
#define TAO_NS_RT_FACTORY_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Default_Factory.h"

/**
 * @class TAO_NS_RT_Factory
 *
 * @brief The Factory for the RT aware Notify classes.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RT_Factory : public TAO_NS_Default_Factory
{
public:
  /// Constuctor
  TAO_NS_RT_Factory (void);

  /// Destructor
  virtual ~TAO_NS_RT_Factory ();

  /// Create ProxySupplier Collection
  virtual void create (TAO_NS_ProxySupplier_Collection*& collection ACE_ENV_ARG_DECL);

  /// Create ProxyConsumer Collection
  virtual void create (TAO_NS_ProxyConsumer_Collection*& collection ACE_ENV_ARG_DECL);

  /// Create EventChannel Collection
  virtual void create (TAO_NS_EventChannel_Collection*& collection ACE_ENV_ARG_DECL);

  /// Create ConsumerAdmin Collection
  virtual void create (TAO_NS_ConsumerAdmin_Collection*& collection ACE_ENV_ARG_DECL);

  /// Create SupplierAdmin Collection
  virtual void create (TAO_NS_SupplierAdmin_Collection*& collection ACE_ENV_ARG_DECL);

  /// Create Proxy Collection
  virtual void create (TAO_NS_Proxy_Collection*& collection ACE_ENV_ARG_DECL);

  /// Create EventChannelDefault_Factory
  virtual void create (TAO_NS_EventChannelFactory*& channel_factory ACE_ENV_ARG_DECL);

  /// Create EventChannel
  virtual void create (TAO_NS_EventChannel*& channel ACE_ENV_ARG_DECL);

  /// Create SupplierAdmin
  virtual void create (TAO_NS_SupplierAdmin*& admin ACE_ENV_ARG_DECL);

  /// Create ConsumerAdmin
  virtual void create (TAO_NS_ConsumerAdmin*& admin ACE_ENV_ARG_DECL);

  /// Create ProxyPushConsumer
  virtual void create (TAO_NS_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL);

  /// Create ProxyPushSupplier
  virtual void create (TAO_NS_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL);

  /// Create CosEC_ProxyPushConsumer
  virtual void create (TAO_NS_CosEC_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL);

  /// Create CosEC_ProxyPushSupplier
  virtual void create (TAO_NS_CosEC_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL);

  /// Create StructuredProxyPushConsumer
  virtual void create (TAO_NS_StructuredProxyPushConsumer*& proxy ACE_ENV_ARG_DECL);

  /// Create StructuredProxyPushSupplier
  virtual void create (TAO_NS_StructuredProxyPushSupplier*& proxy ACE_ENV_ARG_DECL);

  /// Create SequenceProxyPushConsumer
  virtual void create (TAO_NS_SequenceProxyPushConsumer*& proxy ACE_ENV_ARG_DECL);

  /// Create SequenceProxyPushSupplier
  virtual void create (TAO_NS_SequenceProxyPushSupplier*& proxy ACE_ENV_ARG_DECL);
};

ACE_FACTORY_DECLARE (TAO_RT_Notify, TAO_NS_RT_Factory)

#if defined (__ACE_INLINE__)
#include "RT_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RT_FACTORY_H */
