/* -*- C++ -*- */
/**
 *  @file Default_Factory.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_DEFAULT_FACTORY_H
#define TAO_Notify_DEFAULT_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Default_Factory
 *
 * @brief Default_Factory interface for creating Notify Objects.
 */
class TAO_Notify_Serv_Export TAO_Notify_Default_Factory : public TAO_Notify_Factory
{
public:
  /// Constructor
  TAO_Notify_Default_Factory ();

  /// Destructor
  ~TAO_Notify_Default_Factory () override;

  /// Create ProxySupplier Collection
  void create (TAO_Notify_ProxySupplier_Collection*& collection) override;

  /// Create ProxyConsumer Collection
  void create (TAO_Notify_ProxyConsumer_Collection*& collection) override;

  /// Create EventChannel Collection
  void create (TAO_Notify_EventChannel_Collection*& collection) override;

  /// Create ConsumerAdmin Collection
  void create (TAO_Notify_ConsumerAdmin_Collection*& collection) override;

  /// Create SupplierAdmin Collection
  void create (TAO_Notify_SupplierAdmin_Collection*& collection) override;

  /// Create Proxy Collection
  void create (TAO_Notify_Proxy_Collection*& collection) override;

  /// Create EventChannelDefault_Factory
  void create (TAO_Notify_EventChannelFactory*& channel_factory,
                       const char* factory_name) override;

  /// Create EventChannel
  void create (TAO_Notify_EventChannel*& channel,
                       const char* ec_name) override;

  /// Create SupplierAdmin
  void create (TAO_Notify_SupplierAdmin*& admin) override;

  /// Create ConsumerAdmin
  void create (TAO_Notify_ConsumerAdmin*& admin) override;

  /// Create ProxyPushConsumer
  void create (TAO_Notify_ProxyPushConsumer*& proxy) override;

  /// Create ProxyPushSupplier
  void create (TAO_Notify_ProxyPushSupplier*& proxy) override;

  /// Create CosEC_ProxyPushConsumer
  void create (TAO_Notify_CosEC_ProxyPushConsumer*& proxy) override;

  /// Create CosEC_ProxyPushSupplier
  void create (TAO_Notify_CosEC_ProxyPushSupplier*& proxy) override;

  /// Create StructuredProxyPushConsumer
  void create (TAO_Notify_StructuredProxyPushConsumer*& proxy) override;

  /// Create StructuredProxyPushSupplier
  void create (TAO_Notify_StructuredProxyPushSupplier*& proxy) override;

  /// Create SequenceProxyPushConsumer
  void create (TAO_Notify_SequenceProxyPushConsumer*& proxy) override;

  /// Create SequenceProxyPushSupplier
  void create (TAO_Notify_SequenceProxyPushSupplier*& proxy) override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_Notify_Serv, TAO_Notify_Default_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_Notify_DEFAULT_FACTORY_H */
