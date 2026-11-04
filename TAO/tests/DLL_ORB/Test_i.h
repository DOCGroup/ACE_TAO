// -*- C++ -*-

//=============================================================================
/**
 * @file Test_i.h
 *
 * Implementation header for the "Test" IDL interface for the DLL ORB
 * test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TEST_I_H
#define TEST_I_H

#include "TestS.h"

/**
 * @class Test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "Test" interface used in this test.
 */
class Test_i : public virtual POA_Test
{
public:
  /// Constructor.
  Test_i ();
  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  /// Simple two-way operation used to excercise the ORB transport
  /// internals.
  void invoke_me () override;

  /// Shutdown the ORB.
  void shutdown () override;

  /// Set the ORB to be shutdown by the shutdown() method in this
  /// class.
  void orb (CORBA::ORB_ptr orb);

private:
  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_ {};
};

#endif  /* TEST_I_H */
