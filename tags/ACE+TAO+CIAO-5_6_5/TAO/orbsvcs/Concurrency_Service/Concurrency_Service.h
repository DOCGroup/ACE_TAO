
//=============================================================================
/**
 *  @file    Concurrency_Service.h
 *
 *  $Id$
 *
 *    This class implements a subset of the functionality of the
 *    CORBA Concurrency Service.
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class Concurrency_Service
 *
 * @brief Defines a class that encapsulates the implementation of the
 * concurrency service.
 *
 * This class uses the TAO_ORB_Manager, the TAO_Naming_Client, and
 * the TAO_Concurrency_Server classes to implement a server for
 * the concurrency service.
 */
class Concurrency_Service : public TAO_ORB_Manager
{
public:
  // = Initialization and termination methods.
  /// Default Constructor.
  Concurrency_Service (void);

  /// Constructor taking the command-line arguments.
  Concurrency_Service (int argc,
                       ACE_TCHAR **argv);

  /// Destructor.
  ~Concurrency_Service (void);

  /// Initialize the Concurrency Service with the arguments.
  int init (int argc,
            ACE_TCHAR **argv);

  /// Run the Concurrency_Service.
  int run (void);

private:
  /// Parses the commandline arguments.
  int parse_args (int argc, ACE_TCHAR** argv);

  /**
   * Initialize the name server and register the concurency server
   * factory with it. Maybe later it will be convinient to register
   * the individual lock sets with the naming service. At present the
   * clients must keep track of the reference and pass 'by hand'
   */
  int init_naming_service (void);

  /// Flag to tell wheter the naming service will be used.
  int use_naming_service_;

  /// The name of the file where we output the IOR.
  const char *ior_file_name_;

  /// The name of a file where the process stores its pid.
  const char *pid_file_name_;

  /// The ORB manager
  TAO_ORB_Manager orb_manager_;

  /// An instance of the name client used for registering the factory
  /// objects.
  TAO_Naming_Client naming_client_;

  /// An instance of the concurrency server.
  TAO_Concurrency_Server my_concurrency_server_;

  /// Factory var to register with the naming service.
  CosConcurrencyControl::LockSetFactory_var lockset_factory_;

  /// Naming context for the concurrency service.
  CosNaming::NamingContext_var concurrency_context_;

  /// Naming context for the naming service.
  CosNaming::NamingContext_var naming_context_;
};

#endif /* _CONCURRENCY_SERVICE_H */
