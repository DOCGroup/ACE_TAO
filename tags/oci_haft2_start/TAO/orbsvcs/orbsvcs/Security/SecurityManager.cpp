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
TAO_SecurityManager::supported_mechanisms (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_SecurityManager::own_credentials (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    monitor,
                    this->lock_,
                    0);

  SecurityLevel2::CredentialsList *creds_list = 0;
  ACE_NEW_THROW_EX (creds_list,
                    SecurityLevel2::CredentialsList (this->own_credentials_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return creds_list;
}

SecurityLevel2::RequiredRights_ptr
TAO_SecurityManager::required_rights_object (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_SecurityManager::principal_authenticator (ACE_ENV_SINGLE_ARG_DECL)
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
                          TAO_PrincipalAuthenticator (this),
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
TAO_SecurityManager::access_decision (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_SecurityManager::audit_decision (ACE_ENV_SINGLE_ARG_DECL)
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
                                             ACE_ENV_ARG_DECL)
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
    SecurityLevel2::Credentials_ptr creds
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

  // Check if the given Credentials are already in the "own
  // credentials" list.

  const CORBA::ULong len = this->own_credentials_.length ();

  // @@ A linear search.  Yuck!
  for (CORBA::ULong i = 0; i < len; ++i)
    if (this->own_credentials_[i].in () == creds)
      {
        if (i == len - 1)
          this->own_credentials_.length (len - 1);
        else
          {
            // Avoid building a new CredentialsList.  Just invalidate
            // the Credentials in the current position in the
            // CredentialsList.
            //
            // This is has the disadvantage that it will leave "holes"
            // in the CredentialsList.
            this->own_credentials_[i] = SecurityLevel2::Credentials::_nil ();
          }

        return;
      }

  ACE_THROW (CORBA::BAD_PARAM ());
}

CORBA::Policy_ptr
TAO_SecurityManager::get_security_policy (CORBA::PolicyType /* policy_type */
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    CORBA::Policy::_nil ());
}

void
TAO_SecurityManager::add_own_credentials (SecurityLevel2::Credentials_ptr creds
                                          ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

  // Check if the given Credentials are already in the "own
  // credentials" list.

  const CORBA::ULong len = this->own_credentials_.length ();

  CORBA::ULong empty_slot = 0;

  // @@ A linear search.  Yuck!
  for (CORBA::ULong i = 0; i < len; ++i)
    if (this->own_credentials_[i].in () == creds)
      ACE_THROW (CORBA::BAD_PARAM ());
    else if (empty_slot == 0
             && CORBA::is_nil (this->own_credentials_[i]))
      empty_slot = i;

  if (empty_slot != 0)
    {
      this->own_credentials_[empty_slot] =
        SecurityLevel2::Credentials::_duplicate (creds);
    }
  else
    {
      this->own_credentials_.length (len + 1);
      this->own_credentials_[len] =
        SecurityLevel2::Credentials::_duplicate (creds);
    }
}
