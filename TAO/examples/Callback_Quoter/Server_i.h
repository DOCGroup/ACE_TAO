/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Server_i.h
//
// = DESCRIPTION
//    Definition of the Callback_Quoter Server_i class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#if !defined (SERVER_I_H)
#define SERVER_I_H

#include "Supplier_i.h"
#include "SupplierS.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class Server_i
{
  // = TITLE
  //    The class defines the callback quoter server. It sets up the Orb
  //    manager and registers the Supplier servant object.

public:
  // = Initialization and termination methods.
  Server_i (void);
  // Constructor.

  ~Server_i (void);
  // Destructor.

  int init (int argc,
	    char *argv[],
	    CORBA::Environment &env);
  // Initialize the server.

  int run (CORBA::Environment &env);
  // Run the ORB.

private:
  int parse_args (void);
  // Parses the command line arguments.

  int init_naming_service (CORBA::Environment &env);
  // Initialises the name server and registers the Echo server object
  // name with the name server.

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

  TAO_ORB_Manager orb_manager_;
  // The tao orb manager object.

  Supplier_i supplier_i_;
  // The servant object registered with the orb.

  int using_naming_service_;
  // This specifies whether the naming service is to be used.
};

#endif /* SERVER_I_H */
