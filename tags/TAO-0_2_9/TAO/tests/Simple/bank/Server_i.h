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
//    A CORBA server that initializes the Account and AccountManager server
//    implementations and the ORB.
//
// = AUTHORS
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#if !defined (SERVER_I_H)
#define SERVER_I_H

#include "Bank_i.h"
#include "AccountManager_i.h"
#include "tao/TAO.h"

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"


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

  int init (int argc, char *argv[], CORBA::Environment &env);
  // Initialize the Server state - parsing arguments and waiting.

  int run (CORBA::Environment &env);
  // Run the orb.

private:
  Bank_i servant_;
  // Servant for the Account interface.

  int parse_args (void);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  int init_naming_service (CORBA::Environment &env);
  // Initialises the name server and registers the Account and AccountManager
  // objects with it.

  TAO_Naming_Server my_name_server_;
  // An instance of the name server used for registering the Account and Accoun  // tManager objects

  Bank_i *bank_impl_;
  // Implementation of the Account object.

  AccountManager_i *account_manager_impl_;
  // Implementation of the Account Manager Object

  CosNaming::NamingContext_var bank_context_;
  // Naming context for the Account and AccountManager Objects.

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the Naming Service

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* TIME_IMPL_H */
