/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
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
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECT_Throughput (void);

  virtual ~ECT_Throughput (void);

  enum {
    MAX_CONSUMERS = 16,
    // Maximum number of consumers.
    MAX_SUPPLIERS = 16
    // Maximum number of suppliers.
  };

  int run (int argc, char* argv[]);
  // Execute the test.

  virtual void shutdown_consumer (void* consumer_cookie,
                                  CORBA::Environment&);
  // Callback method for consumers, each consumer will call this
  // method once it receives all the shutdown events from the
  // suppliers.

private:
  int parse_args (int argc, char* argv[]);
  // parse the command line args

  void connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                          CORBA::Environment &_env);
  void disconnect_consumers (CORBA::Environment &_env);
  // Connect and disconnect the consumers.

  void connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                          CORBA::Environment &_env);
  void disconnect_suppliers (CORBA::Environment &_env);
  // Connect the suppliers.

  void activate_suppliers (CORBA::Environment &_env);
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

  int event_a_;
  int event_b_;
  // We send two types of events, with different contents.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  CORBA::ORB_var orb_;
  // A reference to the ORB, to shut it down properly.

  ACE_SYNCH_MUTEX lock_;
  int active_count_;
  // How many consumers are still receiving events.

  int reactive_ec_;
  // If not zero then we use a remote EC.
};

#endif /* ECT_TRHOUGHPUT_H */
