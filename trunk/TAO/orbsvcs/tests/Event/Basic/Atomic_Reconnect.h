/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel tests
//
// = FILENAME
//   Atomic_Reconnect.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_ATOMIC_RECONNECT_H
#define EC_ATOMIC_RECONNECT_H

#include "orbsvcs/Channel_Clients.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "ace/Task.h"

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

  void dump_results (int expected_count,
                     int tolerance);
  // Print out an error message if the event count is too far from the
  // expected count.

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  CORBA::ULong event_count;
  // Keep track of the number of events received.

  CORBA::ULong event0_count;
  // Number of events of type "0" received.

private:
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The proxy

  const char* name_;
  // The name

  ACE_SYNCH_MUTEX lock_;
  // Synchronize access to the counter
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
                int published_source,
                int published_type,
                int event_source,
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
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // Our proxy

  int event_source_;
  // The event source

  int event_type_;
  // The event type
};

// ****************************************************************

class Task : public ACE_Task_Base
{
public:
  Task (Supplier *supplier,
        const RtecEventComm::EventSet &event);
  // Create the task...

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void stop (void);
  CORBA::ULong push_count (void);

  void run (CORBA::Environment &ACE_TRY_ENV);
  // Run a single iteration of the test

private:
  Supplier *supplier_;
  // The supplier we are turning into an active object

  RtecEventComm::EventSet event_;
  // The event we push through the supplier

  int stop_flag_;
  // Set to 1 when the test must stop

  CORBA::ULong push_count_;
  // Count the number of push() calls

  ACE_SYNCH_MUTEX lock_;
};

#endif /* EC_ATOMIC_RECONNECT_H */
