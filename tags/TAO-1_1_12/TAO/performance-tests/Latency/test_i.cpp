// $Id$

#include "test_i.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Latency, test_i, "$Id$")

CORBA::ULongLong
Test_i::test_method (CORBA::ULongLong stamp,
                     CORBA::Environment&) ACE_THROW_SPEC (())
{
  for (int i = 0; i != this->workload_; ++i)
    {
      ACE_Time_Value tv (0, 0);
      const int n = 37;
      ACE::is_prime (n, 2, n / 2);
      ACE_OS::sleep (tv);
    }
  return stamp;
}

void
Test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
