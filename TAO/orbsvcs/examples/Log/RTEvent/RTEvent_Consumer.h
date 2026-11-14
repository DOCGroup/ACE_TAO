/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   RTEvent_Consumer.h
 *
 *  An example of using the RTEvent_Logging_Service.
 *  The RTEvent_Consumer consumes log-generated events.
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef RTEVENT_CONSUMER_H
#define RTEVENT_CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RTEventLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief Simple consumer of events.
 *
 * Registers for one event type.
 */
class Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  Consumer () = default;

  Consumer (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer &operator= (Consumer &&) = delete;

  /// Run the test.
  int run (int argc, ACE_TCHAR *argv[]);

  /// The skeleton methods.
  void push (RtecEventComm::EventSet const &events) override;
  void disconnect_push_consumer () override;

private:
  /// Number of events received.
  CORBA::ULong event_count_ {};

  /// The orb, just a pointer because the ORB does not outlive the run() method.
  CORBA::ORB_ptr orb_ {};

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// The Event Log Factory that generates the events to be consumed.
  RTEventLogAdmin::EventLogFactory_var event_log_factory_;

  /// The proxy to which this consumer is connected.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_;
};

#endif /* RTEVENT_CONSUMER_H */
