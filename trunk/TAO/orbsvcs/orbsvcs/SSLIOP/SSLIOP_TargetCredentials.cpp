// -*- C++ -*-

#include "SSLIOP_TargetCredentials.h"


ACE_RCSID (TAO_SSLIOP,
           SSLIOP_TargetCredentials,
           "$Id$")


TAO_SSLIOP_TargetCredentials::TAO_SSLIOP_TargetCredentials (X509 *cert,
                                                            EVP_PKEY *evp)
  : TAO_SSLIOP_Credentials (cert, evp)
{
}

SecurityLevel2::Credentials_ptr
TAO_SSLIOP_TargetCredentials::copy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_SSLIOP_TargetCredentials *c = 0;
  ACE_NEW_THROW_EX (c,
                    TAO_SSLIOP_TargetCredentials (this->x509_.in (),
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

// ---------------------------

TAO_SSLIOP_TargetCredentials_ptr
TAO_SSLIOP_TargetCredentials::_narrow (CORBA::Object_ptr obj
                                       TAO_ENV_ARG_DECL)
{
  return
    TAO_SSLIOP_TargetCredentials::_unchecked_narrow (obj
                                                     TAO_ENV_ARG_PARAMETER);
}

TAO_SSLIOP_TargetCredentials_ptr
TAO_SSLIOP_TargetCredentials::_unchecked_narrow (CORBA::Object_ptr obj
                                                 TAO_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (obj))
    return TAO_SSLIOP_TargetCredentials::_nil ();
  return
        ACE_reinterpret_cast
          (
            TAO_SSLIOP_TargetCredentials_ptr,
              obj->_tao_QueryInterface
                (
                  ACE_reinterpret_cast
                    (
                      ptr_arith_t,
                      &TAO_SSLIOP_TargetCredentials::_tao_class_id
                    )
                )
          );
}

TAO_SSLIOP_TargetCredentials_ptr
TAO_SSLIOP_TargetCredentials::_duplicate (
  TAO_SSLIOP_TargetCredentials_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();

  return obj;
}

void *
TAO_SSLIOP_TargetCredentials::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &TAO_SSLIOP_TargetCredentials::_tao_class_id))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
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
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_tao_class_id))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

// -----------------------------------------------------------

int TAO_SSLIOP_TargetCredentials::_tao_class_id = 0;
