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
//    This class implements a simple CORBA client that access a Grid
//    server.
//
// = AUTHORS
//
// ============================================================================

# include "GridC.h"

class Client_i
{
  // = TITLE
  //    Simple Client implementation.
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

  void test_grid (void);
  // Make the grid.

private:
  int read_ior (char *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the server.

  int shutdown_;
  // Flag for server shutdown.

  CORBA::Environment env_;
  // Environment variable.

  Grid_Factory_var server_;
  // Server object ptr.

  Grid_ptr grid_;
  // Pointer to the grid

  CORBA::Short height_;
  // Height of the grid.

  CORBA::Short width_;
  // Width of the grid.

  CORBA::Short setx_;
  // Cell identifier in which a value meeds to be stored.

  CORBA::Short sety_;
  // Cell identifier in which a value meeds to be stored.

  CORBA::Long setvalue_;
  // The value that needs to be stored in the cell.

  CORBA::ORB_var orb_;
  // Remember our orb.
};
