// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_ProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Jon Astle (jon@astle45.fsnet.co.uk)
 */
//=============================================================================


#ifndef TAO_CEC_PROXYPUSHSUPPLIER_H
#define TAO_CEC_PROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
#include "orbsvcs/CosTypedEventChannelAdminS.h"
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

#include "orbsvcs/CosEvent/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
class TAO_Event_Serv_Export TAO_CEC_ProxyPushSupplier : public POA_CosEventChannelAdmin::ProxyPushSupplier
{
public:
  typedef CosEventChannelAdmin::ProxyPushSupplier_ptr _ptr_type;
  typedef CosEventChannelAdmin::ProxyPushSupplier_var _var_type;

  /// constructor...
  TAO_CEC_ProxyPushSupplier (TAO_CEC_EventChannel* event_channel,
                             const ACE_Time_Value &timeout);

  /// typed ec constructor
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  TAO_CEC_ProxyPushSupplier (TAO_CEC_TypedEventChannel* typed_event_channel,
                             const ACE_Time_Value &timeout);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// destructor...
  virtual ~TAO_CEC_ProxyPushSupplier (void);

  /// Activate in the POA
  virtual void activate (
      CosEventChannelAdmin::ProxyPushSupplier_ptr &);

  /// Deactivate from the POA
  virtual void deactivate (void);

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
  virtual void shutdown (void);

  /// Internal methods to push an event to each consumer.
  virtual void push (const CORBA::Any &event);
  virtual void push_nocopy (CORBA::Any &event);
  /// Internal methods to invoke a typed event to each consumer.
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  virtual void invoke (const TAO_CEC_TypedEvent& typed_event);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// Pushes to the consumer, verifies that it is connected.
  void push_to_consumer (const CORBA::Any &event);
  void reactive_push_to_consumer (const CORBA::Any &event);
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  void invoke_to_consumer (const TAO_CEC_TypedEvent &typed_event);
  void reactive_invoke_to_consumer (const TAO_CEC_TypedEvent &typed_event);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /**
   * Invoke the _non_existent() pseudo-operation on the consumer. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean consumer_non_existent (CORBA::Boolean_out disconnected);

  // = The CosEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                CosEventComm::PushConsumer_ptr push_consumer);
  virtual void disconnect_push_supplier (void);

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (void);
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

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

  /// Assigns the parameter to both consumer_ and nopolicy_consumer_, and
  /// applies policies (when appropriate) to consumer_.
  CosEventComm::PushConsumer_ptr apply_policy
  (CosEventComm::PushConsumer_ptr c);

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  CosTypedEventComm::TypedPushConsumer_ptr apply_policy
  (CosTypedEventComm::TypedPushConsumer_ptr c);
#endif

  CORBA::Object_ptr apply_policy_obj (CORBA::Object_ptr c);

private:
  /// The Event Channel that owns this object.
  TAO_CEC_EventChannel* event_channel_;

  ACE_Time_Value timeout_;

  /// The Typed Event Channel that owns this object.
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  TAO_CEC_TypedEventChannel *typed_event_channel_;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The consumer -- use apply_policy() instead of assigning directly to
  /// consumer_.  This will keep consumer_ and nopolicy_consumer_ in sync.
  CosEventComm::PushConsumer_var consumer_;

  /// The consumer without any policies applied
  CosEventComm::PushConsumer_var nopolicy_consumer_;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  /// The typed consumer -- use apply_policy() instead of assigning directly to
  /// typed_consumer_.  This will keep typed_consumer_ and
  /// nopolicy_typed_consumer_ in sync.
  CosTypedEventComm::TypedPushConsumer_var typed_consumer_;

  /// The consumer object returned from get_typed_consumer()
  CORBA::Object_var typed_consumer_obj_;

  /// The typed consumer without any policies applied
  CosTypedEventComm::TypedPushConsumer_var nopolicy_typed_consumer_;

#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /// Store the default POA.
  PortableServer::POA_var default_POA_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CEC_PROXYPUSHSUPPLIER_H */
