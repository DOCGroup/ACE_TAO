// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/NestedUpCalls
//
// = FILENAME
//   mt_client.h
//
// = AUTHORS
//   Aniruddha Gokhale, Sumedh Mungee, Sergio Flores-Gaitan, Alexander
//   Babu Arulanthu and Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_NUC_MT_CLIENT_H
#define TAO_NUC_MT_CLIENT_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "reactor_i.h"

class NestedUpCalls_MT_Client : public ACE_Task_Base
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
  NestedUpCalls_MT_Client (void);
  ~NestedUpCalls_MT_Client (void);

  int run (void);
  // Execute client example code.

  int init (int argc, char *argv[]);
  // Initialize the client communication endpoint with server.

  virtual int svc (void);
  // The task entry point

private:
  int parse_args (int argc, char* argv[]);
  // Parses the arguments passed on the command line.

private:
  char *nested_up_calls_reactor_key_;
  // Key of reactor obj ref.

  int shutdown_;
  // Flag to tell server to shutdown.

  u_int call_count_;
  // # of calls made to functions.

  int nthreads_;
  // # of threads

  CORBA::ORB_var orb_;
  // Remember our orb.

  Reactor_var reactor_;
  // reactor pointer for NestedUpCalls.
};

#endif /* TAO_NUC_MT_CLIENT_H */
