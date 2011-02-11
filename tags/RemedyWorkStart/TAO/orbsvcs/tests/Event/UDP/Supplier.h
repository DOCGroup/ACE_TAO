/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Supplier
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "TestC.h"

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
  Supplier (bool valuetype);
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
  void insert_into_any (CORBA::Any& any, Components::EventBase* vb);

  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;
  // The proxy

  bool valuetype_;

  CORBA::ULong event_count_;
};

#endif /* SUPPLIER_H */
