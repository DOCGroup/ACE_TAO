// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#include <atomic>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Simple consumer object that subscribes to one event type.
class Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  Consumer () = default;
  Consumer (Consumer const&) = delete;
  Consumer (Consumer&&) = delete;
  Consumer& operator= (Consumer const&) = delete;
  Consumer& operator= (Consumer&&) = delete;

  /// Connects to the Event Channel.
  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnects from the Event Channel.
  void disconnect ();

  /// Receives events and disconnect notifications from the Event Channel.
  void push (RtecEventComm::EventSet const& events) override;
  void disconnect_push_consumer () override;

private:
  /// Number of events received.
  std::atomic<CORBA::ULong> event_count_ {};

  /// Proxy used to communicate with the Event Channel.
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_;
};

#endif /* CONSUMER_H */
