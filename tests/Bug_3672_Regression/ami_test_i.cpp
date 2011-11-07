
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
#include "ace/OS_NS_unistd.h"

AMI_Test_i::AMI_Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Long
AMI_Test_i::foo (CORBA::Long_out out_l)
{
  out_l = 931233;

  ACE_OS::sleep (10);

  return 931234;
}

void
AMI_Test_i::shutdown (void)
{
  ACE_OS::sleep (5);

  this->orb_->shutdown (0);
}

