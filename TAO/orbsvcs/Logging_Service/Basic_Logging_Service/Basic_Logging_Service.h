/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Basic_Logging_Service.h
 *
 *  Front End of the Telecom Log Service
 *  Basic_Logging_Service
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
  Basic_Logging_Service ();
  // Constructor.

  ~Basic_Logging_Service () override;
  // Destructor.

  Basic_Logging_Service (Basic_Logging_Service const &) = delete;
  Basic_Logging_Service (Basic_Logging_Service &&) = delete;
  Basic_Logging_Service &operator= (Basic_Logging_Service const &) = delete;
  Basic_Logging_Service &operator= (Basic_Logging_Service &&) = delete;

  int init (int argc, ACE_TCHAR * argv[]);
  // Initializes the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  int run ();
  // Run the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  void shutdown ();
  // Shutdown the Telecom Log Service.
  // Returns 0 on success, -1 on error.

protected:
  void init_ORB (int & argc, ACE_TCHAR * argv []);
  // initialize the ORB.

  int parse_args (int argc, ACE_TCHAR * argv []);
  // Parses the command line arguments.

  void resolve_naming_service ();
  // Resolve the naming service.

  int svc () override;
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

  ACE_CString service_name_ {"BasicLogFactory"};
  // The name we use to bind with the NameService

  ACE_TCHAR const * ior_file_name_ {};
  // The name of the file where we output the factory IOR.

  ACE_TCHAR const * pid_file_name_ {};
  // The name of the file where we output the process id.

  bool bind_to_naming_service_ {true};
  // If true, bind to naming service

  int nthreads_ {};
  // Number of worker threads.
};
#endif /* BASIC_LOGGING_SERVICE_H */
