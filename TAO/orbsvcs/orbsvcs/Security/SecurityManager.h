// -*- C++ -*-

// ===================================================================
/**
 *  @file   SecurityManager.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_SECURITY_MANAGER_H
#define TAO_SECURITY_MANAGER_H

#include "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"

/**
 * @class TAO_SecurityManager
 *
 * @brief Implementation of the SecurityLevel2::SecurityManager
 *        interface.
 *
 * A reference to the SecurityManager object can be retrieved by
 * calling CORBA::ORB::resolve_initial_references ("SecurityManager");
 */
class TAO_Security_Export TAO_SecurityManager
  : public virtual SecurityLevel2::SecurityManager,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_SecurityManager (void);

  /**
   * @name SecurityLevel2::SecurityManager Methods
   *
   * Methods exposed by the SecurityLevel2::SecurityManager
   * interface.
   */
  //@{
  /// Return a sequence containing the security mechanisms supported
  /// by the Security Service.
  virtual Security::MechandOptionsList * supported_mechanisms (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the list of Credentials currently associated with the
  /// application itself, i.e. its own credentials.
  virtual SecurityLevel2::CredentialsList * own_credentials (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the RequiredRights object.
  /**
   * The RequiredRights object is only useful to the application if
   * the application needs to perform its own rights-based access
   * control.
   */
  virtual SecurityLevel2::RequiredRights_ptr required_rights_object (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the PrincipalAuthenticator object.
  /**
   * The PrincipalAuthenticator is used to authenticate principals and
   * create credentials for them.
   */
  virtual SecurityLevel2::PrincipalAuthenticator_ptr principal_authenticator (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the AccessDecision object.
  /**
   * The AccessDecision object can be used to determine accessibility
   * of objects.
   */
  virtual SecurityLevel2::AccessDecision_ptr access_decision (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the AuditDecision object.
  /**
   * The AuditDecision object can be used to determine what needs to
   * be audited for a given object.
   */
  virtual SecurityLevel2::AuditDecision_ptr audit_decision (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the TargetCredentials associated with the given object
  /// reference.
  virtual SecurityLevel2::TargetCredentials_ptr get_target_credentials (
      CORBA::Object_ptr obj_ref
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove the given credential from the application's own
  /// Credentials list.
  /**
   *  @note A CORBA::BAD_PARAM exception will be thrown if the given
   *        credential is not in the SecurityManager's "own
   *        credentials" list.
   */
  virtual void remove_own_credentials (
      SecurityLevel2::Credentials_ptr creds
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the security policy currently in effect corresponding to
  /// the given policy type.
  virtual CORBA::Policy_ptr get_security_policy (
      CORBA::PolicyType policy_type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /**
   * @name TAO-specific Methods
   *
   * Methods used internally by TAO's Security Service.
   */
  //@{
  /// Add the given credentials to the SecurityManager's "own
  /// credentials" list.
  /**
   * This method is generally only used by the PrincipalAuthenticator
   * when credentials are created and authenticated.
   */
  void add_own_credentials (SecurityLevel2::Credentials_ptr creds
                            TAO_ENV_ARG_DECL);
  //@}

protected:

  /// Destructor
  /**
   * Protected destructor to enforce the fact this class is reference
   * counted, and should not be destroyed using delete() by anything
   * other than the reference counting mechanism.
   */
  ~TAO_SecurityManager (void);

private:

  /// Lock used to synchronize access to the SecurityManager's state.
  TAO_SYNCH_MUTEX lock_;

  /// List of SecurityManager's own credentials, i.e. those created by
  /// its PrincipalAuthenticator.
  SecurityLevel2::CredentialsList own_credentials_;

  /// Reference to the PrinicipalAuthenticator.
  SecurityLevel2::PrincipalAuthenticator_var principal_authenticator_;

};

#include "ace/post.h"

#endif  /* TAO_SECURITY_MANAGER_H */
