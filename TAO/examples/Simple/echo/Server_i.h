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
//    A CORBA server that initializes the echo server implementation
//    and the ORB.
//
// = AUTHORS
//   Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#if !defined (SERVER_I_H)
#define SERVER_I_H

#include "Echo_i.h"
#include "EchoS.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingS.h"
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
  int parse_args (void);
  // Parses the commandline arguments.

  int thru_naming_service (CORBA::Environment &env);
  // Initialises the name server and registers the Echo server object
  // name with the name server.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  Echo_i servant_;
  // Servant for the Echo interface.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the naming service.

  TAO_Naming_Server naming_server_;
  // An instance of the name server, wherein the naming context
  // containg the factory of objects will be registered.

  int using_naming_srv_;
  // This specifies whether the naming service is to be used.
};

#endif /* ECHO_IMPL_H */
