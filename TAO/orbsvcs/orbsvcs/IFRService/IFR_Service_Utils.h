/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    IFR_Service_Utils.h
//
// = DESCRIPTION
//      Implement wrappers useful to IFR Service clients and servers.
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu>
//    Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
//    Priyanka Gontla <pgontla@doc.ece.uci.edu>
//
// ============================================================================

#ifndef TAO_IFR_SERVICE_UTILS_H
#define TAO_IFR_SERVICE_UTILS_H
#include "ace/pre.h"

#include "tao/corba.h"
#include "tao/PortableServer/PortableServerC.h"
#include "tao/ORB.h"
#include "ifr_service_export.h"

// Forward decl;
class IFR_ServantLocator;
class TAO_IOR_Multicast;
class ACE_Configuration;
class TAO_Repository_i;

class TAO_IFRService_Export TAO_IFR_Server
{
public:
  TAO_IFR_Server (void);
  // Default constructor.
 
  int init_with_orb (int argc,
                     ACE_TCHAR *argv [],
                     CORBA::ORB_ptr orb,
                     int use_multicast_server=1 );
  // Initialize the IFR Service with the command line arguments and
  // the ORB.

  int fini (void);
  // Destroy the child POA created in <init_with_orb>

  ~TAO_IFR_Server (void);
  // Destructor.

protected:
  
  int create_poas (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Two persistent POAs, one using a servant locator.

  int create_locator (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Create a servant locator and register it with its POA.

  int open_config (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Open an ACE_Configuration of the appropriate type.

  int create_repository (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Create and initialize the repository.

  int init_multicast_server (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Enable the Interface Repository to answer multicast requests
  // for its IOR.

  CORBA::ORB_var orb_;
  // Reference to our ORB.

  PortableServer::POA_var root_poa_;
  // Root POA reference.

  PortableServer::POA_var repo_poa_;
  // The Repository's POA reference.

  PortableServer::POA_var ir_object_poa_;
  // POA reference for all other IFR objects.

  IFR_ServantLocator *servant_locator_impl_;
  // Our servant locator implementation instance.

  TAO_IOR_Multicast *ior_multicast_;
  // Event handler that responds to multicast requests.

  ACE_Configuration *config_;
  // Database for the IFR.

  TAO_Repository_i *repo_impl_;
  // The IFR implementation instance.

  CORBA_Repository_ptr repository_;
  // The Interface Repository object reference.

  CORBA::String_var ifr_ior_;
  // Interface Repository's IOR.
};

#include "ace/post.h"
#endif /* TAO_IFR_SERVICE_UTILS_H */
