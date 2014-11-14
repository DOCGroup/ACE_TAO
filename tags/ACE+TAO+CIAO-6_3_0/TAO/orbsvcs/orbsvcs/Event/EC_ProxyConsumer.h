// -*- C++ -*-
/**
 *  @file   EC_ProxyConsumer.h
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

#ifndef TAO_EC_PROXYCONSUMER_H
#define TAO_EC_PROXYCONSUMER_H

#include /**/ "ace/pre.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"

#include "orbsvcs/RtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_Supplier_Filter;

/**
 * @class TAO_EC_ProxyPushConsumer
 *
 * @brief ProxyPushConsumer
 *
 * Implement the RtecEventChannelAdmin::ProxyPushConsumer interface,
 * remember that this class is used to communicate with a
 * PushSupplier, so, in effect, this is the ambassador for a
 * supplier inside the event channel.
 *
 * <H2>Memory Management</H2>
 * It makes a copy of the SupplierQOS and the supplier object
 * reference.
 * It uses bind/unbind to control the lifetime of the
 * Supplier_Filter object.
 * The object commits suicide when disconnect_push_consumer() is
 * called.
 *
 * <H2>Locking</H2>
 * No provisions for locking, access must be serialized
 * externally.
 */
class TAO_RTEvent_Serv_Export TAO_EC_ProxyPushConsumer
{
public:
  typedef RtecEventChannelAdmin::ProxyPushConsumer Interface;
  typedef RtecEventChannelAdmin::ProxyPushConsumer_var _var_type;
  typedef RtecEventChannelAdmin::ProxyPushConsumer_ptr _ptr_type;

  /// constructor...
  TAO_EC_ProxyPushConsumer (TAO_EC_Event_Channel_Base* event_channel);

  /// destructor...
  virtual ~TAO_EC_ProxyPushConsumer (void);

  /// Activate in the POA
  virtual void activate (
          RtecEventChannelAdmin::ProxyPushConsumer_ptr &proxy)
    = 0;

  /// Deactivate from the POA
  virtual void deactivate (void);

  /// Disconnect this from
  virtual void disconnect_push_consumer () = 0;

  /// Return false if no supplier is connected...
  CORBA::Boolean is_connected (void) const;

  /// Return the consumer object reference. It returns nil() if it has
  /// not connected yet.
  RtecEventComm::PushSupplier_ptr supplier (void) const;

  /// The QoS (subscription) used to connect to the EC.
  const RtecEventChannelAdmin::SupplierQOS& publications (void) const;

  /**
   * Invoke the _non_existent() pseudo-operation on the supplier. If
   * it is disconnected then it returns true and sets the
   * <disconnected> flag.
   */
  CORBA::Boolean supplier_non_existent (CORBA::Boolean_out disconnected);

  /// Concrete implementations can use this methods to keep track of
  /// the consumers interested in this events.
  virtual void connected (TAO_EC_ProxyPushSupplier* supplier);
  virtual void reconnected (TAO_EC_ProxyPushSupplier* supplier);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier);

  /// Usually implemented as no-ops, but some configurations may
  /// require this methods.
  virtual void connected (TAO_EC_ProxyPushConsumer* consumer);
  virtual void reconnected (TAO_EC_ProxyPushConsumer* consumer);
  virtual void disconnected (TAO_EC_ProxyPushConsumer* consumer);

  /// The event channel is shutting down
  virtual void shutdown (void);

  /// The QoS (subscription) used to connect to the EC, assumes the
  /// locks are held, use with care!
  const RtecEventChannelAdmin::SupplierQOS& publications_i (void) const;

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);


protected:
  /// Set the supplier, used by some implementations to change the
  /// policies used when invoking operations on the supplier.
  void supplier (RtecEventComm::PushSupplier_ptr supplier);
  void supplier_i (RtecEventComm::PushSupplier_ptr supplier);

  friend class TAO_EC_ProxyPushConsumer_Guard;
  // The guard needs access to the following protected methods.

  /// The private version (without locking) of is_connected().
  CORBA::Boolean is_connected_i (void) const;

  /// Return the current filter, assumes the locks are held.
  TAO_EC_Supplier_Filter *filter_i (void) const;

  /// Release the filter and the supplier
  void cleanup_i (void);

  /// The supplier admin, used for activation and memory managment.
  TAO_EC_Event_Channel_Base* event_channel_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The supplier....
  RtecEventComm::PushSupplier_var supplier_;

  /// If the flag is not zero then we are connected, notice that the
  /// supplier can be nil.
  bool connected_;

  /// The publication and QoS information...
  RtecEventChannelAdmin::SupplierQOS qos_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;

  /// The strategy to do filtering close to the supplier
  TAO_EC_Supplier_Filter* filter_;

private:
  /// Template method hooks.
  virtual void shutdown_hook (void);
  virtual void refcount_zero_hook (void);

  virtual PortableServer::ObjectId object_id (void) = 0;
};

// ****************************************************************

/**
 * @class TAO_EC_ProxyPushConsumer_Guard
 *
 * @brief A Guard for the ProxyPushConsumer reference count
 *
 * This is a helper class used in the implementation of
 * ProxyPushConumer.  It provides a Guard mechanism to increment
 * the reference count on the proxy and its filter, eliminating
 * the need to hold mutexes during long operations.
 */
class TAO_RTEvent_Serv_Export TAO_EC_ProxyPushConsumer_Guard
{
public:
  /// Constructor
  TAO_EC_ProxyPushConsumer_Guard (ACE_Lock *lock,
                                  CORBA::ULong &refcount,
                                  TAO_EC_Event_Channel_Base *ec,
                                  TAO_EC_ProxyPushConsumer *proxy);

  /// Destructor
  ~TAO_EC_ProxyPushConsumer_Guard (void);

  /// Returns true if the reference count successfully acquired
  bool locked (void) const;

  TAO_EC_Supplier_Filter *filter;

private:
  /// The lock used to protect the reference count
  ACE_Lock *lock_;

  /// The reference count
  CORBA::ULong &refcount_;

  /// The event channel used to destroy the proxy
  // TAO_EC_Event_Channel_Base *event_channel_;

  /// The proxy whose lifetime is controlled by the reference count
  TAO_EC_ProxyPushConsumer *proxy_;

  /// This flag is set to true if the reference count was successfully
  /// acquired.
  bool locked_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_EC_PROXYCONSUMER_H */
