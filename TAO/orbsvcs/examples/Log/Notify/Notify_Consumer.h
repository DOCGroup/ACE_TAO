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

class Consumer
  : public POA_CosNotifyComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of log generated events.
  //
public:
  Consumer () = default;
  // Constructor

  Consumer (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer &operator= (Consumer &&) = delete;

  int run (int argc, ACE_TCHAR* argv[]);
  // Run the test

protected:
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_ {};
  // The proxy_supplier id.

  // = Methods

  // = NotifyPublish method
  void offer_change (CosNotification::EventTypeSeq const &added,
                     CosNotification::EventTypeSeq const &removed) override;

  // = StructuredPushSupplier methods
  void push (CORBA::Any const &event) override;


  void disconnect_push_consumer () override;

private:
  CORBA::ULong event_count_ {};
  // Keep track of the number of events received.

  // = Data Members
  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  DsNotifyLogAdmin::NotifyLogFactory_var notify_log_factory_;

  CosNotifyChannelAdmin::ProxyPushSupplier_var proxy_supplier_;
};

#endif /* NOTIFY_CONSUMER_H */
