/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_FACTORY_H
#define TAO_CEC_FACTORY_H
#include /**/ "ace/pre.h"

#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "ace/Service_Object.h"

class ACE_Lock;

class TAO_CEC_EventChannel;

class TAO_CEC_Dispatching;
class TAO_CEC_Pulling_Strategy;
class TAO_CEC_ConsumerAdmin;
class TAO_CEC_SupplierAdmin;
class TAO_CEC_ProxyPushConsumer;
class TAO_CEC_ProxyPullConsumer;
class TAO_CEC_ProxyPushSupplier;
class TAO_CEC_ProxyPullSupplier;
template<class PROXY> class TAO_ESF_Proxy_Collection;
class TAO_CEC_ConsumerControl;
class TAO_CEC_SupplierControl;

typedef TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPushConsumer> TAO_CEC_ProxyPushConsumer_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPullConsumer> TAO_CEC_ProxyPullConsumer_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPushSupplier> TAO_CEC_ProxyPushSupplier_Collection;
typedef TAO_ESF_Proxy_Collection<TAO_CEC_ProxyPullSupplier> TAO_CEC_ProxyPullSupplier_Collection;

/**
 * @class TAO_CEC_Factory
 *
 * @brief Abstract factory for the CosEventChannel components.
 *
 * The CosEventChannel implementation can be configured at
 * initialization time through several strategies and
 * components. This class defines the interface of an Abstract
 * Factory that creates all such components.
 * = MEMORY MANAGMENT
 * The objects it creates are owned by this class, the client must
 * invoke the corresponding destroy() method to release them.
 * Some implementations may require a different instance for the
 * CEC_Factory for each instance of a CEC_EventChannel.
 */
class TAO_Event_Export TAO_CEC_Factory : public ACE_Service_Object
{
public:
  /// destructor...
  virtual ~TAO_CEC_Factory (void);

  /// Create and destroy the dispatching module.
  virtual TAO_CEC_Dispatching*
      create_dispatching (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_dispatching (TAO_CEC_Dispatching*)  = 0;

  /// Create and destroy the pulling strategy.
  virtual TAO_CEC_Pulling_Strategy*
      create_pulling_strategy (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_pulling_strategy (TAO_CEC_Pulling_Strategy*)  = 0;

  /// Create and destroy the consumer admin implementation.
  virtual TAO_CEC_ConsumerAdmin*
      create_consumer_admin (TAO_CEC_EventChannel*)  = 0;
  virtual void
      destroy_consumer_admin (TAO_CEC_ConsumerAdmin*) = 0;

  /// Create and destroy the supplier admin implementation.
  virtual TAO_CEC_SupplierAdmin*
      create_supplier_admin (TAO_CEC_EventChannel*)  = 0;
  virtual void
      destroy_supplier_admin (TAO_CEC_SupplierAdmin*) = 0;

  /// Create and destroy a ProxyPushSupplier
  virtual TAO_CEC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_push_supplier (TAO_CEC_ProxyPushSupplier*) = 0;

  /// Create and destroy a ProxyPullSupplier
  virtual TAO_CEC_ProxyPullSupplier*
      create_proxy_pull_supplier (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_pull_supplier (TAO_CEC_ProxyPullSupplier*) = 0;

  /// Create and destroy a ProxyPushConsumer
  virtual TAO_CEC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_push_consumer (TAO_CEC_ProxyPushConsumer*) = 0;

  /// Create and destroy a ProxyPullConsumer
  virtual TAO_CEC_ProxyPullConsumer*
      create_proxy_pull_consumer (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_pull_consumer (TAO_CEC_ProxyPullConsumer*) = 0;

  /// Create and destroy a collection of TAO_CEC_ProxyPushConsumers
  virtual TAO_CEC_ProxyPushConsumer_Collection*
      create_proxy_push_consumer_collection (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_push_consumer_collection (TAO_CEC_ProxyPushConsumer_Collection*) = 0;

  /// Create and destroy a collection of TAO_CEC_ProxyPullConsumers
  virtual TAO_CEC_ProxyPullConsumer_Collection*
      create_proxy_pull_consumer_collection (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_pull_consumer_collection (TAO_CEC_ProxyPullConsumer_Collection*) = 0;

  /// Create and destroy a collection of TAO_CEC_ProxyPushSuppliers
  virtual TAO_CEC_ProxyPushSupplier_Collection*
      create_proxy_push_supplier_collection (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_push_supplier_collection (TAO_CEC_ProxyPushSupplier_Collection*) = 0;

  /// Create and destroy a collection of TAO_CEC_ProxyPullSuppliers
  virtual TAO_CEC_ProxyPullSupplier_Collection*
      create_proxy_pull_supplier_collection (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_proxy_pull_supplier_collection (TAO_CEC_ProxyPullSupplier_Collection*) = 0;

  /// Create and destroy the locking strategies for both
  /// ProxyPushConsumers and ProxyPushSuppliers
  virtual ACE_Lock* create_consumer_lock (void) = 0;
  virtual void destroy_consumer_lock (ACE_Lock*) = 0;
  virtual ACE_Lock* create_supplier_lock (void) = 0;
  virtual void destroy_supplier_lock (ACE_Lock*) = 0;

  /// The ConsumerControl and SupplierControl strategies are used to
  /// discard non-existent consumers and suppliers
  virtual TAO_CEC_ConsumerControl*
      create_consumer_control (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_consumer_control (TAO_CEC_ConsumerControl*) = 0;
  virtual TAO_CEC_SupplierControl*
      create_supplier_control (TAO_CEC_EventChannel*) = 0;
  virtual void
      destroy_supplier_control (TAO_CEC_SupplierControl*) = 0;
};

#if defined (__ACE_INLINE__)
#include "CEC_Factory.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CEC_FACTORY_H */
