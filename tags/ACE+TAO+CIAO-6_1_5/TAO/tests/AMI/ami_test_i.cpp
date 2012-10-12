
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

AMI_Test_i::AMI_Test_i (CORBA::ORB_ptr orb,
                        CORBA::Long in_l,
                        const char * in_str,
                        bool check_params)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     number_ ((CORBA::Long) 931232),
     yadda_ ((CORBA::Long) 140474),
     in_l_ (in_l),
     in_str_(CORBA::string_dup(in_str)),
     check_params_(check_params)
{
}

CORBA::Long
AMI_Test_i::foo (CORBA::Long_out out_l,
                 CORBA::Long in_l,
                 const char* in_str)
{
  out_l = 931233;

  //if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::foo:  %d %C\n",
              in_l,
              in_str));

  if (in_l == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Throwing Exception: A::DidTheRightThing\n"));
      throw A::DidTheRightThing(42, "Hello world");
    }

  if (check_params_)
    {
      if (in_l_ != in_l || ACE_OS::strcmp(in_str_.in (), in_str) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Parameter corruption on in parameters: %d %d %C %C.\n",
                      in_l_, in_l, in_str_.in (), in_str));
          return 0;
        }
    }

  return 931234;
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
  return yadda_;
}

void
AMI_Test_i::yadda (CORBA::Long yadda)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::(set_)yadda\n"));
  yadda_ = yadda;
}

void
AMI_Test_i::inout_arg_test (char *&)
{
  // No action, this operation is to test code generation for INOUT
  // args in AMI operations.
}

