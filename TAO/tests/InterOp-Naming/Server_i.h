//$Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/InterOp-Naming
//
// = FILENAME
//   Server_i.h
//
// = DESCRIPTION
//    The class is the driver for the INS test server.
//
// = AUTHOR
//   Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef SERVER_I_H
#define SERVER_I_H

#include "tao/PortableServer/ORB_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "INS_i.h"

class Server_i
{

public:
  // = Initialization and termination methods.

  Server_i (void);
  // Constructor.

  ~Server_i (void);
  // Destructor.

  int init (int argc,
            char *argv[]
            TAO_ENV_ARG_DECL);
  // Initialize the Server state - parsing arguments and waiting.

  int add_IOR_to_table (CORBA::String_var ior);
  // Add an IOR to the ORB-IOR table.

  int run (TAO_ENV_SINGLE_ARG_DECL);
  // Run the orb.

protected:

  INS_i servant_;
  // Servant class

  int parse_args (void);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager - a helper class for accessing the POA and
  // registering objects.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  char * ins_;
  // Used test the INS.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* SERVER_I_H */
