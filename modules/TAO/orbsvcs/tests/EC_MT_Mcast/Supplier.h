// $Id$
// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //   It simply publishes one event type, when the perform_push()
  //   method is invoked it pushes the event through the event service
  //
public:
  Supplier (void);
  // Constructor

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin);
  // Connect to the event channel

  void disconnect (void);
  // Disconnect from the event channel

  void perform_push (void);
  // Push a single event

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (void);
  // The skeleton methods.

private:
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;
  // The proxy
};

#endif /* SUPPLIER_H */
