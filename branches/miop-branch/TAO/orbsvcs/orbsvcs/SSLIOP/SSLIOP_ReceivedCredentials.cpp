// -*- C++ -*-

#include "SSLIOP_ReceivedCredentials.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_ReceivedCredentials,
           "$Id$")

TAO_SSLIOP_ReceivedCredentials::TAO_SSLIOP_ReceivedCredentials (X509 *cert)
  : TAO_SSLIOP_Credentials (cert)
{
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_ReceivedCredentials::copy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_SSLIOP_ReceivedCredentials *c = 0;
  ACE_NEW_THROW_EX (c,
                    TAO_SSLIOP_ReceivedCredentials (this->x509_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (SecurityLevel2::Credentials::_nil ());

  return c;
}

Security::InvocationCredentialsType
TAO_SSLIOP_ReceivedCredentials::credentials_type (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Security::SecReceivedCredentials;
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_ReceivedCredentials::accepting_credentials (
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
TAO_SSLIOP_ReceivedCredentials::association_options_used (
    TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    Security::Integrity
                    | Security::Confidentiality
                    | Security::NoDelegation);
}

Security::DelegationState
TAO_SSLIOP_ReceivedCredentials::delegation_state (
  TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // It is not possible to perform credentials delegation with SSLIOP,
  // meaning that the peer (i.e., the remote principal) is always the
  // invocation initiator.
  return Security::SecInitiator;
}

Security::DelegationMode
TAO_SSLIOP_ReceivedCredentials::delegation_mode (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // SSLIOP does not support delegation.
  return Security::SecDelModeNoDelegation;
}
