// -*- C++ -*-

#include "PrincipalAuthenticator.h"
#include "PrincipalAuthenticator_Impl.h"

ACE_RCSID (Security,
           PrincipalAuthenticator,
           "$Id$")


TAO_PrincipalAuthenticator::TAO_PrincipalAuthenticator (void)
  : authenticators_ ()
{
}

TAO_PrincipalAuthenticator::~TAO_PrincipalAuthenticator (void)
{
}

Security::AuthenticationMethodList *
TAO_PrincipalAuthenticator::get_supported_authen_methods (
    const char * /* mechanism */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
#if 0
  Security::AuthenticationMethodList *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    Security::AuthenticationMethodList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  Security::AuthenticationMethodList_var list = tmp;

WHAT DO WE DO WITH THE "mechanism" paremter!

//   CORBA::ULong count = this->authenticators_.size ();
//   list->length (count);

//   for (CORBA::ULong i = 0; i < count; ++i)
//     list[i] = this->authenticators_[i]->authentication_method ();

//   return list._retn ();
#else
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
#endif  /* 0 */
}

Security::AuthenticationStatus
TAO_PrincipalAuthenticator::authenticate (
    Security::AuthenticationMethod method,
    const char * mechanism,
    const char * security_name,
    const CORBA::Any & auth_data,
    const Security::AttributeList & privileges,
    SecurityLevel2::Credentials_out creds,
    CORBA::Any_out continuation_data,
    CORBA::Any_out auth_specific_data
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  size_t count = this->authenticators_.size ();

  for (size_t i = 0; i < count; ++i)
    {
      TAO_PrincipalAuthenticator_Impl *authenticator =
        this->authenticators_[i];

      if (ACE_OS::strcmp (mechanism,
                          authenticator->mechanism ()) == 0)
          return
            authenticator->authenticate (method,
                                         security_name,
                                         auth_data,
                                         privileges,
                                         creds,
                                         continuation_data,
                                         auth_specific_data
                                         TAO_ENV_ARG_PARAMETER);
    }

  return Security::SecAuthFailure;
}

Security::AuthenticationStatus
TAO_PrincipalAuthenticator::continue_authentication (
    const CORBA::Any & /* response_data */,
    SecurityLevel2::Credentials_ptr /* creds */,
    CORBA::Any_out /* continuation_data */,
    CORBA::Any_out /* auth_specific_data */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    Security::SecAuthFailure);
}

void
TAO_PrincipalAuthenticator::register_principal_authenticator (
  TAO_PrincipalAuthenticator_Impl *authenticator)
{
  if (authenticator != 0)
    {
      size_t old_size = this->authenticators_.size ();
      this->authenticators_.size (old_size + 1);
      this->authenticators_[old_size] = authenticator;
    }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<TAO_PrincipalAuthenticator_Impl *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<TAO_PrincipalAuthenticator_Impl *>

#endif  /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
