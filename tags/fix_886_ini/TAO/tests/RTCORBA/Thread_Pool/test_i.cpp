// $Id$

#include "test_i.h"
#include "tao/debug.h"

ACE_RCSID(Thread_Pools, test_i, "$Id$")

test_i::test_i (CORBA::ORB_ptr orb,
                long msec_sleep)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    nap_time_ ()
{
  this->nap_time_.msec (msec_sleep);
}

CORBA::Long
test_i::method (CORBA::Long client_id,
                CORBA::Long iteration,
                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG,
              "Request in thread %t for client %d iteration %d\n",
              client_id,
              iteration));

  ACE_OS::sleep (this->nap_time_);
  return iteration;
}

void
test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0,
                        ACE_TRY_ENV);
  ACE_CHECK;
}
