/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Counting_Consumer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_COUNTING_CONSUMER_H
#define EC_COUNTING_CONSUMER_H

#include "ectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

class EC_Test_Export EC_Counting_Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object to implement EC tests.
  //
  // = DESCRIPTION
  //   This is a simple consumer that counts the events it receives.
  //
public:
  EC_Counting_Consumer (const char* name);
  // Constructor

  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                const RtecEventChannelAdmin::ConsumerQOS &qos
                TAO_ENV_ARG_DECL);
  void disconnect (TAO_ENV_SINGLE_ARG_DECL);
  void deactivate (TAO_ENV_SINGLE_ARG_DECL);
  // Simple connect/disconnect methods..

  void dump_results (int expected_count, int tolerance);
  // Print out an error message if the event count is too far from the
  // expected count.

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

  CORBA::ULong event_count;
  // Keep track of the number of events received.

  CORBA::ULong disconnect_count;
  // Keep track of the number of disconnect calls received.

protected:
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // The proxy

  const char* name_;
  // The name
};

#endif /* ECT_CONSUMER_H */
