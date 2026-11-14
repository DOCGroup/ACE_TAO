//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Irfan Pyarali
 */
// ===================================================================

#include "testS.h"
#include "tao/RTCORBA/RTCORBA.h"

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          RTCORBA::Priority low_priority,
          RTCORBA::Priority high_priority);

  ~test_i () override = default;

  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  /// Initialize the server.
  void initialize (CORBA::ULong total_iterations) override;

  /// Test method.
  void method (CORBA::ULong work,
               char const *iteration) override;

  /// Our POA.
  PortableServer::POA_ptr _default_POA () override;

  /// Work (burn CPU).
  void work (CORBA::ULong work);

  /// Estimates work iterations performed per second.
  int estimate_iterations ();

private:
  /// ORB.
  CORBA::ORB_var orb_;

  /// Our POA.
  PortableServer::POA_var poa_;

  /// Value for low priority.
  RTCORBA::Priority const low_priority_;

  /// Value for high priority.
  RTCORBA::Priority const high_priority_;

  // RTCORBA Current.
  RTCORBA::Current_var rt_current_;

  /// Number of work iterations that can be performed in one second.
  int work_iterations_in_one_sec_ {};

  /// Prime number used for burning CPU.
  CORBA::ULong prime_number_ {9619};

  /// Current iteration.
  CORBA::ULong current_iteration_ {};

  /// Total iterations for this test.
  CORBA::ULong total_iterations_ {};
};
