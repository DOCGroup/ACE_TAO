// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls
//
// = FILENAME
//    client.h
//
// = DESCRIPTION
//
//
// = AUTHORS
//    Aniruddha Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#ifndef TAO_NUC_CLIENT_H
#define TAO_NUC_CLIENT_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "reactor_i.h"
#include "ace/Auto_Ptr.h"

class NestedUpCalls_Client
{
  // = TITLE
  //    Defines a class that encapsulates behaviour of the NestedUpCalls client
  //    example.  Provides a better understanding of the logic in an
  //    object oriented way.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    NestedUpCalls CORBA server.  All the complexity for initializing the
  //    server is hidden in the class.  Just the run() interface is needed.
public:
  // = Constructor and destructor.
  NestedUpCalls_Client (void);
  ~NestedUpCalls_Client (void);

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

  const char *nested_up_calls_reactor_key_;
  // Key of reactor obj ref.

  int shutdown_;
  // Flag to tell server to shutdown.

  u_int call_count_;
  // # of calls made to functions.

  int quiet_;
  // The test is quiet...

  CORBA::ORB_var orb_;
  // Remember our orb.

  Reactor_var reactor_;
  // reactor pointer for NestedUpCalls.

  FILE *nested_up_calls_reactor_ior_file_;
  // File from which to obtain the IOR.

  ACE_HANDLE f_handle_;
  // File handle to read the IOR.

  int use_naming_service_;
  // Flag to tell client not to use Namingservice to find the NestedUpCalls
  // reactor.
};

#endif /* TAO_NUC_CLIENT_H */
