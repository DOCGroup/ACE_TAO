//
// $Id$ 
//
#include "LatencyTest.h"

LatencyTest_Impl::LatencyTest_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}


CORBA::Long
LatencyTest_Impl::makeCall (CORBA::Long send_time
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Sending Time back \n"));
  return send_time;
}

void
LatencyTest_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
