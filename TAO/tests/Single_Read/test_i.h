
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SINGLE_READ_TEST_I_H
#define TAO_SINGLE_READ_TEST_I_H

#include "ace/SString.h"
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
  test_i (CORBA::ORB_ptr orb, ACE_TString const& file_name);
  test_i (test_i const&) = delete;
  test_i (test_i&&) = delete;
  test_i& operator= (test_i const&) = delete;
  test_i& operator= (test_i&&) = delete;

  // = The test interface methods.
  void method (CORBA::ULong request_number,
               test::data const&) override;

  void shutdown () override;

private:
  /// The ORB.
  CORBA::ORB_var const orb_;

  bool client_done_ {};

  ACE_TString const client_done_file_;
};

#endif /* TAO_SINGLE_READ_TEST_I_H*/
