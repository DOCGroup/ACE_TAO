/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel tests
//
// = FILENAME
//   Disconnect.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_DISCONNECT_H
#define EC_DISCONNECT_H

#include "orbsvcs/RtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //
public:
  Consumer (void);
  // Constructor

  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                const RtecEventChannelAdmin::ConsumerQOS &qos,
                CORBA::Environment &ACE_TRY_ENV);
  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Simple connect/disconnect methods..

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  CORBA::ULong disconnect_count;
  // Keep track of the number of disconnect calls received.

private:
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The proxy
};

class Supplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //
public:
  Supplier (void);
  // Constructor

  // = The RtecEventComm::PushSupplier methods

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                const RtecEventChannelAdmin::SupplierQOS &qos,
                CORBA::Environment &ACE_TRY_ENV);
  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Simple connect/disconnect methods..

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  CORBA::ULong disconnect_count;
  // Keep track of the number of disconnect calls received.

private:
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // Our proxy
};

#endif /* EC_DISCONNECT_H */
