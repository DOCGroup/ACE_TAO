// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    POA.h
//
// = DESCRIPTION
//     POA
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#ifndef TAO_POA_POLICY_SET_H
#define TAO_POA_POLICY_SET_H
#include "ace/pre.h"

#include "POA_Policies.h"
#include "tao/Policy_Set.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_POA_Policy_Validator;

class TAO_PortableServer_Export TAO_POA_Policy_Set
{
public:

  TAO_POA_Policy_Set ();

  TAO_POA_Policy_Set (const TAO_POA_Policy_Set &rhs);

  ~TAO_POA_Policy_Set (void);

  /// Returns the policy at the specified index.
  /// CORBA::Policy::_nil () is returned if the policy doesn't exist
  CORBA::Policy *get_policy_by_index (CORBA::ULong index);
  CORBA::ULong num_policies (void) const;

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  void merge_policies (const CORBA::PolicyList &policies
                       TAO_ENV_ARG_DECL);

  void merge_policy (const CORBA::Policy_ptr policy
                     TAO_ENV_ARG_DECL);

  void validate_policies (TAO_POA_Policy_Validator &validator,
                          TAO_ORB_Core &orb_core
                          TAO_ENV_ARG_DECL);

  // Add all of the client exposed policies to the specified list.
  void add_client_exposed_fixed_policies (CORBA::PolicyList *client_exposed_policies
                                          TAO_ENV_ARG_DECL);

  TAO_Policy_Set &policies (void);

protected:
  TAO_Policy_Set impl_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "POA_Policy_Set.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_POA_POLICY_SET_H */
