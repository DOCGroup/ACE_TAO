// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * Implementation header for the "test" IDL interface for the
 * request interceptor flow test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class test_i : public virtual POA_Test
{
public:
  /// Constructor.
  test_i (CORBA::ORB_ptr orb);

  /// The client-side test operation.
  void client_test (Test::TestScenario scenario) override;

  /// The server-side test operation.
  void server_test (Test::TestScenario scenario,
                    CORBA::ULongSeq_out myseq) override;

  /// Shutdown the ORB.
  void shutdown () override;

private:
  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif  /* TEST_I_H */
