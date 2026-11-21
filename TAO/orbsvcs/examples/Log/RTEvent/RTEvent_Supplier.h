/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   RTEvent_Supplier.h
 *
 *  An example of using the RTEvent_Logging_Service.
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef RTEVENT_SUPPLIER_H
#define RTEVENT_SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RTEventLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief Simple supplier of events.
 *
 * Publishes one event type.
 */
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  Supplier () = default;

  Supplier (Supplier const &) = delete;
  Supplier (Supplier &&) = delete;
  Supplier &operator= (Supplier const &) = delete;
  Supplier &operator= (Supplier &&) = delete;

  /// Run the test.
  int run (int argc, ACE_TCHAR *argv[]);

  /// The skeleton method.
  void disconnect_push_supplier () override;

private:
  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// The Event Log Factory that generates the events to be consumed.
  RTEventLogAdmin::EventLogFactory_var event_log_factory_;

  /// The proxy to which this supplier is connected.
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;
};

#endif /* RTEVENT_SUPPLIER_H */
