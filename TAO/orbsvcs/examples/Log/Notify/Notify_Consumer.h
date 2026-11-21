/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Notify_Consumer.h
 *
 *  An example of using the Notify_Logging_Service.
 *  The Notify_Consumer consumes log-generated events.
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef NOTIFY_CONSUMER_H
#define NOTIFY_CONSUMER_H

#include "orbsvcs/DsNotifyLogAdminS.h"
#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNotifyCommS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief Simple consumer of log-generated events.
 */
class Consumer
  : public POA_CosNotifyComm::PushConsumer
{
public:
  Consumer () = default;

  Consumer (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer &operator= (Consumer &&) = delete;

  /// Run the test.
  int run (int argc, ACE_TCHAR *argv[]);

protected:
  /// The proxy supplier ID.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_ {};

  void offer_change (CosNotification::EventTypeSeq const &added,
                     CosNotification::EventTypeSeq const &removed) override;

  void push (CORBA::Any const &event) override;
  void disconnect_push_consumer () override;

private:
  /// Number of events received.
  CORBA::ULong event_count_ {};

  /// The ORB used by this consumer.
  CORBA::ORB_var orb_;

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  DsNotifyLogAdmin::NotifyLogFactory_var notify_log_factory_;

  CosNotifyChannelAdmin::ProxyPushSupplier_var proxy_supplier_;
};

#endif /* NOTIFY_CONSUMER_H */
