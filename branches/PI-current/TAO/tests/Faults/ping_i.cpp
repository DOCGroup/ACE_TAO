// $Id$

#include "ping_i.h"

#if !defined(__ACE_INLINE__)
#include "ping_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Faults, test_i, "$Id$")

void
Ping_i::ping (Ping_ptr callback,
      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Ping_i::ping (%P|%t)\n"));
  callback->pong (ACE_TRY_ENV);
}

void
Ping_i::pong (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Ping_i::pong (%P|%t)\n"));
}

void
Ping_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}

PortableServer::POA_ptr
Ping_i::_default_POA (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
