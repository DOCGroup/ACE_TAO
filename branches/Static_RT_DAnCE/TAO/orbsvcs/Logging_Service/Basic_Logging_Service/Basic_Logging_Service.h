/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Basic_Logging_Service.h
 *
 *  $Id$
 *
 *  Front End of the Telecom Log Service
 *  Basic_Logging_Service
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef BASIC_LOGGING_SERVICE_H
#define BASIC_LOGGING_SERVICE_H

#include "ace/Task.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/Log/BasicLogFactory_i.h"

class Basic_Logging_Service
  : public ACE_Task_Base
{
  // = TITLE
  //   Basic_Logging_Service
  //
  // = DESCRIPTION
  //   Implementation of the Telecom Log Service

 public:
  // = Initialization and termination methods.
  Basic_Logging_Service (void);
  // Constructor.

  virtual ~Basic_Logging_Service (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initializes the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // Run the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  void shutdown (void);
  // Shutdown the Telecom Log Service.
  // Returns 0 on success, -1 on error.

protected:
  void init_ORB (int& argc, char *argv []);
  // initialize the ORB.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  void resolve_naming_service (void);
  // Resolve the naming service.

  int svc ();
  // Run worker threads.

  // = Data members
  TAO_BasicLogFactory_i basic_log_factory_;
  // The Basic Log Factory.

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

  bool bind_to_naming_service_;
  // If true, bind to naming service

  int nthreads_;
  // Number of worker threads.
};
#endif /* BASIC_LOGGING_SERVICE_H */
