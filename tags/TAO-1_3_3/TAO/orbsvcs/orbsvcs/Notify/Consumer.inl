// $Id$

#include "ProxySupplier.h"

ACE_INLINE TAO_SYNCH_MUTEX*
TAO_NS_Consumer::proxy_lock (void)
{
  return &this->proxy_->lock_;
}

ACE_INLINE TAO_NS_ProxySupplier*
TAO_NS_Consumer::proxy_supplier (void)
{
  return this->proxy_;
}

ACE_INLINE CORBA::Boolean
TAO_NS_Consumer::is_suspended (void)
{
  return this->is_suspended_;
}

ACE_INLINE void
TAO_NS_Consumer::suspend (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->is_suspended_ = 1;
}
