/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * Defines a simple Push Supplier.
 *
 *
 */
//=============================================================================


#ifndef COSECSUPPLIER_H_
#define COSECSUPPLIER_H_

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "Multiple.h"

class Supplier : public POA_CosEventComm::PushSupplier,
                      public Multiple
{
  // = TITLE
  //   Supplier
  //
  // = DESCRIPTION
  //   The Supplier is a simple Push Supplier that connects to
  //   the CosEC and sends events to it.
public:
  // = Initializatiopn and termination methods.
  /// Constructor.
  Supplier ();

  /// This method connects the supplier to the EC.
  void open (CosEventChannelAdmin::EventChannel_ptr event_channel);

  /// Disconnect from the EC.
  void close (void);

  void connect (void);

  /// Disconnect from the EC, but do not forget about it or close it.
  void disconnect (void);

  /// Send one event.
  void send_event (const CORBA::Any &data);

  /// The POA_CosEventComm::PushSupplier method.
  virtual void disconnect_push_supplier (void);

  /// Sends the events to the EC.
  void run (void);

  // = Multiple methods.
  /// Parses the command line arguments.
  virtual int parse_args (int argc, ACE_TCHAR *argv []);
private:
  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  /// We talk to the EC using this proxy.
  CosEventChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// The number of Events to send to the EC.
  int event_count_;
};

#endif /* COSECSUPPLIER_H_ */
