/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   Logging_Service.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   Telecom Log Service front end.
//
// ============================================================================

#ifndef LOGGING_SERVICE_H
#define LOGGING_SERVICE_H

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/Log/BasicLogFactory_i.h"

class Logging_Service
{
  // = TITLE
  //   Logging_Service
  //
  // = DESCRIPTION
  //   Implementation of the Telecom Log Service

 public:
  // = Initialization and termination methods.
  Logging_Service (void);
  // Constructor.

  virtual ~Logging_Service (void);
  // Destructor.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  void startup (int argc, char *argv[]
               TAO_ENV_ARG_DECL);
  // Initializes the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // run the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  void shutdown (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Shutdown the Telecom Log Service.
  // Returns 0 on success, -1 on error.

protected:
  void init_ORB (int& argc, char *argv []
                 TAO_ENV_ARG_DECL);
  // initialize the ORB.

  void resolve_naming_service (TAO_ENV_SINGLE_ARG_DECL);
  // Resolve the naming service.

  // = Data members
  const char* basic_log_factory_name_;
  // The Log Factory name.

  BasicLogFactory_i basic_log_factory_;
  // The Basic Log Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_;
  // A naming context.
};
#endif /* LOGGING_SERVICE_H */
