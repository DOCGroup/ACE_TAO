/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_ProxyPullConsumer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_CEC_PROXYPULLCONSUMER_H
#include "ace/pre.h"
#define TAO_CEC_PROXYPULLCONSUMER_H

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Worker.h"
#include "event_export.h"

class TAO_CEC_EventChannel;
class TAO_CEC_Dispatching;
class TAO_CEC_ProxyPullSupplier;

class TAO_Event_Export TAO_CEC_ProxyPullConsumer : public POA_CosEventChannelAdmin::ProxyPullConsumer
{
  // = TITLE
  //   ProxyPullConsumer
  //
  // = DESCRIPTION
  //   Implement the CosEventChannelAdmin::ProxyPullConsumer interface,
  //   remember that this class is used to communicate with a
  //   PullSupplier, so, in effect, this is the ambassador for a
  //   supplier inside the event channel.
  //
  // = MEMORY MANAGMENT
  //   The object commits suicide when disconnect_pull_consumer() is
  //   called.
  //
public:
  TAO_CEC_ProxyPullConsumer (TAO_CEC_EventChannel* event_channel);
  // constructor...

  virtual ~TAO_CEC_ProxyPullConsumer (void);
  // destructor...

  CORBA::Boolean is_connected (void) const;
  // Return 0 if no supplier is connected...

  CosEventComm::PullSupplier_ptr supplier (void) const;
  // Return the consumer object reference. It returns nil() if it has
  // not connected yet.

  CORBA::Any* try_pull_from_supplier (CORBA::Boolean_out has_event,
                                      CORBA::Environment &env);
  CORBA::Any* pull_from_supplier (CORBA::Environment &env);
  // Pulls from the supplier, verifies that it is connected.

  CORBA::Boolean supplier_non_existent (CORBA::Boolean_out disconnected,
                                        CORBA::Environment &ACE_TRY_ENV);
  // Invoke the _non_existent() pseudo-operation on the supplier. If
  // it is disconnected then it returns true and sets the
  // <disconnected> flag.

  virtual void shutdown (CORBA::Environment&);
  // The event channel is shutting down

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count.

  // = The CosEventChannelAdmin::ProxyPullConsumer methods...
  virtual void connect_pull_supplier (
                CosEventComm::PullSupplier_ptr pull_supplier,
                CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected));
  virtual void disconnect_pull_consumer (CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // = The Servant methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);
  virtual void _add_ref (CORBA_Environment &ACE_TRY_ENV =
                             TAO_default_environment ());
  virtual void _remove_ref (CORBA_Environment &ACE_TRY_ENV =
                                TAO_default_environment ());

protected:
  void supplier (CosEventComm::PullSupplier_ptr supplier);
  void supplier_i (CosEventComm::PullSupplier_ptr supplier);
  // Set the supplier, used by some implementations to change the
  // policies used when invoking operations on the supplier.

  CORBA::Boolean is_connected_i (void) const;
  // The private version (without locking) of is_connected().

  void cleanup_i (void);
  // Release the supplier

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivate from the POA

private:
  TAO_CEC_EventChannel* event_channel_;
  // The supplier admin, used for activation and memory managment.

  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::ULong refcount_;
  // The reference count.

  CosEventComm::PullSupplier_var supplier_;
  // The supplier....

  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

#if defined (__ACE_INLINE__)
#include "CEC_ProxyPullConsumer.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_PROXYPULLCONSUMER_H */
