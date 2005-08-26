/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Notify_Logging_Service.h
 *
 *  $Id$
 *
 *  Front End of the Telecom Log Service
 *  Notify_Logging_Service
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef NOTIFY_LOGGING_SERVICE_H
#define NOTIFY_LOGGING_SERVICE_H

#include "ace/Task.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/DsNotifyLogAdminC.h"
#include "orbsvcs/Log/NotifyLogFactory_i.h"

#define NOTIFY_KEY "NotifyLogFactory"

class Notify_Logging_Service
  : public ACE_Task_Base
{
  // = TITLE
  //   Notify_Logging_Service
  //
  // = DESCRIPTION
  //   Implementation of the Telecom Log Service

 public:
  // = Initialization and termination methods.
  Notify_Logging_Service (void);
  // Constructor.

  virtual ~Notify_Logging_Service (void);
  // Destructor.

  int init (int argc, char *argv[] ACE_ENV_ARG_DECL);
  // Initializes the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Run the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // Shutdown the Telecom Service.
  // Returns 0 on success, -1 on error.

protected:
  int init_ORB (int& argc, char *argv []
                ACE_ENV_ARG_DECL);
  // initialize the ORB.

  int parse_args (int argc, char *argv[]);
  // Parses the command line arguments.

  void resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Resolve the naming service.

  int svc ();
  // Run worker threads.

  // = Data members

  TAO_Notify_Service* notify_service_;

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
  // The Factory.

  TAO_NotifyLogFactory_i *notify_log_factory_;
  // The Notify Log Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_;
  // A naming context.

  const char* service_name_;
  // The name we use to bind with the NameService

  const char* ior_file_name_;
  // The name of the file where we output the factory IOR.

  const char* pid_file_name_;
  // The name of the file where we output the process id.

  int bind_to_naming_service_;
  // If true, bind to naming service

  int nthreads_;
  // Number of worker threads.
};
#endif /* NOTIFY_LOGGING_SERVICE_H */
