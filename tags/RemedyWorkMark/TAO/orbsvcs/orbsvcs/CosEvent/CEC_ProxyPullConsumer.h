// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_ProxyPullConsumer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef TAO_CEC_PROXYPULLCONSUMER_H
#define TAO_CEC_PROXYPULLCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/CosEvent/event_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;
class TAO_CEC_Dispatching;
class TAO_CEC_ProxyPullSupplier;

/**
 * @class TAO_CEC_ProxyPullConsumer
 *
 * @brief ProxyPullConsumer
 *
 * Implement the CosEventChannelAdmin::ProxyPullConsumer interface,
 * remember that this class is used to communicate with a
 * PullSupplier, so, in effect, this is the ambassador for a
 * supplier inside the event channel.
 * = MEMORY MANAGMENT
 * The object commits suicide when disconnect_pull_consumer() is
 * called.
 */
class TAO_Event_Serv_Export TAO_CEC_ProxyPullConsumer : public POA_CosEventChannelAdmin::ProxyPullConsumer
{
public:
  typedef CosEventChannelAdmin::ProxyPullConsumer_ptr _ptr_type;
  typedef CosEventChannelAdmin::ProxyPullConsumer_var _var_type;

  /// constructor...
  TAO_CEC_ProxyPullConsumer (TAO_CEC_EventChannel* event_channel,
                             const ACE_Time_Value &timeout);

  /// destructor...
  virtual ~TAO_CEC_ProxyPullConsumer (void);

  /// Activate in the POA
  virtual void  activate (
      CosEventChannelAdmin::ProxyPullConsumer_ptr &activated_proxy);

  /// Deactivate from the POA
  virtual void deactivate (void);

  /// Return 0 if no supplier is connected...
  CORBA::Boolean is_connected (void) const;

  /// Return the consumer object reference. It returns nil() if it has
  /// not connected yet.
  CosEventComm::PullSupplier_ptr supplier (void) const;

  /// Pulls from the supplier, verifies that it is connected.
  CORBA::Any* try_pull_from_supplier (CORBA::Boolean_out has_event);
  CORBA::Any* pull_from_supplier (void);

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

  // = The CosEventChannelAdmin::ProxyPullConsumer methods...
  virtual void connect_pull_supplier (
                CosEventComm::PullSupplier_ptr pull_supplier);
  virtual void disconnect_pull_consumer (void);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (void);
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

protected:
  /// Set the supplier, used by some implementations to change the
  /// policies used when invoking operations on the supplier.
  void supplier (CosEventComm::PullSupplier_ptr supplier);
  void supplier_i (CosEventComm::PullSupplier_ptr supplier);

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the supplier
  void cleanup_i (void);

  /// Assigns the parameter to both supplier_ and nopolicy_supplier_, and
  /// applies policies (when appropriate) to supplier_.
  CosEventComm::PullSupplier_ptr apply_policy
  (CosEventComm::PullSupplier_ptr s);

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
  CosEventComm::PullSupplier_var supplier_;

  /// The supplier without any policies applied
  CosEventComm::PullSupplier_var nopolicy_supplier_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPullConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_PROXYPULLCONSUMER_H */
