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
#include "ace/pre.h"

#include "orbsvcs/CosEventChannelAdminS.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"

class TAO_CEC_EventChannel;
class TAO_CEC_Dispatching;
class TAO_CEC_ProxyPushSupplier;

class TAO_Event_Export TAO_CEC_ProxyPushConsumer : public POA_CosEventChannelAdmin::ProxyPushConsumer
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
  typedef CosEventChannelAdmin::ProxyPushConsumer_ptr _ptr_type;
  typedef CosEventChannelAdmin::ProxyPushConsumer_var _var_type;

  TAO_CEC_ProxyPushConsumer (TAO_CEC_EventChannel* event_channel);
  // constructor...

  virtual ~TAO_CEC_ProxyPushConsumer (void);
  // destructor...

  virtual CosEventChannelAdmin::ProxyPushConsumer_ptr activate (CORBA::Environment &ACE_TRY_ENV) ACE_THROW_SPEC ((CORBA::SystemException));
  // Activate in the POA

  virtual void deactivate (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Deactivate from the POA

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

  friend class TAO_CEC_ProxyPushConsumer_Guard;
  // The guard needs access to the following protected methods.

  CORBA::Boolean is_connected_i (void) const;
  // The private version (without locking) of is_connected().

  void cleanup_i (void);
  // Release the supplier

private:
  TAO_CEC_EventChannel* event_channel_;
  // The supplier admin, used for activation and memory managment.

  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::ULong refcount_;
  // The reference count.

  CosEventComm::PushSupplier_var supplier_;
  // The supplier....

  int connected_;
  // If the flag is not zero then we are connected, notice that the
  // supplier can be nil.

  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

// ****************************************************************

class TAO_Event_Export TAO_CEC_ProxyPushConsumer_Guard
{
  // = TITLE
  //   A Guard for the ProxyPushConsumer reference count
  //
  // = DESCRIPTION
  //   This is a helper class used in the implementation of
  //   ProxyPushConumer.  It provides a Guard mechanism to increment
  //   the reference count on the proxy, eliminating the need to hold
  //   mutexes during long operations.
  //
public:
  TAO_CEC_ProxyPushConsumer_Guard (ACE_Lock *lock,
                                   CORBA::ULong &refcount,
                                   TAO_CEC_EventChannel *ec,
                                   TAO_CEC_ProxyPushConsumer *proxy);
  // Constructor

  ~TAO_CEC_ProxyPushConsumer_Guard (void);
  // Destructor

  int locked (void) const;
  // Returns 1 if the reference count successfully acquired

private:
  ACE_Lock *lock_;
  // The lock used to protect the reference count

  CORBA::ULong &refcount_;
  // The reference count

  TAO_CEC_EventChannel *event_channel_;
  // The event channel used to destroy the proxy

  TAO_CEC_ProxyPushConsumer *proxy_;
  // The proxy whose lifetime is controlled by the reference count

  int locked_;
  // This flag is set to 1 if the reference count was successfully
  // acquired.
};

#if defined (__ACE_INLINE__)
#include "CEC_ProxyPushConsumer.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_PROXYPUSHCONSUMER_H */
