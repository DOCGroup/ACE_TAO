/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Counting_Supplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_COUNTING_SUPPLIER_H
#define EC_COUNTING_SUPPLIER_H

#include "ectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Task.h"

class EC_Test_Export EC_Counting_Supplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //
public:
  EC_Counting_Supplier (void);
  // Constructor

  // = The RtecEventComm::PushSupplier methods

  void activate (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                 int period,
                 CORBA::Environment &ACE_TRY_ENV);
  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Connect as a consumer to receive a TIMEOUT every <period>
  // milliseconds.
  // The class pushes an event (in its supplier role) every time it
  // receives the timeout.

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                int published_source,
                int published_type,
                int event_source,
                int event_type,
                CORBA::Environment &ACE_TRY_ENV);
  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                const RtecEventChannelAdmin::SupplierQOS &qos,
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

  CORBA::ULong event_count;
  // Count the number of events sent

  CORBA::ULong disconnect_count;
  // Count the number of disconnect_push_supplier calls

private:
  ACE_PushConsumer_Adapter<EC_Counting_Supplier> consumer_adapter_;
  // Adapter...

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The adapter proxy...

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // Our proxy

  int event_source_;
  // The event source

  int event_type_;
  // The event type
};

class EC_Test_Export EC_Counting_Supplier_Task : public ACE_Task_Base
{
public:
  EC_Counting_Supplier_Task (EC_Counting_Supplier *supplier);
  // Create the task...

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void stop (void);
  CORBA::ULong push_count (void);

  void run (CORBA::Environment &ACE_TRY_ENV);
  // Run a single iteration of the test

private:
  EC_Counting_Supplier *supplier_;
  // The supplier we are turning into an active object

  RtecEventComm::EventSet event_;
  // The event we push through the supplier

  int stop_flag_;
  // Set to 1 when the test must stop

  CORBA::ULong push_count_;
  // Count the number of push() calls

  TAO_SYNCH_MUTEX lock_;
};

#endif /* ECT_SUPPLIER_H */
