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
// = DESCRIPTION
//   Implement the RtecEventChannelAdmin::ProxyPushConsumer interface,
//   remember that this class is used to communicate with a
//   PushSupplier, so, in effect, this is the ambassador for a
//   supplier inside the event channel.
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

#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Dispatching;
class TAO_EC_Filter_Builder;

class TAO_EC_ProxyPushConsumer : public TAO_EC_Filter
{
  // = TITLE
  //   ProxyPushConsumer
  //
  // = DESCRIPTION
  //   Implements the ProxyPushConsumer interface, i.e. the object
  //   used to communicate with a particular consumer.
  //
  // = MEMORY MANAGMENT
  //   It makes a copy of the SupplierQOS and the supplier object
  //   reference.
  //   The object commits suicide when disconnect_push_consumer() is
  //   called.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
public:
  TAO_EC_ProxyPushConsumer (TAO_EC_SupplierAdmin* supplier_admin);
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

  virtual void connected (TAO_EC_ProxyPushSupplier* supplier) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier) = 0;
  // Concrete implementations can use this methods to keep track of
  // the consumers interested in this events.

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
		RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS& qos,
                CORBA::Environment &);
  virtual void push (const RtecEventComm::EventSet& event,
                     CORBA::Environment &);
  virtual void disconnect_push_consumer (CORBA::Environment &);

private:
  TAO_EC_SupplierAdmin* supplier_admin_;
  // The supplier admin, used for activation and memory managment.

  RtecEventComm::PushSupplier_var supplier_;
  // The supplier....

  RtecEventChannelAdmin::SupplierQOS qos_;
  // The publication and QoS information...
};

#if defined (__ACE_INLINE__)
#include "EC_ProxyConsumer.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_PROXYCONSUMER_H */
