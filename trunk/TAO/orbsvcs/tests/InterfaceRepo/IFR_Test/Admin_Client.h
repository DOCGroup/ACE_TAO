// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/InterfaceRepo
//
// = FILENAME
//    Admin_Client.h
//
// = DESCRIPTION
//    This class tests the functionality of the IFR methods by inserting
//    IR objects into the repository by hand, querying them, moving them,
//    and destroying them.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (ADMIN_CLIENT_H)
#define ADMIN_CLIENT_H

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

const CORBA::ULong NUMBER_OF_TESTS = 11;

class Admin_Client
{
  // = TITLE
  //     Administrating IFR Client Implementation
  //
  // = DESCRIPTION
  //     Class wrapper for a client which puts the Interface Repository
  //     methods through their paces.
public:
  Admin_Client (void);
  // Constructor

  ~Admin_Client (void);
  // Destructor

  int init (int argc,
            ACE_TCHAR *argv[]);
  // Initialize the ORB and get the IFR object reference.

  int run (void);
  // Execute test code.

private:
  void array_test (void);
  void enum_test (void);
  void alias_test (void);
  void native_test (void);
  void struct_test (void);
  void union_test (void);
  void exception_test (void);
  void constant_test (void);
  void interface_test (void);
  void move_test (void);
  void module_test (void);
  // The various IFR tests.

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);
  // Process the command line arguments.

  void (Admin_Client::*test_array_[NUMBER_OF_TESTS])(void);
  // Array of pointers to the test functions.

  static const char *test_names_[];
  // Array of test names used in selection of one test.

  CORBA::Boolean debug_;
  // Flag to output results of IFR queries.

  CORBA::Boolean all_tests_;
  // Are we running all the tests or just one?

  CORBA::ULong which_test_;
  // Array index of the selected test function.

  CORBA::ULong iterations_;
  // # of times to run each test.

  CORBA::ORB_var orb_;
  // Storage of the ORB reference.

  CORBA::Repository_var repo_;
  // Storage of the IFR reference.
};

#endif /* ADMIN_CLIENT_H */
