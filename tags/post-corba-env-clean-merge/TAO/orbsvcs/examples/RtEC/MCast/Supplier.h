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

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin
                TAO_ENV_ARG_DECL);
  // Connect to the event channel

  void disconnect (TAO_ENV_SINGLE_ARG_DECL);
  // Disconnect from the event channel

  void perform_push (TAO_ENV_SINGLE_ARG_DECL);
  // Push a single event

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_;
  // The proxy
};

#endif /* SUPPLIER_H */
