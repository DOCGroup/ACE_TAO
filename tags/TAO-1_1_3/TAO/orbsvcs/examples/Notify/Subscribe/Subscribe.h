/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Filter.h
//
// = DESCRIPTION
//   Class to demo structured event filtering.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_FILTER_CLIENT_H
#define NOTIFY_FILTER_CLIENT_H

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

class TAO_Notify_StructuredPushConsumer;
class TAO_Notify_StructuredPushSupplier;

class Subscribe
{
  // = TITLE
  //
  // = DESCRIPTION
  //

 public:
  // = Initialization and Termination
  Subscribe (void);
  ~Subscribe ();

  void init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // Init the Client.

  void run (CORBA::Environment &ACE_TRY_ENV);
  // Run the demo.

 protected:
  void init_ORB (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // Initializes the ORB.

  void resolve_naming_service (CORBA::Environment &ACE_TRY_ENV);
  // Try to get hold of a running naming service.

  void resolve_Notify_factory (CORBA::Environment &ACE_TRY_ENV);
  // Try to resolve the Notify factory from the Naming service.

  void create_EC (CORBA::Environment &ACE_TRY_ENV);
  // Create an EC.

  void create_supplieradmin(CORBA::Environment &ACE_TRY_ENV);
  // Create the Supplier Admin.

  void create_consumeradmin (CORBA::Environment &ACE_TRY_ENV);
  // Create the Consumer Admin.

  void create_consumers (CORBA::Environment &ACE_TRY_ENV);
  // Create and initialize the consumers.

  void create_suppliers (CORBA::Environment &ACE_TRY_ENV);
  // create and initialize the suppliers.

  void send_events (CORBA::Environment &ACE_TRY_ENV);
  // send the events.

  // = Data Members
  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
  // Channel factory.

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
  // The group operator between admin-proxy's.

  CosNotification::QoSProperties initial_qos_;
  // Initial qos specified to the factory when creating the EC.

  CosNotification::AdminProperties initial_admin_;
  // Initial admin props specified to the factory when creating the EC.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  TAO_Notify_StructuredPushConsumer* consumer_1_;
  TAO_Notify_StructuredPushConsumer* consumer_2_;

  TAO_Notify_StructuredPushSupplier* supplier_1_;
  TAO_Notify_StructuredPushSupplier* supplier_2_;
};
#endif /* NOTIFY_FILTER_CLIENT_H */
