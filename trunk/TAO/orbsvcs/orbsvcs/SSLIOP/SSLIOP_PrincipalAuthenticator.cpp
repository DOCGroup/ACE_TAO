#include "SSLIOP_PrincipalAuthenticator.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_PrincipalAuthenticator,
           "$Id$")


TAO_SSLIOP_PrincipalAuthenticator::TAO_SSLIOP_PrincipalAuthenticator (void)
{
}

TAO_SSLIOP_PrincipalAuthenticator::~TAO_SSLIOP_PrincipalAuthenticator (void)
{
}

Security::AuthenticationMethod
TAO_SSLIOP_PrincipalAuthenticator::authentication_method (void) const
{
  ACE_NOTSUP_RETURN (0);
}

const char *
TAO_SSLIOP_PrincipalAuthenticator::mechanism (void) const
{
  ACE_NOTSUP_RETURN (0);
}

Security::AuthenticationStatus
TAO_SSLIOP_PrincipalAuthenticator::authenticate (
    Security::AuthenticationMethod /* method */,
    const char * /* security_name */,
    const CORBA::Any & /* auth_data */,
    const Security::AttributeList & /* privileges */,
    SecurityLevel2::Credentials_out /* creds */,
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

Security::AuthenticationStatus
TAO_SSLIOP_PrincipalAuthenticator::continue_authentication (
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
