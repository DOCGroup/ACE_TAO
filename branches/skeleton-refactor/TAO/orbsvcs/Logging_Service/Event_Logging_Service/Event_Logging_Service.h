/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Event_Logging_Service.h
 *
 *  $Id$
 *
 *  Front End of the Telecom Log Service 
 *  Event_Logging_Service
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef EVENT_LOGGING_SERVICE_H
#define EVENT_LOGGING_SERVICE_H

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/Log/EventLogFactory_i.h"

class Event_Logging_Service
{
  // = TITLE
  //   Event_Logging_Service
  //
  // = DESCRIPTION
  //   Implementation of the Telecom EventLog Service

 public:
  // = Initialization and termination methods.
  Event_Logging_Service (void);
  // Constructor.

  virtual ~Event_Logging_Service (void);
  // Destructor.

  int startup (int argc, char *argv[]
               ACE_ENV_ARG_DECL);
  // Initializes the Telecom EventLog Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // run the Telecom EventLog Service.
  // Returns 0 on success, -1 on error.

  void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // Shutdown the Telecom Log Service.
  // Returns 0 on success, -1 on error.

protected:
  void init_ORB (int& argc, char *argv []
                 ACE_ENV_ARG_DECL);
  // initialize the ORB.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  void resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Resolve the naming service.

  // = Data members
  TAO_EventLogFactory_i event_log_factory_;
  // The Event Log Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_;
  // A naming context.
	
  const char* service_name_;
  // The name we use to bind with the NameService

  const char* ior_file_name_;
  // The name of the file were we output the Event_Service IOR.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  int bind_to_naming_service_;
  // If true, bind to naming service
};
#endif /* EVENT_LOGGING_SERVICE_H */
