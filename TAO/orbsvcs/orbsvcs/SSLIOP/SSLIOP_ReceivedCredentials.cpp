// -*- C++ -*-

#include "SSLIOP_ReceivedCredentials.h"

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
                                         ACE_ENV_ARG_DECL)
{
  return
    TAO_SSLIOP_ReceivedCredentials::_unchecked_narrow (obj
                                                       ACE_ENV_ARG_PARAMETER);
}

TAO_SSLIOP_ReceivedCredentials_ptr
TAO_SSLIOP_ReceivedCredentials::_unchecked_narrow (CORBA::Object_ptr obj
                                                   ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (obj))
    return TAO_SSLIOP_ReceivedCredentials::_nil ();
  return
        ACE_reinterpret_cast
          (
            TAO_SSLIOP_ReceivedCredentials_ptr,
              obj->_tao_QueryInterface
                (
                  ACE_reinterpret_cast
                    (
                      ptrdiff_t,
                      &TAO_SSLIOP_ReceivedCredentials::_tao_class_id
                    )
                )
          );
}

TAO_SSLIOP_ReceivedCredentials_ptr
TAO_SSLIOP_ReceivedCredentials::_duplicate (
  TAO_SSLIOP_ReceivedCredentials_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();

  return obj;
}

void *
TAO_SSLIOP_ReceivedCredentials::_tao_QueryInterface (ptrdiff_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptrdiff_t,
      &TAO_SSLIOP_ReceivedCredentials::_tao_class_id))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptrdiff_t,
      &::SecurityLevel2::Credentials::_tao_class_id))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            SecurityLevel2::Credentials_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast (ptrdiff_t, &CORBA::Object::_tao_class_id))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

// -----------------------------------------------------------

int TAO_SSLIOP_ReceivedCredentials::_tao_class_id = 0;
