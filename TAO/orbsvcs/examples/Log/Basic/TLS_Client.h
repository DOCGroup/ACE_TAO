/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   TLS_Client.h
 *
 *  An example of using the Basic_Logging_Service
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef TLS_CLIENT_H
#define TLS_CLIENT_H

#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

/**
 * @brief Telecom Log Service client.
 *
 * Demonstrates how to use the basic logging service.
 */
class TLS_Client
{
public:
  TLS_Client () = default;

  TLS_Client (TLS_Client const &) = delete;
  TLS_Client (TLS_Client &&) = delete;
  TLS_Client &operator= (TLS_Client const &) = delete;
  TLS_Client &operator= (TLS_Client &&) = delete;

  /// Initializes the client.
  void init (int argc, ACE_TCHAR *argv []);

  /// Runs the tests.
  void run_tests ();

 protected:
  /// Gets hold of a running naming service.
  void resolve_naming_service ();

  /// Resolves the TLS factory from the Naming service.
  void resolve_TLS_Basic_factory ();

  /// The ORB used by this client.
  CORBA::ORB_var orb_;

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// The basic log factory from the Log Service.
  DsLogAdmin::BasicLogFactory_var basic_log_factory_;
};
#endif /* TLS_CLIENT_H */
