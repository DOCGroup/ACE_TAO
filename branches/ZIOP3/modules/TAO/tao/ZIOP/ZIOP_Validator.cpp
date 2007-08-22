#include "tao/ZIOP/ZIOP_Validator.h"
#include "tao/ZIOP/ZIOP_Policy_i.h"
#include "tao/Policy_Set.h"
#include "tao/ORB_Core.h"

ACE_RCSID (ZIOP_GIOP,
           ZIOPPolicy_Validator,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ZIOPPolicy_Validator::TAO_ZIOPPolicy_Validator (TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core)
{
}


void
TAO_ZIOPPolicy_Validator::validate_impl (TAO_Policy_Set &policies)
{
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_ZIOP);

  if (policy.in () == 0)
    return;

  ZIOP::CompressorIdPolicy_var srp =
    ZIOP::CompressorIdPolicy::_narrow (policy.in ());

  if (srp.in () == 0)
    return;

//  ZIOPPolicy::ZIOPectionalPolicyValue val = srp->value ();

//  // Set the flag in the ORB_Core
  //if (val == ZIOPPolicy::BOTH)
    //orb_core_.ZIOP_giop_policy (true);
}

void
TAO_ZIOPPolicy_Validator::merge_policies_impl (TAO_Policy_Set &)
{
}

CORBA::Boolean
TAO_ZIOPPolicy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  return true;//(type == ZIOPPolicy::ZIOPECTIONAL_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
