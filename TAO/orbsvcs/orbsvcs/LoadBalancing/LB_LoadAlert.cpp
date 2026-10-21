#include "orbsvcs/LoadBalancing/LB_LoadAlert.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/LoadBalancing/LB_LoadAlert.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_LoadAlert::TAO_LB_LoadAlert ()
  : alerted_ (false),
    lock_ ()
{
}

TAO_LB_LoadAlert::~TAO_LB_LoadAlert () = default;

void
TAO_LB_LoadAlert::enable_alert ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

  this->alerted_ = true;
}

void
TAO_LB_LoadAlert::disable_alert ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

  this->alerted_ = false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
