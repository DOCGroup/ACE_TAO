// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_TargetCredentials.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::TargetCredentials::TargetCredentials (::X509 *cert,
                                                   ::EVP_PKEY *evp)
  : SSLIOP_Credentials (cert, evp)
{
}


SecurityLevel3::CredentialsType
TAO::SSLIOP::TargetCredentials::creds_type (void)
{
  return SecurityLevel3::CT_TargetCredentials;
}

char *
TAO::SSLIOP::TargetCredentials::context_id (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::Principal *
TAO::SSLIOP::TargetCredentials::client_principal (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::StatementList *
TAO::SSLIOP::TargetCredentials::client_supporting_statements ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::ResourceNameList *
TAO::SSLIOP::TargetCredentials::client_restricted_resources ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::Principal *
TAO::SSLIOP::TargetCredentials::target_principal (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::StatementList *
TAO::SSLIOP::TargetCredentials::target_supporting_statements ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::ResourceNameList *
TAO::SSLIOP::TargetCredentials::target_restricted_resources ()
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::OwnCredentials_ptr
TAO::SSLIOP::TargetCredentials::parent_credentials (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::client_authentication (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_authentication (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::confidentiality (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::integrity (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_embodied (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_endorsed (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO::SSLIOP::TargetCredentials::release (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
