//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Irfan Pyarali
 */
// ===================================================================

#include "testS.h"

class test_i :
  public POA_test
{
public:
  /// Constructor.
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          long msec_sleep);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  /// Test method.
  CORBA::Long method (CORBA::Long client_id,
                      CORBA::Long iteration) override;

  /// Shutdown the server.
  void shutdown () override;

  /// Our POA.
  PortableServer::POA_ptr _default_POA () override;

private:
  /// ORB.
  CORBA::ORB_var const orb_;

  /// Our POA.
  PortableServer::POA_var const poa_;

  /// Time spent in executing the upcall.
  ACE_Time_Value nap_time_ {};
};
