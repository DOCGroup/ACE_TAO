/* -*- C++ -*- */
/**
 * @file ECM_Consumer.h
 * @brief Consumer driver for the custom marshaling event-channel test.
 */

#ifndef ECM_CONSUMER_H
#define ECM_CONSUMER_H

#include <array>

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "tao/PortableServer/Servant_var.h"

class Driver;

/// Receives event payloads and forwards them to the test driver.
class Test_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  explicit Test_Consumer (Driver& driver);
  Test_Consumer (Test_Consumer const&) = delete;
  Test_Consumer (Test_Consumer&&) = delete;
  Test_Consumer& operator= (Test_Consumer const&) = delete;
  Test_Consumer& operator= (Test_Consumer&&) = delete;

  /// Connect this consumer to the event channel.
  void connect (int event_a,
                int event_b,
                RtecEventChannelAdmin::EventChannel_ptr ec);
  /// Disconnect the consumer from the event channel and deactivate it.
  void disconnect ();

  /// Forward received event batches to the driver.
  void push (RtecEventComm::EventSet const& events) override;
  /// Handle disconnection by the event-channel proxy.
  void disconnect_push_consumer () override;

private:
  Driver& driver_;
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
};

/// Coordinates consumers and validates received custom-marshaled data.
class Driver
{
public:
  Driver ();
  Driver (Driver const&) = delete;
  Driver (Driver&&) = delete;
  Driver& operator= (Driver const&) = delete;
  Driver& operator= (Driver&&) = delete;

  static constexpr int MAX_CONSUMERS = 16;

  /// Parse options and run the consumer test.
  int run (int argc, ACE_TCHAR* argv[]);

  /// Process an event batch received by a consumer.
  void push_consumer (RtecEventComm::EventSet const& events);

private:
  /// Parse consumer-specific command-line options.
  int parse_args (int argc, ACE_TCHAR* argv[]);
  /// Connect and disconnect all configured consumers.
  void connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_consumers ();

  std::array<PortableServer::Servant_var<Test_Consumer>, MAX_CONSUMERS> consumers_ {};
  int n_consumers_ {};
  int event_count_ {};
  int event_a_ {};
  int event_b_ {};
  ACE_TCHAR const* pid_file_name_ {};
  TAO_SYNCH_MUTEX recv_count_mutex_;
  int recv_count_ {};
};

#endif /* ECM_CONSUMER_H */
