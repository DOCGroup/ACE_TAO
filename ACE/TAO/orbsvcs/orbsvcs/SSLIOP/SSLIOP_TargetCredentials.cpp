#include "orbsvcs/SSLIOP/SSLIOP_TargetCredentials.h"


ACE_RCSID (SSLIOP,
           SSLIOP_TargetCredentials,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::TargetCredentials::TargetCredentials (::X509 *cert,
                                                   ::EVP_PKEY *evp)
  : SSLIOP_Credentials (cert, evp)
{
}


SecurityLevel3::CredentialsType
TAO::SSLIOP::TargetCredentials::creds_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return SecurityLevel3::CT_TargetCredentials;
}

char *
TAO::SSLIOP::TargetCredentials::context_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::Principal *
TAO::SSLIOP::TargetCredentials::client_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::StatementList *
TAO::SSLIOP::TargetCredentials::client_supporting_statements (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::ResourceNameList *
TAO::SSLIOP::TargetCredentials::client_restricted_resources (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::Principal *
TAO::SSLIOP::TargetCredentials::target_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::StatementList *
TAO::SSLIOP::TargetCredentials::target_supporting_statements (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::ResourceNameList *
TAO::SSLIOP::TargetCredentials::target_restricted_resources (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::OwnCredentials_ptr
TAO::SSLIOP::TargetCredentials::parent_credentials (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    SecurityLevel3::OwnCredentials::_nil ());
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::client_authentication (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_authentication (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::confidentiality (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::integrity (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_embodied (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_endorsed (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO::SSLIOP::TargetCredentials::release (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
