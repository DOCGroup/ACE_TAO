
//=============================================================================
/**
 *  @file    ami_test_i.cpp
 *
 *  $Id$
 *
 *  Implementation of the AMI Test interface.
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#include "ami_test_i.h"
#include "tao/debug.h"

AMI_Test_i::AMI_Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
AMI_Test_i::foo (CORBA::Long value)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):AMI_Test_i::foo:  %d\n",
              value));

  if (value == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Throwing Exception: A::DidTheRightThing\n"));
      throw A::DidTheRightThing();
    }
  else
    {
      throw A::DidTheWrongThing ();
    }
}

void
AMI_Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

