/* -*- C++ -*- */
/**
 *  @file Default_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
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
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Default_Factory : public TAO_Notify_Factory
{
public:
  /// Constructor
  TAO_Notify_Default_Factory (void);

  /// Destructor
  virtual ~TAO_Notify_Default_Factory ();

  /// Create ProxySupplier Collection
  virtual void create (TAO_Notify_ProxySupplier_Collection*& collection);

  /// Create ProxyConsumer Collection
  virtual void create (TAO_Notify_ProxyConsumer_Collection*& collection);

  /// Create EventChannel Collection
  virtual void create (TAO_Notify_EventChannel_Collection*& collection);

  /// Create ConsumerAdmin Collection
  virtual void create (TAO_Notify_ConsumerAdmin_Collection*& collection);

  /// Create SupplierAdmin Collection
  virtual void create (TAO_Notify_SupplierAdmin_Collection*& collection);

  /// Create Proxy Collection
  virtual void create (TAO_Notify_Proxy_Collection*& collection);

  /// Create EventChannelDefault_Factory
  virtual void create (TAO_Notify_EventChannelFactory*& channel_factory,
                       const char* factory_name);

  /// Create EventChannel
  virtual void create (TAO_Notify_EventChannel*& channel,
                       const char* ec_name);

  /// Create SupplierAdmin
  virtual void create (TAO_Notify_SupplierAdmin*& admin);

  /// Create ConsumerAdmin
  virtual void create (TAO_Notify_ConsumerAdmin*& admin);

  /// Create ProxyPushConsumer
  virtual void create (TAO_Notify_ProxyPushConsumer*& proxy);

  /// Create ProxyPushSupplier
  virtual void create (TAO_Notify_ProxyPushSupplier*& proxy);

  /// Create CosEC_ProxyPushConsumer
  virtual void create (TAO_Notify_CosEC_ProxyPushConsumer*& proxy);

  /// Create CosEC_ProxyPushSupplier
  virtual void create (TAO_Notify_CosEC_ProxyPushSupplier*& proxy);

  /// Create StructuredProxyPushConsumer
  virtual void create (TAO_Notify_StructuredProxyPushConsumer*& proxy);

  /// Create StructuredProxyPushSupplier
  virtual void create (TAO_Notify_StructuredProxyPushSupplier*& proxy);

  /// Create SequenceProxyPushConsumer
  virtual void create (TAO_Notify_SequenceProxyPushConsumer*& proxy);

  /// Create SequenceProxyPushSupplier
  virtual void create (TAO_Notify_SequenceProxyPushSupplier*& proxy);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_Notify_Serv, TAO_Notify_Default_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_Notify_DEFAULT_FACTORY_H */
