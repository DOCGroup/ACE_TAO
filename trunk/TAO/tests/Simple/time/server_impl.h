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
//    A CORBA server that initializes the time server implementation
//    and the ORB.
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
  //   CORBA Server implementation.
public:
  // = Initialization and termination methods.
  Server_Impl (void);
  // Constructor.

  ~Server_Impl (void);
  // Destructor.

  int init (int argc, char *argv[], CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and waiting.

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
