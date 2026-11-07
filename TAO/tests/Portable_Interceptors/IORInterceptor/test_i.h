// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * Implementation header for the "test" IDL interface for the
 * IORInterceptor test.
 *
 * @author Ossama Othman <ossama@uci.edu>
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
 * This class exists only to make it possible to generate an IOR to
 * which the test IOR interceptor will add a tagged component.
 */
class test_i : public virtual POA_FOO::test
{
public:
  /// Constructor.
  explicit test_i (CORBA::ORB_ptr orb);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  /// Shutdown the ORB.
  void shutdown () override;

private:
  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif  /* TEST_I_H */
