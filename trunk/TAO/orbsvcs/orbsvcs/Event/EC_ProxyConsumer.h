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

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_SupplierFiltering;

class TAO_ORBSVCS_Export TAO_EC_ProxyPushConsumer : public POA_RtecEventChannelAdmin::ProxyPushConsumer
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
  //   SupplierFiltering object.
  //   The object commits suicide when disconnect_push_consumer() is
  //   called.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
public:
  TAO_EC_ProxyPushConsumer (TAO_EC_Event_Channel* event_channel);
  // constructor...

  virtual ~TAO_EC_ProxyPushConsumer (void);
  // destructor...

  CORBA::Boolean is_connected (void) const;
  // Return 0 if no supplier is connected...

  RtecEventComm::PushSupplier_ptr supplier (void) const;
  // Return the consumer object reference. It returns nil() if it has
  // not connected yet.

  const RtecEventChannelAdmin::SupplierQOS& publications (void) const;
  // The QoS (subscription) used to connect to the EC.

  virtual void connected (TAO_EC_ProxyPushSupplier* supplier,
			  CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier,
			     CORBA::Environment &env);
  // Concrete implementations can use this methods to keep track of
  // the consumers interested in this events.

  virtual void connected (TAO_EC_ProxyPushConsumer* consumer,
			  CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushConsumer* consumer,
			     CORBA::Environment &env);
  // Usually implemented as no-ops, but some configurations may
  // require this methods.

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);
  // Override the ServantBase method.

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count.

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
		RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos,
                CORBA::Environment &);
  virtual void push (const RtecEventComm::EventSet& event,
                     CORBA::Environment &);
  virtual void disconnect_push_consumer (CORBA::Environment &);

  // = Servant reference counting methods.
  virtual void _add_ref (CORBA_Environment &ACE_TRY_ENV =
                             CORBA::default_environment ());
  virtual void _remove_ref (CORBA_Environment &ACE_TRY_ENV =
                                CORBA::default_environment ());

private:
  CORBA::Boolean is_connected_i (void) const;
  // The private version (without locking) of is_connected().

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

  TAO_EC_SupplierFiltering* filter_;
  // The strategy to do filtering close to the supplier
};

#if defined (__ACE_INLINE__)
#include "EC_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_PROXYCONSUMER_H */
