// $Id$

#include "tao/orbconf.h"

///
/// ImplRepo related.
///
#if (TAO_HAS_MINIMUM_CORBA == 0)

#include "tao/PortableServer/ImplRepo_i.h"

ServerObject_i::ServerObject_i (CORBA::ORB_ptr orb,
                                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
ServerObject_i::ping (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerObject_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::POA_ptr
ServerObject_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
