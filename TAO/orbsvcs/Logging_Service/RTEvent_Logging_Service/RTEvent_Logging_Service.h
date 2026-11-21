/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   RTEvent_Logging_Service.h
 *
 *  Front End of the Telecom Log Service
 *  RTEvent_Logging_Service
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef RTEVENT_LOGGING_SERVICE_H
#define RTEVENT_LOGGING_SERVICE_H

#include "ace/Task.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Log/RTEventLogFactory_i.h"

#include <optional>
#include <string>

/// Implements the Telecom Log Service.
class RTEvent_Logging_Service
  : public ACE_Task_Base
{

public:
  RTEvent_Logging_Service () = default;

  ~RTEvent_Logging_Service () override = default;

  RTEvent_Logging_Service (RTEvent_Logging_Service const &) = delete;
  RTEvent_Logging_Service (RTEvent_Logging_Service &&) = delete;
  RTEvent_Logging_Service &operator= (RTEvent_Logging_Service const &) = delete;
  RTEvent_Logging_Service &operator= (RTEvent_Logging_Service &&) = delete;

  /// Initializes the Telecom Log Service.
  int init (int argc, ACE_TCHAR * argv[]) override;

  /// Runs the Telecom Log Service.
  /// @return 0 on success, -1 on error.
  int run ();

  /// Shuts down the Telecom Log Service.
  void shutdown ();

protected:
  /// Initializes the ORB.
  void init_ORB (int & argc, ACE_TCHAR * argv []);

  /// Parses the command-line arguments.
  int parse_args (int argc, ACE_TCHAR * argv[]);

  /// Resolves the naming service.
  void resolve_naming_service ();

  /// Runs worker threads.
  int svc () override;

private:
  /// @name Data members
  /// @{

  /// The ORB used by this service.
  CORBA::ORB_var orb_ {};

  /// The root POA.
  PortableServer::POA_var poa_ {};

  /// Keeps the factory servant alive while it is active in the POA.
  PortableServer::ServantBase_var rtevent_log_factory_owner_ {};

  /// A naming context.
  CosNaming::NamingContext_var naming_ {};

  /// The name used to bind with the NameService.
  std::string service_name_ {"RTEventLogFactory"};

  /// The name of the file used to output the factory IOR.
  std::optional<std::basic_string<ACE_TCHAR>> ior_file_name_ {};

  /// The name of the file where the process stores its PID.
  std::optional<std::basic_string<ACE_TCHAR>> pid_file_name_ {};

  /// Whether to bind to the naming service.
  bool bind_to_naming_service_ {true};

  /// Number of worker threads.
  int nthreads_ {};

  /// @}
};

#endif /* RTEVENT_LOGGING_SERVICE_H */
