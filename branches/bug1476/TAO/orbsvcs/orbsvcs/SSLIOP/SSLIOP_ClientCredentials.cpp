// $Id$

#include "SSLIOP_ClientCredentials.h"


ACE_RCSID (SSLIOP,
           SSLIOP_ClientCredentials,
           "$Id$")


TAO::SSLIOP::ClientCredentials::ClientCredentials (X509 *cert, EVP_PKEY *evp)
  : Credentials (cert, evp)
{
}

TAO::SSLIOP::ClientCredentials::~ClientCredentials (void)
{
}

SecurityLevel3::CredentialsType
TAO::SSLIOP::ClientCredentials::creds_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return SecurityLevel3::CT_ClientCredentials;
}

char *
TAO::SSLIOP::ClientCredentials::context_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::Principal *
TAO::SSLIOP::ClientCredentials::client_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::StatementList *
TAO::SSLIOP::ClientCredentials::client_supporting_statements (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::ResourceNameList *
TAO::SSLIOP::ClientCredentials::client_restricted_resources (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::Principal *
TAO::SSLIOP::ClientCredentials::target_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::StatementList *
TAO::SSLIOP::ClientCredentials::target_supporting_statements (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::ResourceNameList *
TAO::SSLIOP::ClientCredentials::target_restricted_resources (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::OwnCredentials_ptr
TAO::SSLIOP::ClientCredentials::parent_credentials (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    SecurityLevel3::OwnCredentials::_nil ());
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::client_authentication (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::target_authentication (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::confidentiality (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::integrity (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}
