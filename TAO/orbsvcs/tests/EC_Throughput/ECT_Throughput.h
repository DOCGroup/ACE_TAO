/* -*- C++ -*- */
// $Id$
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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ECT_Throughput : public ECT_Driver
{
public:
  ECT_Throughput (void);

  virtual ~ECT_Throughput (void);

  enum {
    MAX_CONSUMERS = 16,
    // Maximum number of consumers.
    MAX_SUPPLIERS = 16
    // Maximum number of suppliers.
  };

  int run (int argc, ACE_TCHAR* argv[]);
  // Execute the test.

  virtual void shutdown_consumer (void* consumer_cookie);
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.

private:
  int parse_args (int argc, ACE_TCHAR* argv[]);
  // parse the command line args

  void connect_consumers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_consumers (void);
  // Connect and disconnect the consumers.

  void connect_suppliers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_suppliers (void);
  // Connect the suppliers.

  void activate_suppliers (void);
  // Activate the suppliers, i.e. they start generating events.

  void dump_results (void);
  // Dump the results for each supplier.

private:
  Test_Consumer* consumers_[ECT_Throughput::MAX_CONSUMERS];
  // The consumer array.

  int n_consumers_;
  // The number of consumers.

  Test_Supplier* suppliers_[ECT_Throughput::MAX_SUPPLIERS];
  // The suppliers array.

  int n_suppliers_;
  // The number of suppliers.

  int burst_count_;
  // How many bursts we will send from each supplier.

  int burst_size_;
  // The number of events

  int event_size_;
  // The size of the payload on each event.

  int burst_pause_;
  // The time between each event burst, in microseconds.

  int consumer_type_start_;
  int consumer_type_count_;
  int consumer_type_shift_;
  // The consumers subscribe to different sets of events, as follows:
  // Consumer0: [start          , start           + count)
  // Consumer1: [start + 1*shift, start + 1*shift + count)
  // Consumer2: [start + 2*shift, start + 2*shift + count)
  // And so on.

  int supplier_type_start_;
  int supplier_type_count_;
  int supplier_type_shift_;
  // The suppliers generate different sets of events, as follows:
  // Supplier0: [start          , start           + count)
  // Supplier1: [start + 1*shift, start + 1*shift + count)
  // Supplier2: [start + 2*shift, start + 2*shift + count)
  // And so on.

  const ACE_TCHAR* pid_file_name_;
  // The name of a file where the process stores its pid

  CORBA::ORB_var orb_;
  // A reference to the ORB, to shut it down properly.

  TAO_SYNCH_MUTEX lock_;
  int active_count_;
  // How many consumers are still receiving events.

  int ec_concurrency_hwm_;
  // Set the HWM for the concurrency in the EC.

  int thr_create_flags_;
  // The thread creation flags
};

#endif /* ECT_TRHOUGHPUT_H */
