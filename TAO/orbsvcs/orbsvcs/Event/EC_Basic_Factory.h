/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Basic_Factory
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

#ifndef TAO_EC_BASIC_FACTORY_H
#define TAO_EC_BASIC_FACTORY_H

#include "EC_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_SupplierFiltering;

class TAO_ORBSVCS_Export TAO_EC_Basic_Factory : public TAO_EC_Factory
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
  //
public:
  TAO_EC_Basic_Factory (void);
  // Constructor

  virtual ~TAO_EC_Basic_Factory (void);
  // destructor...

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
};

#if defined (__ACE_INLINE__)
#include "EC_Basic_Factory.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_BASIC_FACTORY_H */
