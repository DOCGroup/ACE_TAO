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

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushConsumer;

class TAO_ORBSVCS_Export TAO_EC_ProxyPushSupplier : public POA_RtecEventChannelAdmin::ProxyPushSupplier, public TAO_EC_Filter
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
                          CORBA::Environment &env = TAO_default_environment ());
  virtual void disconnected (TAO_EC_ProxyPushConsumer *consumer,
                             CORBA::Environment &env = TAO_default_environment ());
  // Concrete implementations can use this methods to keep track of
  // the suppliers that publish its events.

  virtual void connected (TAO_EC_ProxyPushSupplier *supplier,
                          CORBA::Environment &env = TAO_default_environment ());
  virtual void disconnected (TAO_EC_ProxyPushSupplier *supplier,
                             CORBA::Environment &env = TAO_default_environment ());
  // Usually implemented as no-ops, but some configurations may
  // require this methods.

  virtual void shutdown (CORBA::Environment &env = TAO_default_environment ());
  // The event channel is shutting down

  void push_to_consumer (const RtecEventComm::EventSet &event,
                         CORBA::Environment &env = TAO_default_environment ());
  void reactive_push_to_consumer (const RtecEventComm::EventSet &event,
                                  CORBA::Environment &env = TAO_default_environment ());
  // Pushes to the consumer, verifies that it is connected and that it
  // is not suspended.

  void push_timeout (TAO_EC_Filter *timeout_filter,
                     const RtecEventComm::EventSet &event,
                     TAO_EC_QOS_Info &qos_info,
                     CORBA::Environment &env = TAO_default_environment ());
  // Callback from the timeout filters

  // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos,
                CORBA::Environment & = TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannelAdmin::AlreadyConnected,
                       RtecEventChannelAdmin::TypeError));
  virtual void disconnect_push_supplier (CORBA::Environment & = TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void suspend_connection (CORBA::Environment & = TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void resume_connection (CORBA::Environment & = TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count.

  // = The TAO_EC_Filter methods, only push() is implemented...
  virtual int filter (const RtecEventComm::EventSet &event,
                      TAO_EC_QOS_Info& qos_info,
                      CORBA::Environment &env = TAO_default_environment ());
  virtual int filter_nocopy (RtecEventComm::EventSet &event,
                             TAO_EC_QOS_Info &qos_info,
                             CORBA::Environment &env = TAO_default_environment ());
  virtual void push (const RtecEventComm::EventSet &event,
                     TAO_EC_QOS_Info& qos_info,
                     CORBA::Environment &env = TAO_default_environment ());
  virtual void push_nocopy (RtecEventComm::EventSet &event,
                            TAO_EC_QOS_Info &qos_info,
                            CORBA::Environment &env = TAO_default_environment ());
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader &header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader &header,
                                const TAO_EC_QOS_Info &qos_info,
                                CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env = TAO_default_environment ());
  virtual void _add_ref (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  virtual void _remove_ref (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());

private:
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

#endif /* TAO_EC_PROXYSUPPLIER_H */
