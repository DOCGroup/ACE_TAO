// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Pluggable
//
// = FILENAME
//    PP_Test_Client.h
//
// = DESCRIPTION
//      This class implements some basic no-op twoway and oneway requests
//      to time the latency of the pluggable protocol implementation.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef _PP_TEST_CLIENT_H
#define _PP_TEST_CLIENT_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "PP_TestC.h"

class PP_Test_Client
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of the PP_Test client
  //    example.  Provides a better understanding of the logic in an
  //    object oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    PP_Teset CORBA server.  All the complexity for initializing the
  //    server is hidden in the class.  Just the run() interface is needed.
public:
  // = Constructor and destructor.
  PP_Test_Client (int shutdown = 0);
  // Use <testing_collocation> to control some client's behaviors.

  ~PP_Test_Client (void);

  int run (void);
  // Execute client example code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.
  // <collocation_test_ior> is used to pass in the ior file name.

private:
  int read_ior (char *filename);
  // Function to read the cubit factory ior from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  void send_oneway (void);
  // Oneway operation test.

  void send_void (void);
  // Twoway operation test.

  int run_void (void);
  // This method runs only the send_void() test.

  int run_oneway (void);
  // This method runs only the send_oneway() test.

  int shutdown_server (int do_shutdown);
  // Invoke the method with <do_shutdown> != 0 to shutdown the server.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  CORBA::ORB_var orb_;
  // Remember our orb.

  char *factory_key_;
  // Key of factory obj ref.

  CORBA::ULong loop_count_;
  // Number of times to do the send operations.

  int shutdown_;
  // Flag to tell server to shutdown.

  Pluggable_Test_Factory_var factory_;
  // factory pointer for cubit.

  Pluggable_Test_var objref_;
  // Pluggable_Test obj ref.

  CORBA::ULong call_count_;
  // # of calls made to functions.

  CORBA::ULong error_count_;
  // # of errors incurred in the lifetime of the application.

  FILE *factory_ior_file_;
  // File from which to obtain the IOR.

  ACE_HANDLE f_handle_;
  // File handle to read the IOR.

  int only_void_;
  // Run only the cube_void() test.

  int only_oneway_;
  // Run only the cube_oneway() test.
};

#endif /* _PP_TEST_CLIENT_H */
