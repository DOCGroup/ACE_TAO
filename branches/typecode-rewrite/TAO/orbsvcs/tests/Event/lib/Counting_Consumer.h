/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Counting_Consumer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_COUNTING_CONSUMER_H
#define EC_COUNTING_CONSUMER_H

#include "ectest_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

/**
 * @class EC_Counting_Consumer
 *
 * @brief Simple consumer object to implement EC tests.
 *
 * This is a simple consumer that counts the events it receives.
 */
class EC_Test_Export EC_Counting_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  EC_Counting_Consumer (const char* name);

  /// Simple connect/disconnect methods..
  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                const RtecEventChannelAdmin::ConsumerQOS &qos
                ACE_ENV_ARG_DECL);
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  void deactivate (ACE_ENV_SINGLE_ARG_DECL);

  /// Print out an error message if the event count is too far from the
  /// expected count.
  void dump_results (int expected_count, int tolerance);

  // = The RtecEventComm::PushConsumer methods

  /// The skeleton methods.
  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Keep track of the number of events received.
  CORBA::ULong event_count;

  /// Keep track of the number of disconnect calls received.
  CORBA::ULong disconnect_count;

protected:
  /// The proxy
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// The name
  const char* name_;
};

#endif /* ECT_CONSUMER_H */
