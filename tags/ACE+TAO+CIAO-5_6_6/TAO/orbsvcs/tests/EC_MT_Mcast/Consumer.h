// $Id$
// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of events.
  //   It simply subscribes to one event type.
  //
public:
  Consumer (void);
  // Constructor

  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);
  // Connect to the Event Channel

  void disconnect (void);
  // Disconnect from the event channel

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);
  // The skeleton methods.

private:
  CORBA::ULong event_count_;
  // Keep track of the number of events received.

  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_;
  // The proxy
};

#endif /* CONSUMER_H */
