/* -*- C++ -*- */
/**
 * @file ECT_Throughput.h
 * @brief Driver for the collocated Event Channel throughput test.
 */

#ifndef ECT_THROUGHPUT_H
#define ECT_THROUGHPUT_H

#include "ECT_Driver.h"
#include "ECT_Consumer.h"
#include "ECT_Supplier.h"
#include "orbsvcs/Event_Service_Constants.h"
#include <array>
#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Runs a collocated event-channel throughput test.
class ECT_Throughput : public ECT_Driver
{
public:
  ECT_Throughput () = default;
  ECT_Throughput (ECT_Throughput const&) = delete;
  ECT_Throughput (ECT_Throughput&&) = delete;
  ECT_Throughput& operator= (ECT_Throughput const&) = delete;
  ECT_Throughput& operator= (ECT_Throughput&&) = delete;

  static constexpr int MAX_CONSUMERS {16};
  static constexpr int MAX_SUPPLIERS {16};

  /// Parse options, run the test, and report throughput statistics.
  int run (int argc, ACE_TCHAR* argv[]);

  /// Called when a consumer receives all supplier shutdown events.
  void shutdown_consumer () override;

private:
  /// Parse test-specific command-line options.
  int parse_args (int argc, ACE_TCHAR* argv[]);

  /// Connect and disconnect the configured consumers.
  void connect_consumers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_consumers ();

  /// Connect and disconnect the configured suppliers.
  void connect_suppliers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_suppliers ();

  /// Start the supplier tasks.
  void activate_suppliers ();

  /// Print per-supplier and aggregate throughput results.
  void dump_results ();

private:
  struct Options
  {
    int n_consumers {1};
    int n_suppliers {1};
    int burst_count {10};
    int burst_size {100};
    int event_size {128};
    int burst_pause {100};
    int consumer_type_start {ACE_ES_EVENT_UNDEFINED};
    int consumer_type_count {1};
    int consumer_type_shift {};
    int supplier_type_start {ACE_ES_EVENT_UNDEFINED};
    int supplier_type_count {1};
    int supplier_type_shift {};
    ACE_TCHAR const* pid_file_name {};
    int ec_concurrency_hwm {1};
    int thr_create_flags {THR_NEW_LWP | THR_BOUND | THR_SCHED_FIFO};
  } options_;

  /// Own consumers; disconnect() releases their POA references.
  std::array<std::unique_ptr<Test_Consumer>, MAX_CONSUMERS> consumers_ {};
  /// Own suppliers; disconnect() releases their POA references.
  std::array<std::unique_ptr<Test_Supplier>, MAX_SUPPLIERS> suppliers_ {};

  /// ORB used to shut down the collocated test.
  CORBA::ORB_var orb_ {};

  TAO_SYNCH_MUTEX lock_;
  int active_count_ {};
};

#endif /* ECT_TRHOUGHPUT_H */
