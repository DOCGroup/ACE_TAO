// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    Server_i.h
//
// = DESCRIPTION
//    A CORBA server that initializes the AccountManager server
//    implementation and the ORB.
//
// = AUTHORS
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef SERVER_I_H
#define SERVER_I_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "Account_i.h"
#include "AccountManager_i.h"

class Server_i
{
  // = TITLE
  //   CORBA Server implementation.
public:
  // = Initialization and termination methods.
  Server_i (void);
  // Constructor.

  ~Server_i (void);
  // Destructor.

  int init (int argc,
            char *argv[],
            CORBA::Environment &env);
  // Initialize the Server state - parsing arguments and waiting.

  int run (CORBA::Environment &env);
  // Run the orb.

private:
  Account_i servant_;
  // Servant for the Account interface.

  int parse_args (void);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  int init_naming_service (CORBA::Environment &env);
  // Initialises the name server and registers the <AccountManager>
  // objects with it.

  TAO_Naming_Server my_name_server_;
  // An instance of the name server used for registering the
  // <AccountManager> objects.

  AccountManager_i *account_manager_impl_;
  // Implementation of the <AccountManager> object.

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the Naming Service.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* SERVER_I_H */
