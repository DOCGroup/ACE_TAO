// -*- C++ -*-
//
// $Id$
#ifndef FT_UNIT_TESTS_H_
#define FT_UNIT_TESTS_H_
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_ReplicationManagerC.h"

/**
 * Fault Tolerant CORBA unit tests
 */
class  FT_UnitTests
{
  //////////////////////
  // non-CORBA interface
public:
  /**
   * Default constructor.
   */
  FT_UnitTests (CORBA::ORB_var orb);

  /**
   * Virtual destructor.
   */
  virtual ~FT_UnitTests ();

  /**
   * Parse command line arguments.
   */
  int parse_args (int argc, char * argv[]);

  /**
   * initialize for the tests
   */
  int init ();

  /**
   * Run a unit test
   * @param the test-number of the test to run.
   * @return 0 if test ran successfully, any other 
   *         value is a failure.
   */
  int run_test (int test_number);

  /**
   * Runs all unit tests.
   * @returns 0 if all tests were sucessful, otherwise 
   *          returns the number of failed tests.
   */
  int run_all_tests ();

  /**
   * Returns the number of tests.
   */
  int test_count ();

  /**
   * Returns a description of the given test number
   */
  const char * test_desc (int test_number);

  /////////////////
  // The Tests
private:
  int test_001 (int run_test, CORBA::String_var & desc);
  int test_002 (int run_test, CORBA::String_var & desc);
  int test_003 (int run_test, CORBA::String_var & desc);
  int test_004 (int run_test, CORBA::String_var & desc);
  int test_005 (int run_test, CORBA::String_var & desc);
  int test_006 (int run_test, CORBA::String_var & desc);
  int test_007 (int run_test, CORBA::String_var & desc);
  int test_008 (int run_test, CORBA::String_var & desc);
  int test_009 (int run_test, CORBA::String_var & desc);
  int test_010 (int run_test, CORBA::String_var & desc);
  int test_011 (int run_test, CORBA::String_var & desc);

  /////////////////
  // Implementation
private:
  int readIORFile(const char * fileName, CORBA::String_var & ior);

  ///////////////
  // Data Members
private:

  // The orb
  CORBA::ORB_var orb_;

  // IOR of the replication manager.
  FT::ReplicationManager_var rm_;

  // an iogr for use in the unit tests
  CORBA::Object_var test_iogr_;

  // A list of FactoryInfos ofr the unit tests
  FT::FactoryInfos factories_;
};


#endif /* FT_UNIT_TESTS_H_  */
