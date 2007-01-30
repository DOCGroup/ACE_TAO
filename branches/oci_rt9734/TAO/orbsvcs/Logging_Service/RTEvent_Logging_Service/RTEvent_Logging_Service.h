/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   RTEvent_Logging_Service.h
 *
 *  $Id$
 *
 *  Front End of the Telecom Log Service
 *  RTEvent_Logging_Service
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef RTEVENT_LOGGING_SERVICE_H
#define RTEVENT_LOGGING_SERVICE_H

#include "ace/Task.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Log/RTEventLogFactory_i.h"

class RTEvent_Logging_Service
  : public ACE_Task_Base
{
  // = TITLE
  //   RTEvent_Logging_Service
  //
  // = DESCRIPTION
  //   Implementation of the Telecom Log Service

public:
  // = Initialization and termination methods.
  RTEvent_Logging_Service (void);
  // Constructor.

  virtual ~RTEvent_Logging_Service (void);
  // Destructor.

  int init (int argc, char* argv[]);
  // Initializes the Telecom Log Service.

  int run (void);
    // Run the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  void shutdown (void);
  // Shutdown the Telecom Log Service.
  // Returns 0 on success, -1 on error.

protected:
  void init_ORB (int& argc, char *argv []);
  // initializes the ORB.

  int parse_args (int argc, char* argv[]);
  // parse the command line args

  void resolve_naming_service (void);
  // Resolve the naming service.

  int svc ();
  // Run worker threads.

private:
  // = Data members
  TAO_RTEventLogFactory_i *rtevent_log_factory_;
  // The RTEvent Log Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_;
  // A naming context.

  const char* service_name_;
  // The name we use to bind with the NameService

  const char* ior_file_name_;
  // The name of the file were we output the factory IOR.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  bool bind_to_naming_service_;
  // If true, bind to naming service

  int nthreads_;
  // Number of worker threads.
};

#endif /* RTEVENT_LOGGING_SERVICE_H */
