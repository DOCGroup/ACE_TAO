#ifndef TEST_I_HH
#define TEST_I_HH
#include "testS.h"

class test_i :
public POA_test
{
public:
  /// Constructor.
  test_i (CORBA::ORB_ptr orb);

  /// Test method.
  CORBA::Long test_method () override;
  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};
#endif
