// $Id$

#include "test_i.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_unistd.h"

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));
  ACE_Time_Value tv (0, (x % 1000));
  ACE_OS::sleep (tv);
  return x;
}

void
Simple_Server_i::shutdown (void)
{
  // We dont shutdown the ORB. We allow the server to go away as it
  // wants.
  // this->orb_->shutdown (0);
}
