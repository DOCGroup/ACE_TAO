/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   Notify_Service.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   Notification Service front end.
//
// ============================================================================

#ifndef NOTIFY_SERVICE_H
#define NOTIFY_SERVICE_H

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"

class Notify_Service
{
  // = TITLE
  //   Notify_Service
  //
  // = DESCRIPTION
  //   Implementation of the Notification Service front end.

 public:
  // = Initialization and termination methods.
  Notify_Service (void);
  // Constructor.

  virtual ~Notify_Service (void);
  // Destructor.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  void startup (int argc, char *argv[],
               CORBA::Environment &ACE_TRY_ENV);
  // Initializes the Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // run the Service.
  // Returns 0 on success, -1 on error.

  void shutdown (CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  // Shutdown the Service.
  // Returns 0 on success, -1 on error.

  CosNotifyChannelAdmin::EventChannelFactory_var obj;
  // temp hack.
protected:
  void init_ORB (int& argc, char *argv [],
                 CORBA::Environment &ACE_TRY_ENV);
  // initialize the ORB.

  void resolve_naming_service (CORBA::Environment &ACE_TRY_ENV);
  // Resolve the naming service.

  // = Data members
  const char* notify_factory_name_;
  // The Factory name.

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
  // The Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_;
  // A naming context.
};
#endif /* NOTIFY_SERVICE_H */
