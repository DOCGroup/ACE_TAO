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
#include "tao/RTCORBA/RTCORBA.h"

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:
  /// Constructor.
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          RTCORBA::Priority low_priority,
          RTCORBA::Priority high_priority);

  /// Initialize the server.
  void initialize (CORBA::ULong total_iterations
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Test method.
  void method (CORBA::ULong work,
               const char *iteration
               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Our POA.
  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

  /// Work (burn CPU).
  void work (CORBA::ULong work);

  /// Estimates work iterations performed per second.
  int estimate_iterations (void);

private:
  /// ORB.
  CORBA::ORB_var orb_;

  /// Our POA.
  PortableServer::POA_var poa_;

  /// Value for low priority.
  RTCORBA::Priority low_priority_;

  /// Value for high priority.
  RTCORBA::Priority high_priority_;

  // RTCORBA Current.
  RTCORBA::Current_var rt_current_;

  /// Number of work iterations that can be performed in one second.
  int work_iterations_in_one_sec_;

  /// Prime number used for burning CPU.
  CORBA::ULong prime_number_;

  /// Current iteration.
  CORBA::ULong current_iteration_;

  /// Total iterations for this test.
  CORBA::ULong total_iterations_;
};
