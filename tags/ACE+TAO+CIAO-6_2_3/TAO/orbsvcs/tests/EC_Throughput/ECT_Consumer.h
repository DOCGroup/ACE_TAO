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
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Task.h"
#include "ace/OS_NS_time.h"
#include "ace/Throughput_Stats.h"

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
                 int n_suppliers,
                 int stall_length = 0);

  void connect (RtecScheduler::Scheduler_ptr scheduler,
                const char* name,
                int type_start,
                int type_count,
                RtecEventChannelAdmin::EventChannel_ptr ec);
  // This method connects the consumer to the EC.

  void disconnect (void);
  // Disconnect from the EC.

  void dump_results (const ACE_TCHAR* name,
                     ACE_Basic_Stats::scale_factor_type global_scale_factor);
  // Print out the results

  void accumulate (ACE_Throughput_Stats& stats) const;
  // Add our throughput and latency statistics to <stats>

  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);
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

  TAO_SYNCH_MUTEX lock_;
  int recv_count_;
  ACE_hrtime_t first_event_;
  // How many events we have received.

  ACE_Throughput_Stats throughput_;
  // Used for reporting stats.

  int shutdown_count_;
  // How many shutdown events we have received.

  int stall_length_;
  // How long (in seconds) we wait to continue processing
  // after receiving the first push.
};

#endif /* ECT_CONSUMER_H */
