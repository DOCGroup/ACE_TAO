/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_ProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_PROXYPUSHSUPPLIER_H
#define TAO_CEC_PROXYPUSHSUPPLIER_H
#include "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushConsumer;

/**
 * @class TAO_CEC_ProxyPushSupplier
 *
 * @brief ProxyPushSupplier
 *
 * Implement the CosEventChannelAdmin::ProxyPushSupplier interface,
 * remember that this class is used to communicate with a
 * PushConsumer, so, in effect, this is the ambassador for a
 * consumer inside the event channel.
 * = MEMORY MANAGMENT
 * It does not assume ownership of the TAO_CEC_Dispatching object.
 * It makes a copy of the ConsumerQOS and the consumer object
 * reference.
 * = LOCKING
 * Locking is strategized, the event channel acts as a factory for
 * the locking strategies.
 */
class TAO_Event_Export TAO_CEC_ProxyPushSupplier : public POA_CosEventChannelAdmin::ProxyPushSupplier
{
public:
  typedef CosEventChannelAdmin::ProxyPushSupplier_ptr _ptr_type;
  typedef CosEventChannelAdmin::ProxyPushSupplier_var _var_type;

  /// constructor...
  TAO_CEC_ProxyPushSupplier (TAO_CEC_EventChannel* event_channel);

  /// destructor...
  virtual ~TAO_CEC_ProxyPushSupplier (void);

  /// Activate in the POA
  virtual CosEventChannelAdmin::ProxyPushSupplier_ptr activate (TAO_ENV_SINGLE_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException));

  /// Deactivate from the POA
  virtual void deactivate (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return 0 if no consumer is connected...
  CORBA::Boolean is_connected (void) const;

  /**
   * Return the consumer object reference. It returns nil() if it has
   * not connected yet.
   * NOTE: This method does not return a new reference!!! Doing so
   * will increase the locking overhead on the critical path.
   */
  CosEventComm::PushConsumer_ptr consumer (void) const;

  /// The event channel is shutting down
  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL);

  /// Internal methods to push an event to each consumer.
  virtual void push (const CORBA::Any &event
                     TAO_ENV_ARG_DECL);
  virtual void push_nocopy (CORBA::Any &event
                            TAO_ENV_ARG_DECL);

  /// Pushes to the consumer, verifies that it is connected.
  void push_to_consumer (const CORBA::Any &event
                         TAO_ENV_ARG_DECL);
  void reactive_push_to_consumer (const CORBA::Any &event
                                  TAO_ENV_ARG_DECL);

  /**
   * Invoke the _non_existent() pseudo-operation on the consumer. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean consumer_non_existent (CORBA::Boolean_out disconnected
                                        TAO_ENV_ARG_DECL);

  // = The CosEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                CosEventComm::PushConsumer_ptr push_consumer
                TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected,
                       CosEventChannelAdmin::TypeError));
  virtual void disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL);
  virtual void _add_ref (TAO_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (TAO_ENV_SINGLE_ARG_DECL);

protected:
  /// Set the consumer, used by some implementations to change the
  /// policies used when invoking operations on the consumer.
  void consumer (CosEventComm::PushConsumer_ptr consumer);
  void consumer_i (CosEventComm::PushConsumer_ptr consumer);

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the child and the consumer
  void cleanup_i (void);

private:
  /// The Event Channel that owns this object.
  TAO_CEC_EventChannel* event_channel_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The consumer....
  CosEventComm::PushConsumer_var consumer_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

#if defined (__ACE_INLINE__)
#include "CEC_ProxyPushSupplier.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_PROXYPUSHSUPPLIER_H */
