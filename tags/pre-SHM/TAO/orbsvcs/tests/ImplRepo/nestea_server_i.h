// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
//
// = FILENAME
//    nestea_server_i.h
//
// = DESCRIPTION
//    Server that sets up the ORB and handles the registration and execution
//    of the Nestea Bookshelf Server.
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (NESTEA_SERVER_I_H)
#define NESTEA_SERVER_I_H

#include "tao/corba.h"
#include "nestea_i.h"

class Nestea_Server_i
{
  // = TITLE
  //   Nestea Bookshelf Server Implementation Class
  //
  // = DESCRIPTION
  //   Sets up everything necessary to get the Nestea Bookshelf Server 
  //   running.
  //   
public:
  // = Initialization and termination methods.
  Nestea_Server_i (const char *filename = "nestea.dat");
  // Default constructor

  ~Nestea_Server_i (void);
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

  PortableServer::POA_var nestea_poa_;
  // The POA for the Nestea Server.

  PortableServer::POAManager_var poa_manager_;
  // The POA manager.
  
  Nestea_i *server_impl_;
  // The Nestea Bookshelf Server Implementation.  

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.
};

#endif /* NESTEA_SERVER_I_H */
