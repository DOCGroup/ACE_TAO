/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_ProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @authors Carlos O'Ryan (coryan@cs.wustl.edu)
 *           Jon Astle (jon@astle45.fsnet.co.uk)
 */
//=============================================================================


#ifndef TAO_CEC_PROXYPUSHSUPPLIER_H
#define TAO_CEC_PROXYPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
#include "orbsvcs/CosTypedEventChannelAdminS.h"
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushConsumer;
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
class TAO_CEC_TypedEvent;
class TAO_CEC_TypedEventChannel;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

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

  /// typed ec constructor
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  TAO_CEC_ProxyPushSupplier (TAO_CEC_TypedEventChannel* typed_event_channel);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// destructor...
  virtual ~TAO_CEC_ProxyPushSupplier (void);

  /// Activate in the POA
  virtual void activate (
      CosEventChannelAdmin::ProxyPushSupplier_ptr &
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Deactivate from the POA
  virtual void deactivate (ACE_ENV_SINGLE_ARG_DECL)
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
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Internal methods to push an event to each consumer.
  virtual void push (const CORBA::Any &event
                     ACE_ENV_ARG_DECL);
  virtual void push_nocopy (CORBA::Any &event
                            ACE_ENV_ARG_DECL);
  /// Internal methods to invoke a typed event to each consumer.
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  virtual void invoke (const TAO_CEC_TypedEvent& typed_event
                       ACE_ENV_ARG_DECL);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// Pushes to the consumer, verifies that it is connected.
  void push_to_consumer (const CORBA::Any &event
                         ACE_ENV_ARG_DECL);
  void reactive_push_to_consumer (const CORBA::Any &event
                                  ACE_ENV_ARG_DECL);
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  void invoke_to_consumer (const TAO_CEC_TypedEvent &typed_event
                           ACE_ENV_ARG_DECL);
  void reactive_invoke_to_consumer (const TAO_CEC_TypedEvent &typed_event
                                    ACE_ENV_ARG_DECL);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /**
   * Invoke the _non_existent() pseudo-operation on the consumer. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean consumer_non_existent (CORBA::Boolean_out disconnected
                                        ACE_ENV_ARG_DECL);

  // = The CosEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                CosEventComm::PushConsumer_ptr push_consumer
                ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected,
                       CosEventChannelAdmin::TypeError));
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// Set the consumer, used by some implementations to change the
  /// policies used when invoking operations on the consumer.
  void consumer (CosEventComm::PushConsumer_ptr consumer);
  void consumer_i (CosEventComm::PushConsumer_ptr consumer);
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  void consumer (CosTypedEventComm::TypedPushConsumer_ptr typed_consumer);
  void consumer_i (CosTypedEventComm::TypedPushConsumer_ptr typed_consumer);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the child and the consumer
  void cleanup_i (void);

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  CORBA::Boolean is_typed_ec (void) const;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

private:
  /// The Event Channel that owns this object.
  TAO_CEC_EventChannel* event_channel_;

  /// The Typed Event Channel that owns this object.
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  TAO_CEC_TypedEventChannel *typed_event_channel_;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The consumer....
  CosEventComm::PushConsumer_var consumer_;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  /// The typed consumer....
  CosTypedEventComm::TypedPushConsumer_var typed_consumer_;

  /// The consumer object returned from get_typed_consumer()
  CORBA::Object_var typed_consumer_obj_;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

#if defined (__ACE_INLINE__)
#include "CEC_ProxyPushSupplier.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CEC_PROXYPUSHSUPPLIER_H */
