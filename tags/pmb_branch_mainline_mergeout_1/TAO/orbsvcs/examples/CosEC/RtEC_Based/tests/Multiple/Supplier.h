/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   Supplier.h
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
  Supplier ();
  // Constructor.

  void open (CosEventChannelAdmin::EventChannel_ptr event_channel
             ACE_ENV_ARG_DECL);
  // This method connects the supplier to the EC.

  void close (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the EC.

  void connect (ACE_ENV_SINGLE_ARG_DECL);

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the EC, but do not forget about it or close it.

  void send_event (const CORBA::Any &data
                   ACE_ENV_ARG_DECL);
  // Send one event.

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
  // The POA_CosEventComm::PushSupplier method.

  void run (void);
  // Sends the events to the EC.

  // = Multiple methods.
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
