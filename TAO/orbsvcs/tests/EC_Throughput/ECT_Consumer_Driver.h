/* -*- C++ -*- */
/**
 * @file ECT_Consumer_Driver.h
 * @brief Driver for the remote Event Channel throughput consumer.
 */

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

/// Connects consumers to a remote event channel and reports throughput.
class ECT_Consumer_Driver : public ECT_Driver
{
public:
  ECT_Consumer_Driver () = default;

  ECT_Consumer_Driver (ECT_Consumer_Driver const&) = delete;
  ECT_Consumer_Driver (ECT_Consumer_Driver&&) = delete;
  ECT_Consumer_Driver& operator= (ECT_Consumer_Driver const&) = delete;
  ECT_Consumer_Driver& operator= (ECT_Consumer_Driver&&) = delete;

  static constexpr int MAX_CONSUMERS {16};

  /// Connect to the named services, receive events, and print results.
  int run (int argc, ACE_TCHAR* argv[]);

  /// Called after a consumer receives all supplier shutdown events.
  void shutdown_consumer () override;

private:
  /// Parse test-specific command-line options.
  int parse_args (int argc, ACE_TCHAR* argv[]);

  /// Connect and disconnect all configured consumers.
  void connect_consumers (RtecScheduler::Scheduler_ptr scheduler,
                          RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_consumers ();

  /// Print per-consumer and aggregate throughput results.
  void dump_results ();

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

  /// Own consumers; disconnect() releases their POA references.
  std::array<std::unique_ptr<Test_Consumer>, MAX_CONSUMERS> consumers_ {};

  /// ORB used by this driver.
  CORBA::ORB_var orb_ {};

  TAO_SYNCH_MUTEX lock_;
  int active_count_ {};
};

#endif /* ECT_CONSUMER_DRIVER_H */
