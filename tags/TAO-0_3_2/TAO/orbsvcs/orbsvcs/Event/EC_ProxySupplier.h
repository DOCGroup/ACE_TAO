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
// = DESCRIPTION
//   Implement the RtecEventChannelAdmin::ProxyPushSupplier interface,
//   remember that this class is used to communicate with a
//   PushConsumer, so, in effect, this is the ambassador for a
//   consumer inside the event channel.
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

class TAO_EC_ProxyPushSupplier : public POA_RtecEventChannelAdmin::ProxyPushSupplier, public TAO_EC_Filter
{
  // = TITLE
  //   ProxyPushSupplier
  //
  // = DESCRIPTION
  //   Implements the ProxyPushSupplier interface, i.e. the object
  //   used to communicate with a particular consumer.
  //
  // = MEMORY MANAGMENT
  //   This object will be reference counted.
  //   It does not assume ownership of the TAO_EC_Dispatching object.
  //   It makes a copy of the ConsumerQOS and the consumer object
  //   reference.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
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

  const RtecEventChannelAdmin::ConsumerQOS& subscriptions (void) const;
  // The QoS (subscription) used to connect to the EC.

  virtual void connected (TAO_EC_ProxyPushConsumer* consumer,
			  CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushConsumer* consumer,
			     CORBA::Environment &env);
  // Concrete implementations can use this methods to keep track of
  // the suppliers that publish its events.

  virtual void connected (TAO_EC_ProxyPushSupplier* supplier,
			  CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier,
			     CORBA::Environment &env);
  // Usually implemented as no-ops, but some configurations may
  // require this methods.

  void set_default_POA (PortableServer::POA_ptr poa);
  // Set this servant's default POA

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);
  // Override the ServantBase method.

  // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
		RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS& qos,
                CORBA::Environment &);
  virtual void disconnect_push_supplier (CORBA::Environment &);
  virtual void suspend_connection (CORBA::Environment &);
  virtual void resume_connection (CORBA::Environment &);

  // = The TAO_EC_Filter methods, only push() is implemented...
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info,
                      CORBA::Environment& env);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info,
                             CORBA::Environment& env);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info,
                     CORBA::Environment& env); 
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info,
                            CORBA::Environment& env); 
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual void event_ids (TAO_EC_Filter::Headers& headerset);

private:
  TAO_EC_Event_Channel* event_channel_;
  // The Event Channel that owns this object.

  RtecEventComm::PushConsumer_var consumer_;
  // The consumer....

  CORBA::Boolean suspended_;
  // Is this consumer suspended?

  RtecEventChannelAdmin::ConsumerQOS qos_;
  // The subscription and QoS information...

  PortableServer::POA_var default_POA_;
  // Store the default POA.

private:
  TAO_EC_Filter* child_;
};

#if defined (__ACE_INLINE__)
#include "EC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_PROXYSUPPLIER_H */
