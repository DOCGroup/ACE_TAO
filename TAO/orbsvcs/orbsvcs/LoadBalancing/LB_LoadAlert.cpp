#include "orbsvcs/LoadBalancing/LB_LoadAlert.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/LoadBalancing/LB_LoadAlert.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_LoadAlert::TAO_LB_LoadAlert ()
  : alerted_ (0),
    lock_ ()
{
}

TAO_LB_LoadAlert::~TAO_LB_LoadAlert ()
{
}

void
TAO_LB_LoadAlert::enable_alert ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

  this->alerted_ = 1;
}

void
TAO_LB_LoadAlert::disable_alert ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

  this->alerted_ = 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
