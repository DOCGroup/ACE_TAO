// $Id$

// ========================================================================
//
// = LIBRARY
//    orbsvcs/IFR_Service
//
// = FILENAME
//    IFR_Service.h
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =======================================================================

#ifndef IFR_SERVICE_H
#define IFR_SERVICE_H

#include "tao/PortableServer/PortableServerC.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class IFR_ServantLocator;
class TAO_IOR_Multicast;
class ACE_Configuration;
class TAO_Repository_i;

class IFR_Service
{
  // = TITLE
  //    IFR_Service
  //
  // = DESCRIPTION
  //    A class that initializes, runs and shuts down
  //    the Interface Repository service.
public:

  IFR_Service (void);
  // Default constructor.

  ~IFR_Service (void);
  // Destructor

  int init (int argc,
            char *argv[],
            CORBA::Environment &ACE_TRY_ENV);
  // Initialize the IFR service.

  int run (CORBA::Environment &ACE_TRY_ENV);
  // Run the IFR service.

  int fini (CORBA::Environment &ACE_TRY_ENV);
  // Shut down the IFR service.

protected:
  int create_poas (CORBA::Environment &ACE_TRY_ENV);
  // Two persistent POAs, one using a servant locator.

  int create_locator (CORBA::Environment &ACE_TRY_ENV);
  // Create a servant locator and register it with its POA.

  int open_config (CORBA::Environment &ACE_TRY_ENV);
  // Open an ACE_Configuration of the appropriate type.

  int create_repository (CORBA::Environment &ACE_TRY_ENV);
  // Create and initialize the repository.

  int init_multicast_server (CORBA::Environment &ACE_TRY_ENV);
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

#endif /* IFR_SERVICE_H */
