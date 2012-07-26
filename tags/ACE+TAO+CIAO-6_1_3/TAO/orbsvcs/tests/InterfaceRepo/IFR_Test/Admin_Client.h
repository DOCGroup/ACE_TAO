// -*- C++ -*-

//=============================================================================
/**
 *  @file    Admin_Client.h
 *
 *  $Id$
 *
 *  This class tests the functionality of the IFR methods by inserting
 *  IR objects into the repository by hand, querying them, moving them,
 *  and destroying them.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (ADMIN_CLIENT_H)
#define ADMIN_CLIENT_H

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

const CORBA::ULong NUMBER_OF_TESTS = 11;

/**
 * @class Admin_Client
 *
 * @brief Administrating IFR Client Implementation
 *
 * Class wrapper for a client which puts the Interface Repository
 * methods through their paces.
 */
class Admin_Client
{
public:
  /// Constructor
  Admin_Client (void);

  /// Destructor
  ~Admin_Client (void);

  /// Initialize the ORB and get the IFR object reference.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Execute test code.
  int run (void);

private:
  /// The various IFR tests.
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

  /// Process the command line arguments.
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// Array of pointers to the test functions.
  void (Admin_Client::*test_array_[NUMBER_OF_TESTS])(void);

  /// Array of test names used in selection of one test.
  static const char *test_names_[];

  /// Flag to output results of IFR queries.
  CORBA::Boolean debug_;

  /// Are we running all the tests or just one?
  CORBA::Boolean all_tests_;

  /// Array index of the selected test function.
  CORBA::ULong which_test_;

  /// # of times to run each test.
  CORBA::ULong iterations_;

  /// Storage of the ORB reference.
  CORBA::ORB_var orb_;

  /// Storage of the IFR reference.
  CORBA::Repository_var repo_;
};

#endif /* ADMIN_CLIENT_H */
