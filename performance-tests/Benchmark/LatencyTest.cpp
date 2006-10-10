//
// $Id$
//
#include "LatencyTest.h"

LatencyTest::LatencyTest (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}


CORBA::Long
LatencyTest::makeCall (CORBA::Long send_time
                       ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

void
LatencyTest::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
