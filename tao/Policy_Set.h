// -*- C++ -*-

//=============================================================================
/**
 *  @file   Policy_Set.h
 *
 *  $Id$
 *
 *  A Policy Container that provides O(1) time access for policy that
 *  support caching (see orbconf.h).
 *
 *  @author Angelo Cosaro (corsaro@cs.wustl.edu)
 *  @author Frank Hunleth (fhunleth@cs.wustl.edu)
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef TAO_POLICY_SET_H
#define TAO_POLICY_SET_H

#include /**/ "ace/pre.h"

#include "tao/PolicyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Policy_Set
 *
 * @brief The policy manager implementation.
 *
 * This class is used to implement both the CORBA::PolicyManager
 * and the CORBA::PolicyCurrent interfaces.
 */
class TAO_Export TAO_Policy_Set
{
public:

  /** Creates a TAO_Policy_Set that has a given scope. The
   * scope is used to determinate whether or not a given policy can
   * be set for the given Policy Manager Implementation.
   */
  TAO_Policy_Set (TAO_Policy_Scope scope);

  /// Copy constructor.
  TAO_Policy_Set (const TAO_Policy_Set &rhs);

  /// Destructor
  ~TAO_Policy_Set (void);

  /// Copy the state from @a source, it uses the copy() operator to
  /// obtain independent copies of all the policies.
  void copy_from (TAO_Policy_Set* source);

  /**
   * Modify the list of policies to include @a policies.
   * If @a set_add is @c CORBA::SET_OVERRIDE then we replace all the
   * old policies. If it is @c CORBA::ADD_OVERRIDE we simply add the
   * policies in @a policies.
   * No attempt is made to validate the policies for consistency.
   */
  void set_policy_overrides (const CORBA::PolicyList & policies,
                             CORBA::SetOverrideType set_add);

  /// Get the values (if any) for the policies in @a types, if @a
  /// types is an empty list the method returns *all* the current
  /// policies.
  CORBA::PolicyList * get_policy_overrides (const CORBA::PolicyTypeSeq & types);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy);

  /// Obtain a cached policy for speedy lookups.
  /**
   * This method just returns a const reference to the policy to avoid
   * obtaining a lock to increment the reference count.  As such, it
   * can only be used for single threaded cases or cases where the
   * policies cannot be removed such as at the object and thread level
   * scopes.  This method is most likely not appropriate for accessing
   * policies at the ORB level scope in any situation.
   */
  CORBA::Policy_ptr get_cached_const_policy (TAO_Cached_Policy_Type type) const;

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  /// Utility method to set a single policy.
  void set_policy (const CORBA::Policy_ptr policy);

  /// Returns the policy at the specified index.
  /// @c CORBA::Policy::_nil () is returned if the policy doesn't
  /// exist.
  CORBA::Policy *get_policy_by_index (CORBA::ULong index) const;
  CORBA::ULong num_policies (void) const;

private:
  TAO_Policy_Set & operator= (const TAO_Policy_Set&);

  /// Remove and destroy all the policy objects owned by this policy
  /// manager.
  void cleanup_i (void);

  /// Utility method to determine if a policy's scope is compatible with ours.
  CORBA::Boolean compatible_scope (TAO_Policy_Scope policy_scope) const;

private:
  /// Policies set for this Policy_Manager
  CORBA::PolicyList policy_list_;

  /// List of caches.
  CORBA::Policy *cached_policies_[TAO_CACHED_POLICY_MAX_CACHED];

  /// Scope associated to the Policy Manager Impl
  TAO_Policy_Scope scope_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Policy_Set.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_POLICY_SET_H */
