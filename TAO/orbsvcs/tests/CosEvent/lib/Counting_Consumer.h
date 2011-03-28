/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Counting_Consumer
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef CEC_COUNTING_CONSUMER_H
#define CEC_COUNTING_CONSUMER_H

#include "cectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "ace/Task.h"

/**
 * @class CEC_Counting_Consumer
 *
 * @brief Simple consumer object to implement EC tests.
 *
 * This is a simple consumer that counts the events it receives.
 */
class CEC_Test_Export CEC_Counting_Consumer : public POA_CosEventComm::PushConsumer
{
public:
  /// Constructor
  CEC_Counting_Consumer (const char* name);

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);
  void disconnect (void);

  /// Print out an error message if the event count is too far from the
  /// expected count.
  void dump_results (int expected_count, int tolerance);

  // = The CosEventComm::PushConsumer methods

  /// The skeleton methods.
  virtual void push (const CORBA::Any& events);
  virtual void disconnect_push_consumer (void);

  /// Keep track of the number of events received.
  CORBA::ULong event_count;

  /// Keep track of the number of disconnect calls received.
  CORBA::ULong disconnect_count;

protected:
  /// The proxy
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// The name
  const char* name_;
};

// ****************************************************************

/**
 * @class CEC_Pull_Counting_Consumer
 *
 * @brief Simple consumer object to implement EC tests.
 *
 * This is a simple consumer that counts the events it receives.
 */
class CEC_Test_Export CEC_Pull_Counting_Consumer : public POA_CosEventComm::PullConsumer
{
public:
  /// Constructor
  CEC_Pull_Counting_Consumer (const char* name);

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);
  void disconnect (void);

  /// Print out an error message if the event count is too far from the
  /// expected count.
  void dump_results (int expected_count, int tolerance);

  CORBA::Any *pull (void);
  CORBA::Any *try_pull (CORBA::Boolean_out has_event);

  // = The CosEventComm::PullConsumer methods
  /// The skeleton methods.
  virtual void disconnect_pull_consumer (void);

  /// Keep track of the number of events received.
  CORBA::ULong event_count;

  /// Keep track of the number of disconnect calls received.
  CORBA::ULong disconnect_count;

protected:
  /// The proxy
  CosEventChannelAdmin::ProxyPullSupplier_var supplier_proxy_;

  /// The name
  const char* name_;
};

// ****************************************************************

class CEC_Test_Export CEC_Counting_Consumer_Task : public ACE_Task_Base
{
public:
  /// Create the task...
  CEC_Counting_Consumer_Task (CEC_Pull_Counting_Consumer *consumer,
                              int milliseconds = 0);

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void stop (void);
  CORBA::ULong pull_count (void);

  /// Run a single iteration of the test
  void run (void);

private:
  /// The consumer we are turning into an active object
  CEC_Pull_Counting_Consumer *consumer_;

  /// Set to 1 when the test must stop
  int stop_flag_;

  /// Count the number of pull() calls
  CORBA::ULong pull_count_;

  /// If not zero then pause for <milliseconds> before sending each
  /// event.
  int milliseconds_;

  TAO_SYNCH_MUTEX lock_;
};

#endif /* ECT_CONSUMER_H */
