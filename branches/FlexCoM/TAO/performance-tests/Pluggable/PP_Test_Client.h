
//=============================================================================
/**
 *  @file    PP_Test_Client.h
 *
 *  $Id$
 *
 *    This class implements some basic no-op twoway and oneway requests
 *    to time the latency of the pluggable protocol implementation.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef _PP_TEST_CLIENT_H
#define _PP_TEST_CLIENT_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PP_TestC.h"

/**
 * @class PP_Test_Client
 *
 * @brief Defines a class that encapsulates behaviour of the PP_Test client
 * example.  Provides a better understanding of the logic in an
 * object oriented way.
 *
 * This class declares an interface to run the example client for
 * PP_Teset CORBA server.  All the complexity for initializing the
 * server is hidden in the class.  Just the run() interface is needed.
 */
class PP_Test_Client
{
public:
  // = Constructor and destructor.
  /// Use <testing_collocation> to control some client's behaviors.
  PP_Test_Client (int shutdown = 0);

  ~PP_Test_Client (void);

  /// Execute client example code.
  int run (void);

  /// Initialize the client communication endpoint with server.
  /// <collocation_test_ior> is used to pass in the ior file name.
  int init (int argc, ACE_TCHAR **argv);

private:
  /// Function to read the cubit factory ior from a file.
  int read_ior (ACE_TCHAR *filename);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// Oneway operation test.
  void send_oneway (void);

  /// Twoway operation test.
  void send_void (void);

  /// This method runs only the send_void() test.
  int run_void (void);

  /// This method runs only the send_oneway() test.
  int run_oneway (void);

  /// Invoke the method with <do_shutdown> != 0 to shutdown the server.
  int shutdown_server (int do_shutdown);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// Key of factory obj ref.
  ACE_TCHAR *factory_key_;

  /// Number of times to do the send operations.
  CORBA::ULong loop_count_;

  /// Flag to tell server to shutdown.
  int shutdown_;

  /// factory pointer for cubit.
  Pluggable_Test_Factory_var factory_;

  /// Pluggable_Test obj ref.
  Pluggable_Test_var objref_;

  /// # of calls made to functions.
  CORBA::ULong call_count_;

  /// # of errors incurred in the lifetime of the application.
  CORBA::ULong error_count_;

  /// File from which to obtain the IOR.
  FILE *factory_ior_file_;

  /// File handle to read the IOR.
  ACE_HANDLE f_handle_;

  /// Run only the cube_void() test.
  int only_void_;

  /// Run only the cube_oneway() test.
  int only_oneway_;
};

#endif /* _PP_TEST_CLIENT_H */
