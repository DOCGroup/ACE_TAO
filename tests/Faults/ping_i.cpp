// $Id$

#include "ping_i.h"

#if !defined(__ACE_INLINE__)
#include "ping_i.inl"
#endif /* __ACE_INLINE__ */

void
PingObject_i::ping (PingObject_ptr callback)
{
  callback->pong ();
}

void
PingObject_i::pong (void)
{
}

void
PingObject_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

PortableServer::POA_ptr
PingObject_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
