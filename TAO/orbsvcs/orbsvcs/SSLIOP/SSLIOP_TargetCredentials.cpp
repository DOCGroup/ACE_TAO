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
TAO::SSLIOP::TargetCredentials::creds_type (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return SecurityLevel3::CT_TargetCredentials;
}

char *
TAO::SSLIOP::TargetCredentials::context_id (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

SecurityLevel3::Principal *
TAO::SSLIOP::TargetCredentials::client_principal (void)
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
TAO::SSLIOP::TargetCredentials::target_principal (void)
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
TAO::SSLIOP::TargetCredentials::parent_credentials (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    SecurityLevel3::OwnCredentials::_nil ());
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::client_authentication (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_authentication (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::confidentiality (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::integrity (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_embodied (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
TAO::SSLIOP::TargetCredentials::target_endorsed (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
TAO::SSLIOP::TargetCredentials::release (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
