/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests
//
// = FILENAME
//    NS_client.h
//
// = DESCRIPTION
//      This class tests the facilities to connect to the naming
//      service and to resolve the name for the concurrency service client.
//
// = AUTHORS
//      Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//      Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

class CosNaming_Client
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of the CosNaming
  //    client example.  Provides a better understanding of the logic
  //    in an object oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    CosNaming CORBA server.  All the complexity for initializing
  //    the server is hidden in the class.  Just the run() interface
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

  int resolve_name(char *c, char *n);
  // Resolves the name given on the command-line (-n option).

  void list_contents(void);
  // Lists the contents of the naming service.

  CORBA::ORB_var orb_;
  // Our ORB.

  TAO_Naming_Client my_name_client_;
  // And our Naming Client!

  int list_contents_;
  // Iterate through the naming service and list the contents.

  int resolve_name_;
  // Flag set by the -n option.

  char *name_to_resolve_;
  // Parameter given to the -n option.

  char *context_to_resolve_;
  // Parameter given to the -c option.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  int exit_later_;
  // Flag to tell server to not exit immediately.
};
