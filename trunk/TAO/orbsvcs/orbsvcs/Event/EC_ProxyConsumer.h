/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ProxyConsumer
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

class TAO_RTEvent_Export TAO_EC_ProxyPushConsumer : public POA_RtecEventChannelAdmin::ProxyPushConsumer
{
  // = TITLE
  //   ProxyPushConsumer
  //
  // = DESCRIPTION
  //   Implement the RtecEventChannelAdmin::ProxyPushConsumer interface,
  //   remember that this class is used to communicate with a
  //   PushSupplier, so, in effect, this is the ambassador for a
  //   supplier inside the event channel.
  //
  // = MEMORY MANAGMENT
  //   It makes a copy of the SupplierQOS and the supplier object
  //   reference.
  //   It uses bind/unbind to control the lifetime of the
  //   Supplier_Filter object.
  //   The object commits suicide when disconnect_push_consumer() is
  //   called.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
public:
  typedef RtecEventChannelAdmin::ProxyPushConsumer Interface;
  typedef RtecEventChannelAdmin::ProxyPushConsumer_var _var_type;

  TAO_EC_ProxyPushConsumer (TAO_EC_Event_Channel* event_channel);
  // constructor...

  virtual ~TAO_EC_ProxyPushConsumer (void);
  // destructor...

  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr activate (CORBA::Environment &ACE_TRY_ENV) ACE_THROW_SPEC (());
  // Activate in the POA

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate from the POA

  CORBA::Boolean is_connected (void) const;
  // Return 0 if no supplier is connected...

  RtecEventComm::PushSupplier_ptr supplier (void) const;
  // Return the consumer object reference. It returns nil() if it has
  // not connected yet.

  const RtecEventChannelAdmin::SupplierQOS& publications (void) const;
  // The QoS (subscription) used to connect to the EC.

  CORBA::Boolean supplier_non_existent (CORBA::Boolean_out disconnected,
                                        CORBA::Environment &ACE_TRY_ENV);
  // Invoke the _non_existent() pseudo-operation on the supplier. If
  // it is disconnected then it returns true and sets the
  // <disconnected> flag.

  virtual void connected (TAO_EC_ProxyPushSupplier* supplier,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_EC_ProxyPushSupplier* supplier,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier,
                             CORBA::Environment &env);
  // Concrete implementations can use this methods to keep track of
  // the consumers interested in this events.

  virtual void connected (TAO_EC_ProxyPushConsumer* consumer,
                          CORBA::Environment &env);
  virtual void reconnected (TAO_EC_ProxyPushConsumer* consumer,
                            CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushConsumer* consumer,
                             CORBA::Environment &env);
  // Usually implemented as no-ops, but some configurations may
  // require this methods.

  virtual void shutdown (CORBA::Environment&);
  // The event channel is shutting down

  const RtecEventChannelAdmin::SupplierQOS& publications_i (void) const;
  // The QoS (subscription) used to connect to the EC, assumes the
  // locks are held, use with care!

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count.

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos,
                CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannelAdmin::AlreadyConnected));
  virtual void push (const RtecEventComm::EventSet& event,
                     CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);
  virtual void _add_ref (CORBA_Environment &ACE_TRY_ENV =
                             TAO_default_environment ());
  virtual void _remove_ref (CORBA_Environment &ACE_TRY_ENV =
                                TAO_default_environment ());

protected:
  void supplier (RtecEventComm::PushSupplier_ptr supplier);
  void supplier_i (RtecEventComm::PushSupplier_ptr supplier);
  // Set the supplier, used by some implementations to change the
  // policies used when invoking operations on the supplier.

  friend class TAO_EC_ProxyPushConsumer_Guard;
  // The guard needs access to the following protected methods.

  CORBA::Boolean is_connected_i (void) const;
  // The private version (without locking) of is_connected().

  TAO_EC_Supplier_Filter *filter_i (void) const;
  // Return the current filter, assumes the locks are held.

  void cleanup_i (void);
  // Release the filter and the supplier

private:
  TAO_EC_Event_Channel* event_channel_;
  // The supplier admin, used for activation and memory managment.

  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::ULong refcount_;
  // The reference count.

  RtecEventComm::PushSupplier_var supplier_;
  // The supplier....

  RtecEventChannelAdmin::SupplierQOS qos_;
  // The publication and QoS information...

  PortableServer::POA_var default_POA_;
  // Store the default POA.

  TAO_EC_Supplier_Filter* filter_;
  // The strategy to do filtering close to the supplier
};

// ****************************************************************

class TAO_RTEvent_Export TAO_EC_ProxyPushConsumer_Guard
{
  // = TITLE
  //   A Guard for the ProxyPushConsumer reference count
  //
  // = DESCRIPTION
  //   This is a helper class used in the implementation of
  //   ProxyPushConumer.  It provides a Guard mechanism to increment
  //   the reference count on the proxy and its filter, eliminating
  //   the need to hold mutexes during long operations.
  //
public:
  TAO_EC_ProxyPushConsumer_Guard (ACE_Lock *lock,
                                  CORBA::ULong &refcount,
                                  TAO_EC_Event_Channel *ec,
                                  TAO_EC_ProxyPushConsumer *proxy);
  // Constructor

  ~TAO_EC_ProxyPushConsumer_Guard (void);
  // Destructor

  int locked (void) const;
  // Returns 1 if the reference count successfully acquired
  
  TAO_EC_Supplier_Filter *filter;

private:
  ACE_Lock *lock_;
  // The lock used to protect the reference count

  CORBA::ULong &refcount_;
  // The reference count

  TAO_EC_Event_Channel *event_channel_;
  // The event channel used to destroy the proxy

  TAO_EC_ProxyPushConsumer *proxy_;
  // The proxy whose lifetime is controlled by the reference count

  int locked_;
  // This flag is set to 1 if the reference count was successfully
  // acquired.
};

#if defined (__ACE_INLINE__)
#include "EC_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_PROXYCONSUMER_H */
