/* -*- C++ -*- */
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
#include "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel;
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
class TAO_RTEvent_Export TAO_EC_ProxyPushConsumer : public POA_RtecEventChannelAdmin::ProxyPushConsumer
{
public:
  typedef RtecEventChannelAdmin::ProxyPushConsumer Interface;
  typedef RtecEventChannelAdmin::ProxyPushConsumer_var _var_type;

  /// constructor...
  TAO_EC_ProxyPushConsumer (TAO_EC_Event_Channel* event_channel);

  /// destructor...
  virtual ~TAO_EC_ProxyPushConsumer (void);

  /// Activate in the POA
  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr activate (TAO_ENV_SINGLE_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException));

  /// Deactivate from the POA
  void deactivate (TAO_ENV_SINGLE_ARG_DECL);

  /// Return 0 if no supplier is connected...
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
  CORBA::Boolean supplier_non_existent (CORBA::Boolean_out disconnected
                                        TAO_ENV_ARG_DECL);

  /// Concrete implementations can use this methods to keep track of
  /// the consumers interested in this events.
  virtual void connected (TAO_EC_ProxyPushSupplier* supplier
                          TAO_ENV_ARG_DECL);
  virtual void reconnected (TAO_EC_ProxyPushSupplier* supplier
                            TAO_ENV_ARG_DECL);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier
                             TAO_ENV_ARG_DECL);

  /// Usually implemented as no-ops, but some configurations may
  /// require this methods.
  virtual void connected (TAO_EC_ProxyPushConsumer* consumer
                          TAO_ENV_ARG_DECL);
  virtual void reconnected (TAO_EC_ProxyPushConsumer* consumer
                            TAO_ENV_ARG_DECL);
  virtual void disconnected (TAO_EC_ProxyPushConsumer* consumer
                             TAO_ENV_ARG_DECL);

  /// The event channel is shutting down
  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// The QoS (subscription) used to connect to the EC, assumes the
  /// locks are held, use with care!
  const RtecEventChannelAdmin::SupplierQOS& publications_i (void) const;

  /// Increment and decrement the reference count.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos
                TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannelAdmin::AlreadyConnected));
  virtual void push (const RtecEventComm::EventSet& event
                     TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL);
  virtual void _add_ref (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  virtual void _remove_ref (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

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

private:
  /// The supplier admin, used for activation and memory managment.
  TAO_EC_Event_Channel* event_channel_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The reference count.
  CORBA::ULong refcount_;

  /// The supplier....
  RtecEventComm::PushSupplier_var supplier_;

  /// If the flag is not zero then we are connected, notice that the
  /// supplier can be nil.
  int connected_;

  /// The publication and QoS information...
  RtecEventChannelAdmin::SupplierQOS qos_;

  /// Store the default POA.
  PortableServer::POA_var default_POA_;

  /// The strategy to do filtering close to the supplier
  TAO_EC_Supplier_Filter* filter_;
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
class TAO_RTEvent_Export TAO_EC_ProxyPushConsumer_Guard
{
public:
  /// Constructor
  TAO_EC_ProxyPushConsumer_Guard (ACE_Lock *lock,
                                  CORBA::ULong &refcount,
                                  TAO_EC_Event_Channel *ec,
                                  TAO_EC_ProxyPushConsumer *proxy);

  /// Destructor
  ~TAO_EC_ProxyPushConsumer_Guard (void);

  /// Returns 1 if the reference count successfully acquired
  int locked (void) const;

  TAO_EC_Supplier_Filter *filter;

private:
  /// The lock used to protect the reference count
  ACE_Lock *lock_;

  /// The reference count
  CORBA::ULong &refcount_;

  /// The event channel used to destroy the proxy
  TAO_EC_Event_Channel *event_channel_;

  /// The proxy whose lifetime is controlled by the reference count
  TAO_EC_ProxyPushConsumer *proxy_;

  /// This flag is set to 1 if the reference count was successfully
  /// acquired.
  int locked_;
};

#if defined (__ACE_INLINE__)
#include "EC_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_PROXYCONSUMER_H */
