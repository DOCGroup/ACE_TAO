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

#include "Time_impl.h"
#include "tao/TAO.h"

class Server_Impl
{
  // = TITLE
  //   CORBA Server Implementation
  //
  // = DESCRIPTION
  //   
public:
  // = Initialization and termination methods.
  Server_Impl (void);
  // Default constructor

  ~Server_Impl (void);
  // Destructor

  int init (int argc, char **argv, CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and waiting

  int run (CORBA::Environment& env);
  // Run the orb 

private:
  Time_Impl server_impl;

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

#endif /* TIME_IMPL_H */
