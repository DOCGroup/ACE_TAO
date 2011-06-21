// -*- C++ -*-
// $Id$

#include "tao/BiDir_GIOP/BiDirPolicy_Validator.h"
#include "tao/BiDir_GIOP/BiDir_Policy_i.h"
#include "tao/Policy_Set.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_BiDirPolicy_Validator::TAO_BiDirPolicy_Validator (TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core)
{
}


void
TAO_BiDirPolicy_Validator::validate_impl (TAO_Policy_Set &policies)
{
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_BIDIRECTIONAL_GIOP);

  if (CORBA::is_nil (policy.in ()))
    return;

  BiDirPolicy::BidirectionalPolicy_var srp =
    BiDirPolicy::BidirectionalPolicy::_narrow (policy.in ());

  if (CORBA::is_nil (srp.in ()))
    return;

  BiDirPolicy::BidirectionalPolicyValue val = srp->value ();

  // Set the flag in the ORB_Core
  if (val == BiDirPolicy::BOTH)
    orb_core_.bidir_giop_policy (true);
}

void
TAO_BiDirPolicy_Validator::merge_policies_impl (TAO_Policy_Set &)
{
}

CORBA::Boolean
TAO_BiDirPolicy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  return (type == BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
