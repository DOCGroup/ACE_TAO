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

#ifndef ECT_CONSUMER_DRIVER_H
#define ECT_CONSUMER_DRIVER_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "ECT_Driver.h"
#include "ECT_Consumer.h"

class ECT_Consumer_Driver : public ECT_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECT_Consumer_Driver (void);
  virtual ~ECT_Consumer_Driver (void);

  enum {
    MAX_CONSUMERS = 16
    // Maximum number of consumers.
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

  void connect_consumers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec,
                          CORBA::Environment &_env);
  void disconnect_consumers (CORBA::Environment &_env);
  // Connect and disconnect the consumers.

  void dump_results (void);
  // Print out the results

private:
  Test_Consumer* consumers_[ECT_Consumer_Driver::MAX_CONSUMERS];
  // The consumer array.

  int n_consumers_;
  // The number of consumers.

  int n_suppliers_;
  // How many suppliers are sending events, used for shutdown, each
  // supplier sends a shutdown message after it finishes, the consumer
  // finishes when all the suppliers do.

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
};

#endif /* ECT_CONSUMER_DRIVER_H */
