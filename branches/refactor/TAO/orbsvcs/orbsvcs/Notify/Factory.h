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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "ace/CORBA_macros.h"
#include "ace/Service_Object.h"

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
class TAO_NS_ProxyConsumer;
class TAO_NS_ProxySupplier;
class TAO_NS_ConsumerAdmin;
class TAO_NS_SupplierAdmin;
class TAO_NS_Proxy;
class TAO_NS_Consumer;
class TAO_NS_Supplier;
class TAO_NS_Peer;

template <class PROXY> class TAO_ESF_Proxy_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_NS_ProxyConsumer> TAO_NS_ProxyConsumer_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_ProxySupplier> TAO_NS_ProxySupplier_Collection;

typedef TAO_ESF_Proxy_Collection<TAO_NS_EventChannel> TAO_NS_EventChannel_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_ConsumerAdmin> TAO_NS_ConsumerAdmin_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_SupplierAdmin> TAO_NS_SupplierAdmin_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_NS_Proxy> TAO_NS_Proxy_Collection;

/**
 * @class TAO_NS_Factory
 *
 * @brief Factory interface for creating Notify Objects.
 *
 */
class /*TAO_Notify_Export*/ TAO_NS_Factory : public ACE_Service_Object
{
public:
  /// Create ProxySupplier Collection
  virtual void create (TAO_NS_ProxySupplier_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create ProxyConsumer Collection
  virtual void create (TAO_NS_ProxyConsumer_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create EventChannel Collection
  virtual void create (TAO_NS_EventChannel_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create ConsumerAdmin Collection
  virtual void create (TAO_NS_ConsumerAdmin_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create SupplierAdmin Collection
  virtual void create (TAO_NS_SupplierAdmin_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create Proxy Collection
  virtual void create (TAO_NS_Proxy_Collection*& collection ACE_ENV_ARG_DECL) = 0;

  /// Create EventChannelFactory
  virtual void create (TAO_NS_EventChannelFactory*& channel_factory ACE_ENV_ARG_DECL) = 0;

  /// Create EventChannel
  virtual void create (TAO_NS_EventChannel*& channel ACE_ENV_ARG_DECL) = 0;

  /// Create SupplierAdmin
  virtual void create (TAO_NS_SupplierAdmin*& admin ACE_ENV_ARG_DECL) = 0;

  /// Create ConsumerAdmin
  virtual void create (TAO_NS_ConsumerAdmin*& admin ACE_ENV_ARG_DECL) = 0;

  /// Create ProxyPushConsumer
  virtual void create (TAO_NS_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create ProxyPushSupplier
  virtual void create (TAO_NS_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create CosEC_ProxyPushConsumer
  virtual void create (TAO_NS_CosEC_ProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create CosEC_ProxyPushSupplier
  virtual void create (TAO_NS_CosEC_ProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create StructuredProxyPushConsumer
  virtual void create (TAO_NS_StructuredProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create StructuredProxyPushSupplier
  virtual void create (TAO_NS_StructuredProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create SequenceProxyPushConsumer
  virtual void create (TAO_NS_SequenceProxyPushConsumer*& proxy ACE_ENV_ARG_DECL) = 0;

  /// Create SequenceProxyPushSupplier
  virtual void create (TAO_NS_SequenceProxyPushSupplier*& proxy ACE_ENV_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_NS_FACTORY_H */
