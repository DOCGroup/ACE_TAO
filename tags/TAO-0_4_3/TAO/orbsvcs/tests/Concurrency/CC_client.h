// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Concurrency
//
// = FILENAME
//    CC_client.h
//
// = DESCRIPTION
//      This class implements a client used to test the CosConcurrency
//      service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "orbsvcs/CosNamingC.h"
#include "CC_tests.h"
#include "CC_naming_service.h"
#include "CC_command.h"
#include "CC_command.tab.h"

#ifndef _CC_CLIENT_H_
#define _CC_CLIENT_H_

// Stuff to be used by the command file parser
int ace_cc_yyparse(void);
int line_no = 1;
char line_buf[500];
CC_CommandList *cmdlist;
extern FILE *ace_cc_yyin;
// end of parser stuff


class CC_Client
{
  // = TITLE
  //    Defines a class that encapsulates the behaviour of a clien of
  //    the concurrency service.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the test client for
  //    the concurrency service.
public:
  // = Initialization and termination methods.
  CC_Client (void);
  // Default constructor.

  ~CC_Client (void);
  // Destructor.

  int run (void);
  // Run the test.

  int init (int argc, char **argv);
  // Initialize the test with the parameters from the command line.

private:
  int init_naming_service (void);
  // Function to initialize the naming service.

  int parse_args (void);
  // Function to parse the command line arguments.

  int read_ior (char *filename);
  // Function to read the ior from the given file.

  int run_basic_tests (void);
  // Runs the basic tests (on a single lock set). Returns CC_SUCCESS
  // upon success CC_FAIL otherwise.

  int run_extended_tests (char *params);
  // Runs the extended tests (on more lock sets). Returns CC_SUCCESS
  // upon success CC_FAIL otherwise.

  int check_extended_test_params(char *params);
  // Checks the parameters given to the run_extended_tests() function.
  // returns -1 if an error is found.

  CC_naming_service *naming_service_;
  // A pointer to the naming service used for this test.

  FILE *cc_factory_ior_file_;
  // File from which to obtain the IOR.

  char *cc_factory_key_;
  // The factory key for the lock set factory.

  ACE_HANDLE f_handle_;
  // File handle to read the IOR.

  int shutdown_;
  // Flag to tell server to shutdown.

  CORBA::ORB_var orb_;
  // Remember our orb.

  //  CosConcurrencyControl::LockSetFactory_var factory_;
  // factory pointer for the lock set. @@TAO maybe to be used when naming
  // service is not used (for simple testing)

  int argc_;
  // The number of arguments passed on the command line.

  char **argv_;
  // The arguments from the command line.

  int use_naming_service_;
  // Flag to tell the client whether to use the naming service or not
  // to find the concurrency control factory.

  int run_basic_tests_;
  // flag to tell which test should be run.

  int run_extended_tests_;
  // flag to tell which test should be run.

  char *extended_tests_params_;
  // Parameters to the extended tests. The parameters are of the
  // following format: <test>:<parm1>:<parm2>...

  int use_script_file_;
  // Flag to tell whethter to use a script file to run the tests.

  char *script_file_;
  // The script file to run

  void print_usage (void);
  // Prints out the options to the program.
};
#endif /* !defined (_CC_CLIENT_H_) */
