//
// $Id$
//
#include "Roundtrip.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(TAO_RTEC_Perf_RTCORBA_Baseline, Roundtrip, "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , gsf_ (ACE_High_Res_Timer::calibrate ())
{
}

Test::Timestamp
Roundtrip::test_method (Test::Timestamp send_time,
                        CORBA::Long workload_in_usecs
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_hrtime_t start = ACE_OS::gethrtime ();
  CORBA::Long elapsed = 0;
  while (elapsed < workload_in_usecs)
    {
      // ACE_OS::sleep (0);
      elapsed = (ACE_OS::gethrtime () - start) / this->gsf_;
    }
  return send_time;
}

void
Roundtrip::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
