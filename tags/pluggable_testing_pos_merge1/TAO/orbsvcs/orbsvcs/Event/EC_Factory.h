/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Factory
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_FACTORY_H
#define TAO_EC_FACTORY_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "ace/Service_Object.h"

class ACE_Lock;

class TAO_EC_Event_Channel;

class TAO_EC_Dispatching;
class TAO_EC_Filter_Builder;
class TAO_EC_Supplier_Filter_Builder;
class TAO_EC_ConsumerAdmin;
class TAO_EC_SupplierAdmin;
class TAO_EC_ProxyPushConsumer;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_ObserverStrategy;
class TAO_EC_ProxyPushSupplier_Set;
class TAO_EC_Timeout_Generator;
class TAO_EC_Scheduling_Strategy;

class TAO_ORBSVCS_Export TAO_EC_Factory : public ACE_Service_Object
{
  // = TITLE
  //   Abstract base class for the Event Channel components.
  //
  // = DESCRIPTION
  //   Defines the EC_Factory interface.
  //
  // = MEMORY MANAGMENT
  //   The objects it creates are owned by this class, the client must
  //   invoke the corresponding destroy() method to release them.
  //   Some implementations may require a different instance for the
  //   EC_Factory for each instance of Event_Channel.
  //
public:
  virtual ~TAO_EC_Factory (void);
  // destructor...

  virtual TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel*) = 0;
  virtual void
      destroy_dispatching (TAO_EC_Dispatching*)  = 0;
  // Create and destroy the dispatching module.

  virtual TAO_EC_Filter_Builder*
      create_filter_builder (TAO_EC_Event_Channel*)   = 0;
  virtual void
      destroy_filter_builder (TAO_EC_Filter_Builder*) = 0;
  // Create and destroy the filter builder.

  virtual TAO_EC_Supplier_Filter_Builder*
      create_supplier_filter_builder (TAO_EC_Event_Channel*)   = 0;
  virtual void
      destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder*) = 0;
  // Create and destroy the filter builder.

  virtual TAO_EC_ConsumerAdmin*
      create_consumer_admin (TAO_EC_Event_Channel*)  = 0;
  virtual void
      destroy_consumer_admin (TAO_EC_ConsumerAdmin*) = 0;
  // Create and destroy the consumer admin implementation.

  virtual TAO_EC_SupplierAdmin*
      create_supplier_admin (TAO_EC_Event_Channel*)  = 0;
  virtual void
      destroy_supplier_admin (TAO_EC_SupplierAdmin*) = 0;
  // Create and destroy the supplier admin implementation.

  virtual TAO_EC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_EC_Event_Channel*) = 0;
  virtual void
      destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier*) = 0;
  // Create and destroy a ProxyPushSupplier

  virtual TAO_EC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_EC_Event_Channel*) = 0;
  virtual void
      destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer*) = 0;
  // Create and destroy a ProxyPushConsumer

  virtual TAO_EC_Timeout_Generator*
      create_timeout_generator (TAO_EC_Event_Channel*)  = 0;
  virtual void
      destroy_timeout_generator (TAO_EC_Timeout_Generator*) = 0;
  // Create and destroy the timer module.

  virtual TAO_EC_ObserverStrategy*
      create_observer_strategy (TAO_EC_Event_Channel*) = 0;
  virtual void
      destroy_observer_strategy (TAO_EC_ObserverStrategy*) = 0;
  // Create and destroy the observer strategy.

  virtual TAO_EC_Scheduling_Strategy*
      create_scheduling_strategy (TAO_EC_Event_Channel*) = 0;
  virtual void
      destroy_scheduling_strategy (TAO_EC_Scheduling_Strategy*) = 0;
  // Create and destroy the observer strategy.

  virtual TAO_EC_ProxyPushSupplier_Set*
      create_proxy_push_supplier_set (TAO_EC_Event_Channel*) = 0;
  virtual void
      destroy_proxy_push_supplier_set (TAO_EC_ProxyPushSupplier_Set*) = 0;
  // Create and destroy a ProxyPushSupplier_Set

  virtual ACE_Lock* create_consumer_lock (void) = 0;
  virtual void destroy_consumer_lock (ACE_Lock*) = 0;
  virtual ACE_Lock* create_supplier_lock (void) = 0;
  virtual void destroy_supplier_lock (ACE_Lock*) = 0;
  // Create and destroy the locking strategies for both
  // ProxyPushConsumers and ProxyPushSuppliers

  virtual ACE_Lock* create_consumer_admin_lock (void) = 0;
  virtual void destroy_consumer_admin_lock (ACE_Lock*) = 0;
  virtual ACE_Lock* create_supplier_admin_lock (void) = 0;
  virtual void destroy_supplier_admin_lock (ACE_Lock*) = 0;
  // Create and destroy the locking strategies for both
  // ConsumerAdmin and SupplierAdmin classes.
};

#if defined (__ACE_INLINE__)
#include "EC_Factory.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_FACTORY_H */
