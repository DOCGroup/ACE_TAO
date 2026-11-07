
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test implementation.
 */
class test_i : public POA_test
{
public:
  /// ctor.
  explicit test_i (CORBA::ORB_ptr orb);
  test_i (test_i const&) = delete;
  test_i (test_i&&) = delete;
  test_i& operator= (test_i const&) = delete;
  test_i& operator= (test_i&&) = delete;

  // = The test interface methods.
  CORBA::ULong method (CORBA::ULong work) override;

  void shutdown () override;

private:
  /// The ORB.
  CORBA::ORB_var const orb_;
};
