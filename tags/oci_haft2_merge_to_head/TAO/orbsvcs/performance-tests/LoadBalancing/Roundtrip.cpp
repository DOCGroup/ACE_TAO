//
// $Id$
//
#include "Roundtrip.h"

ACE_RCSID(Single_Threaded,
          Roundtrip,
          "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  count_ = 0;
}

Test::Timestamp
Roundtrip::test_method (Test::Timestamp send_time, 
		        Test::number cl_number,
		        Test::number it_number
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  long prime_number;

  if ( it_number < 100)
  {
    printf("Client number is %d\n", cl_number);
  }

  if ((it_number % 2) == 0)
  {
    prime_number = ACE::is_prime (20000, 2, 10000); 
    return send_time;
  }
  else
  {
    return send_time;
  }
}

void
Roundtrip::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
