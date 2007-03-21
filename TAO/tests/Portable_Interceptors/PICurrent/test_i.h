// -*- C++ -*-

//=============================================================================
/**
 * @file    test_i.h
 *
 * $Id$
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

  /// Constructor.
  test_i (PortableInterceptor::Current_ptr current,
          PortableInterceptor::SlotId id,
          CORBA::ORB_ptr orb);

  /// Destructor.
  ~test_i (void);

  /// Main servant test method.
  virtual void invoke_me (void);

  /// No-op method used so that a client request interceptor will be
  /// invoked when invoking this method from the above invoke_me()
  /// method.  Say that 10 times fast. :-)
  virtual void invoke_you (void);

  /// Test just a set slot
  virtual void invoke_we (void);

  /// Shutdown the ORB.
  virtual void shutdown (void);

private:

  /// Reference to the PICurrent object.
  PortableInterceptor::Current_var current_;

  /// SlotId in the PICurrent object assigned to this test.
  PortableInterceptor::SlotId slot_id_;

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

};

#endif  /* TEST_I_H */
