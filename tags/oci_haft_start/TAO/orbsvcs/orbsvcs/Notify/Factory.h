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

#ifndef TAO_Notify_FACTORY_H
#define TAO_Notify_FACTORY_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "ace/CORBA_macros.h"
#include "ace/Service_Object.h"

class ACE_Lock;
class TAO_Notify_Object;
class TAO_Notify_EventChannelFactory;
class TAO_Notify_EventChannel;
class TAO_Notify_SupplierAdmin;
class TAO_Notify_ConsumerAdmin;
class TAO_Notify_ProxyPushConsumer;
class TAO_Notify_CosEC_ProxyPushSupplier;
class TAO_Notify_CosEC_ProxyPushConsumer;
class TAO_Notify_ProxyPushSupplier;
class TAO_Notify_StructuredProxyPushConsumer;
class TAO_Notify_StructuredProxyPushSupplier;
class TAO_Notify_SequenceProxyPushConsumer;
class TAO_Notify_SequenceProxyPushSupplier;
class TAO_Notify_ProxyConsumer;
class TAO_Notify_ProxySupplier;
class TAO_Notify_ConsumerAdmin;
class TAO_Notify_SupplierAdmin;
class TAO_Notify_Proxy;
class TAO_Notify_Consumer;
class TAO_Notify_Supplier;
class TAO_Notify_Peer;

template <class PROXY> class TAO_ESF_Proxy_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_Notify_ProxyConsumer> TAO_Notify_ProxyConsumer_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_Notify_ProxySupplier> TAO_Notify_ProxySupplier_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_Notify_EventChannel> TAO_Notify_EventChannel_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_Notify_ConsumerAdmin> TAO_Notify_ConsumerAdmin_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_Notify_SupplierAdmin> TAO_Notify_SupplierAdmin_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_Notify_Proxy> TAO_Notify_Proxy_Collection;

/**
 * @class TAO_Notify_Factory
 *
 * @brief Factory interface for creating Notify Objects.
 *
 */
class /*TAO_Notify_Export*/ TAO_Notify_Factory : public ACE_Service_Object
{
public:
  /// Create ProxySupplier Collection
  virtual void create (TAO_Notify_ProxySupplier_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create ProxyConsumer Collection
  virtual void create (TAO_Notify_ProxyConsumer_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create EventChannel Collection
  virtual void create (TAO_Notify_EventChannel_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create ConsumerAdmin Collection
  virtual void create (TAO_Notify_ConsumerAdmin_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create SupplierAdmin Collection
  virtual void create (TAO_Notify_SupplierAdmin_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create Proxy Collection
  virtual void create (TAO_Notify_Proxy_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create EventChannelFactory
  virtual void create (TAO_Notify_EventChannelFactory*& channel_factory ACE_ENV_ARG_DECL) = 0;

  /// Create EventChannel
  virtual void create (TAO_Notify_EventChannel*& channel ACE_ENV_ARG_DECL) = 0;

  /// Create SupplierAdmin
  virtual void create (TAO_Notify_SupplierAdmin*& admin ACE_ENV_ARG_DECL) = 0;

  /// Create ConsumerAdmin
  virtual void create (TAO_Notify_ConsumerAdmin*& admin ACE_ENV_ARG_DECL) = 0;

  /// Create ProxyPushConsumer
  virtual void create (TAO_Notify_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create ProxyPushSupplier
  virtual void create (TAO_Notify_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create CosEC_ProxyPushConsumer
  virtual void create (TAO_Notify_CosEC_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create CosEC_ProxyPushSupplier
  virtual void create (TAO_Notify_CosEC_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create StructuredProxyPushConsumer
  virtual void create (TAO_Notify_StructuredProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create StructuredProxyPushSupplier
  virtual void create (TAO_Notify_StructuredProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create SequenceProxyPushConsumer
  virtual void create (TAO_Notify_SequenceProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create SequenceProxyPushSupplier
  virtual void create (TAO_Notify_SequenceProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_Notify_FACTORY_H */
