// $Id$

#include "Roundtrip.h"
#include "ace/High_Res_Timer.h"

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , gsf_ (ACE_High_Res_Timer::calibrate ())
{
}

Test::Timestamp
Roundtrip::test_method (Test::Timestamp send_time,
                        CORBA::Long workload_in_usecs)
{
  ACE_hrtime_t start = ACE_OS::gethrtime ();
  CORBA::Long elapsed = 0;

  while (elapsed < workload_in_usecs)
    {
      elapsed =
        static_cast<CORBA::Long> (
            (ACE_OS::gethrtime () - start) / this->gsf_
          );
    }

  return send_time;
}

void
Roundtrip::shutdown (void)
{
  this->orb_->shutdown (0);
}
