/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   CosECSupplier.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   Defines a simple Push Supplier.
//
// ============================================================================

#ifndef COSECSUPPLIER_H_
#define COSECSUPPLIER_H_

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminS.h"

class CosECSupplier : public POA_CosEventComm::PushSupplier
{
  // = TITLE
  //   CosECSupplier
  //
  // = DESCRIPTION
  //   The CosECSupplier is a simple Push Supplier that connects to
  //   the CosEC and sends events to it.
public:
  void open (CosEventChannelAdmin::EventChannel_ptr event_channel,
             CORBA::Environment &TAO_TRY_ENV);
  // This method connects the supplier to the EC.

  void close (CORBA::Environment &TAO_TRY_ENV);
  // Disconnect from the EC.

  void connect (CORBA::Environment &TAO_TRY_ENV);

  void disconnect (CORBA::Environment &TAO_TRY_ENV);
  // Disconnect from the EC, but do not forget about it or close it.

  void send_event (const CORBA::Any &data,
                   CORBA::Environment &TAO_TRY_ENV);
  // Send one event.

  virtual void disconnect_push_supplier (CORBA::Environment &);
  // The POA_CosEventComm::PushSupplier method.

private:
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC using this proxy.

  CosEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  // We talk to the EC using this proxy.
};

#endif /* COSECSUPPLIER_H_ */
