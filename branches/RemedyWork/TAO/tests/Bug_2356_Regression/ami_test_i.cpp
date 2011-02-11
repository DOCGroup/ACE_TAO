
//=============================================================================
/**
 *  @file    ami_test_i.cpp
 *
 *  $Id$
 *
 *  Implementation of the AMI Test interface.
 *
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
  if (value == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                 "Throwing Exception: A::DidTheRightThing\n"));
#if defined(ACE_HAS_WCHAR) || defined(ACE_HAS_XPG4_MULTIBYTE_CHAR)
      CORBA::WChar const* wstring = L"Hello world";
#else
      CORBA::WChar const empty[] = { 0 };
      CORBA::WChar const* wstring = empty;
#endif
      throw A::DidTheRightThing(42, wstring);
    }
}

void
AMI_Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

