/* -*- C++ -*- */
/**
 *  @file Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_FACTORY_H
#define TAO_NS_FACTORY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"

#include "Types.h"

class ACE_Lock;
class TAO_NS_Object;
class TAO_NS_EventChannelFactory;
class TAO_NS_EventChannel;
class TAO_NS_SupplierAdmin;
class TAO_NS_ConsumerAdmin;
class TAO_NS_ProxyPushConsumer;
class TAO_NS_CosEC_ProxyPushSupplier;
class TAO_NS_CosEC_ProxyPushConsumer;
class TAO_NS_ProxyPushSupplier;
class TAO_NS_StructuredProxyPushConsumer;
class TAO_NS_StructuredProxyPushSupplier;
class TAO_NS_SequenceProxyPushConsumer;
class TAO_NS_SequenceProxyPushSupplier;

/**
 * @class TAO_NS_Factory
 *
 * @brief Factory interface for creating Notify Objects.
 *
 */
class TAO_Notify_Export TAO_NS_Factory
{
public:
  /// Constuctor
  TAO_NS_Factory (void);

  /// Destructor
  virtual ~TAO_NS_Factory ();

  /// Create Object Lock.
  virtual ACE_Lock* create_object_lock (ACE_ENV_SINGLE_ARG_DECL);

  /// Create Collection
  virtual void create (TAO_NS_ProxySupplier_Collection*& collection ACE_ENV_ARG_DECL);
  virtual void create (TAO_NS_ProxyConsumer_Collection*& collection ACE_ENV_ARG_DECL);

  virtual void create (TAO_NS_EventChannel_Collection*& collection ACE_ENV_ARG_DECL);
  virtual void create (TAO_NS_Admin_Collection*& collection ACE_ENV_ARG_DECL);
  virtual void create (TAO_NS_Proxy_Collection*& collection ACE_ENV_ARG_DECL);

  virtual void create (TAO_NS_Peer_Collection*& collection ACE_ENV_ARG_DECL);
  virtual void create (TAO_NS_Consumer_Collection*& collection ACE_ENV_ARG_DECL);
  virtual void create (TAO_NS_Supplier_Collection*& collection ACE_ENV_ARG_DECL);

  /// Create EventChannelFactory
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

#if defined (__ACE_INLINE__)
#include "Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_FACTORY_H */
