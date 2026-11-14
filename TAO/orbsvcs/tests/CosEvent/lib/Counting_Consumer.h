/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Counting_Consumer.h
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
#include <string>

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
  explicit CEC_Counting_Consumer (std::string const &name);
  CEC_Counting_Consumer (CEC_Counting_Consumer const &) = delete;
  CEC_Counting_Consumer (CEC_Counting_Consumer &&) = delete;
  CEC_Counting_Consumer &operator= (CEC_Counting_Consumer const &) = delete;
  CEC_Counting_Consumer &operator= (CEC_Counting_Consumer &&) = delete;

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);
  void disconnect ();

  /// Print out an error message if the event count is too far from the
  /// expected count.
  void dump_results (int expected_count, int tolerance);

  // = The CosEventComm::PushConsumer methods

  /// The skeleton methods.
  void push (CORBA::Any const &events) override;
  void disconnect_push_consumer () override;

  /// Keep track of the number of events received.
  CORBA::ULong event_count {};

  /// Keep track of the number of disconnect calls received.
  CORBA::ULong disconnect_count {};

protected:
  /// The proxy
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// The name
  std::string name_;
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
  explicit CEC_Pull_Counting_Consumer (std::string const &name);
  CEC_Pull_Counting_Consumer (CEC_Pull_Counting_Consumer const &) = delete;
  CEC_Pull_Counting_Consumer (CEC_Pull_Counting_Consumer &&) = delete;
  CEC_Pull_Counting_Consumer &operator= (CEC_Pull_Counting_Consumer const &) = delete;
  CEC_Pull_Counting_Consumer &operator= (CEC_Pull_Counting_Consumer &&) = delete;

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);
  void disconnect ();

  /// Print out an error message if the event count is too far from the
  /// expected count.
  void dump_results (int expected_count, int tolerance);

  CORBA::Any *pull ();
  CORBA::Any *try_pull (CORBA::Boolean_out has_event);

  // = The CosEventComm::PullConsumer methods
  /// The skeleton methods.
  void disconnect_pull_consumer () override;

  /// Keep track of the number of events received.
  CORBA::ULong event_count {};

  /// Keep track of the number of disconnect calls received.
  CORBA::ULong disconnect_count {};

protected:
  /// The proxy
  CosEventChannelAdmin::ProxyPullSupplier_var supplier_proxy_;

  /// The name
  std::string name_;
};

// ****************************************************************

class CEC_Test_Export CEC_Counting_Consumer_Task : public ACE_Task_Base
{
public:
  /// Create the task...
  CEC_Counting_Consumer_Task (CEC_Pull_Counting_Consumer *consumer,
                              int milliseconds = 0);
  CEC_Counting_Consumer_Task (CEC_Counting_Consumer_Task const &) = delete;
  CEC_Counting_Consumer_Task (CEC_Counting_Consumer_Task &&) = delete;
  CEC_Counting_Consumer_Task &operator= (CEC_Counting_Consumer_Task const &) = delete;
  CEC_Counting_Consumer_Task &operator= (CEC_Counting_Consumer_Task &&) = delete;

  // = Check the ACE_Task_Base documentation.
  int svc () override;

  void stop ();
  CORBA::ULong pull_count () const;

  /// Run a single iteration of the test
  void run ();

private:
  /// Borrowed consumer; it outlives this task and the task is joined first.
  CEC_Pull_Counting_Consumer * const consumer_;

  /// Set when the test must stop.
  bool stop_flag_ {};

  /// Count the number of pull() calls
  CORBA::ULong pull_count_ {};

  /// If nonzero, pause for <milliseconds> before each
  /// event.
  int const milliseconds_;

  mutable TAO_SYNCH_MUTEX lock_;
};

#endif /* ECT_CONSUMER_H */
