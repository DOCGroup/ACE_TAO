/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_ProxyPushConsumer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_CEC_PROXYPUSHCONSUMER_H
#define TAO_CEC_PROXYPUSHCONSUMER_H

#include "orbsvcs/CosEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Worker.h"

class TAO_CEC_EventChannel;
class TAO_CEC_Dispatching;
class TAO_CEC_ProxyPushSupplier;

class TAO_ORBSVCS_Export TAO_CEC_ProxyPushConsumer : public POA_CosEventChannelAdmin::ProxyPushConsumer
{
  // = TITLE
  //   ProxyPushConsumer
  //
  // = DESCRIPTION
  //   Implement the CosEventChannelAdmin::ProxyPushConsumer interface,
  //   remember that this class is used to communicate with a
  //   PushSupplier, so, in effect, this is the ambassador for a
  //   supplier inside the event channel.
  //
  // = MEMORY MANAGMENT
  //   The object commits suicide when disconnect_push_consumer() is
  //   called.
  //
public:
  TAO_CEC_ProxyPushConsumer (TAO_CEC_EventChannel* event_channel);
  // constructor...

  virtual ~TAO_CEC_ProxyPushConsumer (void);
  // destructor...

  CORBA::Boolean is_connected (void) const;
  // Return 0 if no supplier is connected...

  CosEventComm::PushSupplier_ptr supplier (void) const;
  // Return the consumer object reference. It returns nil() if it has
  // not connected yet.

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

  // = The CosEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                CosEventComm::PushSupplier_ptr push_supplier,
                CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected));
  virtual void push (const CORBA::Any& event,
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
  void supplier (CosEventComm::PushSupplier_ptr supplier);
  void supplier_i (CosEventComm::PushSupplier_ptr supplier);
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

  CosEventComm::PushSupplier_var supplier_;
  // The supplier....

  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

#if defined (__ACE_INLINE__)
#include "CEC_ProxyPushConsumer.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_PROXYPUSHCONSUMER_H */
