// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    client_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client that access a Time
//    server. 
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#include "TimeC.h"

class client_i
{
  // = TITLE
  //     Simple Client implementation.
  //
  // = DESCRIPTION
  //     Class wrapper for a client that gets the server IOR and then
  //     makes several calls to the server before optionally shutting
  //     it down.
public:
  // = Initialization and termination methods.
  client_i (void);
  // Constructor.

  ~client_i (void);
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

  void time (void);
  // Return the current time of day on another machine.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the server.

  u_int loop_count_;
  // Number of times to invoke the <time> operation.

  int shutdown_;
  // Flag for server shutdown.

  CORBA::Environment env_;
  // Environment variable.

  Time_var server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // Remember our orb.
};
