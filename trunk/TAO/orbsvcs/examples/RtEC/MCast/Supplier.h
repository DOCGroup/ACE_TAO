/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Supplier
 *
 * @brief Simple supplier object
 *
 * This class is a supplier of events.
 * It simply publishes one event type, when the perform_push()
 * method is invoked it pushes the event through the event service
 */
class Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  /// Constructor
  Supplier (void);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the event channel
  void disconnect (void);

  /// Push a single event
  void perform_push (void);

  // = The RtecEventComm::PushSupplier methods

  /// The skeleton methods.
  virtual void disconnect_push_supplier (void);

private:
  /// The proxy
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;
};

#endif /* SUPPLIER_H */
