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

#ifndef TAO_EC_CONSUMERADMIN_H
#define TAO_EC_CONSUMERADMIN_H

#include "ace/Containers.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_ProxyPushConsumer;

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
  //   It does not assume ownership of the TAO_EC_Event_Channel object
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel* event_channel);
  // constructor...

  virtual ~TAO_EC_ConsumerAdmin (void);
  // destructor...

  typedef ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier*> SupplierSet;
  typedef ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier*> SupplierSetIterator;

  SupplierSetIterator begin (void);
  SupplierSetIterator end (void);
  // Iterators over the set of ProxyPushSuppliers

  void set_default_POA (PortableServer::POA_ptr poa);
  // Set this servant's default POA

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

  // = The RtecEventChannelAdmin::ConsumerAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (CORBA::Environment &);

private:
  TAO_EC_Event_Channel *event_channel_;
  // The Event Channel we belong to

  PortableServer::POA_var default_POA_;
  // Store the default POA.

  SupplierSet all_suppliers_;
};

#if defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_CONSUMERADMIN_H */
