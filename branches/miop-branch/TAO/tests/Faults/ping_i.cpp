// $Id$

#include "ping_i.h"

#if !defined(__ACE_INLINE__)
#include "ping_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Faults, test_i, "$Id$")

void
PingObject_i::ping (PingObject_ptr callback
      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  callback->pong (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
PingObject_i::pong (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
PingObject_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
PingObject_i::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
