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
//      This class implements a client used to test the CosConcuurency
//      service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "orbsvcs/orbsvcs/CosConcurrencyControlC.h"

class CC_Client
{
  // = TITLE
  //    Defines a class that encapsulates the behaviour of a clien of the
  //    concurrency service.
  //
  // = DESCRIPTION
  //    This class declares an interface to run the test client for the
  //    concurrency service.
  //
public:
  CC_Client(void);
  // Default constructor

  ~CC_Client(void);

  int run(void);
  // Run the test

  int init(int argc, char **argv);
  // Initialize the test with the parameters from the command line

private:
  int init_naming_service(void);
  // Function to initialize the naming service.

  int parse_args(void);
  // Function to parse the command line arguments

  int read_ior(char *filename);
  // Function to read the ior from the given file

  FILE *cc_factory_ior_file_;
  // File from which to obtain the IOR.

  ACE_HANDLE f_handle_;
  // File handle to read the IOR.

  char *cc_factory_key_;
  // Key of factory obj ref.

  int shutdown_;
  // Flag to tell server to shutdown.

  CosConcurrencyControl::LockSet_ptr cc_lock_set_;
  // Pointer to the lock set

  CORBA::ORB_var orb_;
  // Remember our orb.

  CosConcurrencyControl::LockSetFactory_var factory_;
  // factory pointer for the lock set.

 int argc_;
  // The number of arguments passed on the command line

  char **argv_;
  // The arguments from the command line

  int use_naming_service_;
  // Flag to tell the client whether to use the naming service or not
  // to find the concurrency control factory.
};
