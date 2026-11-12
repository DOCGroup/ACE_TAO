/* -*- C++ -*- */
/**
 * @file ECM_Supplier.h
 * @brief Supplier driver for the custom marshaling event-channel test.
 */

#ifndef ECM_SUPPLIER_H
#define ECM_SUPPLIER_H

#include <array>
#include <memory>

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Channel_Clients_T.h"

class ECMS_Driver;

/// Sends custom-marshaled event payloads through the event channel.
class Test_Supplier : public ACE_Task<ACE_SYNCH>
{
public:
  explicit Test_Supplier (ECMS_Driver& driver);
  Test_Supplier (Test_Supplier const&) = delete;
  Test_Supplier (Test_Supplier&&) = delete;
  Test_Supplier& operator= (Test_Supplier const&) = delete;
  Test_Supplier& operator= (Test_Supplier&&) = delete;

  /// Run the supplier task in its worker thread.
  int svc () override;

  /// Connect the supplier servant to the event channel.
  void connect (char const* name,
                int event_a,
                int event_b,
                RtecEventChannelAdmin::EventChannel_ptr ec);
  /// Disconnect the supplier from the event channel.
  void disconnect ();

  /// Handle disconnection forwarded by the supplier adapter.
  void disconnect_push_supplier ();

  /// Return this supplier's event source ID.
  RtecEventComm::EventSourceID supplier_id () const;

  /// Return the proxy used to send events to the event channel.
  RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy ();

private:
  ECMS_Driver& driver_;
  RtecEventComm::EventSourceID supplier_id_ {};
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  ACE_PushSupplier_Adapter<Test_Supplier> supplier_;
};

/// Coordinates supplier tasks and custom event payload construction.
class ECMS_Driver
{
public:
  ECMS_Driver ();
  ECMS_Driver (ECMS_Driver const&) = delete;
  ECMS_Driver (ECMS_Driver&&) = delete;
  ECMS_Driver& operator= (ECMS_Driver const&) = delete;
  ECMS_Driver& operator= (ECMS_Driver&&) = delete;

  static constexpr int MAX_SUPPLIERS = 16;

  /// Parse options and run the supplier test.
  int run (int argc, ACE_TCHAR* argv[]);

  /// Build and send events for one supplier task.
  int supplier_task (Test_Supplier& supplier);

private:
  /// Parse supplier-specific command-line options.
  int parse_args (int argc, ACE_TCHAR* argv[]);
  /// Connect and disconnect all configured suppliers.
  void connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec);
  void disconnect_suppliers ();
  /// Start event generation for all suppliers.
  void activate_suppliers ();

  std::array<std::unique_ptr<Test_Supplier>, MAX_SUPPLIERS> suppliers_ {};
  int n_suppliers_ {};
  int event_count_ {};
  int event_period_ {};
  int event_size_ {};
  int event_a_ {};
  int event_b_ {};
  ACE_TCHAR const* pid_file_name_ {};
};

#endif /* ECM_SUPPLIER_H */
