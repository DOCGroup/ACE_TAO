/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel testsuite
//
// = FILENAME
//   Counting_Consumer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef CEC_COUNTING_CONSUMER_H
#define CEC_COUNTING_CONSUMER_H

#include "cectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "ace/Task.h"

class CEC_Test_Export CEC_Counting_Consumer : public POA_CosEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object to implement EC tests.
  //
  // = DESCRIPTION
  //   This is a simple consumer that counts the events it receives.
  //
public:
  CEC_Counting_Consumer (const char* name);
  // Constructor

  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin
                ACE_ENV_ARG_DECL);
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Simple connect/disconnect methods..

  void dump_results (int expected_count, int tolerance);
  // Print out an error message if the event count is too far from the
  // expected count.

  // = The CosEventComm::PushConsumer methods

  virtual void push (const CORBA::Any& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  CORBA::ULong event_count;
  // Keep track of the number of events received.

  CORBA::ULong disconnect_count;
  // Keep track of the number of disconnect calls received.

protected:
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The proxy

  const char* name_;
  // The name
};

// ****************************************************************

class CEC_Test_Export CEC_Pull_Counting_Consumer : public POA_CosEventComm::PullConsumer
{
  // = TITLE
  //   Simple consumer object to implement EC tests.
  //
  // = DESCRIPTION
  //   This is a simple consumer that counts the events it receives.
  //
public:
  CEC_Pull_Counting_Consumer (const char* name);
  // Constructor

  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin
                ACE_ENV_ARG_DECL);
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Simple connect/disconnect methods..

  void dump_results (int expected_count, int tolerance);
  // Print out an error message if the event count is too far from the
  // expected count.

  CORBA::Any *pull (ACE_ENV_SINGLE_ARG_DECL);
  CORBA::Any *try_pull (CORBA::Boolean_out has_event
                        ACE_ENV_ARG_DECL);

  // = The CosEventComm::PullConsumer methods
  virtual void disconnect_pull_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  CORBA::ULong event_count;
  // Keep track of the number of events received.

  CORBA::ULong disconnect_count;
  // Keep track of the number of disconnect calls received.

protected:
  CosEventChannelAdmin::ProxyPullSupplier_var supplier_proxy_;
  // The proxy

  const char* name_;
  // The name
};

// ****************************************************************

class CEC_Test_Export CEC_Counting_Consumer_Task : public ACE_Task_Base
{
public:
  CEC_Counting_Consumer_Task (CEC_Pull_Counting_Consumer *consumer,
                              int milliseconds = 0);
  // Create the task...

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void stop (void);
  CORBA::ULong pull_count (void);

  void run (ACE_ENV_SINGLE_ARG_DECL);
  // Run a single iteration of the test

private:
  CEC_Pull_Counting_Consumer *consumer_;
  // The consumer we are turning into an active object

  int stop_flag_;
  // Set to 1 when the test must stop

  CORBA::ULong pull_count_;
  // Count the number of pull() calls

  int milliseconds_;
  // If not zero then pause for <milliseconds> before sending each
  // event.

  TAO_SYNCH_MUTEX lock_;
};

#endif /* ECT_CONSUMER_H */
