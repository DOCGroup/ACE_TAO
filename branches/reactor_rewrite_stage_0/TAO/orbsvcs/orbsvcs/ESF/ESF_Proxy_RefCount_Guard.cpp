// $Id$

#ifndef TAO_ESF_PROXY_REFCOUNT_GUARD_CPP
#define TAO_ESF_PROXY_REFCOUNT_GUARD_CPP

#include "ESF_Proxy_RefCount_Guard.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Proxy_RefCount_Guard.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ESF, ESF_Proxy_RefCount_Guard, "$Id$")

template<class EC, class P>
TAO_ESF_Proxy_RefCount_Guard<EC,P>::~TAO_ESF_Proxy_RefCount_Guard (void)
{
  // Checking for 0 is safe, once the variable reaches 0 the value
  // will stay there.
  // @@ But what if the thread is switched to another processor just
  // after release the mutex?
  if (this->refcount_ == 0)
    {
      this->event_channel_->destroy_proxy (this->proxy_);
    }
}

#endif /* TAO_ESF_PROXY_REFCOUNT_GUARD_CPP */
