/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_SupplierFiltering
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Define the TAO_EC_SupplierFiltering interface and some simple
//   implementations.
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

#ifndef TAO_EC_SUPPLIERFILTERING_H
#define TAO_EC_SUPPLIERFILTERING_H

#include "orbsvcs/RtecEventCommC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_ProxyPushSupplier;
class TAO_EC_ProxyPushConsumer;
class TAO_EC_Event_Channel;

class TAO_EC_SupplierFiltering
{
  // = TITLE
  //   The strategy to handle filtering close to the supplier.
  //
  // = DESCRIPTION
  //   After an event is received by the a ProxyPushConsumer it must
  //   be dispatched to the right set of ProxyPushSuppliers;
  //   determining this set is the task of this class.
  //   Notice that this is an implicit form of filtering, because the
  //   ProxyPushSuppliers outside the set will *not* receive the
  //   event.
  //
  //   Several implementations are possible:
  //
  //   - Each ProxyPushConsumer keeps a list of ProxyPushSuppliers,
  //     using the subscriptions and publications to find potential
  //     matches.
  //   - Each ProxyPushConsumer keeps several such lists, indexed by
  //     event type and/or source, this has the advantage of further
  //     minimizing the set of ProxyPushSuppliers invoked.
  //   - A single list of consumers is kept (global for the event
  //     channel), such a list results is faster updates and requires
  //     an smaller memory footprint.
  //   - Multiple global lists are kept, indexed by type and/or
  //     source, this is a tradeoff between the solutions above.
  //   - No list is kept, the events are sent to the consumers which
  //     must filter out what they want, this is good when no
  //     filtering is wanted or when the amount of filtering is coarse
  //     i.e. each event goes to a large subset of the
  //     ProxyPushSuppliers.
  //
  //  Different applications will require to use different
  //  implementations of this class; as usual the EC_Factory will
  //  create the right instance.
  //
public:
  virtual ~TAO_EC_SupplierFiltering (void);
  // Destructor

  virtual void bind (TAO_EC_ProxyPushConsumer* consumer) = 0;
  // Whenever a ProxyPushConsumer is initialized it calls this method
  // to inform the FilteringStrategy of its identity.
  // Strategies that do not keep ProxyPushConsumer specific
  // information, or that are shared between multiple
  // ProxyPushConsumers can ignore this message.

  virtual void unbind (TAO_EC_ProxyPushConsumer* consumer) = 0;
  // Wheneve a ProxyPushConsumer is about to be destroyed it calls
  // this method to inform the FilteringStrategy that it should
  // release any resources related to it.
  // Filtering strategies that are bound to a particular ProxyConsumer
  // can use this opportunity to destroy themselves; filtering
  // strategies that do not keep ProxyPushConsumer specific
  // information can simply ignore the message.

  virtual void connected (TAO_EC_ProxyPushSupplier* supplier,
			  CORBA::Environment &env) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier,
			     CORBA::Environment &env) = 0;
  // Concrete implementations can use this methods to keep track of
  // the consumers interested in this events.

  virtual void push (const RtecEventComm::EventSet& event,
                     CORBA::Environment &) = 0;
  // The ProxyPushConsumer delegates on this class to actually send
  // the event.
};

// ****************************************************************

class TAO_EC_Null_SupplierFiltering : public TAO_EC_SupplierFiltering
{
  // = TITLE
  //   A null filtering strategy.
  //
  // = DESCRIPTION
  //   This strategy does no filtering, it is useful for backends of
  //   the CosEventChannel, testing and broadcasters; it uses the
  //   ConsumerAdmin to find all the consumers and pushes the event to
  //   all of them.
  //
public:
  TAO_EC_Null_SupplierFiltering (TAO_EC_Event_Channel* ec);
  // Constructor

  // = The TAO_EC_SupplierFiltering methods.
  virtual void bind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void unbind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void connected (TAO_EC_ProxyPushSupplier* supplier,
			  CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier,
			     CORBA::Environment &env);
  virtual void push (const RtecEventComm::EventSet& event,
                     CORBA::Environment &);

private:
  TAO_EC_Event_Channel *event_channel_;
  // The event channel, used to locate the set of consumers.
};

#if defined (__ACE_INLINE__)
#include "EC_SupplierFiltering.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_SUPPLIERFILTERING_H */
