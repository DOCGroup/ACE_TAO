//
// $Id$
//
#include "LatencyTest.h"
#include "ciao/Logger/Log_Macros.h"

LatencyTest_Impl::LatencyTest_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}


CORBA::Long
LatencyTest_Impl::makeCall (CORBA::Long send_time)
{
  CIAO_DEBUG ((LM_DEBUG, CLINFO"Sending Time back\n"));

  return send_time;
}

void
LatencyTest_Impl::shutdown ()
{
  this->orb_->shutdown (0);
}
