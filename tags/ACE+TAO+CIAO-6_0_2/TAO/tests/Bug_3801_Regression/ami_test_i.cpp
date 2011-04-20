
//=============================================================================
/**
 *  @file    ami_test_i.cpp
 *
 *  $Id$
 *
 *  Implementation of the AMI Test interface.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "ami_test_i.h"
#include "tao/debug.h"

AMI_Test_i::AMI_Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
AMI_Test_i::inout_arg_test (char *&)
{
  // No action, this operation is to test code generation for INOUT
  // args in AMI operations.
}

void
AMI_Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

CORBA::Long
AMI_Test_i::yadda (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::(get_)yadda\n"));
  throw ::A::DidTheRightThingB (42, "Hello world");
}

void
AMI_Test_i::yadda (CORBA::Long)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::(set_)yadda\n"));
  throw ::A::DidTheRightThing (42, "Hello world");
}

CORBA::Long
AMI_Test_i::dadda (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::(get_)dadda\n"));
  throw ::A::DidTheRightThing (42, "Hello world");
}
