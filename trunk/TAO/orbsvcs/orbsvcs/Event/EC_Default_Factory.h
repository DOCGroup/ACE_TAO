/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Default_Factory
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

#ifndef TAO_EC_DEFAULT_FACTORY_H
#define TAO_EC_DEFAULT_FACTORY_H

#include "EC_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_SupplierFiltering;

class TAO_ORBSVCS_Export TAO_EC_Default_Factory : public TAO_EC_Factory
{
  // = TITLE
  //   The factory for a simple event channel.
  //
  // = DESCRIPTION
  //   An slightly more advanced configuration than the
  //   EC_Null_Factory, this class configure an event channel that can
  //   support filtering and correlation. Still dispatching is not
  //   prioritized and all the filtering is done at the consumer level.
  //   A fixed POA is used for servant activation.
  //   This object creates a single instance of the Supplier
  //
  // = MEMORY MANAGMENT
  //   A single event channel instance can be used with an instance of
  //   this class.
  //
public:
  TAO_EC_Default_Factory (void);
  // Constructor

  virtual ~TAO_EC_Default_Factory (void);
  // destructor...

  // = The Service_Object entry points
  virtual int init (int argc, char* argv[]);
  virtual int fini (void);

  TAO_EC_SupplierFiltering*
      supplier_filtering (TAO_EC_Event_Channel *ec);
  // Helper method, the supplier filtering could be shared between all 
  // the ProxyPushConsumer objects. In that case it is created and
  // owned by the factory.
  // Alternatively each ProxyPushConsumer could have its own, in that
  // case this method creates them on demand.

  // = The EC_Factory methods
  virtual TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel*);
  virtual void
      destroy_dispatching (TAO_EC_Dispatching*);
  virtual TAO_EC_Filter_Builder*
      create_filter_builder (TAO_EC_Event_Channel*);
  virtual void
      destroy_filter_builder (TAO_EC_Filter_Builder*);
  virtual TAO_EC_ConsumerAdmin*
      create_consumer_admin (TAO_EC_Event_Channel*);
  virtual void
      destroy_consumer_admin (TAO_EC_ConsumerAdmin*);
  virtual TAO_EC_SupplierAdmin*
      create_supplier_admin (TAO_EC_Event_Channel*);
  virtual void
      destroy_supplier_admin (TAO_EC_SupplierAdmin*);
  virtual TAO_EC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_EC_Event_Channel*);
  virtual void
      destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier*);
  virtual TAO_EC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_EC_Event_Channel*);
  virtual void
      destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer*);
  virtual TAO_EC_Timeout_Generator*
      create_timeout_generator (TAO_EC_Event_Channel*);
  virtual void
      destroy_timeout_generator (TAO_EC_Timeout_Generator*);
  virtual TAO_EC_ObserverStrategy*
      create_observer_strategy (TAO_EC_Event_Channel*);
  virtual void
      destroy_observer_strategy (TAO_EC_ObserverStrategy*);
  virtual TAO_EC_ProxyPushSupplier_Set*
      create_proxy_push_supplier_set (TAO_EC_Event_Channel*);
  virtual void
      destroy_proxy_push_supplier_set (TAO_EC_ProxyPushSupplier_Set*);

  virtual PortableServer::POA_ptr
       consumer_poa (CORBA::Environment& env);
  virtual PortableServer::POA_ptr
       supplier_poa (CORBA::Environment& env);
  virtual int
       consumer_poa (PortableServer::POA_ptr);
  virtual int
       supplier_poa (PortableServer::POA_ptr);

  virtual ACE_Lock* create_consumer_lock (void);
  virtual void destroy_consumer_lock (ACE_Lock*);
  virtual ACE_Lock* create_supplier_lock (void);
  virtual void destroy_supplier_lock (ACE_Lock*);

  virtual ACE_Lock* create_consumer_admin_lock (void);
  virtual void destroy_consumer_admin_lock (ACE_Lock*);
  virtual ACE_Lock* create_supplier_admin_lock (void);
  virtual void destroy_supplier_admin_lock (ACE_Lock*);

private:
  int dispatching_;
  int filtering_;
  int timeout_;
  int observer_;
  int supplier_set_;
  int consumer_lock_;
  int supplier_lock_;
  int consumer_admin_lock_;
  int supplier_admin_lock_;
  // Several flags to control the kind of object created.
  
  PortableServer::POA_var consumer_poa_;
  PortableServer::POA_var supplier_poa_;
  // The POAs

  TAO_EC_SupplierFiltering* supplier_filtering_;
  // The filtering strategy

  ACE_SYNCH_MUTEX lock_;
  // A mutex to control the internal state of the factory
};

#if defined (__ACE_INLINE__)
#include "EC_Default_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_EC_Default_Factory)
ACE_FACTORY_DECLARE (TAO_ORBSVCS, TAO_EC_Default_Factory)

#endif /* TAO_EC_DEFAULT_FACTORY_H */
