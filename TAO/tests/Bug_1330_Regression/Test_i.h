// -*- C++ -*-

#if !defined BUG_1330_REGRESSION_TEST_I_H
 #define BUG_1330_REGRESSION_TEST_I_H

#include "TestS.h"

class Test_i: public POA_Test
{
public:
  /// Constructor
  explicit Test_i (CORBA::ORB_ptr orb);
  Test_i (Test_i const &) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i &&) = delete;

  /// Destructor
  ~Test_i () override = default;

  /// A method
  void test_method () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#endif /* ! BUG_1330_REGRESSION_TEST_I_H */
