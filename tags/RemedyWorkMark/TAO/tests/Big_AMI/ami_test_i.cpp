
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

CORBA::Long
AMI_Test_i::foo (CORBA::Long_out out_l,
                 CORBA::Long in_l,
                 const char* in_str,
                 const A::Payload &)
{
  out_l = 931233;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%N:%l:(%P:%t):AMI_Test_i::foo:  %d %s\n",
                in_l,
                in_str));

  return 931234;
}

void
AMI_Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
