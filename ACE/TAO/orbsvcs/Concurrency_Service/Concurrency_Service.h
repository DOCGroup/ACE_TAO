// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs/Concurrency_Service/Concurency_Service
//
// = FILENAME
//    Concurrency_Service.h
//
// = DESCRIPTION
//      This class implements a subset of the functionality of the
//      CORBA Concurrency Service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#ifndef _CONCURRENCY_SERVICE_H
#define _CONCURRENCY_SERVICE_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/Concurrency/Concurrency_Utils.h"
#include "orbsvcs/Naming/Naming_Client.h"

class Concurrency_Service : public TAO_ORB_Manager
{
  // = TITLE
  //   Defines a class that encapsulates the implementation of the
  //   concurrency service.
  // = DESCRIPTION
  //   This class uses the TAO_ORB_Manager, the TAO_Naming_Client, and
  //   the TAO_Concurrency_Server classes to implement a server for
  //   the concurrency service.
public:
  // = Initialization and termination methods.
  Concurrency_Service (void);
  // Default Constructor.

  Concurrency_Service (int argc,
                       ACE_TCHAR **argv
                       ACE_ENV_ARG_DECL);
  // Constructor taking the command-line arguments.

  ~Concurrency_Service (void);
  // Destructor.

  int init (int argc,
            ACE_TCHAR **argv
            ACE_ENV_ARG_DECL);
  // Initialize the Concurrency Service with the arguments.

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Run the Concurrency_Service.

private:
  int parse_args (int argc, ACE_TCHAR** argv);
  // Parses the commandline arguments.

  int init_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Initialize the name server and register the concurency server
  // factory with it. Maybe later it will be convinient to register
  // the individual lock sets with the naming service. At present the
  // clients must keep track of the reference and pass 'by hand'

  int use_naming_service_;
  // Flag to tell wheter the naming service will be used.

  const char *ior_file_name_;
  // The name of the file where we output the IOR.

  const char *pid_file_name_;
  // The name of a file where the process stores its pid.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager

  TAO_Naming_Client naming_client_;
  // An instance of the name client used for registering the factory
  // objects.

  TAO_Concurrency_Server my_concurrency_server_;
  // An instance of the concurrency server.

  CosConcurrencyControl::LockSetFactory_var lockset_factory_;
  // Factory var to register with the naming service.

  CosNaming::NamingContext_var concurrency_context_;
  // Naming context for the concurrency service.

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the naming service.
};

#endif /* _CONCURRENCY_SERVICE_H */
