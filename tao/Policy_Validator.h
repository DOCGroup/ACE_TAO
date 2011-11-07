// -*- C++ -*-

//=============================================================================
/**
 *  @file    Policy_Validator.h
 *
 *  $Id$
 *
 *  This file contains the declaration for the POA policy validator
 *  interface.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Frank Hunleth  <fhuntleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_POLICY_VALIDATOR_H
#define TAO_POLICY_VALIDATOR_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_Policy_Set;
class TAO_ORB_Core;

namespace CORBA
{
  typedef ULong PolicyType;
}

/**
 * @class TAO_Policy_Validator
 *
 * @brief An abstract class for plugging in different Policy Validator
 *  mechanisms.
 *
 *  This class was moved from the PortableServer library. The Portable
 *  Serever library used this abstraction to add policy validators to
 *  for the policies in the POA. This class seems so much useful for
 *  passing policy information between different loaded libraries.
 */
class TAO_Export TAO_Policy_Validator
{
public:
  /// Constructor.
  TAO_Policy_Validator (TAO_ORB_Core &orb_core);

  /// Destructor.
  virtual ~TAO_Policy_Validator (void);

  /**
   * Validate that the policies in the specified set
   * are consistent and legal.  Throw an appropriate exception
   * if that is not the case.
   */
  void validate (TAO_Policy_Set &policies);

  /**
   * Add/merge policies.
   **/
  void merge_policies (TAO_Policy_Set &policies);

  /**
   * Return whether the specified policy type is legal for the
   * current configuration.  This is needed since the user can
   * potentially specify policies that are unknown to an
   * validate () routine, and these need to be caught.
   */
  CORBA::Boolean legal_policy (CORBA::PolicyType type);

  /**
   * Adds a validator to the "chain of validators". The <code>
   * TAO_POA_Policy_Validator </code> implements something that looks
   * like a chain of responsability in which different member of the
   * chain have the responsability of validating a given set of
   * policies.
   */
  void add_validator (TAO_Policy_Validator *validator);

  /**
   * Accessor for the stored ORB core reference
   */
 TAO_ORB_Core & orb_core() const;

protected:
  virtual void validate_impl (TAO_Policy_Set &policies) = 0;

  virtual void merge_policies_impl (TAO_Policy_Set &policies) = 0;

  virtual CORBA::Boolean legal_policy_impl (CORBA::PolicyType type) = 0;

  TAO_ORB_Core &orb_core_;

private:
  void operator= (const TAO_Policy_Validator &);
  TAO_Policy_Validator (const TAO_Policy_Validator &);

private:
  TAO_Policy_Validator *next_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_POLICY_VALIDATOR_H */
