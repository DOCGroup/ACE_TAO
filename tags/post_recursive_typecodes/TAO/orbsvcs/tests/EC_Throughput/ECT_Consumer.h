/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   This test to measure how many events per minute can the EC
//   process, it also serves as an example how how to encode complex
//   data types in a octet sequence.
//
// ============================================================================

#ifndef ECT_CONSUMER_H
#define ECT_CONSUMER_H

#include "ECT_Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"
#include "ace/Task.h"

class Test_Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Receive the events.
  //
  // = DESCRIPTION
  //   This class is a consumer of events. It subscribes for a
  //   continous ranges of event types, this permits studying the
  //   effect of the number of subscriptions for each particular kind
  //   of event on the EC.
  //
public:
  Test_Consumer (ECT_Driver* driver,
                 void* cookie,
                 int n_suppliers);

  void connect (RtecScheduler::Scheduler_ptr scheduler,
                const char* name,
                int type_start,
                int type_count,
                RtecEventChannelAdmin::EventChannel_ptr ec,
                CORBA::Environment& _env);
  // This method connects the consumer to the EC.

  void disconnect (CORBA::Environment &_env);
  // Disconnect from the EC.

  void dump_results (const char* name);
  // Print out the results

  void accumulate (ECT_Driver::Throughput_Stats& stats) const;
  // Add our throughput statistics to <stats>

  void accumulate (ECT_Driver::Latency_Stats& stats) const;
  // Add our latency statistics to <stats>

  virtual void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env);
  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The skeleton methods.

private:
  ECT_Driver* driver_;
  // The main driver for the test.

  void* cookie_;
  // A magic cookie passed by the driver that we pass back in our
  // callbacks.

  int n_suppliers_;
  // The number of suppliers that are feeding this consumer, we
  // terminate once we receive a shutdown event from each supplier.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  ACE_SYNCH_MUTEX lock_;
  int recv_count_;
  // How many events we have received.

  ECT_Driver::Throughput_Stats throughput_;
  // Used for reporting stats.

  ECT_Driver::Latency_Stats latency_;
  // Used for reporting stats.

  int shutdown_count_;
  // How many shutdown events we have received.
};

#endif /* ECT_CONSUMER_H */
