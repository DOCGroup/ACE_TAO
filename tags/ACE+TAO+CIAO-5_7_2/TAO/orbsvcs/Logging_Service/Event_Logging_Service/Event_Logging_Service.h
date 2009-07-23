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

#include "ace/Task.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/Log/EventLogFactory_i.h"

class Event_Logging_Service
  : public ACE_Task_Base
{
  // = TITLE
  //   Event_Logging_Service
  //
  // = DESCRIPTION
  //   Implementation of the Telecom Log Service

 public:
  // = Initialization and termination methods.
  Event_Logging_Service (void);
  // Constructor.

  virtual ~Event_Logging_Service (void);
  // Destructor.

  int init (int argc, ACE_TCHAR *argv[]);
  // Initializes the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // Run the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  void shutdown (void);
  // Shutdown the Telecom Log Service.
  // Returns 0 on success, -1 on error.

protected:
  void init_ORB (int& argc, ACE_TCHAR *argv []);
  // initialize the ORB.

  int parse_args (int argc, ACE_TCHAR *argv []);
  // Parses the command line arguments.

  void resolve_naming_service (void);
  // Resolve the naming service.

  int svc ();
  // Run worker threads.

  // = Data members
  TAO_EventLogFactory_i *event_log_factory_;
  // The Event Log Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_;
  // A naming context.

  ACE_CString service_name_;
  // The name we use to bind with the NameService

  const ACE_TCHAR* ior_file_name_;
  // The name of the file where we output the factory IOR.

  const ACE_TCHAR* pid_file_name_;
  // The name of the file where we output the process id.

  bool bind_to_naming_service_;
  // If true, bind to naming service

  int nthreads_;
  // Number of worker threads.
};
#endif /* EVENT_LOGGING_SERVICE_H */
