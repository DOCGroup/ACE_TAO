// $Id$

#include "test_i.h"
#include "tao/debug.h"
#include "ace/OS_NS_unistd.h"
#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Simple_Server: Request in thread %t\n"));
  ACE_Time_Value tv (0, 15000);
  ACE_OS::sleep (tv);
  return x;
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

//---------------------------------------------------------------------------

CORBA::Long
Another_One_i::test_method (CORBA::Long x)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Another_One: Request in thread %t\n"));
  ACE_Time_Value tv (0, 15000);
  ACE_OS::sleep (tv);
  return x;
}
