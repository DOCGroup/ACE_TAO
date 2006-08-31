// -*- C++ -*-

//=============================================================================
/**
 * @file    Unit_Test.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_UNIT_TEST_H_
#define _CUTS_UNIT_TEST_H_

#include "CUTS_Test_export.h"
#include <string>
#include <sstream>

//=============================================================================
/**
 * @class CUTS_Unit_Test
 *
 * @brief Abstract base class (ABC) for all unit test in CUTS.
 */
//=============================================================================

class CUTS_TEST_Export CUTS_Unit_Test
{
public:
  /// Destructor.
  virtual ~CUTS_Unit_Test (void);

  /**
   * Run the unit test.
   *
   * @param[in]   argc      Number of command line arguments
   * @param[in]   argv      Commmand-line arguments for the test
   * @retval      0         The test passed.
   * @retval      > 0       The test failed.
   */
  virtual int run (int argc, char * argv []) = 0;

  /**
   * Get the name of the unit test.
   *
   * @return    NULL-terminated string.
   */
  const char * name (void) const;

  /**
   * Get the message for the unit test.
   *
   * @return    NULL-terminated string.
   */
  const char * message (void) const;

protected:
  /**
   * Initializing constructor.
   *
   * param[in]      name        Name of the test.
   */
  CUTS_Unit_Test (const char * name);

  /// Stream for generating messages.
  std::string message_;

private:
  /// Name of the unit test.
  std::string name_;
};

#endif  // !defined _CUTS_UNIT_TEST_H_
