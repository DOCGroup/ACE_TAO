// -*- C++ -*-

// ===================================================================
/**
 *  @file   MechanismPolicy.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_MECHANISM_POLICY_H
#define TAO_MECHANISM_POLICY_H

#include "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"

/**
 * @class TAO_MechanismPolicy
 *
 * @brief Implementation of the SecurityLevel2::MechanismPolicy interface.
 *
 * This policy can be used to specify which mechanisms may be used for
 * invocation on a per-object basis.  For example, it can be set
 * as a policy override using the standard
 * CORBA::Object::_set_policy_overrides() method.
 * @note Only security mechanism will be used for a given invocation.
 * @par
 * This policy can be created by using the CORBA::ORB::create_policy()
 * method by passing it the Security::SecMechanismPolicy policy type,
 * and the appropriate Security::MechanismTypeList (inserted into a
 * CORBA::Any).
 */
class TAO_Security_Export TAO_MechanismPolicy
  : public virtual SecurityLevel2::MechanismPolicy,
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

  /// Return the list of mechanism types associated with this policy.
  virtual Security::MechanismTypeList* mechanisms (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Constructor
  TAO_MechanismPolicy (const Security::MechanismTypeList &mechanisms);

  /// Destructor
  /**
   * Protected destructor to enforce the fact this class is reference
   * counted, and should not be destroyed using delete() by anything
   * other than the reference counting mechanism.
   */
  ~TAO_MechanismPolicy (void);

private:

  /// List of mechanisms that can be used for a given invocation.
  /// Only one is used at any given time.
  Security::MechanismTypeList mechanisms_;

};

#include "ace/post.h"

#endif  /* TAO_MECHANISM_POLICY_H */
