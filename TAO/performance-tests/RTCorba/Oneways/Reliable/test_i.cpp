// $Id$

#include "test_i.h"
#include "ace/ACE.h"

ACE_RCSID(Reliable, test_i, "$Id$")

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

void
Test_i::oneway_work_test (CORBA::ULong work
                          TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
Test_i::oneway_payload_test (const Test::data &
                             TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Test_i::twoway_work_test (CORBA::ULong work
                          TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
Test_i::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED TAO_ENV_SINGLE_ARG_PARAMETER)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        TAO_ENV_ARG_PARAMETER);
}
