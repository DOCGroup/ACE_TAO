// -*- C++ -*-

#include "SecurityManager.h"

ACE_RCSID (Security,
           SecurityManager,
           "$Id$")


#include "PrincipalAuthenticator.h"


TAO_SecurityManager::TAO_SecurityManager (void)
  : lock_ (),
    principal_authenticator_ ()
{
}

TAO_SecurityManager::~TAO_SecurityManager (void)
{
}

Security::MechandOptionsList *
TAO_SecurityManager::supported_mechanisms (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

SecurityLevel2::CredentialsList *
TAO_SecurityManager::own_credentials (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

SecurityLevel2::RequiredRights_ptr
TAO_SecurityManager::required_rights_object (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    SecurityLevel2::RequiredRights::_nil ());
}

SecurityLevel2::PrincipalAuthenticator_ptr
TAO_SecurityManager::principal_authenticator (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      monitor,
                      this->lock_,
                      SecurityLevel2::PrincipalAuthenticator::_nil ());

    if (CORBA::is_nil (this->principal_authenticator_.in ()))
      {
        TAO_PrincipalAuthenticator *pa = 0;
        ACE_NEW_THROW_EX (pa,
                          TAO_PrincipalAuthenticator,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
        ACE_CHECK_RETURN (SecurityLevel2::PrincipalAuthenticator::_nil ());

        this->principal_authenticator_ = pa;
      }
  }

  return
    SecurityLevel2::PrincipalAuthenticator::_duplicate (
      this->principal_authenticator_.in ());
}

SecurityLevel2::AccessDecision_ptr
TAO_SecurityManager::access_decision (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    SecurityLevel2::AccessDecision::_nil ());
}

SecurityLevel2::AuditDecision_ptr
TAO_SecurityManager::audit_decision (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    SecurityLevel2::AuditDecision::_nil ());
}

SecurityLevel2::TargetCredentials_ptr
TAO_SecurityManager::get_target_credentials (CORBA::Object_ptr /* obj_ref */
                                             TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    SecurityLevel2::TargetCredentials::_nil ());
}

void
TAO_SecurityManager::remove_own_credentials (
    SecurityLevel2::Credentials_ptr /* creds */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                TAO_DEFAULT_MINOR_CODE,
                ENOTSUP),
               CORBA::COMPLETED_NO));
}

CORBA::Policy_ptr
TAO_SecurityManager::get_security_policy (CORBA::PolicyType /* policy_type */
                                          TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    CORBA::Policy::_nil ());
}
