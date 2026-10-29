
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_BUFFERED_AMI_TEST_I_H
#define TAO_BUFFERED_AMI_TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple implementation.
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
  void method (CORBA::ULong request_number,
               CORBA::ULong_out reply_number) override;

  void shutdown () override;

private:
  /// The ORB.
  CORBA::ORB_var orb_;
};

#endif /* TAO_BUFFERED_AMI_TEST_I_H */
