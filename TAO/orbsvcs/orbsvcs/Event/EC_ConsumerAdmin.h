/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ConsumerAdmin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Implement the RtecEventChannelAdmin::ConsumerAdmin interface.
//   This class is an Abstract Factory for the
//   TAO_EC_ProxyPushSupplier.
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

#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Dispatching;
class TAO_EC_Filter_Builder;

class TAO_EC_ConsumerAdmin : public POA_RtecEventChannelAdmin::ConsumerAdmin
{
  // = TITLE
  //   ProxyPushSupplier
  //
  // = DESCRIPTION
  //   Implements the ConsumerAdmin interface, i.e. the factory for
  //   ProxyPushSupplier objects.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_EC_Dispatching object
  //   or the EC_Filter_Builder object.
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
  TAO_EC_ConsumerAdmin (TAO_EC_Dispatching* dispatching,
                        TAO_EC_Filter_Builder* builder);
  // constructor...

  virtual ~TAO_EC_ConsumerAdmin (void);
  // destructor...

  virtual void activate (TAO_EC_ProxyPushSupplier* proxy,
                         RtecEventChannelAdmin::ProxyPushSupplier_out objref,
                         CORBA::Environment& env) = 0;
  virtual void deactivate (TAO_EC_ProxyPushSupplier* proxy,
                           RtecEventChannelAdmin::ProxyPushSupplier_ptr objref,
                           CORBA::Environment& env) = 0;

  // = The RtecEventChannelAdmin::ConsumerAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (CORBA::Environment &);

private:
  TAO_EC_ConsumerAdmin* consumer_admin_;
  // The consumer admin, used for activation and memory managment.

  TAO_EC_Dispatching *dispatching_;
  // We delegate on this object to handle 

  RtecEventComm::PushConsumer_var consumer_;
  // The consumer....

  CORBA::Boolean suspended_;
  // Is this consumer suspended?

  RtecEventChannelAdmin::ConsumerQOS qos_;
  // The subscription and QoS information...
};

#if defined (__ACE_INLINE__)
#include "EC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_PROXYSUPPLIER_H */
