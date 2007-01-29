// $Id$

#include "ping_i.h"

#if !defined(__ACE_INLINE__)
#include "ping_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Faults, test_i, "$Id$")

void
PingObject_i::ping (PingObject_ptr callback)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  callback->pong ();
}

void
PingObject_i::pong (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
PingObject_i::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}

PortableServer::POA_ptr
PingObject_i::_default_POA (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
