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
#include "CosEC_Multiple.h"

class CosECSupplier : public POA_CosEventComm::PushSupplier,
                      public CosEC_Multiple
{
  // = TITLE
  //   CosECSupplier
  //
  // = DESCRIPTION
  //   The CosECSupplier is a simple Push Supplier that connects to
  //   the CosEC and sends events to it.
public:
  // = Initializatiopn and termination methods.
  CosECSupplier ();
  // Constructor.

  void open (CosEventChannelAdmin::EventChannel_ptr event_channel,
             CORBA::Environment &ACE_TRY_ENV);
  // This method connects the supplier to the EC.

  void close (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the EC.

  void connect (CORBA::Environment &ACE_TRY_ENV);

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the EC, but do not forget about it or close it.

  void send_event (const CORBA::Any &data,
                   CORBA::Environment &ACE_TRY_ENV);
  // Send one event.

  virtual void disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV);
  // The POA_CosEventComm::PushSupplier method.

  void run (void);
  // Sends the events to the EC.

  // = CosEC_Multiple methods.
  virtual int parse_args (int argc, char *argv []);
  // Parses the command line arguments.
private:
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC using this proxy.

  CosEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  // We talk to the EC using this proxy.

  int event_count_;
  // The number of Events to send to the EC.
};

#endif /* COSECSUPPLIER_H_ */
