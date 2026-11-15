
/**
 *  @file   test_i.h
 *
 *  @author Irfan Pyarali
 */

#ifndef TAO_LEADER_FOLLOWERS_TEST_I_H
#define TAO_LEADER_FOLLOWERS_TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test implementation.
 */
class test_i : public POA_test
{
public:
  explicit test_i (CORBA::ORB_ptr orb);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  /// @name Test interface methods
  /// @{
  CORBA::ULong method (CORBA::ULong work) override;

  void shutdown () override;
  /// @}

private:
  /// The ORB.
  CORBA::ORB_var const orb_;
};

#endif /* TAO_LEADER_FOLLOWERS_TEST_I_H */
