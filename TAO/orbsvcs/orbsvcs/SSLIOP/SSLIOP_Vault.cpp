#include "SSLIOP_Vault.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Vault,
           "$Id$")


TAO_SSLIOP_Vault::TAO_SSLIOP_Vault (void)
{
}

TAO_SSLIOP_Vault::~TAO_SSLIOP_Vault (void)
{
}

Security::AuthenticationMethodList *
TAO_SSLIOP_Vault::get_supported_authen_methods (
    const char * /* mechanism */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

Security::OIDList *
TAO_SSLIOP_Vault::supported_mech_oids (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

Security::AuthenticationStatus
TAO_SSLIOP_Vault::acquire_credentials (
    Security::AuthenticationMethod /* method */,
    const char * /* mechanism */,
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
TAO_SSLIOP_Vault::continue_credentials_acquisition (
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

IOP::TaggedComponentList *
TAO_SSLIOP_Vault::create_ior_components (
    SecurityLevel2::Credentials_ptr /* creds_list */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

Security::AssociationStatus
TAO_SSLIOP_Vault::init_security_context (
    SecurityLevel2::Credentials_ptr /* creds */,
    const char * /* target_security_name */,
    CORBA::Object_ptr /* target */,
    Security::DelegationMode /* delegation_mode */,
    const Security::OptionsDirectionPairList & /* association_options */,
    const char * /* mechanism */,
    const Security::Opaque & /* comp_data */,
    const Security::ChannelBindings & /* chan_binding */,
    Security::OpaqueBuffer_out /* security_token */,
    SecurityReplaceable::ClientSecurityContext_out /* security_context */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    Security::SecAssocFailure);
}

Security::AssociationStatus
TAO_SSLIOP_Vault::accept_security_context (
    const SecurityLevel2::CredentialsList & /* creds_list */,
    const Security::ChannelBindings & /* chan_bindings */,
    const Security::Opaque & /* in_token */,
    Security::Opaque_out /* out_token */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    Security::SecAssocFailure);
}

Security::MechandOptionsList *
TAO_SSLIOP_Vault::get_supported_mechs (
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
