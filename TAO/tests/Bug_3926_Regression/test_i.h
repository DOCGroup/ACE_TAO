// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * Implementation header for the "test" IDL interface for the
 * PortableInterceptor::ForwardRequest test.
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
 * This class implements the "test" interface used in this test.
 */
class test_i : public virtual POA_hello::GoodDay
{
public:
  /// Constructor.
  test_i (CORBA::Short num,
          CORBA::ORB_ptr orb);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  /// Destructor.
  ~test_i () override = default;

  /// Return the number assigned to this object.
  CORBA::Short number () override;

  /// Shutdown the ORB.
  void shutdown () override;

private:
  /// The number assigned to this object.
  CORBA::Short number_;

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif  /* TEST_I_H */
