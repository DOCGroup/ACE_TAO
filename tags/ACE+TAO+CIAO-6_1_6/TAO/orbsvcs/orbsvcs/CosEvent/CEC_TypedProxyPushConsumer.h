// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_TypedProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Jon Astle (jon@astle45.fsnet.co.uk)
 *
 *  Based on the untyped version by Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_TYPEDPROXYPUSHCONSUMER_H_
#define TAO_CEC_TYPEDPROXYPUSHCONSUMER_H_
#include /**/ "ace/pre.h"

#include "orbsvcs/CosTypedEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/CosEvent/event_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_TypedEvent;
class TAO_CEC_TypedEventChannel;
class TAO_CEC_DynamicImplementationServer;

class TAO_Event_Serv_Export TAO_CEC_TypedProxyPushConsumer
  : public virtual POA_CosTypedEventChannelAdmin::TypedProxyPushConsumer
{
public:
  typedef CosTypedEventChannelAdmin::TypedProxyPushConsumer_ptr _ptr_type;
  typedef CosTypedEventChannelAdmin::TypedProxyPushConsumer_var _var_type;

  /// Constructor
  TAO_CEC_TypedProxyPushConsumer (
      TAO_CEC_TypedEventChannel* typed_event_channel,
      const ACE_Time_Value &timeout);

  /// Destructor
  virtual ~TAO_CEC_TypedProxyPushConsumer (void);

  /// Activate in the POA
  virtual void activate (
      CosTypedEventChannelAdmin::TypedProxyPushConsumer_ptr &activated_proxy);

  /// Deactivate from the POA
  virtual void deactivate (void);

  /**
   * Invoke the _non_existent() pseudo-operation on the supplier. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean supplier_non_existent (CORBA::Boolean_out disconnected);

  /// The event channel is shutting down
  virtual void shutdown (void);

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = The CosEventChannelAdmin::ProxyPushConsumer methods (abstract overloads)...
  virtual void connect_push_supplier (
                CosEventComm::PushSupplier_ptr push_supplier);

  virtual void push (const CORBA::Any& event);

  virtual void invoke (const TAO_CEC_TypedEvent& typed_event);

  virtual void disconnect_push_consumer (void);

  // = The CosTypedEventComm::TypedPushConsumer methods (abstract overloads)...
  virtual CORBA::Object_ptr get_typed_consumer (void);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (void);
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

protected:
  /// The guard needs access to the following protected methods.
  friend class TAO_CEC_TypedProxyPushConsumer_Guard;

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the supplier
  void cleanup_i (void);

  /// Assigns the parameter to both supplier_ and nopolicy_supplier_, and
  /// applies policies (when appropriate) to supplier_.
  CosEventComm::PushSupplier_ptr apply_policy
  (CosEventComm::PushSupplier_ptr pre);

private:
  /// The typed supplier admin, used for activation and memory managment.
  TAO_CEC_TypedEventChannel* typed_event_channel_;

  ACE_Time_Value timeout_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The typed supplier -- use apply_policy() instead of assigning directly to
  /// typed_supplier_.  This will keep typed_supplier_ and
  /// nopolicy_typed_supplier_ in sync.
  CosEventComm::PushSupplier_var typed_supplier_;

  /// The typed supplier without any policies applied
  CosEventComm::PushSupplier_var nopolicy_typed_supplier_;

  /// The DSI impl
  TAO_CEC_DynamicImplementationServer* dsi_impl_;

  /// The DSI Portable Server OID
  PortableServer::ObjectId_var oid_;

  /// If the flag is not zero then we are connected, notice that the
  /// supplier can be nil.
  int connected_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

// ****************************************************************

/**
 * @class TAO_CEC_TypedProxyPushConsumer_Guard
 *
 * @brief A Guard for the TypedProxyPushConsumer reference count
 *
 * This is a helper class used in the implementation of
 * TypedProxyPushConsumer.  It provides a Guard mechanism to increment
 * the reference count on the proxy, eliminating the need to hold
 * mutexes during long operations.
 */
class TAO_Event_Serv_Export TAO_CEC_TypedProxyPushConsumer_Guard
{
public:
  /// Constructor
  TAO_CEC_TypedProxyPushConsumer_Guard (
      ACE_Lock *lock,
      CORBA::ULong &refcount,
      TAO_CEC_TypedEventChannel *ec,
      TAO_CEC_TypedProxyPushConsumer *proxy
    );

  /// Destructor
  ~TAO_CEC_TypedProxyPushConsumer_Guard (void);

  /// Returns 1 if the reference count successfully acquired
  int locked (void) const;

private:
  /// The lock used to protect the reference count
  ACE_Lock *lock_;

  /// The reference count
  CORBA::ULong &refcount_;

  /// The event channel used to destroy the proxy
  TAO_CEC_TypedEventChannel *typed_event_channel_;

  /// The proxy whose lifetime is controlled by the reference count
  TAO_CEC_TypedProxyPushConsumer *proxy_;

  /// This flag is set to 1 if the reference count was successfully
  /// acquired.
  int locked_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_TypedProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_TYPEDPROXYPUSHCONSUMER_H_ */
