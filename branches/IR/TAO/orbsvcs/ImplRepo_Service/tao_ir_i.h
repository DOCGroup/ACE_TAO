/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/ImplRepo_Service
//
// = FILENAME
//    tao_ir_i.h
//
// = DESCRIPTION
//    This class implements the Implementation Repository helper application.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (TAO_IR_I_H)
#define TAO_IR_I_H

#include "tao/corba.h"
#include "orbsvcs/ImplRepoC.h"

class TAO_IR_i
{
  // = TITLE
  //     TAO's command line helper application
  //
  // = DESCRIPTION
  //     This class talks to the IR and registers/lists/etc.
public:
  // = Constructor and destructor.
  TAO_IR_i (void);
  ~TAO_IR_i (void);

  int run ();
  // Execute client code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  enum Operation {ALL, ADD, REMOVE, UPDATE, LIST};

  void print_usage (Operation op);
  // Print out information about an operation.

  int read_ior (char *filename);
  // Function to read the server ior from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int parse_add_args (void);
  // Parses arguments specific to the ADD command.

  int argc_;
  // # of arguments on the command line.

  ASYS_TCHAR **argv_;
  // arguments from command line.

  CORBA::ORB_var orb_;
  // Remember our orb.
};

#endif /* TAO_IR_I_H */
