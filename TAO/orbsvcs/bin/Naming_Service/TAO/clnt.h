/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/bin/Naming_Service/TAO
//
// = FILENAME
//    clnt.h
//
// = DESCRIPTION
//      This class implements naming service according to the CORBA
//      spec.
//
// = AUTHORS
//      Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "CosNamingC.h"

class CosNaming_Client 
  // = TITLE
  //    Defines a class that encapsulates behaviour of the CosNaming client
  //    example.  Provides a better understanding of the logic in an 
  //    object oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    CosNaming CORBA server.  All the complexity for initializing the 
  //    server is hidden in the class.  Just the run() interface is needed.
{
public:
  // = Constructor and destructor.
  CosNaming_Client (void);
  ~CosNaming_Client (void);

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

  char *hostname_;
  // Hostname of server.

  CORBA::ULong portnum_;
  // default port number of server.

  int exit_later_;
  // Flag to tell server to not exit immediately

  CORBA::Object_ptr factory_;
  // factory pointer for CosNaming.

  CORBA::Object_ptr objref_;
  // storage of the factory objref
  
  CORBA::Environment env_;
  // Environment variable

  //  CORBA::Object_ptr CosNaming_;
  CosNaming::NamingContext_ptr CosNaming_;
  // CosNaming obj ref

  char *cosnaming_factory_key_;
  // CosNaming key.
};
