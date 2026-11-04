//=============================================================================
/**
 *  @file    Policy_Tester.h
 *
 *   This class verifies that the policy are correctly embedded
 *   in the IOR.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef POLICY_TESTER_H_
#define POLICY_TESTER_H_

// -- App. Specific Include --
#include "RT_Properties.h"

// -- TAO Include --
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include <memory>

class Policy_Tester
{
public:
  // Ctor/Dtor.
  Policy_Tester () = default;
  Policy_Tester (Policy_Tester const &) = delete;
  Policy_Tester (Policy_Tester &&) = delete;
  Policy_Tester &operator= (Policy_Tester const &) = delete;
  Policy_Tester &operator= (Policy_Tester &&) = delete;

  /// Runs the test.
  void run ();

  int init (int argc,
            ACE_TCHAR *argv[]);

  void shutdown ();

private:
  // Helper method used internally.
  int create_objects ();

  CORBA::Boolean check_reference (CORBA::Object_ptr object,
                                  char const *msg);

  RTCORBA::RTORB_var rt_orb_ {};
  CORBA::ORB_var orb_ {};
  PortableServer::POA_var poa_ {};
  RTPortableServer::POA_var child_poa_ {};
  std::unique_ptr<RT_Properties> rt_object_properties_ {};
  std::unique_ptr<RT_Properties> rt_poa_properties_ {};
};


#endif /* POLICY_TESTER_H_ */
