/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests
//
// = FILENAME
//    client.h
//
// = DESCRIPTION
//      This class tests the facilities to connect to the naming service.
//
// = AUTHORS
//      Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include "tao/corba.h"
#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

class CosNaming_Client 
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of the CosNaming
  //    client example.  Provides a better understanding of the logic
  //    in an object-oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    CosNaming CORBA server.  All the complexity for initializing
  //    the server is hidden in the class.  Just the <run> interface
  //    is needed.
public:
  // = Initialization and termination methods.
  CosNaming_Client (void);
  // Constructor.

  ~CosNaming_Client (void);
  // Destructor.

  int run (void);
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  int parse_args (void);
  // Parses the arguments passed on the command line.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  int exit_later_;
  // Flag to tell server to not exit immediately.

  TAO_ORB_Manager orbmgr_;
  // Our ORB manager helper class.

  TAO_Naming_Client naming_client_;
  // Our naming client helper class.
};
