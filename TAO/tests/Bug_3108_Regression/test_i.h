#ifndef TEST_I_HH
#define TEST_I_HH
#include "testS.h"

class test_i :
public POA_test
{
public:
  /// Constructor.
  explicit test_i (CORBA::ORB_ptr orb);
  test_i (test_i const &) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i &&) = delete;

  /// Test method.
  CORBA::Long test_method () override;
  void shutdown () override;

private:
  CORBA::ORB_var const orb_;
};
#endif
