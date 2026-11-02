// -*- C++ -*-

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
class test_i : public virtual POA_RedirectionTest::test
{
public:
  /// Constructor.
  test_i (CORBA::Short num,
          CORBA::ORB_ptr orb);

  /// Destructor.
  ~test_i () override;

  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

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
