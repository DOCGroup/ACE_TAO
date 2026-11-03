// -*- C++ -*-

//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_TIMED_BUFFERED_ONEWAYS_TEST_I_H
#define TAO_TIMED_BUFFERED_ONEWAYS_TEST_I_H

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
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i & operator= (test_i const &) = delete;
  test_i & operator= (test_i &&) = delete;

  // = The test interface methods.
  void method (CORBA::ULong request_number, CORBA::Long start_time,
               test::data const &, CORBA::ULong work) override;

  void flush () override;

  void shutdown (CORBA::Long start_time) override;

private:
  /// The ORB.
  CORBA::ORB_var const orb_;
};

#endif /* TAO_TIMED_BUFFERED_ONEWAYS_TEST_I_H */
