/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_SupplierAdmin
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

#ifndef TAO_EC_SUPPLIERADMIN_H
#define TAO_EC_SUPPLIERADMIN_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_ProxyPushConsumer;

class TAO_ORBSVCS_Export TAO_EC_SupplierAdmin : public POA_RtecEventChannelAdmin::SupplierAdmin
{
  // = TITLE
  //   ProxyPushSupplier
  //
  // = DESCRIPTION
  //   Implement the RtecEventChannelAdmin::SupplierAdmin interface.
  //   This class is an Abstract Factory for the
  //   TAO_EC_ProxyPushConsumer.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_EC_Event_Channel object
  //
  // = LOCKING
  //   @@ TODO
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_EC_SupplierAdmin (TAO_EC_Event_Channel* event_channel);
  // constructor...

  virtual ~TAO_EC_SupplierAdmin (void);
  // destructor...

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);
  // Override the ServantBase method.

  virtual void connected (TAO_EC_ProxyPushConsumer*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*,
                             CORBA::Environment&);
  // Used to inform the EC that a Consumer has connected or
  // disconnected from it.

  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);
  // Used to inform the EC that a Supplier has connected or
  // disconnected from it.

  virtual void shutdown (CORBA::Environment&);
  // The event channel is shutting down, inform all the consumers of
  // this

  // = The RtecEventChannelAdmin::SupplierAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (CORBA::Environment &);

  typedef ACE_Unbounded_Set<TAO_EC_ProxyPushConsumer*> ConsumerSet;
  typedef ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushConsumer*> ConsumerSetIterator;

  ConsumerSetIterator begin (void);
  ConsumerSetIterator end (void);
  // Iterators over the set of ProxyPushConsumers

private:
  TAO_EC_Event_Channel *event_channel_;
  // The Event Channel we belong to

  PortableServer::POA_var default_POA_;
  // Store the default POA.

  ACE_Lock* lock_;
  // The locking strategy

  ConsumerSet all_consumers_;
  // The set of consumers...
};

#if defined (__ACE_INLINE__)
#include "EC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_SUPPLIERADMIN_H */
