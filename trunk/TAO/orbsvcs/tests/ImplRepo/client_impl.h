// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    client_impl.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client.
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#include "Simple_ObjectC.h"
#include "tao/corba.h"

class Client_i
{
  // = TITLE
  //     Simple Client Implementation
  //
  // = DESCRIPTION
  //     Class wrapper for a client which gets the server IOR and then makes
  //     several calls to the server before optionally shutting it down.
public:
  // = Constructor and destructor.
  Client_i (void);
  ~Client_i (void);

  int run ();
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  int read_ior (char *filename);
  // Function to read the server ior from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  void cube_long (int i);
  // calculate the cube from a long.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *server_key_;
  // Key of the obj ref of the server.

  u_int loop_count_;
  // Number of times to do the cube operations.

  int shutdown_;
  // Flag for server shutdown.

  CORBA::Environment env_;
  // Environment variable.

  simple_object_ptr server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // Remember our orb.
};
