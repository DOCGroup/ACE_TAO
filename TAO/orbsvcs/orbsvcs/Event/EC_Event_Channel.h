/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Event_Channel
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   A new implementation of the Real Time Event Services.
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_EVENT_CHANNEL_H
#define TAO_EC_EVENT_CHANNEL_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "EC_Factory.h"

class TAO_ORBSVCS_Export TAO_EC_Event_Channel : public POA_RtecEventChannelAdmin::EventChannel
{
  // = TITLE
  //   The RtecEventChannelAdmin::EventChannel implementation.
  //
  // = DESCRIPTION
  //   This class is the Mediator between all the classes in the EC
  //   implementation, its main task is to redirect the messages to
  //   the right components, to hold and manage the lifetime of the
  //   long lived objects (Timer_Module, SupplierAdmin,
  //   ConsumerAdmin and Dispatching) and to provide a simpler
  //   interface to the EC_Factory.
  //
public:
  TAO_EC_Event_Channel (PortableServer::POA_ptr supplier_poa,
                        PortableServer::POA_ptr consumer_poa,
                        TAO_EC_Factory* factory = 0,
                        int own_factory = 0);
  // constructor
  // If <own_factory> is not 0 it assumes ownership of the factory.
  // If the factory is <nil> it uses the Service_Configurator to load
  // the Factory, if not found it uses TAO_EC_Default_Resource_Factory

  virtual ~TAO_EC_Event_Channel (void);
  // destructor

  virtual void activate (CORBA::Environment& env);
  // Start the internal threads (if any), etc.
  // After this call the EC can be used.

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown any internal threads, cleanup all the internal
  // structures, flush all the messages, etc.

  TAO_EC_Dispatching* dispatching (void) const;
  // Access the dispatching module....

  TAO_EC_Filter_Builder* filter_builder (void) const;
  // Access the filter builder....

  TAO_EC_Supplier_Filter_Builder* supplier_filter_builder (void) const;
  // Access the filter builder....

  TAO_EC_ConsumerAdmin* consumer_admin (void) const;
  // Access the consumer admin implementation, useful for controlling
  // the activation...

  TAO_EC_SupplierAdmin* supplier_admin (void) const;
  // Access the supplier admin implementation, useful for controlling
  // the activation...

  TAO_EC_Timeout_Generator* timeout_generator (void) const;
  // Access the timer module...

  // = The factory methods, they delegate on the EC_Factory.
  TAO_EC_ProxyPushSupplier* create_proxy_push_supplier (void);
  void destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier*);
  // Create and destroy a ProxyPushSupplier

  TAO_EC_ProxyPushConsumer* create_proxy_push_consumer (void);
  void destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer*);
  // Create and destroy a ProxyPushConsumer

  TAO_EC_ProxyPushSupplier_Set* create_proxy_push_supplier_set (void);
  void destroy_proxy_push_supplier_set (TAO_EC_ProxyPushSupplier_Set*);
  // Create and destroy a ProxyPushSupplier_Set

  PortableServer::POA_ptr supplier_poa (void);
  PortableServer::POA_ptr consumer_poa (void);
  // Access the supplier and consumer POAs from the factory.

  ACE_Lock* create_consumer_lock (void);
  void destroy_consumer_lock (ACE_Lock*);
  ACE_Lock* create_supplier_lock (void);
  void destroy_supplier_lock (ACE_Lock*);
  // Locking strategies for the ProxyPushConsumer and
  // ProxyPushSupplier objects

  ACE_Lock* create_consumer_admin_lock (void);
  void destroy_consumer_admin_lock (ACE_Lock*);
  ACE_Lock* create_supplier_admin_lock (void);
  void destroy_supplier_admin_lock (ACE_Lock*);
  // Locking strategies for the ConsumerAdmin and SupplierAdmin
  // objects

  virtual void connected (TAO_EC_ProxyPushConsumer*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*,
                             CORBA::Environment&);
  // Used to inform the EC that a Consumer has connected or
  // disconnected from it.

  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);
  // Used to inform the EC that a Supplier has connected or
  // disconnected from it.

  // = The RtecEventChannelAdmin::EventChannel methods...
  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
      for_consumers (CORBA::Environment& env);
  // The default implementation is:
  //    this->consumer_admin ()->_this (env);

  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
      for_suppliers (CORBA::Environment& env);
  // The default implementation is:
  //    this->supplier_admin ()->_this (env);

  virtual void destroy (CORBA::Environment &env);
  // Commit suicide.

  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr,
		       CORBA::Environment &env)
      TAO_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                       RtecEventChannel::EventChannel::CANT_APPEND_OBSERVER));
  virtual void
      remove_observer (RtecEventChannelAdmin::Observer_Handle,
                       CORBA::Environment &env)
      TAO_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                       RtecEventChannel::EventChannel::CANT_REMOVE_OBSERVER));

private:
  PortableServer::POA_var supplier_poa_;
  PortableServer::POA_var consumer_poa_;
  // The POAs used to activate "supplier-side" and "consumer-side"
  // objects.

  TAO_EC_Factory *factory_;
  // This is the abstract factory that creates all the objects that
  // compose an event channel, the event channel simply acts as a
  // Mediator among them.

  int own_factory_;
  // Flag that indicates if we own the factory.

  TAO_EC_Dispatching *dispatching_;
  // The dispatching "module"

  TAO_EC_Filter_Builder *filter_builder_;
  // The filter builder

  TAO_EC_Supplier_Filter_Builder *supplier_filter_builder_;
  // The filter builder for suppliers

  TAO_EC_ConsumerAdmin *consumer_admin_;
  // The ConsumerAdmin implementation

  TAO_EC_SupplierAdmin *supplier_admin_;
  // The SupplierAdmin implementation

  TAO_EC_Timeout_Generator *timeout_generator_;
  // The timeout generator

  TAO_EC_ObserverStrategy *observer_strategy_;
  // The observer strategy
};

#if defined (__ACE_INLINE__)
#include "EC_Event_Channel.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_EVENT_CHANNEL_H */
