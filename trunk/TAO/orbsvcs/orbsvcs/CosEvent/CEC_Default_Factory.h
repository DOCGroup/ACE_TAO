// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_Default_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_DEFAULT_FACTORY_H
#define TAO_CEC_DEFAULT_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/CEC_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEvent/CEC_Defaults.h"

#include "ace/Service_Config.h"
#include "ace/Time_Value.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CEC_Default_Factory
 *
 * @brief A generic factory for EC experimentation.
 *
 * This class allows the user to experiment with different EC
 * configurations.  Using a command-line like interface the user
 * can specify which strategies will this factory generate.
 * Since the class can be dynamically loaded the strategies can be
 * set in the service configurator file.
 * = MEMORY MANAGMENT
 */
class TAO_Event_Serv_Export TAO_CEC_Default_Factory : public TAO_CEC_Factory
{
public:
  /// Constructor
  TAO_CEC_Default_Factory (void);

  /// destructor...
  virtual ~TAO_CEC_Default_Factory (void);

  /// Helper function to register the default factory into the service
  /// configurator.
  static int init_svcs (void);

  // = The Service_Object entry points
  virtual int init (int argc, ACE_TCHAR* argv[]);
  virtual int fini (void);

  // = The CEC_Factory methods
  virtual TAO_CEC_Dispatching*
      create_dispatching (TAO_CEC_EventChannel*);
  virtual void
      destroy_dispatching (TAO_CEC_Dispatching*);
  virtual TAO_CEC_Pulling_Strategy*
      create_pulling_strategy (TAO_CEC_EventChannel*);
  virtual void
      destroy_pulling_strategy (TAO_CEC_Pulling_Strategy*);
  virtual TAO_CEC_ConsumerAdmin*
      create_consumer_admin (TAO_CEC_EventChannel*);
  virtual void
      destroy_consumer_admin (TAO_CEC_ConsumerAdmin*);
  virtual TAO_CEC_SupplierAdmin*
      create_supplier_admin (TAO_CEC_EventChannel*);
  virtual void
      destroy_supplier_admin (TAO_CEC_SupplierAdmin*);
  virtual TAO_CEC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_push_supplier (TAO_CEC_ProxyPushSupplier*);
  virtual TAO_CEC_ProxyPullSupplier*
      create_proxy_pull_supplier (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_pull_supplier (TAO_CEC_ProxyPullSupplier*);
  virtual TAO_CEC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_push_consumer (TAO_CEC_ProxyPushConsumer*);
  virtual TAO_CEC_ProxyPullConsumer*
      create_proxy_pull_consumer (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_pull_consumer (TAO_CEC_ProxyPullConsumer*);
  virtual TAO_CEC_ProxyPushConsumer_Collection*
      create_proxy_push_consumer_collection (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_push_consumer_collection (
          TAO_CEC_ProxyPushConsumer_Collection*
        );
  virtual TAO_CEC_ProxyPullConsumer_Collection*
      create_proxy_pull_consumer_collection (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_pull_consumer_collection (
          TAO_CEC_ProxyPullConsumer_Collection*
        );
  virtual TAO_CEC_ProxyPushSupplier_Collection*
    create_proxy_push_supplier_collection (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_push_supplier_collection (
          TAO_CEC_ProxyPushSupplier_Collection*
        );
  virtual TAO_CEC_ProxyPullSupplier_Collection*
    create_proxy_pull_supplier_collection (TAO_CEC_EventChannel*);
  virtual void
      destroy_proxy_pull_supplier_collection (
          TAO_CEC_ProxyPullSupplier_Collection*
        );

  virtual ACE_Lock* create_consumer_lock (void);
  virtual void destroy_consumer_lock (ACE_Lock*);
  virtual ACE_Lock* create_supplier_lock (void);
  virtual void destroy_supplier_lock (ACE_Lock*);

  virtual TAO_CEC_ConsumerControl*
      create_consumer_control (TAO_CEC_EventChannel*);
  virtual void
      destroy_consumer_control (TAO_CEC_ConsumerControl*);
  virtual TAO_CEC_SupplierControl*
      create_supplier_control (TAO_CEC_EventChannel*);
  virtual void
      destroy_supplier_control (TAO_CEC_SupplierControl*);

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  virtual TAO_CEC_Dispatching*
      create_dispatching (TAO_CEC_TypedEventChannel*);
  virtual TAO_CEC_TypedConsumerAdmin*
      create_consumer_admin (TAO_CEC_TypedEventChannel*);
  virtual void
      destroy_consumer_admin (TAO_CEC_TypedConsumerAdmin*);
  virtual TAO_CEC_TypedSupplierAdmin*
      create_supplier_admin (TAO_CEC_TypedEventChannel*);
  virtual void
      destroy_supplier_admin (TAO_CEC_TypedSupplierAdmin*);
  virtual TAO_CEC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_CEC_TypedEventChannel*);
  virtual TAO_CEC_ProxyPushSupplier_Collection*
      create_proxy_push_supplier_collection (TAO_CEC_TypedEventChannel*);
  virtual TAO_CEC_TypedProxyPushConsumer*
      create_proxy_push_consumer (TAO_CEC_TypedEventChannel*);
  virtual void
      destroy_proxy_push_consumer (TAO_CEC_TypedProxyPushConsumer*);
  virtual TAO_CEC_TypedProxyPushConsumer_Collection*
      create_proxy_push_consumer_collection (TAO_CEC_TypedEventChannel*);
  virtual void
      destroy_proxy_push_consumer_collection (
          TAO_CEC_TypedProxyPushConsumer_Collection*
        );
  virtual TAO_CEC_ConsumerControl*
      create_consumer_control (TAO_CEC_TypedEventChannel*);
  virtual TAO_CEC_SupplierControl*
      create_supplier_control (TAO_CEC_TypedEventChannel*);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  virtual CORBA::Policy_ptr
  create_roundtrip_timeout_policy (const ACE_Time_Value &timeout);

private:
  /// Parse an argument to set the type of collections used.
  int parse_collection_arg (ACE_TCHAR* opt);

private:
  /// Several flags to control the kind of object created.
  int dispatching_;
  int pulling_strategy_;
  int consumer_collection_;
  int supplier_collection_;
  int consumer_lock_;
  int supplier_lock_;

  /// The MT dispatching priority has several arguments that could be
  /// controlled here...
  int dispatching_threads_;
  int dispatching_threads_flags_;
  int dispatching_threads_priority_;
  int dispatching_threads_force_active_;

  /// How often (in microseconds) are the pull suppliers polled by the
  /// reactive pulling strategy.
  int reactive_pulling_period_;

  /// Use this ORB to locate global resources.
  char *orbid_;

  /// Indicate ORB ID has been duplicated (memory allocated).
  int orbid_dupped_;

  /// The consumer and supplier control policies.
  int consumer_control_;
  int supplier_control_;

  /// The consumer and supplier control periods in usecs
  int consumer_control_period_;
  int supplier_control_period_;

  /// The consumer and supplier control timeout in usecs
  ACE_Time_Value consumer_control_timeout_;
  ACE_Time_Value supplier_control_timeout_;

  /// The consumer and supplier operation timeouts.
  /// Only in effect if the corresponding "reactive control" is enabled.
  /// Applies the given timeout as the round-trip time policy on the
  /// object reference.
  ACE_Time_Value consumer_timeout_;
  ACE_Time_Value supplier_timeout_;

  /// The number of retries before disconnecting a proxy
  unsigned int proxy_disconnect_retries_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Default_Factory.inl"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_CEC_Default_Factory)
ACE_FACTORY_DECLARE (TAO_Event_Serv, TAO_CEC_Default_Factory)

#include /**/ "ace/post.h"

#endif /* TAO_CEC_DEFAULT_FACTORY_H */
