// -*- C++ -*-

// ===================================================================
/**
 *  @file   EstablishTrustPolicy.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_ESTABLISH_TRUST_POLICY_H
#define TAO_ESTABLISH_TRUST_POLICY_H

#include /**/ "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"

/**
 * @class TAO_EstablishTrustPolicy
 *
 * @brief Implementation of the SecurityLevel2::EstablishTrustPolicy.
 *
 * This policy can be used to enable or disable establishment of trust
 * in the client or the target or both on a per-object basis.  For
 * example, it can be set as a policy override using the standard
 * CORBA::Object::_set_policy_overrides() method.
 * @par
 * This policy can be created by using the CORBA::ORB::create_policy()
 * method by passing it the Security::SecEstablishTrustPolicy policy
 * type, and the appropriate Security::EstablishTrust structure
 * (inserted into a CORBA::Any).
 */
class TAO_Security_Export TAO_EstablishTrustPolicy
  : public virtual SecurityLevel2::EstablishTrustPolicy,
    public virtual TAO_Local_RefCounted_Object
{
  friend class TAO_Security_PolicyFactory;

public:

  /**
   * @name CORBA::Policy Methods
   */
  //@{
  virtual CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /// Return the "establish trust" value associated with this
  /// policy.
  virtual Security::EstablishTrust trust (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Constructor
  TAO_EstablishTrustPolicy (const Security::EstablishTrust &trust);

  /// Destructor
  /**
   * Protected destructor to enforce the fact this class is reference
   * counted, and should not be destroyed using delete() by anything
   * other than the reference counting mechanism.
   */
  ~TAO_EstablishTrustPolicy (void);

private:

  /// Quality of protection which can be specified for an object
  /// reference and used to protect messages.
  Security::EstablishTrust trust_;

};

#include /**/ "ace/post.h"

#endif  /* TAO_ESTABLISH_TRUST_POLICY_H */
