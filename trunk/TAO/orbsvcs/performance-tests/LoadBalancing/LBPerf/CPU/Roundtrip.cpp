//
// $Id$
//
#include "Roundtrip.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"

ACE_RCSID(LoadBalancing,
          Roundtrip,
          "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    count_ (0)
{
}

Test::Timestamp
Roundtrip::test_method (Test::Timestamp send_time,
                        Test::number  cl_number)
{
  if (cl_number % 2 == 0)
  {
    for (int i = 1; i < 10000; i++)
    {
      (void) ACE::is_prime (900090, 2, 400090);
    }
  }
  else
  {
    for (int i = 1; i < 5000; i++)
    {
      (void) ACE::is_prime (900090, 2, 400090);
    }
  }

  return send_time;
}

void
Roundtrip::shutdown (void)
{
  this->orb_->shutdown (0);
}
