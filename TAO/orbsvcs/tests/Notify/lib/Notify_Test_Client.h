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
#include "notify_test_export.h"

class TAO_NOTIFY_TEST_Export Notify_Test_Client
{
  // = TITLE
  //   Notify_Test_Client
  // = DESCRIPTION
  //   Shows how consumers Notify_Test_Client for events.

public:
  // = Initialization and Termination
  Notify_Test_Client (void);
  virtual ~Notify_Test_Client ();

  virtual int init (int argc, char *argv [] TAO_ENV_ARG_DECL);
  // starts the orb and resolves the notify factory via a naming service.

  virtual int parse_args (int argc, char* argv[]);
  // Allow the user to override this empty method

  int ORB_run (void);
  // Call ORB::run to accept requests.

  void shutdown (TAO_ENV_SINGLE_ARG_DECL);
  // Shutdown the ORB

  CORBA::Boolean& done (void);
  // Access the done boolean.

  CORBA::ORB_ptr orb (void);
  // Access the ORB.  This class retains ownership.

  PortableServer::POA_ptr root_poa (void);
  // Access the Root POA.  This class retains ownership.

  CosNaming::NamingContext_ptr naming_context (void);
  // Access the Naming Context.  This class retains ownership.

  CosNotifyChannelAdmin::EventChannelFactory_ptr notify_factory (void);
  // Access the Notify Factory.  This class retains ownership.

  CosNotifyChannelAdmin::EventChannel_ptr create_event_channel (
                                              const char* name,
                                              int resolve
                                              TAO_ENV_ARG_DECL
                                            );
  // Create an Event Channel.  Ownership is passed to the caller.

protected:
  int init_ORB (int argc,
                char *argv []
                TAO_ENV_ARG_DECL);
  // Initializes the ORB.

  void resolve_naming_service (TAO_ENV_SINGLE_ARG_DECL);
  // Try to get hold of a running naming service.

  void resolve_Notify_factory (TAO_ENV_SINGLE_ARG_DECL);
  // Try to resolve the Notify factory from the Naming service.

  // = Data Members
  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
  // Channel factory.

  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
  // The group operator between admin-proxy's.

  CosNotification::QoSProperties initial_qos_;
  // Initial qos specified to the factory when creating the EC.

  CosNotification::AdminProperties initial_admin_;
  // Initial admin props specified to the factory when creating the EC.

  CORBA::Boolean done_;
  // Set when we should shutdown.
};

#endif /* NOTIFY_TEST_CLIENT_H */
