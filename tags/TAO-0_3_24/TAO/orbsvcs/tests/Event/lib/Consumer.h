/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Consumer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_CONSUMER_H
#define EC_CONSUMER_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

class EC_Test_Export EC_Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object to implement EC tests.
  //
  // = DESCRIPTION
  //   This class is a consumer of events.
  //   The class is just a helper to simplify common tasks in EC
  //   tests, such as subscribing for a range of events, disconnecting
  //   from the EC, informing the driver of shutdown messages, etc.
  //
  //   There are several ways to connect and disconnect this class,
  //   and it is up to the driver program to use the right one.
  //
public:
  EC_Consumer (EC_Driver* driver, void* cookie);
  // Constructor

  virtual void connect (
      RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type,
      CORBA::Environment &ACE_TRY_ENV);
  // The driver program can build the QoS attributes and obtain the
  // ConsumerAdmin, we do the rest.

  virtual void connect (
      const RtecEventChannelAdmin::ConsumerQOS& qos,
      int shutdown_event_type,
      CORBA::Environment &ACE_TRY_ENV);
  // The driver program can build the QoS attributes and we use
  // whatevet supplier_proxy we already have (useful for reconnection
  // tests).

  virtual int connected (void) const;
  // returns 0 if it is not connected

  void disconnect (CORBA::Environment& ACE_TRY_ENV);
  // The application can invoke this method to disconnect from the EC
  // and deactivate this class.

  void accumulate (EC_Driver::Throughput_Stats& throughput,
                   EC_Driver::Latency_Stats& latency) const;
  // Accumulate our statistics to the totals.

  virtual void dump_results (const char* name);
  // Printout the statistics

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env);
  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The skeleton methods.

private:
  EC_Driver* driver_;
  // The main driver for the test.

  void* cookie_;
  // A magic cookie passed by the driver that we pass back in our
  // callbacks.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  ACE_SYNCH_MUTEX lock_;
  // Protect internal state

  EC_Driver::Throughput_Stats throughput_;
  // Used for reporting stats

  EC_Driver::Latency_Stats latency_;
  // Used for reporting stats

  int push_count_;
  // The number of push() calls

  int shutdown_event_type_;
  // The type used to indicate shutdown
};

#endif /* ECT_CONSUMER_H */
