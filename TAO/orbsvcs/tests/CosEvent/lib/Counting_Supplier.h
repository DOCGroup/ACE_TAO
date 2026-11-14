/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Counting_Supplier.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef CEC_COUNTING_SUPPLIER_H
#define CEC_COUNTING_SUPPLIER_H

#include "cectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "ace/Task.h"

/**
 * @class CEC_Counting_Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 */
class CEC_Test_Export CEC_Counting_Supplier : public POA_CosEventComm::PushSupplier
{
public:
  CEC_Counting_Supplier () = default;
  CEC_Counting_Supplier (CEC_Counting_Supplier const &) = delete;
  CEC_Counting_Supplier (CEC_Counting_Supplier &&) = delete;
  CEC_Counting_Supplier &operator= (CEC_Counting_Supplier const &) = delete;
  CEC_Counting_Supplier &operator= (CEC_Counting_Supplier &&) = delete;

  // = The CosEventComm::PushSupplier methods

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin);
  void disconnect ();

  /// Push an event.
  void push (CORBA::Any const &events);

  /// The skeleton methods.
  void disconnect_push_supplier () override;

  /// Count the number of events sent
  CORBA::ULong event_count {};

  /// Count the number of disconnect_push_supplier calls
  CORBA::ULong disconnect_count {};

private:
  /// Our proxy
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
};

class CEC_Test_Export CEC_Counting_Supplier_Task : public ACE_Task_Base
{
public:
  /// Create the task...
  CEC_Counting_Supplier_Task (CEC_Counting_Supplier *supplier,
                              int milliseconds = 0);
  CEC_Counting_Supplier_Task (CEC_Counting_Supplier_Task const &) = delete;
  CEC_Counting_Supplier_Task (CEC_Counting_Supplier_Task &&) = delete;
  CEC_Counting_Supplier_Task &operator= (CEC_Counting_Supplier_Task const &) = delete;
  CEC_Counting_Supplier_Task &operator= (CEC_Counting_Supplier_Task &&) = delete;

  // = Check the ACE_Task_Base documentation.
  int svc () override;

  void stop ();
  CORBA::ULong push_count () const;

  /// Run a single iteration of the test
  void run ();

private:
  /// Borrowed supplier; it outlives this task and the task is joined first.
  CEC_Counting_Supplier * const supplier_;

  /// Set when the test must stop.
  bool stop_flag_ {};

  /// Count the number of push() calls
  CORBA::ULong push_count_ {};

  /// If nonzero, pause for <milliseconds> before each
  /// event.
  int const milliseconds_;

  mutable TAO_SYNCH_MUTEX lock_;
};

// ****************************************************************

/**
 * @class CEC_Pull_Counting_Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 */
class CEC_Test_Export CEC_Pull_Counting_Supplier : public POA_CosEventComm::PullSupplier
{
public:
  CEC_Pull_Counting_Supplier () = default;
  CEC_Pull_Counting_Supplier (CEC_Pull_Counting_Supplier const &) = delete;
  CEC_Pull_Counting_Supplier (CEC_Pull_Counting_Supplier &&) = delete;
  CEC_Pull_Counting_Supplier &operator= (CEC_Pull_Counting_Supplier const &) = delete;
  CEC_Pull_Counting_Supplier &operator= (CEC_Pull_Counting_Supplier &&) = delete;

  // = The CosEventComm::PullSupplier methods

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin);
  void disconnect ();

  // The PullSupplier methods.
  CORBA::Any* pull () override;
  CORBA::Any* try_pull (CORBA::Boolean_out has_event) override;
  void disconnect_pull_supplier () override;

  /// Count the number of events sent
  CORBA::ULong event_count {};

  /// Count the number of disconnect_pull_supplier calls
  CORBA::ULong disconnect_count {};

private:
  /// Our proxy
  CosEventChannelAdmin::ProxyPullConsumer_var consumer_proxy_;
};

#endif /* ECT_SUPPLIER_H */
