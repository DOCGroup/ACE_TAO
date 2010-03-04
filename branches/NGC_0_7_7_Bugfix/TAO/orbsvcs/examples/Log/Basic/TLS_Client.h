/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   TLS_Client.h
 *
 *  $Id$
 *
 *  An example of using the Basic_Logging_Service
 *
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef TLS_CLIENT_H
#define TLS_CLIENT_H

#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

class TLS_Client
{
  // = TITLE
  //   Telecom Log Service Client
  // = DESCRIPTION
  //   This client demonstates how to use the log service.
 public:
  // = Initialization and Termination
  TLS_Client (void);
  ~TLS_Client ();

  void init (int argc, ACE_TCHAR *argv []);
  // Init the Client.

  void run_tests (void);
  // Run the tests..

 protected:
  void init_ORB (int argc, ACE_TCHAR *argv []);
  // Initializes the ORB.

  void resolve_naming_service (void);
  // Try to get hold of a running naming service.

  void resolve_TLS_Basic_factory (void);
  // Try to resolve the TLS factory from the Naming service.

  // = Data Members
  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  DsLogAdmin::BasicLogFactory_var basic_log_factory_;
  // The basic log factory from the Log Service.



};
#endif /* TLS_CLIENT_H */
