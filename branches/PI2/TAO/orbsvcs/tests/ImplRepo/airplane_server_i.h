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

#include "tao/corba.h"
#include "airplane_i.h"

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

  int init (int argc, char **argv, CORBA::Environment &ACE_TRY_ENV);
  // Initialize the Server state - parsing arguments and waiting

  int run (CORBA::Environment &ACE_TRY_ENV);
  // Run the orb 

private:
  int parse_args (void);
  // Parses the commandline arguments.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.

  CORBA::ORB_var orb_;
  // The ORB.

  PortableServer::POA_var airplane_poa_;
  // The POA for the Airplane Server.

  PortableServer::POAManager_var poa_manager_;
  // The POA manager.

  Airplane_i *server_impl_;
  // The Paper Airplane Server Implementation.  

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.
};

#endif /* AIRPLANE_SERVER_I_H */
