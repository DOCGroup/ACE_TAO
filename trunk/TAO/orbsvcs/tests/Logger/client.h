/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/Logger
//
// = FILENAME
//    client.h
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
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of the Logger
  //    client example.  Provides a better understanding of the logic
  //    in an object oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    Logger CORBA server.  All the complexity for initializing the
  //    server is hidden in the class.  Just the run() interface is
  //    needed.
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

  void setup_record (Logger::Log_Record newrec,
		     Logger::Log_Priority lp, const char* msg);
  // Prepares the logger recors for logging
  
  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  int test_nesting_;
  // Test nested naming context or not.

  CORBA::ORB_var orb_;
  // Keep a pointer to the orb, to avoid accidental deletes.

  Logger_var logger_1_;
  // Logger obj ref

  Logger_var logger_2_;
  // Logger obj ref
};
