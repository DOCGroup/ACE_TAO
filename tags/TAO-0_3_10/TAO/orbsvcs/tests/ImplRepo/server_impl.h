// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    server_impl.h
//
// = DESCRIPTION
//    Class which implements a CORBA server which sets up the simple object
//    implementation and the orb.
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (SERVER_IMPL_H)
#define SERVER_IMPL_H

#include "simple_object_impl.h"
#include "tao/TAO.h"

class Server_i
{
  // = TITLE
  //   CORBA Server Implementation
  //
  // = DESCRIPTION
  //   
public:
  // = Initialization and termination methods.
  Server_i (void);
  // Default constructor

  ~Server_i (void);
  // Destructor

  int init (int argc, char **argv, CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and waiting

  int run (CORBA::Environment& env);
  // Run the orb 

private:
  Simple_i server_impl;
  Ping_i ping_impl;

  int read_ir_ior (void);

  int register_with_ir (void);

  int parse_args (void);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  char *ir_server_key_;
  
  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  int register_;
  // Flag to indicate whether ImplRepo registration should take 
  // place

  int use_ir_;
  // Flag to indicate whether the IR should be used or not.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* SERVER_IMPL_H */
