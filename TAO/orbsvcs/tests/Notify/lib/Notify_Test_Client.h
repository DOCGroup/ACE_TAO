/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Notify_Test_Client.h
//
// = DESCRIPTION
//   Prototype Test Driver for all the Notify tests.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_TEST_CLIENT_H
#define NOTIFY_TEST_CLIENT_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

class Notify_Test_Client
{
  // = TITLE
  //   Notify_Test_Client
  // = DESCRIPTION
  //   Shows how consumers Notify_Test_Client for events.

 public:
  // = Initialization and Termination
  Notify_Test_Client (void);
  virtual ~Notify_Test_Client ();

  static int test_main (int argc, char *argv [], Notify_Test_Client& client);
  // Handles the generic 'main' entry operations.

  virtual void init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // Init the Client.

  int ORB_run (void);
  // Call ORB::run to accept requests.

  void shutdown (CORBA::Environment &ACE_TRY_ENV);
  // Shutdown the ORB, destroy the event channel.

  virtual void init_concrete (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV) = 0;
  // Abstract method to be overloaded by the concrete prototype.
  // This is called from <init> to allow the concrete prototype to do its
  // initialization.

  virtual void run_test (CORBA::Environment &ACE_TRY_ENV) = 0;
  // Run the test.
  // The concrete prototype must implement this.

 protected:
  void init_ORB (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV);
  // Initializes the ORB.

  void resolve_naming_service (CORBA::Environment &ACE_TRY_ENV);
  // Try to get hold of a running naming service.

  void resolve_Notify_factory (CORBA::Environment &ACE_TRY_ENV);
  // Try to resolve the Notify factory from the Naming service.

  void create_EC (CORBA::Environment &ACE_TRY_ENV);
  // Create an EC.

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
};

#endif /* NOTIFY_TEST_CLIENT_H */
