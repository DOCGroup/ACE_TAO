/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/bin/Logger
//
// = FILENAME
//    clnt.h
//
// = DESCRIPTION
//    This program tests an implementation of a logger service.  It uses the
//    Logger_Factory server to create a number of logger objects.  It then
//    uses their object references to test functions supported by the
//    logger server. 
//
// = AUTHORS
//      Sergio Flores-Gaitan
//
// ============================================================================


#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/LoggerC.h"

class Logger_Client 
  // = TITLE
  //    Defines a class that encapsulates behaviour of the Logger client
  //    example.  Provides a better understanding of the logic in an 
  //    object oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    Logger CORBA server.  All the complexity for initializing the 
  //    server is hidden in the class.  Just the run() interface is needed.
{
public:
  // = Constructor and destructor.
  Logger_Client (void);
  ~Logger_Client (void);

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

  const char *logger_factory_key_;
  // Key of factory obj ref.

  char *hostname_;
  // Hostname of server.

  CORBA::ULong portnum_;
  // default port number of server.

  int exit_later_;
  // Flag to tell server to not exit immediately

  Logger_Factory_ptr factory_;
  // factory pointer for logger.

  CORBA::Object_ptr objref_;
  // storage of the factory objref
  
  CORBA::Environment env_;
  // Environment variable

  CORBA::ORB_ptr orb_ptr_;
  // Remember our orb

  Logger_ptr logger_1_;
  // Logger obj ref

  Logger_ptr logger_2_;
  // Logger obj ref

  CosNaming::NamingContext_ptr naming_service_;
  // naming service
};
