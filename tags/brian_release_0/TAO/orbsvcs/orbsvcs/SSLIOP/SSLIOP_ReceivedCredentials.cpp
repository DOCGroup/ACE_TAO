// -*- C++ -*-

#include "SSLIOP_ReceivedCredentials.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_ReceivedCredentials,
           "$Id$")


TAO_SSLIOP_ReceivedCredentials::TAO_SSLIOP_ReceivedCredentials (X509 *cert,
                                                                EVP_PKEY *evp)
  : TAO_SSLIOP_Credentials (cert, evp)
{
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_ReceivedCredentials::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_SSLIOP_ReceivedCredentials *c = 0;
  ACE_NEW_THROW_EX (c,
                    TAO_SSLIOP_ReceivedCredentials (this->x509_.in (),
                                                    this->evp_.in ()),
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
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Security::SecReceivedCredentials;
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_ReceivedCredentials::accepting_credentials (
    ACE_ENV_SINGLE_ARG_DECL)
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
    ACE_ENV_SINGLE_ARG_DECL)
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
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // It is not possible to perform credentials delegation with SSLIOP,
  // meaning that the peer (i.e., the remote principal) is always the
  // invocation initiator.
  return Security::SecInitiator;
}

Security::DelegationMode
TAO_SSLIOP_ReceivedCredentials::delegation_mode (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // SSLIOP does not support delegation.
  return Security::SecDelModeNoDelegation;
}

// ---------------------------

TAO_SSLIOP_ReceivedCredentials_ptr
TAO_SSLIOP_ReceivedCredentials::_narrow (CORBA::Object_ptr obj
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
  return  TAO_SSLIOP_ReceivedCredentials::_duplicate (
              dynamic_cast<TAO_SSLIOP_ReceivedCredentials *> (obj));
}

TAO_SSLIOP_ReceivedCredentials_ptr
TAO_SSLIOP_ReceivedCredentials::_duplicate (
  TAO_SSLIOP_ReceivedCredentials_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();

  return obj;
}

// -----------------------------------------------------------

int TAO_SSLIOP_ReceivedCredentials::_tao_class_id = 0;
