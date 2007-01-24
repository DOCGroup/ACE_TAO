// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_OwnCredentials.h"


ACE_RCSID (SSLIOP,
           SSLIOP_OwnCredentials,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::OwnCredentials::OwnCredentials (X509 *cert, EVP_PKEY *evp)
  : SSLIOP_Credentials (cert, evp)
{
}

TAO::SSLIOP::OwnCredentials::~OwnCredentials (void)
{
}

TAO::SSLIOP::OwnCredentials_ptr
TAO::SSLIOP::OwnCredentials::_duplicate (TAO::SSLIOP::OwnCredentials_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();

  return obj;
}

TAO::SSLIOP::OwnCredentials_ptr
TAO::SSLIOP::OwnCredentials::_narrow (CORBA::Object_ptr obj
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  return  TAO::SSLIOP::OwnCredentials::_duplicate (
              dynamic_cast<TAO::SSLIOP::OwnCredentials *> (obj));
}

TAO::SSLIOP::OwnCredentials_ptr
TAO::SSLIOP::OwnCredentials::_nil (void)
{
  return (OwnCredentials *) 0;

}

SecurityLevel3::CredentialsType
TAO::SSLIOP::OwnCredentials::creds_type (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return SecurityLevel3::CT_OwnCredentials;
}

SecurityLevel3::CredsInitiator_ptr
TAO::SSLIOP::OwnCredentials::creds_initiator (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    SecurityLevel3::CredsInitiator::_nil ());
}

SecurityLevel3::CredsAcceptor_ptr
TAO::SSLIOP::OwnCredentials::creds_acceptor (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    SecurityLevel3::CredsAcceptor::_nil ());
}

void
TAO::SSLIOP::OwnCredentials::release_credentials (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->creds_state_ = SecurityLevel3::CS_PendingRelease;
}

TAO_END_VERSIONED_NAMESPACE_DECL
