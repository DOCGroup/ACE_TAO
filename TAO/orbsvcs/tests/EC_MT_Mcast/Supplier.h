// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Simple supplier object that publishes one event type.
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  Supplier () = default;
  Supplier (Supplier const&) = delete;
  Supplier (Supplier&&) = delete;
  Supplier& operator= (Supplier const&) = delete;
  Supplier& operator= (Supplier&&) = delete;

  /// Connects to the Event Channel.
  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnects from the Event Channel.
  void disconnect ();

  /// Pushes one event through the Event Channel.
  void perform_push ();

  /// Receives disconnect notifications from the Event Channel.
  void disconnect_push_supplier () override;

private:
  /// Proxy used to communicate with the Event Channel.
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;
};

#endif /* SUPPLIER_H */
