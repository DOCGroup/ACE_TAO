// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class test_i : public virtual POA_TestModule::test
{
public:
  /// Constructor.
  test_i ();

  /// Destructor.
  ~test_i ();

private:
};

#endif  /* TEST_I_H */
