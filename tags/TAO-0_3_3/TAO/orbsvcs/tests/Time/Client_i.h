// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Time_Service
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client that accesses a Time Service
//    server.
//
// = AUTHORS
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/TimeServiceC.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class Client_i
{
  // = TITLE
  //     Client implementation to access the Time Service server.
  //
  // = DESCRIPTION
  //     Class wrapper for a client that gets the IOR of the clerk
  //     running on its machine and uses it to get the globally
  //     synchronized time
public:
  // = Initialization and termination methods.
  Client_i (void);
  // Constructor.

  ~Client_i (void);
  // Destructor.

  int run (void);
  // Execute client example code.

  int init (int argc, char *argv[]);
  // Initialize the client communication endpoint with server.

  void test_for_secure_universal_time (void);
  // This test throws an exception that the operation
  // <secure_universal_time> has not been currently implemented in the
  // given server.

  void test_for_universal_time (void);
  // Tests if the server returns a valid time.

  void test_for_new_universal_time (void);
  // Tests if the server returns a new UTO filled with the given time.

  void test_for_uto_from_utc (void);
  // Tests if the returned UTO has the same value as the given UTC.

  void test_for_new_interval (void);
  // Tests if a new TIO is created according to the given bounds.

private:
  int read_ior (char *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

   int obtain_initial_references (void);
  // To initialize the naming service and get a clerk IOR.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the server.

  u_int loop_count_;
  // Number of times to invoke the <time> operation.

  int shutdown_;
  // Flag for server shutdown.

  CORBA::Environment env_;
  // Environment variable.

  CosTime::TimeService_var clerk_;
  // Time Service Server object ptr.

  TAO_Naming_Client my_name_client_;
  // An instance of the name client used for resolving the Time
  // Service object.

  CORBA::ORB_var orb_;
  // Remember our orb.
};
