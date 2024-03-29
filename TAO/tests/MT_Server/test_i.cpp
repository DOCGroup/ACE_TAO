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
    ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));
  ACE_Time_Value tv (0, 15000);
  ACE_OS::sleep (tv);
  return x;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
