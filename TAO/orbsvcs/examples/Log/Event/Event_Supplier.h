/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Event_Supplier.h
 *
 *  An example of using the Event_Logging_Service.
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef EVENT_SUPPLIER_H
#define EVENT_SUPPLIER_H

#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief Simple supplier object.
 *
 * This class is a supplier of events.
 */
class Supplier : public POA_CosEventComm::PushSupplier
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
  /// The ORB used by this supplier.
  CORBA::ORB_var orb_;

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// The Event Log Factory that generates the events to be consumed.
  DsEventLogAdmin::EventLogFactory_var event_log_factory_;

  /// The proxy to which this supplier is connected.
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_;
};

#endif /* EVENT_SUPPLIER_H */
