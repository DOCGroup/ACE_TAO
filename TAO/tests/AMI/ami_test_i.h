// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/AMI
//
// = FILENAME
//   ami_test_i.h
//
// = AUTHOR
//   Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#ifndef TAO_AMI_TEST_I_H
#define TAO_AMI_TEST_I_H

#ifdef TAO_HAS_AMI_EXCEPTIONS
#include "ami_testS-hand.h"
#else
#include "ami_testS.h"
#endif

class AMI_Test_i : public POA_A::AMI_Test
{
  // = TITLE
  //   AMI Test implementation
  //
  // = DESCRIPTION
  //   Implements the AMI_Test interface in test.idl
  //
public:
  AMI_Test_i (CORBA::ORB_ptr orb);
  // ctor

  // The AMI_Test methods.
  CORBA::Long foo (CORBA::Long_out out_l,
                   CORBA::Long in_l,
                   const char* in_str
                   TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     A::DidTheRightThing));

  void shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Long yadda (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  void yadda (CORBA::Long yadda
              TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;

  CORBA::Long number_;

  CORBA::Long yadda_;
};


#endif /* TAO_AMI_TEST_I_H */
