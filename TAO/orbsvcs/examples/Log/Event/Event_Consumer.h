/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Event_Consumer.h
 *
 *  An example of using the Event_Logging_Service.
 *  The Event_Consumer consumes log-generated events.
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef EVENT_CONSUMER_H
#define EVENT_CONSUMER_H

#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief Simple consumer object.
 *
 * This class is a consumer of log-generated events.
 */
class Consumer : public POA_CosEventComm::PushConsumer
{
public:
  Consumer () = default;

  Consumer (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer &operator= (Consumer &&) = delete;

  /// Run the test.
  int run (int argc, ACE_TCHAR *argv[]);

  void push (CORBA::Any const &event) override;

  void disconnect_push_consumer () override;

private:
  /// Number of events received.
  CORBA::ULong event_count_ {};

  /// The ORB reference used by the disconnect callback.
  CORBA::ORB_var orb_;

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// The Event Log Factory that generates the events to be consumed.
  DsEventLogAdmin::EventLogFactory_var event_log_factory_;

  /// The proxy to which this consumer is connected.
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_;
};

#endif /* EVENT_CONSUMER_H */
