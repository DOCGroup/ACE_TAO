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

#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"
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
            char *argv[], 
            CORBA::Environment &env);
  // Initialize the Server state - parsing arguments and waiting.

  int add_IOR_to_table (CORBA::String_var ior);
  // Add an IOR to the ORB-IOR table.
  
  int run (CORBA::Environment &env);
  // Run the orb.
  
protected:

  INS_i servant_;
  // Servant class

  int parse_args (void);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager - a helper class for accessing the POA and
  // registering objects.

  TAO_Naming_Server namingServer;
  // helper class for getting access to Naming Service.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  int naming_;
  // Flag to indicate whether naming service could be used
       
  char * ins_;
  // Used test the INS.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* SERVER_I_H */
