// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
//
// = FILENAME
//    airplane_server_i.h
//
// = DESCRIPTION
//    Server that sets up the ORB and handles the registration and execution
//    of the Paper Airplane Server.
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (AIRPLANE_SERVER_I_H)
#define AIRPLANE_SERVER_I_H

#include "tao/TAO.h"
#include "airplane_i.h"
#include "IR_Helper.h"

class Airplane_Server_i
{
  // = TITLE
  //   Paper Airplane Server Implementation Class
  //
  // = DESCRIPTION
  //   Sets up everything necessary to get the Paper Airplane Server 
  //   running.
  //   
public:
  // = Initialization and termination methods.
  Airplane_Server_i (void);
  // Default constructor

  ~Airplane_Server_i (void);
  // Destructor

  int init (int argc, char **argv, CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and waiting

  int run (CORBA::Environment& env);
  // Run the orb 

private:
  Airplane_i server_impl;
  // The Paper Airplane Server Implementation.  
  
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

  IR_Helper *ir_helper_;
  // Helper class for using the IR.

  int register_with_ir_;
  // Flag for whether or not to register startup with the IR.
};

#endif /* AIRPLANE_SERVER_I_H */
