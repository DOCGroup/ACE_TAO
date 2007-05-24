// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_ProxyPullSupplier.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_PROXYPULLSUPPLIER_H
#define TAO_CEC_PROXYPULLSUPPLIER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Queue.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Unbounded_Queue.h"

#include "orbsvcs/CosEvent/event_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPullConsumer;

/**
 * @class TAO_CEC_ProxyPullSupplier
 *
 * @brief ProxyPullSupplier
 *
 * Implement the CosEventChannelAdmin::ProxyPullSupplier interface,
 * remember that this class is used to communicate with a
 * PullConsumer, so, in effect, this is the ambassador for a
 * consumer inside the event channel.
 * = MEMORY MANAGMENT
 * It does not assume ownership of the TAO_CEC_Dispatching object.
 * It makes a copy of the ConsumerQOS and the consumer object
 * reference.
 * = LOCKING
 * Locking is strategized, the event channel acts as a factory for
 * the locking strategies.
 */
class TAO_Event_Serv_Export TAO_CEC_ProxyPullSupplier : public POA_CosEventChannelAdmin::ProxyPullSupplier
{
public:
  typedef CosEventChannelAdmin::ProxyPullSupplier_ptr _ptr_type;
  typedef CosEventChannelAdmin::ProxyPullSupplier_var _var_type;

  /// constructor...
  TAO_CEC_ProxyPullSupplier (TAO_CEC_EventChannel* event_channel,
                             const ACE_Time_Value &timeout);

  /// destructor...
  virtual ~TAO_CEC_ProxyPullSupplier (void);

  /// Activate in the POA
  virtual void activate (
      CosEventChannelAdmin::ProxyPullSupplier_ptr &activated_proxy);

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
  CosEventComm::PullConsumer_ptr consumer (void) const;

  /// The event channel is shutting down
  virtual void shutdown (void);

  /**
   * Invoke the _non_existent() pseudo-operation on the consumer. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean consumer_non_existent (CORBA::Boolean_out disconnected);

  /// Push an event into the queue.
  void push (const CORBA::Any &event);

  // = The CosEventChannelAdmin::ProxyPullSupplier methods...
  virtual void connect_pull_consumer (
                CosEventComm::PullConsumer_ptr pull_consumer);
  virtual CORBA::Any * pull (void);
  virtual CORBA::Any * try_pull (CORBA::Boolean_out has_event);
  virtual void disconnect_pull_supplier (void);

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
  void consumer (CosEventComm::PullConsumer_ptr consumer);
  void consumer_i (CosEventComm::PullConsumer_ptr consumer);

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Release the child and the consumer
  void cleanup_i (void);

  /// Assigns the parameter to both consumer_ and nopolicy_consumer_, and
  /// applies policies (when appropriate) to consumer_.
  CosEventComm::PullConsumer_ptr apply_policy
  (CosEventComm::PullConsumer_ptr c);

private:
  /// The Event Channel that owns this object.
  TAO_CEC_EventChannel* event_channel_;

  ACE_Time_Value timeout_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The consumer -- use apply_policy() instead of assigning directly to
  /// consumer_.  This will keep consumer_ and nopolicy_consumer_ in sync.
  CosEventComm::PullConsumer_var consumer_;

  /// The consumer without any policies applied
  CosEventComm::PullConsumer_var nopolicy_consumer_;

  /// If the flag is not zero then we are connected, notice that the
  /// consumer can be nil.
  int connected_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;

  /// Use a message queue to pass the
  TAO_SYNCH_MUTEX queue_lock_;
  TAO_SYNCH_CONDITION wait_not_empty_;
  ACE_Unbounded_Queue<CORBA::Any> queue_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_CEC_PROXYPULLSUPPLIER_H */
