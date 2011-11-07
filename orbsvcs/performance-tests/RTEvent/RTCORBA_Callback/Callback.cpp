// $Id$

#include "Callback.h"
#include "Implicit_Deactivator.h"
#include "ace/OS_NS_time.h"

Callback::Callback (int iterations,
                    PortableServer::POA_ptr poa)
  : sample_history_ (iterations)
  , poa_ (PortableServer::POA::_duplicate (poa))
{
}

ACE_Sample_History &
Callback::sample_history (void)
{
  return this->sample_history_;
}

void
Callback::sample (Test::Timestamp the_timestamp)
{
  ACE_hrtime_t elapsed = ACE_OS::gethrtime () - the_timestamp;

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  if (sample_history_.max_samples () == sample_history_.sample_count ())
    return;
  this->sample_history_.sample (elapsed);
}

PortableServer::POA_ptr
Callback::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
