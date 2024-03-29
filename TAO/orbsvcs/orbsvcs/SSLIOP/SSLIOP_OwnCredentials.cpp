#include "orbsvcs/SSLIOP/SSLIOP_OwnCredentials.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::OwnCredentials::OwnCredentials (X509 *cert, EVP_PKEY *evp)
  : SSLIOP_Credentials (cert, evp)
{
}

TAO::SSLIOP::OwnCredentials::~OwnCredentials ()
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
TAO::SSLIOP::OwnCredentials::_narrow (CORBA::Object_ptr obj)
{
  return  TAO::SSLIOP::OwnCredentials::_duplicate (
              dynamic_cast<TAO::SSLIOP::OwnCredentials *> (obj));
}

TAO::SSLIOP::OwnCredentials_ptr
TAO::SSLIOP::OwnCredentials::_nil ()
{
  return (OwnCredentials *) 0;
}

SecurityLevel3::CredentialsType
TAO::SSLIOP::OwnCredentials::creds_type ()
{
  return SecurityLevel3::CT_OwnCredentials;
}

SecurityLevel3::CredsInitiator_ptr
TAO::SSLIOP::OwnCredentials::creds_initiator ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::CredsAcceptor_ptr
TAO::SSLIOP::OwnCredentials::creds_acceptor ()
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO::SSLIOP::OwnCredentials::release_credentials ()
{
  this->creds_state_ = SecurityLevel3::CS_PendingRelease;
}

TAO_END_VERSIONED_NAMESPACE_DECL
