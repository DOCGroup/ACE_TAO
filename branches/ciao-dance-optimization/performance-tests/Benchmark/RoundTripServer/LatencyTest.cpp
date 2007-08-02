//
// $Id$
//
#include "LatencyTest.h"
#include "CIAO_common.h"

LatencyTest_Impl::LatencyTest_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}


CORBA::Long
LatencyTest_Impl::makeCall (CORBA::Long send_time)
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "Sending Time back \n"));

  return send_time;
}

void
LatencyTest_Impl::shutdown ()
{
  this->orb_->shutdown (0);
}
