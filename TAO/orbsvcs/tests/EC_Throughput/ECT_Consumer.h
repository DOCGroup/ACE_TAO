/* -*- C++ -*- */
/**
 * @file ECT_Consumer.h
 * @brief Event Channel throughput test consumer.
 */

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

/// Receives a continuous range of event types and records throughput.
class Test_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Create a consumer associated with its coordinating driver.
  explicit Test_Consumer (ECT_Driver& driver,
                          int n_suppliers,
                          int stall_length = 0);

  Test_Consumer (Test_Consumer const&) = delete;
  Test_Consumer (Test_Consumer&&) = delete;
  Test_Consumer& operator= (Test_Consumer const&) = delete;
  Test_Consumer& operator= (Test_Consumer&&) = delete;

  /// Connect this consumer to the event channel.
  void connect (RtecScheduler::Scheduler_ptr scheduler,
                char const* name,
                int type_start,
                int type_count,
                RtecEventChannelAdmin::EventChannel_ptr ec);
  /// Disconnect this consumer from the event channel and deactivate it.
  void disconnect ();

  /// Print this consumer's throughput statistics.
  void dump_results (ACE_TCHAR const* name,
                     ACE_Basic_Stats::scale_factor_type global_scale_factor);

  /// Add this consumer's measurements to aggregate statistics.
  void accumulate (ACE_Throughput_Stats& stats) const;

  /// Process a batch of received events.
  void push (RtecEventComm::EventSet const& events) override;
  /// Handle disconnection by the event-channel proxy.
  void disconnect_push_consumer () override;

private:
  /// Driver notified after all suppliers send shutdown events.
  ECT_Driver& driver_;

  /// Number of suppliers whose shutdown events are expected.
  int n_suppliers_;

  /// Proxy used to receive events from the event channel.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  TAO_SYNCH_MUTEX lock_;
  /// Number of events received.
  int recv_count_ {};
  /// Time at which the first event arrived.
  ACE_hrtime_t first_event_ {};

  /// Accumulated throughput and latency measurements.
  ACE_Throughput_Stats throughput_;

  /// Number of shutdown events received so far.
  int shutdown_count_ {};

  /// Seconds to wait after receiving the first event.
  int stall_length_;
};

#endif /* ECT_CONSUMER_H */
