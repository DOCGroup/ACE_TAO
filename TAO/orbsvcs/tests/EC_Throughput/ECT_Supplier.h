/* -*- C++ -*- */
/**
 * @file ECT_Supplier.h
 * @brief Event Channel throughput test supplier task.
 */

#ifndef ECT_SUPPLIER_H
#define ECT_SUPPLIER_H

#include "ECT_Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "ace/Task.h"

/// Generates event bursts and records supplier-side throughput.
class Test_Supplier : public ACE_Task<ACE_SYNCH>
{
public:
  /// Create a supplier task with default event types.
  Test_Supplier ();

  Test_Supplier (Test_Supplier const&) = delete;
  Test_Supplier (Test_Supplier&&) = delete;
  Test_Supplier& operator= (Test_Supplier const&) = delete;
  Test_Supplier& operator= (Test_Supplier&&) = delete;

  /// Run the supplier task and send configured event bursts.
  int svc () override;

  /// Connect this supplier to the event channel and configure its QoS.
  void connect (RtecScheduler::Scheduler_ptr scheduler,
                char const* name,
                int burst_count,
                int burst_size,
                int event_size,
                int burst_pause,
                int type_start,
                int type_count,
                RtecEventChannelAdmin::EventChannel_ptr ec);
  /// Disconnect from the event channel and deactivate the servant.
  void disconnect ();

  /// Handle disconnection forwarded by the supplier adapter.
  void disconnect_push_supplier ();

  /// Return the event source ID assigned to this supplier.
  RtecEventComm::EventSourceID supplier_id () const;

  /// Return the proxy used to send events to the channel.
  RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy ();

  /// Print this supplier's throughput statistics.
  void dump_results (ACE_TCHAR const* name,
                     ACE_Basic_Stats::scale_factor_type global_scale_factor);

  /// Add this supplier's measurements to aggregate statistics.
  void accumulate (ACE_Throughput_Stats& stats) const;

private:
  /// Event source ID generated from the supplier name.
  RtecEventComm::EventSourceID supplier_id_ {};

  /// Proxy used to send events to the event channel.
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  /// Adapter that forwards supplier disconnection callbacks.
  ACE_PushSupplier_Adapter<Test_Supplier> supplier_;

  /// Accumulated supplier-side throughput measurements.
  ACE_Throughput_Stats throughput_;

  /// Number of bursts to send.
  int burst_count_ {};
  /// Events per burst.
  int burst_size_ {};
  /// Payload size in bytes for each event.
  int event_size_ {};
  /// Pause between bursts in microseconds.
  int burst_pause_ {};
  /// First event type to publish.
  int type_start_ {};
  /// Number of consecutive event types to publish.
  int type_count_ {};
};

#endif /* ECT_SUPPLIER_H */
