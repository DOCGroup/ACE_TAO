// $Id$

#include "test_i.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Latency, test_i, "$Id$")

void
Test_i::test_method (CORBA::Environment&) ACE_THROW_SPEC (())
{
  ACE_Time_Value tv (0, 0);
  for (int i = 0; i != this->workload_; ++i)
    {
      const int n = 37;
      ACE::is_prime (n, 2, n / 2);
      ACE_OS::sleep (tv);
    }
}

void
Test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
