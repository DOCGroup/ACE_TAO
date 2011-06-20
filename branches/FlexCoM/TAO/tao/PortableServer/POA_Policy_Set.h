// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_Policy_Set.h
 *
 *  $Id$
 *
 *  POA
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_POA_POLICY_SET_H
#define TAO_POA_POLICY_SET_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Policy_Set.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Validator;

class TAO_PortableServer_Export TAO_POA_Policy_Set
{
public:

  TAO_POA_Policy_Set (void);

  TAO_POA_Policy_Set (const TAO_POA_Policy_Set &rhs);

  ~TAO_POA_Policy_Set (void);

  /// Returns the policy at the specified index.
  /// CORBA::Policy::_nil () is returned if the policy doesn't exist
  CORBA::Policy *get_policy_by_index (CORBA::ULong index);

  /// Get the number of policies
  CORBA::ULong num_policies (void) const;

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  void merge_policies (const CORBA::PolicyList &policies);

  void merge_policy (const CORBA::Policy_ptr policy);

  void validate_policies (TAO_Policy_Validator &validator,
                          TAO_ORB_Core &orb_core);

  /// Add all of the client exposed policies to the specified list.
  void add_client_exposed_fixed_policies (CORBA::PolicyList *client_exposed_policies);

  TAO_Policy_Set &policies (void);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy);

protected:
  TAO_Policy_Set impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/POA_Policy_Set.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_POA_POLICY_SET_H */
