//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
// ===================================================================

#include "testS.h"

class test_i : public POA_test
{
public:
  /// Constructor.
  test_i (CORBA::ORB_ptr orb,
          long msec_sleep);

  /// Test method.
  CORBA::Long method (CORBA::Long client_id,
                      CORBA::Long iteration,
                      CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Shutdown the server.
  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// ORB.
  CORBA::ORB_var orb_;

  /// Time spent in executing the upcall.
  ACE_Time_Value nap_time_;
};
