// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Server, test_i, "$Id$")

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x, CORBA::Environment&)
    ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));
  ACE_Time_Value tv (0, 15000);
  ACE_OS::sleep (tv);
  return x;
}

void
Simple_Server_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC (())
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
