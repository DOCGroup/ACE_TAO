/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Supplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_TIMEOUT_H
#define EC_TIMEOUT_H

#include "orbsvcs/Channel_Clients.h"
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
  Consumer (const char* name);
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

  CORBA::ULong event_count;
  // Keep track of the number of events received.

private:
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The proxy

  const char* name_;
  // The name
};

class Supplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //   It simply register for two event typesone event type
  //   The class is just a helper to simplify common tasks in EC
  //   tests, such as subscribing for a range of events, disconnecting
  //   from the EC, informing the driver of shutdown messages, etc.
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
public:
  Supplier (void);
  // Constructor

  // = The RtecEventComm::PushSupplier methods

  void connect (RtecEventChannelAdmin::EventChannel_ptr ec,
                int milliseconds,
                int event_type,
                CORBA::Environment &ACE_TRY_ENV);
  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Simple connect/disconnect methods..

  void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The Consumer side methods.

  virtual void disconnect_push_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  ACE_PushConsumer_Adapter<Supplier> consumer_adapter_;
  // Adapter...

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The adapter proxy...

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // Our proxy

  int event_type_;
  // The event type
};

#endif /* EC_TIMEOUT_H */
