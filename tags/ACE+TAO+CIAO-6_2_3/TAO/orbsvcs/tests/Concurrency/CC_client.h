
//=============================================================================
/**
 *  @file    CC_client.h
 *
 *  $Id$
 *
 *    This class implements a client used to test the CosConcurrency
 *    service.
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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


/**
 * @class CC_Client
 *
 * @brief Defines a class that encapsulates the behaviour of a clien of
 * the concurrency service.
 *
 * This class declares an interface to run the test client for
 * the concurrency service.
 */
class CC_Client
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  CC_Client (void);

  /// Destructor.
  ~CC_Client (void);

  /// Run the test.
  int run (void);

  /// Initialize the test with the parameters from the command line.
  int init (int argc, ACE_TCHAR **argv);

private:
  /// Function to initialize the naming service.
  int init_naming_service (void);

  /// Function to parse the command line arguments.
  int parse_args (void);

  /// Function to read the ior from the given file.
  int read_ior (ACE_TCHAR *filename);

  /// Runs the basic tests (on a single lock set). Returns CC_SUCCESS
  /// upon success CC_FAIL otherwise.
  int run_basic_tests (void);

  /// Runs the extended tests (on more lock sets). Returns CC_SUCCESS
  /// upon success CC_FAIL otherwise.
  int run_extended_tests (ACE_TCHAR *params);

  /// Checks the parameters given to the run_extended_tests() function.
  /// returns -1 if an error is found.
  int check_extended_test_params(ACE_TCHAR *params);

  /// A pointer to the naming service used for this test.
  CC_naming_service *naming_service_;

  /// File from which to obtain the IOR.
  FILE *cc_factory_ior_file_;

  /// The factory key for the lock set factory.
  char *cc_factory_key_;

  /// File handle to read the IOR.
  ACE_HANDLE f_handle_;

  /// Flag to tell server to shutdown.
  int shutdown_;

  /// Remember our orb.
  CORBA::ORB_var orb_;

  //  CosConcurrencyControl::LockSetFactory_var factory_;
  // factory pointer for the lock set. @@TAO maybe to be used when naming
  // service is not used (for simple testing)

  /// The number of arguments passed on the command line.
  int argc_;

  /// The arguments from the command line.
  ACE_TCHAR **argv_;

  /// Flag to tell the client whether to use the naming service or not
  /// to find the concurrency control factory.
  int use_naming_service_;

  /// flag to tell which test should be run.
  int run_basic_tests_;

  /// flag to tell which test should be run.
  int run_extended_tests_;

  /// Parameters to the extended tests. The parameters are of the
  /// following format: <test>:<parm1>:<parm2>...
  ACE_TCHAR *extended_tests_params_;

  /// Flag to tell whethter to use a script file to run the tests.
  int use_script_file_;

  /// The script file to run
  ACE_TCHAR *script_file_;

  /// Prints out the options to the program.
  void print_usage (void);
};
#endif /* !defined (_CC_CLIENT_H_) */
