// -*- C++ -*-

#include "SSLIOP_TargetCredentials.h"


ACE_RCSID (TAO_SSLIOP,
           SSLIOP_TargetCredentials,
           "$Id$")


TAO_SSLIOP_TargetCredentials::TAO_SSLIOP_TargetCredentials (X509 *cert)
  : TAO_SSLIOP_Credentials (cert)
{
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_TargetCredentials::copy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_SSLIOP_TargetCredentials *c = 0;
  ACE_NEW_THROW_EX (c,
                    TAO_SSLIOP_TargetCredentials (this->x509_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (SecurityLevel2::Credentials::_nil ());

  return c;
}

Security::InvocationCredentialsType
TAO_SSLIOP_TargetCredentials::credentials_type (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Security::SecTargetCredentials;
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_TargetCredentials::initiating_credentials (
    TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    SecurityLevel2::Credentials::_nil ());
}

Security::AssociationOptions
TAO_SSLIOP_TargetCredentials::association_options_used (
    TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);  
}
