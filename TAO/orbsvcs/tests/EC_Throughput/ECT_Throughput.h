/* -*- C++ -*- */
//
// ============================================================================
//
// = DESCRIPTION
//   This is a helper class for the throughput tests of the Event
//   Channel.
//   Used for the collocated test.
//
// ============================================================================

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

class ECT_Throughput : public ECT_Driver
{
public:
  ECT_Throughput () = default;
  ~ECT_Throughput () override = default;

  ECT_Throughput (ECT_Throughput const&) = delete;
  ECT_Throughput (ECT_Throughput&&) = delete;
  ECT_Throughput& operator= (ECT_Throughput const&) = delete;
  ECT_Throughput& operator= (ECT_Throughput&&) = delete;

  static constexpr int MAX_CONSUMERS {16};
  static constexpr int MAX_SUPPLIERS {16};

  int run (int argc, ACE_TCHAR* argv[]);
  // Execute the test.

  void shutdown_consumer () override;
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.

private:
  int parse_args (int argc, ACE_TCHAR* argv[]);
  // parse the command line args

  void connect_consumers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_consumers ();
  // Connect and disconnect the consumers.

  void connect_suppliers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_suppliers ();
  // Connect the suppliers.

  void activate_suppliers ();
  // Activate the suppliers, i.e. they start generating events.

  void dump_results ();
  // Dump the results for each supplier.

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

  /// These pointers own consumers; POA references are released by disconnect().
  std::array<std::unique_ptr<Test_Consumer>, MAX_CONSUMERS> consumers_ {};
  /// These pointers own suppliers; POA references are released by disconnect().
  std::array<std::unique_ptr<Test_Supplier>, MAX_SUPPLIERS> suppliers_ {};

  CORBA::ORB_var orb_ {};
  // A reference to the ORB, to shut it down properly.

  TAO_SYNCH_MUTEX lock_;
  int active_count_ {};
  // How many consumers are still receiving events.
};

#endif /* ECT_TRHOUGHPUT_H */
