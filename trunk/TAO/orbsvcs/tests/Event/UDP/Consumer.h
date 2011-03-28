/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Consumer
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Simple consumer object
 *
 * This class is a consumer of events.
 * It simply subscribes to one event type.
 */
class Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  Consumer (bool valuetype);

  /// Connect to the Event Channel
  void connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from the event channel
  void disconnect (void);

  // = The RtecEventComm::PushConsumer methods

  /// The skeleton methods.
  virtual void push (const RtecEventComm::EventSet& events);
  virtual void disconnect_push_consumer (void);

  CORBA::ULong event_count (void) const;
private:
  /// Keep track of the number of events received.
  CORBA::ULong event_count_;

  /// The proxy
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_;

  bool valuetype_;
};

#endif /* CONSUMER_H */
