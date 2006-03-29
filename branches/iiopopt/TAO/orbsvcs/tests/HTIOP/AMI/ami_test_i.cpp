// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/AMI
//
// = FILENAME
//    ami_test_i.cpp
//
// = DESCRIPTION
//    Implementation of the AMI Test interface.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>,
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include "ami_test_i.h"
#include "tao/debug.h"

ACE_RCSID(AMI, ami_test_i, "$Id$")

AMI_Test_i::AMI_Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     number_ ((CORBA::Long) 931232),
     yadda_ ((CORBA::Long) 140474)
{
}

CORBA::Long
AMI_Test_i::foo (CORBA::Long_out out_l,
                 CORBA::Long in_l,
                 const char* in_str
                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   A::DidTheRightThing))
{
  out_l = 931233;

  //if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::foo:  %d %s\n",
              in_l,
              in_str));

  if (in_l == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Throwing Exception: A::DidTheRightThing\n"));
      ACE_THROW_RETURN (A::DidTheRightThing(), 0);
    }

  return 931234;
}



void
AMI_Test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}


CORBA::Long
AMI_Test_i::yadda (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::(get_)yadda\n"));
  return yadda_;
}


void
AMI_Test_i::yadda (CORBA::Long yadda
                   ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::(set_)yadda\n"));
  yadda_ = yadda;
}
