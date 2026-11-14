/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Notify_Supplier.h
 *
 *  An example of using the Notify_Logging_Service.
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef NOTIFY_SUPPLIER_H
#define NOTIFY_SUPPLIER_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/DsNotifyLogAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosNotifyCommS.h"

class Filter_StructuredPushSupplier;

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief Simple supplier object.
 *
 * This class is a supplier of events.
 */
class Supplier
{
public:
  Supplier () = default;
  ~Supplier () = default;

  Supplier (Supplier const &) = delete;
  Supplier (Supplier &&) = delete;
  Supplier &operator= (Supplier const &) = delete;
  Supplier &operator= (Supplier &&) = delete;

  /// Run the test.
  int run (int argc, ACE_TCHAR *argv[]);

private:
  /// The ORB used by this supplier.
  CORBA::ORB_var orb_;

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// The notify log factory from the Log Service.
  DsNotifyLogAdmin::NotifyLogFactory_var notify_log_factory_;

  DsNotifyLogAdmin::NotifyLog_var notify_log_;

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
};

/**
 * @brief Supplier for the filter example.
 */
class Filter_StructuredPushSupplier
  : public POA_CosNotifyComm::StructuredPushSupplier
{
 public:
  Filter_StructuredPushSupplier () = default;

  Filter_StructuredPushSupplier (Filter_StructuredPushSupplier const &) = delete;
  Filter_StructuredPushSupplier (Filter_StructuredPushSupplier &&) = delete;
  Filter_StructuredPushSupplier &operator= (Filter_StructuredPushSupplier const &) = delete;
  Filter_StructuredPushSupplier &operator= (Filter_StructuredPushSupplier &&) = delete;

  /// Connects the supplier to the EventChannel and creates a proxy.
  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnects from the supplier.
  void disconnect ();

  /// Sends one event.
  void send_event (CosNotification::StructuredEvent const &event);

protected:
  /// The proxy to which this supplier is connected.
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;

  /// This supplier's ID.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_ {};

  ~Filter_StructuredPushSupplier () override = default;

  void subscription_change (
        CosNotification::EventTypeSeq const &added,
        CosNotification::EventTypeSeq const &removed) override;

  void disconnect_structured_push_supplier () override;
};

#endif /* NOTIFY_SUPPLIER_H */
