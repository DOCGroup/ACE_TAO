// $Id$

#include "test_i.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

void
Test_i::oneway_work_test (CORBA::ULong work)
{
  for (CORBA::ULong i = 0; i < work; i++)
    {
      // Eat a little CPU
      /* takes about 40.2 usecs on a 167 MHz Ultra2 */
      u_long n = 11UL;
      ACE::is_prime (n, 2, n / 2);
    }
}

void
Test_i::oneway_payload_test (const Test::data &)
{
}

void
Test_i::twoway_work_test (CORBA::ULong work)
{
  for (CORBA::ULong i = 0; i < work; i++)
    {
      // Eat a little CPU
      /* takes about 40.2 usecs on a 167 MHz Ultra2 */
      u_long n = 11UL;
      ACE::is_prime (n, 2, n / 2);
    }
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (0);
}
