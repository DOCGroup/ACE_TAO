// -*- C++ -*-

// ===================================================================
/**
 *  @file   InvocationCredentialsPolicy.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_INVOCATION_CREDENTIALS_POLICY_H
#define TAO_INVOCATION_CREDENTIALS_POLICY_H

#include "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"

/**
 * @class TAO_InvocationCredentialsPolicy
 *
 * @brief Implementation of the
 *        SecurityLevel2::InvocationCredentialsPolicy interface.
 *
 * This policy can be used to specify which credentials may be used
 * for invocations on a per-object basis.  For example, it can be set
 * as a policy override using the standard
 * CORBA::Object::_set_policy_overrides() method.
 * @par
 * If supported by the underlying security mechanism, multiple
 * credentials can be associated with a given invocation in order
 * to gain more privileges.
 * @par
 * This policy can be created by using the CORBA::ORB::create_policy()
 * method by passing it the Security::SecInvocationCredentialsPolicy
 * policy type, and the appropriate
 * SecurityLevel2::CredentialsList (inserted into a CORBA::Any).
 */
class TAO_Security_Export TAO_InvocationCredentialsPolicy
  : public virtual SecurityLevel2::InvocationCredentialsPolicy,
    public virtual TAO_Local_RefCounted_Object
{
  friend class TAO_Security_PolicyFactory;

public:

  /**
   * @name CORBA::Policy Methods
   */
  //@{
  virtual CORBA::PolicyType policy_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /// Return the list of invocation credentials associated with this
  /// policy.
  virtual SecurityLevel2::CredentialsList * creds (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Constructor
  TAO_InvocationCredentialsPolicy (
    const SecurityLevel2::CredentialsList &creds);

  /// Destructor
  /**
   * Protected destructor to enforce the fact this class is reference
   * counted, and should not be destroyed using delete() by anything
   * other than the reference counting mechanism.
   */
  ~TAO_InvocationCredentialsPolicy (void);

private:

  /// List of credentials that can be used for a given invocation.
  /// Only one is used at any given time.
  SecurityLevel2::CredentialsList creds_;

};

#include "ace/post.h"

#endif  /* TAO_INVOCATION_CREDENTIALS_POLICY_H */
