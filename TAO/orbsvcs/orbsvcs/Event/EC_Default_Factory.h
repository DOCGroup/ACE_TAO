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
  //   A generic factory for EC experimentation.
  //
  // = DESCRIPTION
  //   This class allows the user to experiment with different EC
  //   configurations.  Using a command-line like interface the user
  //   can specify which strategies will this factory generate.
  //   Since the class can be dynamically loaded the strategies can be
  //   set in the service configurator file.
  //
  // = MEMORY MANAGMENT
  //
public:
  TAO_EC_Default_Factory (void);
  // Constructor

  virtual ~TAO_EC_Default_Factory (void);
  // destructor...

  static int init_svcs (void);
  // Helper function to register the default factory into the service
  // configurator.

  // = The Service_Object entry points
  virtual int init (int argc, char* argv[]);
  virtual int fini (void);

  // = The EC_Factory methods
  virtual TAO_EC_Dispatching*
      create_dispatching (TAO_EC_Event_Channel*);
  virtual void
      destroy_dispatching (TAO_EC_Dispatching*);
  virtual TAO_EC_Filter_Builder*
      create_filter_builder (TAO_EC_Event_Channel*);
  virtual void
      destroy_filter_builder (TAO_EC_Filter_Builder*);
  virtual TAO_EC_Supplier_Filter_Builder*
      create_supplier_filter_builder (TAO_EC_Event_Channel*);
  virtual void
      destroy_supplier_filter_builder (TAO_EC_Supplier_Filter_Builder*);
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
  int supplier_filtering_;
  int timeout_;
  int observer_;
  int supplier_set_;
  int consumer_lock_;
  int supplier_lock_;
  int consumer_admin_lock_;
  int supplier_admin_lock_;
  // Several flags to control the kind of object created.
};

#if defined (__ACE_INLINE__)
#include "EC_Default_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_EC_Default_Factory)
ACE_FACTORY_DECLARE (TAO_ORBSVCS, TAO_EC_Default_Factory)

#endif /* TAO_EC_DEFAULT_FACTORY_H */
