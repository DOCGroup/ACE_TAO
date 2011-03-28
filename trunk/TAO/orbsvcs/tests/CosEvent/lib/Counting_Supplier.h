/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Counting_Supplier
 *
 *  $Id$
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
  /// Constructor
  CEC_Counting_Supplier (void);

  // = The CosEventComm::PushSupplier methods

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin);
  void disconnect (void);

  /// Push an event.
  void push (const CORBA::Any& events);

  /// The skeleton methods.
  virtual void disconnect_push_supplier (void);

  /// Count the number of events sent
  CORBA::ULong event_count;

  /// Count the number of disconnect_push_supplier calls
  CORBA::ULong disconnect_count;

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

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void stop (void);
  CORBA::ULong push_count (void);

  /// Run a single iteration of the test
  void run (void);

private:
  /// The supplier we are turning into an active object
  CEC_Counting_Supplier *supplier_;

  /// Set to 1 when the test must stop
  int stop_flag_;

  /// Count the number of push() calls
  CORBA::ULong push_count_;

  /// If not zero then pause for <milliseconds> before sending each
  /// event.
  int milliseconds_;

  TAO_SYNCH_MUTEX lock_;
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
  /// Constructor
  CEC_Pull_Counting_Supplier (void);

  // = The CosEventComm::PullSupplier methods

  /// Simple connect/disconnect methods..
  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin);
  void disconnect (void);

  // The PullSupplier methods.
  CORBA::Any* pull (void);
  CORBA::Any* try_pull (CORBA::Boolean_out has_event);
  virtual void disconnect_pull_supplier (void);

  /// Count the number of events sent
  CORBA::ULong event_count;

  /// Count the number of disconnect_pull_supplier calls
  CORBA::ULong disconnect_count;

private:
  /// Our proxy
  CosEventChannelAdmin::ProxyPullConsumer_var consumer_proxy_;
};

#endif /* ECT_SUPPLIER_H */
