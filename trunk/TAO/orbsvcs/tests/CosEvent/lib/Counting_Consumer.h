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

  void connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                CORBA::Environment &ACE_TRY_ENV);
  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Simple connect/disconnect methods..

  void dump_results (int expected_count, int tolerance);
  // Print out an error message if the event count is too far from the
  // expected count.

  // = The CosEventComm::PushConsumer methods

  virtual void push (const CORBA::Any& events,
                     CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (CORBA::Environment &)
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

#endif /* ECT_CONSUMER_H */
