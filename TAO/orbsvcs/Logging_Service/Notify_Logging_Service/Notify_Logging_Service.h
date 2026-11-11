/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Notify_Logging_Service.h
 *
 *  Front End of the Telecom Log Service
 *  Notify_Logging_Service
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

#include <optional>
#include <string>

class Notify_Logging_Service
  : public ACE_Task_Base
{
  // = TITLE
  //   Notify_Logging_Service
  //
  // = DESCRIPTION
  //   Implementation of the Telecom Log Service

 public:
  Notify_Logging_Service () = default;

  ~Notify_Logging_Service () override = default;

  Notify_Logging_Service (Notify_Logging_Service const &) = delete;
  Notify_Logging_Service (Notify_Logging_Service &&) = delete;
  Notify_Logging_Service &operator= (Notify_Logging_Service const &) = delete;
  Notify_Logging_Service &operator= (Notify_Logging_Service &&) = delete;

  int init (int argc, ACE_TCHAR * argv[]) override;
  // Initializes the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  int run ();
  // Run the Telecom Log Service.
  // Returns 0 on success, -1 on error.

  void shutdown ();
  // Shutdown the Telecom Service.
  // Returns 0 on success, -1 on error.

protected:
  int init_ORB (int & argc, ACE_TCHAR * argv []);
  // initialize the ORB.

  int parse_args (int argc, ACE_TCHAR * argv[]);
  // Parses the command line arguments.

  void resolve_naming_service ();
  // Resolve the naming service.

  int svc () override;
  // Run worker threads.

  // = Data members

  // Borrowed from ACE_Dynamic_Service.
  TAO_Notify_Service *notify_service_ {};

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_ {};
  // The Factory.

  CORBA::ORB_var orb_ {};
  // The ORB that we use.

  PortableServer::POA_var poa_ {};
  // Reference to the root poa.

  CosNaming::NamingContext_var naming_ {};
  // A naming context.

  std::string service_name_ {"NotifyLogFactory"};
  // The name we use to bind with the NameService

  std::optional<std::basic_string<ACE_TCHAR>> ior_file_name_ {};
  // The name of the file where we output the factory IOR.

  std::optional<std::basic_string<ACE_TCHAR>> pid_file_name_ {};
  // The name of the file where we output the process id.

  bool bind_to_naming_service_ {true};
  // If true, bind to naming service

  int nthreads_ {};
  // Number of worker threads.
};
#endif /* NOTIFY_LOGGING_SERVICE_H */
