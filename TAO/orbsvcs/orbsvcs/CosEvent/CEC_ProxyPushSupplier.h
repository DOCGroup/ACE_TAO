/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_ProxySupplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_CEC_PROXYPUSHSUPPLIER_H
#define TAO_CEC_PROXYPUSHSUPPLIER_H

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushConsumer;

class TAO_ORBSVCS_Export TAO_CEC_ProxyPushSupplier : public POA_CosEventChannelAdmin::ProxyPushSupplier
{
  // = TITLE
  //   ProxyPushSupplier
  //
  // = DESCRIPTION
  //   Implement the CosEventChannelAdmin::ProxyPushSupplier interface,
  //   remember that this class is used to communicate with a
  //   PushConsumer, so, in effect, this is the ambassador for a
  //   consumer inside the event channel.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_CEC_Dispatching object.
  //   It makes a copy of the ConsumerQOS and the consumer object
  //   reference.
  //
  // = LOCKING
  //   Locking is strategized, the event channel acts as a factory for
  //   the locking strategies.
  //
public:
  TAO_CEC_ProxyPushSupplier (TAO_CEC_EventChannel* event_channel);
  // constructor...

  virtual ~TAO_CEC_ProxyPushSupplier (void);
  // destructor...

  CORBA::Boolean is_connected (void) const;
  // Return 0 if no consumer is connected...

  CosEventComm::PushConsumer_ptr consumer (void) const;
  // Return the consumer object reference. It returns nil() if it has
  // not connected yet.
  // NOTE: This method does not return a new reference!!! Doing so
  // will increase the locking overhead on the critical path.

  virtual void connected (TAO_CEC_ProxyPushConsumer *consumer,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_CEC_ProxyPushConsumer *consumer,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_CEC_ProxyPushConsumer *consumer,
                            CORBA::Environment &env);
  // Concrete implementations can use this methods to keep track of
  // the suppliers that publish its events.

  virtual void connected (TAO_CEC_ProxyPushSupplier *supplier,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_CEC_ProxyPushSupplier *supplier,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_CEC_ProxyPushSupplier *supplier,
                             CORBA::Environment &env);
  // Usually implemented as no-ops, but some configurations may
  // require this methods.

  virtual void shutdown (CORBA::Environment &env);
  // The event channel is shutting down

  virtual void push (const CORBA::Any &event,
                     CORBA::Environment &ACE_TRY_ENV);
  virtual void push_nocopy (CORBA::Any &event,
                            CORBA::Environment &ACE_TRY_ENV);
  // Internal methods to push an event to each consumer.

  void push_to_consumer (const CORBA::Any &event,
                         CORBA::Environment &env);
  void reactive_push_to_consumer (const CORBA::Any &event,
                                  CORBA::Environment &env);
  // Pushes to the consumer, verifies that it is connected.

  CORBA::Boolean consumer_non_existent (CORBA::Boolean_out disconnected,
                                        CORBA::Environment &ACE_TRY_ENV);
  // Invoke the _non_existent() pseudo-operation on the consumer. If
  // it is disconnected then it returns true and sets the
  // <disconnected> flag.

  // = The CosEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                CosEventComm::PushConsumer_ptr push_consumer,
                CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected,
                       CosEventChannelAdmin::TypeError));
  virtual void disconnect_push_supplier (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count.

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
  virtual void _add_ref (CORBA_Environment &ACE_TRY_ENV);
  virtual void _remove_ref (CORBA_Environment &ACE_TRY_ENV);

protected:
  void consumer (CosEventComm::PushConsumer_ptr consumer);
  void consumer_i (CosEventComm::PushConsumer_ptr consumer);
  // Set the consumer, used by some implementations to change the
  // policies used when invoking operations on the consumer.

  CORBA::Boolean is_connected_i (void) const;
  // The private version (without locking) of is_connected().

  void cleanup_i (void);
  // Release the child and the consumer

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate from the POA

private:
  TAO_CEC_EventChannel* event_channel_;
  // The Event Channel that owns this object.

  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::ULong refcount_;
  // The reference count.

  CosEventComm::PushConsumer_var consumer_;
  // The consumer....

  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

#if defined (__ACE_INLINE__)
#include "CEC_ProxyPushSupplier.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_PROXYPUSHSUPPLIER_H */
