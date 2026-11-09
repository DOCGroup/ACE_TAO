// -*- C++ -*-

//=============================================================================
/**
 *  @file    Admin_Client.h
 *
 *  This class tests the functionality of the IFR methods by inserting
 *  IR objects into the repository by hand, querying them, moving them,
 *  and destroying them.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#if !defined (ADMIN_CLIENT_H)
#define ADMIN_CLIENT_H

#include <array>
#include <cstddef>

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

constexpr std::size_t NUMBER_OF_TESTS = 11;

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
  Admin_Client () = default;
  ~Admin_Client () = default;

  Admin_Client (Admin_Client const&) = delete;
  Admin_Client (Admin_Client&&) = delete;
  Admin_Client& operator= (Admin_Client const&) = delete;
  Admin_Client& operator= (Admin_Client&&) = delete;

  /// Initialize the ORB and get the IFR object reference.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Execute test code.
  int run ();

private:
  /// The various IFR tests.
  void array_test ();
  void enum_test ();
  void alias_test ();
  void native_test ();
  void struct_test ();
  void union_test ();
  void exception_test ();
  void constant_test ();
  void interface_test ();
  void move_test ();
  void module_test ();

  /// Process the command line arguments.
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// Array of pointers to the test functions.
  std::array<void (Admin_Client::*)(), NUMBER_OF_TESTS> test_array_ {{
    &Admin_Client::array_test,
    &Admin_Client::enum_test,
    &Admin_Client::alias_test,
    &Admin_Client::native_test,
    &Admin_Client::struct_test,
    &Admin_Client::union_test,
    &Admin_Client::exception_test,
    &Admin_Client::constant_test,
    &Admin_Client::interface_test,
    &Admin_Client::move_test,
    &Admin_Client::module_test
  }};

  /// Array of test names used in selection of one test.
  static constexpr std::array<char const*, NUMBER_OF_TESTS> test_names_ {{
    "array",
    "enum",
    "alias",
    "native",
    "struct",
    "union",
    "exception",
    "constant",
    "interface",
    "move",
    "module"
  }};

  /// Flag to output results of IFR queries.
  bool debug_ {};

  /// Are we running all the tests or just one?
  bool all_tests_ {true};

  /// Array index of the selected test function.
  std::size_t which_test_ {};

  /// # of times to run each test.
  CORBA::ULong iterations_ {1};

  /// Storage of the ORB reference.
  CORBA::ORB_var orb_ {};

  /// Storage of the IFR reference.
  CORBA::Repository_var repo_ {};
};

#endif /* ADMIN_CLIENT_H */
