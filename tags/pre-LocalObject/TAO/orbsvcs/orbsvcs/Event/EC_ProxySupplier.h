/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ProxySupplier
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

#ifndef TAO_EC_PROXYSUPPLIER_H
#define TAO_EC_PROXYSUPPLIER_H
#include "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushConsumer;

class TAO_RTEvent_Export TAO_EC_ProxyPushSupplier : public POA_RtecEventChannelAdmin::ProxyPushSupplier, public TAO_EC_Filter
{
  // = TITLE
  //   ProxyPushSupplier
  //
  // = DESCRIPTION
  //   Implement the RtecEventChannelAdmin::ProxyPushSupplier interface,
  //   remember that this class is used to communicate with a
  //   PushConsumer, so, in effect, this is the ambassador for a
  //   consumer inside the event channel.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_EC_Dispatching object.
  //   It makes a copy of the ConsumerQOS and the consumer object
  //   reference.
  //
  // = LOCKING
  //   Locking is strategized, the event channel acts as a factory for
  //   the locking strategies.
  //
  // = TODO
  //   We don't need to provide a trivial filter, the object itself
  //   could short-circuit the filter() ---> push() cycle when the EC
  //   is properly configured, we need to explore this...
  //
public:
  TAO_EC_ProxyPushSupplier (TAO_EC_Event_Channel* event_channel);
  // constructor...

  virtual ~TAO_EC_ProxyPushSupplier (void);
  // destructor...

  CORBA::Boolean is_connected (void) const;
  // Return 0 if no consumer is connected...

  CORBA::Boolean is_suspended (void) const;
  // Return 1 if it is suspended.

  RtecEventComm::PushConsumer_ptr consumer (void) const;
  // Return the consumer object reference. It returns nil() if it has
  // not connected yet.
  // NOTE: This method does not return a new reference!!! Doing so
  // will increase the locking overhead on the critical path.

  const RtecEventChannelAdmin::ConsumerQOS& subscriptions (void) const;
  // The QoS (subscription) used to connect to the EC.

  virtual void connected (TAO_EC_ProxyPushConsumer *consumer,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_EC_ProxyPushConsumer *consumer,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushConsumer *consumer,
                            CORBA::Environment &env);
  // Concrete implementations can use this methods to keep track of
  // the suppliers that publish its events.

  virtual void connected (TAO_EC_ProxyPushSupplier *supplier,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_EC_ProxyPushSupplier *supplier,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushSupplier *supplier,
                             CORBA::Environment &env);
  // Usually implemented as no-ops, but some configurations may
  // require this methods.

  virtual void shutdown (CORBA::Environment &env);
  // The event channel is shutting down

  void push_to_consumer (RtecEventComm::PushConsumer_ptr consumer,
                         const RtecEventComm::EventSet &event,
                         CORBA::Environment &env);
  void reactive_push_to_consumer (RtecEventComm::PushConsumer_ptr consumer,
                                  const RtecEventComm::EventSet &event,
                                  CORBA::Environment &env);
  // Pushes to the consumer, verifies that it is connected and that it
  // is not suspended.

  CORBA::Boolean consumer_non_existent (CORBA::Boolean_out disconnected,
                                        CORBA::Environment &ACE_TRY_ENV);
  // Invoke the _non_existent() pseudo-operation on the consumer. If
  // it is disconnected then it returns true and sets the
  // <disconnected> flag.

  // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos,
                CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannelAdmin::AlreadyConnected,
                       RtecEventChannelAdmin::TypeError));
  virtual void disconnect_push_supplier (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void suspend_connection (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void resume_connection (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count.

  // = The TAO_EC_Filter methods, only push() is implemented...
  virtual int filter (const RtecEventComm::EventSet &event,
                      TAO_EC_QOS_Info& qos_info,
                      CORBA::Environment &env);
  virtual int filter_nocopy (RtecEventComm::EventSet &event,
                             TAO_EC_QOS_Info &qos_info,
                             CORBA::Environment &env);
  virtual void push (const RtecEventComm::EventSet &event,
                     TAO_EC_QOS_Info& qos_info,
                     CORBA::Environment &env);
  virtual void push_nocopy (RtecEventComm::EventSet &event,
                            TAO_EC_QOS_Info &qos_info,
                            CORBA::Environment &env);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader &header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader &header,
                                const TAO_EC_QOS_Info &qos_info,
                                CORBA::Environment &ACE_TRY_ENV);

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
  virtual void _add_ref (CORBA_Environment &ACE_TRY_ENV);
  virtual void _remove_ref (CORBA_Environment &ACE_TRY_ENV);

protected:
  void consumer (RtecEventComm::PushConsumer_ptr consumer);
  void consumer_i (RtecEventComm::PushConsumer_ptr consumer);
  // Set the consumer, used by some implementations to change the
  // policies used when invoking operations on the consumer.

  CORBA::Boolean is_connected_i (void) const;
  // The private version (without locking) of is_connected().

  void cleanup_i (void);
  // Release the child and the consumer

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate from the POA

private:
  TAO_EC_Event_Channel* event_channel_;
  // The Event Channel that owns this object.

  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::ULong refcount_;
  // The reference count.

  RtecEventComm::PushConsumer_var consumer_;
  // The consumer....

  CORBA::Boolean suspended_;
  // Is this consumer suspended?

  RtecEventChannelAdmin::ConsumerQOS qos_;
  // The subscription and QoS information...

  PortableServer::POA_var default_POA_;
  // Store the default POA.

  TAO_EC_Filter* child_;
  // The filter object
};

#if defined (__ACE_INLINE__)
#include "EC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_PROXYSUPPLIER_H */
