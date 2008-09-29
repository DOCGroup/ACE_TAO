// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO/tests/Exposed_Policies
//
// = FILENAME
//    Policy_Verifier.h
//
// = DESCRIPTION
//     This class verifies that the policy are correctly embedded
//     in the IOR.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef POLICY_TESTER_H_
#define POLICY_TESTER_H_

// -- App. Specific Include --
#include "RT_Properties.h"

// -- TAO Include --
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/RTPortableServer/RTPortableServer.h"

class Policy_Tester
{
public:
  // Ctor/Dtor.
  Policy_Tester (void);
  ~Policy_Tester (void);

  void run (void);
  // Runs the test.

  int init (int argc,
            ACE_TCHAR *argv[]);

  void shutdown (void);
private:
  // Helper method used internally.
  int create_objects (void);

  CORBA::Boolean check_reference (CORBA::Object_ptr object,
                                   const char *msg);

private:

  int is_initialized_;

  RTCORBA::RTORB_var rt_orb_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  RTPortableServer::POA_var child_poa_;
  RT_Properties *rt_object_properties_;
  RT_Properties *rt_poa_properties_;
};


#endif /* POLICY_TESTER_H_ */
