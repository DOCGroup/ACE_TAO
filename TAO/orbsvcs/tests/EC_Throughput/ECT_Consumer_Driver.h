/* -*- C++ -*- */
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

#include "ECT_Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ECT_Consumer.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/Event_Service_Constants.h"
#include <array>
#include <memory>

class ECT_Consumer_Driver : public ECT_Driver
{
  //
  // = TITLE
  //
  // = DESCRIPTION
  //
public:
  ECT_Consumer_Driver () = default;
  ~ECT_Consumer_Driver () override = default;

  ECT_Consumer_Driver (ECT_Consumer_Driver const&) = delete;
  ECT_Consumer_Driver (ECT_Consumer_Driver&&) = delete;
  ECT_Consumer_Driver& operator= (ECT_Consumer_Driver const&) = delete;
  ECT_Consumer_Driver& operator= (ECT_Consumer_Driver&&) = delete;

  static constexpr int MAX_CONSUMERS {16};

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

  void dump_results ();
  // Print out the results

private:
  struct Options
  {
    int n_consumers {1};
    int n_suppliers {1};
    int type_start {ACE_ES_EVENT_UNDEFINED};
    int type_count {1};
    int stall_length {};
    bool shutdown_event_channel {true};
    ACE_TCHAR const* pid_file_name {};
  } options_;

  /// These pointers own consumers; POA references are released by disconnect().
  std::array<std::unique_ptr<Test_Consumer>, MAX_CONSUMERS> consumers_ {};

  CORBA::ORB_var orb_ {};
  // A reference to the ORB, to shut it down properly.

  TAO_SYNCH_MUTEX lock_;
  int active_count_ {};
  // How many consumers are still receiving events.
};

#endif /* ECT_CONSUMER_DRIVER_H */
