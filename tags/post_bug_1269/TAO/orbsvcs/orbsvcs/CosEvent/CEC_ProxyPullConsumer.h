/* -*- C++ -*- */
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
#include "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "event_export.h"

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
class TAO_Event_Export TAO_CEC_ProxyPullConsumer : public POA_CosEventChannelAdmin::ProxyPullConsumer
{
public:
  typedef CosEventChannelAdmin::ProxyPullConsumer_ptr _ptr_type;
  typedef CosEventChannelAdmin::ProxyPullConsumer_var _var_type;

  /// constructor...
  TAO_CEC_ProxyPullConsumer (TAO_CEC_EventChannel* event_channel);

  /// destructor...
  virtual ~TAO_CEC_ProxyPullConsumer (void);

  /// Activate in the POA
  virtual CosEventChannelAdmin::ProxyPullConsumer_ptr activate (ACE_ENV_SINGLE_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException));

  /// Deactivate from the POA
  virtual void deactivate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return 0 if no supplier is connected...
  CORBA::Boolean is_connected (void) const;

  /// Return the consumer object reference. It returns nil() if it has
  /// not connected yet.
  CosEventComm::PullSupplier_ptr supplier (void) const;

  /// Pulls from the supplier, verifies that it is connected.
  CORBA::Any* try_pull_from_supplier (CORBA::Boolean_out has_event
                                      ACE_ENV_ARG_DECL);
  CORBA::Any* pull_from_supplier (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Invoke the _non_existent() pseudo-operation on the supplier. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean supplier_non_existent (CORBA::Boolean_out disconnected
                                        ACE_ENV_ARG_DECL);

  /// The event channel is shutting down
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = The CosEventChannelAdmin::ProxyPullConsumer methods...
  virtual void connect_pull_supplier (
                CosEventComm::PullSupplier_ptr pull_supplier
                ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected));
  virtual void disconnect_pull_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:
  /// Set the supplier, used by some implementations to change the
  /// policies used when invoking operations on the supplier.
  void supplier (CosEventComm::PullSupplier_ptr supplier);
  void supplier_i (CosEventComm::PullSupplier_ptr supplier);

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the supplier
  void cleanup_i (void);

private:
  /// The supplier admin, used for activation and memory managment.
  TAO_CEC_EventChannel* event_channel_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The supplier....
  CosEventComm::PullSupplier_var supplier_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

#if defined (__ACE_INLINE__)
#include "CEC_ProxyPullConsumer.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_PROXYPULLCONSUMER_H */
