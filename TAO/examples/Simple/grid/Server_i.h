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
//    A CORBA server that initializes the grid server implementation
//    and the ORB.
//
// = AUTHORS
//
// ============================================================================

#if !defined (SERVER_I_H)
#define SERVER_I_H

#include "Grid_i.h"
#include "tao/TAO.h"

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
  Grid_Factory_i servant_;
  // Servant for the Grid interface.

  int parse_args (void);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* SERVER_I_H */
