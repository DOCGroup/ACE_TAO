/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel tests
//
// = FILENAME
//   MT_Disconnect.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_MT_DISCONNECT_H
#define EC_MT_DISCONNECT_H

#include "ace/Task.h"
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

// ****************************************************************

class Task : public ACE_Task_Base
{
public:
  Task (RtecEventChannelAdmin::EventChannel_ptr ec,
        int use_callbacks);
  // Create the task...

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void run_iteration (CORBA::Environment &ACE_TRY_ENV);
  // Run a single iteration of the test

private:
  RtecEventChannelAdmin::EventChannel_var event_channel;
  // The event channel used on the test

  int use_callbacks;
  // Does the event channel send any callback messages when a client
  // diconnects
};

#endif /* EC_DISCONNECT_H */
