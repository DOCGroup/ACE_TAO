// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
//
// = FILENAME
//    airplane_client_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client which returns a random
//    paper airplane from the paper airplane server.
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (AIRPLANE_CLIENT_I_H)
#define AIRPLANE_CLIENT_I_H

#include "AirplaneC.h"
#include "tao/corba.h"

class Airplane_Client_i
{
  // = TITLE
  //     Paper Airplane Client Implementation
  //
  // = DESCRIPTION
  //     Class wrapper for a client which gets the server IOR and then makes
  //     a couple of calls to the server.
public:
  // = Constructor and destructor.
  Airplane_Client_i (void);
  ~Airplane_Client_i (void);

  int run ();
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  int read_ior (char *filename);
  // Function to read the server ior from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  void get_planes (size_t count);
  // Ask the Paper Airplane Server for <count> planes.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *server_key_;
  // Key of the obj ref of the server.

  size_t loop_count_;
  // Number of airplanes to query for.

  CORBA::Environment env_;
  // Environment variable.

  Paper_Airplane_Server_ptr server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // Remember our orb.
};

#endif /* AIRPLANE_CLIENT_I_H */
