// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client that access a Echo
//    server.
//
// = AUTHORS
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "EchoC.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

#include <string.h>

class Client_i
{
  // = TITLE
  //     Echo client implementation.
  //
  // = DESCRIPTION
  //     Class wrapper for a client that gets the server IOR and then
  //     makes several calls to the server before optionally shutting
  //     it down.
public:
  // = Initialization and termination methods.
  Client_i (void);
  // Constructor.

  ~Client_i (void);
  // Destructor.

  int run (void);
  // Execute client example code.

  int init (int argc, char *argv[]);
  // Initialize the client communication endpoint with server.

private:
  int read_ior (char *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int via_naming_service(void);
  // This method initialises the naming service
  // and registers the object with the POA.

  void echo (const char *message);
  // Displays the message on the screen.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the server.

  u_int loop_count_;
  // Number of times to invoke the <echo> operation.

  int shutdown_;
  // Flag for server shutdown.

  CORBA::Environment env_;
  // Environment variable.

  TAO_Naming_Client naming_srvs_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  int use_naming_srv_;
  // This variable denotes whether the naming service
  // is used or not.

  Echo_var server_;
  // Server object reference.

  CORBA::ORB_var orb_;
  // Remember our orb.
};
