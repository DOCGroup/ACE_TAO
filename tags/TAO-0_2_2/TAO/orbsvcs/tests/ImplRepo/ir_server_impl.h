// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    ir_server_impl.h
//
// = DESCRIPTION
//    Class which implements a CORBA server which sets up the simple object
//    implementation and the orb.
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (IR_SERVER_IMPL_H)
#define IR_SERVER_IMPL_H

#include "ir_simple_object_impl.h"
#include "tao/TAO.h"

class IR_Server_Impl
{
  // = TITLE
  //   CORBA Server Implementation
  //
  // = DESCRIPTION
  //   
public:
  // = Initialization and termination methods.
  IR_Server_Impl (void);
  // Default constructor

  ~IR_Server_Impl (void);
  // Destructor

  int init (int argc, char **argv, CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and waiting

  int run (CORBA::Environment& env);
  // Run the orb 

private:
  IR_Simple_Impl *server_impl_;

  int parse_args (void);
  // Parses the commandline arguments.

  int read_ior (char *filename);
  // Reads the IOR of the real server from the file

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  char *server_key_;
  // Key of the obj ref of the server.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* IR_SERVER_IMPL_H */
