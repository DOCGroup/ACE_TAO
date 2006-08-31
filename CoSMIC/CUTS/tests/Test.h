// -*- C++ -*-

//=============================================================================
/**
 * @file    Test.h
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_TEST_H_
#define _CUTS_TEST_H_

#include "CUTS_Test_export.h"
#include <string>
#include <list>

// forward declarations
class CUTS_Unit_Test;

typedef
std::list <CUTS_Unit_Test *> CUTS_Unit_Test_List;

//=============================================================================
/**
 * @class CUTS_Test
 *
 * Base class for all test run by the testing client. This class defines
 * the necessary data/methods to provide meaningful information to the
 * testing client for executing the test and reporting messages.
 */
//=============================================================================

class CUTS_TEST_Export CUTS_Test
{
public:
  /**
   * Run the test. This involves running all the unit test.
   *
   * @param     argc      The number of command line arguments.
   * @param     arvg      The command-line arguments for the test.
   */
  virtual void run (int argc, char * argv []);

  /**
   * Get the name of the test.
   *
   * @return    NULL-terminated string.
   */
  virtual const char * name (void) const;

  /// Close the test and release all its resources.
  virtual void close (void);

  virtual size_t passed (void) const;

  virtual size_t failed (void) const;

protected:
  /**
   * Add a unit test to the test.
   *
   * @param[in]     unit_test       Pointer to the unit test.
   */
  void add_unit_test (CUTS_Unit_Test * unit_test);

  /// Initializing constructor.
  CUTS_Test (const char *);

  /// Destructor.
  virtual ~CUTS_Test (void);

  /// Name of the test.
  std::string name_;

  /// Collection of unit test.
  CUTS_Unit_Test_List unit_test_;

  /// Number of passed test.
  size_t passed_;

  /// Number of failed test.
  size_t failed_;
};

#endif  // !defined _CUTS_TEST_H_
