/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel testsuite
//
// = FILENAME
//   Counting_Supplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef CEC_COUNTING_SUPPLIER_H
#define CEC_COUNTING_SUPPLIER_H

#include "cectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "ace/Task.h"

class CEC_Test_Export CEC_Counting_Supplier : public POA_CosEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //
public:
  CEC_Counting_Supplier (void);
  // Constructor

  // = The CosEventComm::PushSupplier methods

  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                CORBA::Environment &ACE_TRY_ENV);
  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Simple connect/disconnect methods..

  void push (const CORBA::Any& events,
             CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Push an event.

  virtual void disconnect_push_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  CORBA::ULong event_count;
  // Count the number of events sent

  CORBA::ULong disconnect_count;
  // Count the number of disconnect_push_supplier calls

private:
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // Our proxy
};

class CEC_Test_Export CEC_Counting_Supplier_Task : public ACE_Task_Base
{
public:
  CEC_Counting_Supplier_Task (CEC_Counting_Supplier *supplier,
                              int milliseconds = 0);
  // Create the task...

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void stop (void);
  CORBA::ULong push_count (void);

  void run (CORBA::Environment &ACE_TRY_ENV);
  // Run a single iteration of the test

private:
  CEC_Counting_Supplier *supplier_;
  // The supplier we are turning into an active object

  int stop_flag_;
  // Set to 1 when the test must stop

  CORBA::ULong push_count_;
  // Count the number of push() calls

  int milliseconds_;
  // If not zero then pause for <milliseconds> before sending each
  // event.

  ACE_SYNCH_MUTEX lock_;
};

// ****************************************************************

class CEC_Test_Export CEC_Pull_Counting_Supplier : public POA_CosEventComm::PullSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //
public:
  CEC_Pull_Counting_Supplier (void);
  // Constructor

  // = The CosEventComm::PullSupplier methods

  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                CORBA::Environment &ACE_TRY_ENV);
  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Simple connect/disconnect methods..

  // The PullSupplier methods.
  CORBA::Any* pull (CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException,CosEventComm::Disconnected));
  CORBA::Any* try_pull (CORBA::Boolean_out has_event,
                        CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException,CosEventComm::Disconnected));
  virtual void disconnect_pull_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong event_count;
  // Count the number of events sent

  CORBA::ULong disconnect_count;
  // Count the number of disconnect_pull_supplier calls

private:
  CosEventChannelAdmin::ProxyPullConsumer_var consumer_proxy_;
  // Our proxy
};

#endif /* ECT_SUPPLIER_H */
