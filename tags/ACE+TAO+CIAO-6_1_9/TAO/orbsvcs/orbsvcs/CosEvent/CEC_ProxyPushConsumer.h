// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_ProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_PROXYPUSHCONSUMER_H
#define TAO_CEC_PROXYPUSHCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosEvent/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;
class TAO_CEC_Dispatching;
class TAO_CEC_ProxyPushSupplier;

/**
 * @class TAO_CEC_ProxyPushConsumer
 *
 * @brief ProxyPushConsumer
 *
 * Implement the CosEventChannelAdmin::ProxyPushConsumer interface,
 * remember that this class is used to communicate with a
 * PushSupplier, so, in effect, this is the ambassador for a
 * supplier inside the event channel.
 * = MEMORY MANAGMENT
 * The object commits suicide when disconnect_push_consumer() is
 * called.
 */
class TAO_Event_Serv_Export TAO_CEC_ProxyPushConsumer
  : public POA_CosEventChannelAdmin::ProxyPushConsumer
{
public:
  typedef CosEventChannelAdmin::ProxyPushConsumer_ptr _ptr_type;
  typedef CosEventChannelAdmin::ProxyPushConsumer_var _var_type;

  /// constructor...
  TAO_CEC_ProxyPushConsumer (TAO_CEC_EventChannel* event_channel,
                             const ACE_Time_Value &timeout);

  /// destructor...
  virtual ~TAO_CEC_ProxyPushConsumer (void);

  /// Activate in the POA
  virtual void activate (
      CosEventChannelAdmin::ProxyPushConsumer_ptr &activated_proxy);

  /// Deactivate from the POA
  virtual void deactivate (void);

  /// Return false if no supplier is connected...
  CORBA::Boolean is_connected (void) const;

  /// Return the consumer object reference. It returns nil() if it has
  /// not connected yet.
  CosEventComm::PushSupplier_ptr supplier (void) const;

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

  // = The CosEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                CosEventComm::PushSupplier_ptr push_supplier);
  virtual void push (const CORBA::Any& event);
  virtual void disconnect_push_consumer (void);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (void);
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

protected:
  /// Set the supplier, used by some implementations to change the
  /// policies used when invoking operations on the supplier.
  void supplier (CosEventComm::PushSupplier_ptr supplier);
  void supplier_i (CosEventComm::PushSupplier_ptr supplier);

  friend class TAO_CEC_ProxyPushConsumer_Guard;
  // The guard needs access to the following protected methods.

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the supplier
  void cleanup_i (void);

  /// Assigns the parameter to both supplier_ and nopolicy_supplier_, and
  /// applies policies (when appropriate) to supplier_.
  CosEventComm::PushSupplier_ptr apply_policy
  (CosEventComm::PushSupplier_ptr s);

private:
  /// The supplier admin, used for activation and memory managment.
  TAO_CEC_EventChannel* event_channel_;

  ACE_Time_Value timeout_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The supplier -- use apply_policy() instead of assigning directly to
  /// supplier_.  This will keep supplier_ and nopolicy_supplier_ in sync.
  CosEventComm::PushSupplier_var supplier_;

  /// The supplier without any policies applied
  CosEventComm::PushSupplier_var nopolicy_supplier_;

  /// If the flag is true then we are connected, notice that the
  /// supplier can be nil.
  bool connected_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

// ****************************************************************

/**
 * @class TAO_CEC_ProxyPushConsumer_Guard
 *
 * @brief A Guard for the ProxyPushConsumer reference count
 *
 * This is a helper class used in the implementation of
 * ProxyPushConumer.  It provides a Guard mechanism to increment
 * the reference count on the proxy, eliminating the need to hold
 * mutexes during long operations.
 */
class TAO_Event_Serv_Export TAO_CEC_ProxyPushConsumer_Guard
{
public:
  /// Constructor
  TAO_CEC_ProxyPushConsumer_Guard (ACE_Lock *lock,
                                   CORBA::ULong &refcount,
                                   TAO_CEC_EventChannel *ec,
                                   TAO_CEC_ProxyPushConsumer *proxy);

  /// Destructor
  ~TAO_CEC_ProxyPushConsumer_Guard (void);

  /// Returns true if the reference count successfully acquired
  bool locked (void) const;

private:
  /// The lock used to protect the reference count
  ACE_Lock *lock_;

  /// The reference count
  CORBA::ULong &refcount_;

  /// The event channel used to destroy the proxy
  TAO_CEC_EventChannel *event_channel_;

  /// The proxy whose lifetime is controlled by the reference count
  TAO_CEC_ProxyPushConsumer *proxy_;

  /// This flag is set to true if the reference count was successfully
  /// acquired.
  bool locked_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_PROXYPUSHCONSUMER_H */
