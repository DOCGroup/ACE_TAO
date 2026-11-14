// -*- C++ -*-

//=============================================================================
/**
 * @file    test_i.h
 *
 * Implementation header for the "test" IDL interface for the
 * PortableInterceptor::Current test.
 *
 * @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

#include "tao/PortableInterceptorC.h"
#include "tao/PI/PI.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class test_i : public virtual POA_PICurrentTest::test
{
public:
  test_i (PortableInterceptor::Current_ptr current,
          PortableInterceptor::SlotId id,
          CORBA::ORB_ptr orb);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  ~test_i () override = default;

  /// Main servant test method.
  void invoke_me () override;

  /// No-op method used so that a client request interceptor will be
  /// invoked when invoking this method from the above invoke_me()
  /// method.  Say that 10 times fast. :-)
  void invoke_you () override;

  /// Test just a set slot
  void invoke_we () override;

  /// Shutdown the ORB.
  void shutdown () override;

private:
  /// Reference to the PICurrent object.
  PortableInterceptor::Current_var current_;

  /// SlotId in the PICurrent object assigned to this test.
  PortableInterceptor::SlotId const slot_id_;

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif  /* TEST_I_H */
