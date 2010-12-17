// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                long msec_sleep)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    nap_time_ ()
{
  this->nap_time_.msec (msec_sleep);
}

CORBA::Long
test_i::method (CORBA::Long client_id,
                CORBA::Long iteration)
{
  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources *tss =
    this->orb_->orb_core ()->get_tss_resources ();

  /// Get the lane attribute in TSS.
  TAO_Thread_Lane *lane =
    static_cast <TAO_Thread_Lane *> (tss->lane_);

  if (lane)
    ACE_DEBUG ((LM_DEBUG,
                "Request in thread %t (pool id = %d; lane id = %d) for client %d iteration %d\n",
                lane->pool ().id (),
                lane->id (),
                client_id,
                iteration));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Request in thread %t (default thread pool) for client %d iteration %d\n",
                client_id,
                iteration));

  ACE_OS::sleep (this->nap_time_);

  return iteration;
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
