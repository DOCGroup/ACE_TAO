/* -*- C++ -*- */
/**
 *  @file   EC_ProxySupplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

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

/**
 * @class TAO_EC_ProxyPushSupplier
 *
 * @brief ProxyPushSupplier
 *
 * Implement the RtecEventChannelAdmin::ProxyPushSupplier interface,
 * remember that this class is used to communicate with a
 * PushConsumer, so, in effect, this is the ambassador for a
 * consumer inside the event channel.
 *
 * <H2>Memory Management</H2>
 * It does not assume ownership of the TAO_EC_Dispatching object.
 * It makes a copy of the ConsumerQOS and the consumer object
 * reference.
 *
 * <H2>Locking</H2>
 * Locking is strategized, the event channel acts as a factory for
 * the locking strategies.
 *
 * @todo We don't need to provide a trivial filter, the object itself
 * could short-circuit the filter() ---> push() cycle when the EC
 * is properly configured, we need to explore this...
 */
class TAO_RTEvent_Export TAO_EC_ProxyPushSupplier : public POA_RtecEventChannelAdmin::ProxyPushSupplier, public TAO_EC_Filter
{
public:
  typedef RtecEventChannelAdmin::ProxyPushSupplier Interface;
  typedef RtecEventChannelAdmin::ProxyPushSupplier_var _var_type;

  /// constructor...
  TAO_EC_ProxyPushSupplier (TAO_EC_Event_Channel* event_channel);

  /// destructor...
  virtual ~TAO_EC_ProxyPushSupplier (void);

  /// Activate in the POA
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr activate (CORBA::Environment &ACE_TRY_ENV) ACE_THROW_SPEC (());

  /// Deactivate from the POA
  virtual void deactivate (CORBA::Environment &ACE_TRY_ENV) ACE_THROW_SPEC (());

  /// Return 0 if no consumer is connected...
  CORBA::Boolean is_connected (void) const;

  /// Return 1 if it is suspended.
  CORBA::Boolean is_suspended (void) const;

  /**
   * Return the consumer object reference. It returns nil() if it has
   * not connected yet.
   * NOTE: This method does not return a new reference!!! Doing so
   * will increase the locking overhead on the critical path.
   */
  RtecEventComm::PushConsumer_ptr consumer (void) const;

  /// The QoS (subscription) used to connect to the EC.
  const RtecEventChannelAdmin::ConsumerQOS& subscriptions (void) const;

  /// Concrete implementations can use this methods to keep track of
  /// the suppliers that publish its events.
  virtual void connected (TAO_EC_ProxyPushConsumer *consumer,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_EC_ProxyPushConsumer *consumer,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushConsumer *consumer,
                            CORBA::Environment &env);

  /// Usually implemented as no-ops, but some configurations may
  /// require this methods.
  virtual void connected (TAO_EC_ProxyPushSupplier *supplier,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_EC_ProxyPushSupplier *supplier,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushSupplier *supplier,
                             CORBA::Environment &env);

  /// The event channel is shutting down
  virtual void shutdown (CORBA::Environment &env);

  /// Pushes to the consumer, verifies that it is connected and that it
  /// is not suspended.
  void push_to_consumer (RtecEventComm::PushConsumer_ptr consumer,
                         const RtecEventComm::EventSet &event,
                         CORBA::Environment &env);
  void reactive_push_to_consumer (RtecEventComm::PushConsumer_ptr consumer,
                                  const RtecEventComm::EventSet &event,
                                  CORBA::Environment &env);

  /**
   * Invoke the _non_existent() pseudo-operation on the consumer. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean consumer_non_existent (CORBA::Boolean_out disconnected,
                                        CORBA::Environment &ACE_TRY_ENV);

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

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

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
  /// Set the consumer, used by some implementations to change the
  /// policies used when invoking operations on the consumer.
  void consumer (RtecEventComm::PushConsumer_ptr consumer);
  void consumer_i (RtecEventComm::PushConsumer_ptr consumer);

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the child and the consumer
  void cleanup_i (void);

private:
  /// The Event Channel that owns this object.
  TAO_EC_Event_Channel* event_channel_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The consumer....
  RtecEventComm::PushConsumer_var consumer_;

  /// Is this consumer suspended?
  CORBA::Boolean suspended_;

  /// The subscription and QoS information...
  RtecEventChannelAdmin::ConsumerQOS qos_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;

  /// The filter object
  TAO_EC_Filter* child_;
};

#if defined (__ACE_INLINE__)
#include "EC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_PROXYSUPPLIER_H */
