// $Id$

#include "ProxySupplier.h"
//#include "Method_Request.h"
#include "Method_Request_Dispatch.h"

ACE_INLINE TAO_SYNCH_MUTEX*
TAO_Notify_Consumer::proxy_lock (void)
{
  return &this->proxy_->lock_;
}

ACE_INLINE TAO_Notify_ProxySupplier*
TAO_Notify_Consumer::proxy_supplier (void)
{
  return this->proxy_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Consumer::is_suspended (void)
{
  return this->is_suspended_;
}

ACE_INLINE void
TAO_Notify_Consumer::suspend (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->is_suspended_ = 1;
}
