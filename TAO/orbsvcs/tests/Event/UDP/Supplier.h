/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "TestC.h"

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
  Supplier (bool valuetype);

  /// Connect to the event channel
  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

  /// Disconnect from the event channel
  void disconnect ();

  /// Push a single event
  void perform_push ();

  // = The RtecEventComm::PushSupplier methods

  /// The skeleton methods.
  virtual void disconnect_push_supplier ();

private:
  void insert_into_any (CORBA::Any& any, Components::EventBase* vb);

  /// The proxy
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;

  bool valuetype_;

  CORBA::ULong event_count_;
};

#endif /* SUPPLIER_H */
