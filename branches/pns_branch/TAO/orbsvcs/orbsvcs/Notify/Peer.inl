// $Id$

#include "Proxy.h"

ACE_INLINE CORBA::ULong
TAO_Notify_Peer::_incr_refcnt (void)
{
  return this->proxy ()->_incr_refcnt ();
}

ACE_INLINE CORBA::ULong
TAO_Notify_Peer::_decr_refcnt (void)
{
  return this->proxy ()->_decr_refcnt ();
}
