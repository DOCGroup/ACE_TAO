// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_ClientCredentials.h"
#include "orbsvcs/SSLIOP/SSLIOP_OwnCredentials.h"


ACE_RCSID (SSLIOP,
           SSLIOP_ClientCredentials,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::ClientCredentials::ClientCredentials (
  X509 * cert,
  EVP_PKEY *evp,
  SSL * ssl)
  : SSLIOP_Credentials (cert, evp),
    ssl_ (TAO::SSLIOP::OpenSSL_traits< ::SSL >::_duplicate (ssl))
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
  SecurityLevel3::OwnCredentials_ptr creds =
    SecurityLevel3::OwnCredentials::_nil ();

  ACE_NEW_THROW_EX (creds,
                    TAO::SSLIOP::OwnCredentials (
                      ::SSL_get_certificate (this->ssl_.in ()),
                      ::SSL_get_privatekey (this->ssl_.in ())),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (creds);

  return creds;
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::client_authentication (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If the client presented no certificate (i.e. cert_.ptr() == 0),
  // the client was not authenticated.  Otherwise, verify the peer's
  // certificate.

  return
    this->x509_.in () != 0
    && SSL_get_verify_result (this->ssl_.in ()) == X509_V_OK;
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::target_authentication (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), false);
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::confidentiality (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), false);
}

CORBA::Boolean
TAO::SSLIOP::ClientCredentials::integrity (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TAO's SSLIOP pluggable transport always provides integrity.  Note
  // that if we 

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
