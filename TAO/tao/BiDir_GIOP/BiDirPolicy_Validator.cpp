#include "BiDirPolicy_Validator.h"
#include "BiDir_Policy_i.h"
#include "tao/Policy_Set.h"
#include "tao/ORB_Core.h"

ACE_RCSID(TAO, BiDirPolicy_Validator, "$Id$")

TAO_BiDirPolicy_Validator::TAO_BiDirPolicy_Validator (TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core)
{
}


void
TAO_BiDirPolicy_Validator::validate_impl (TAO_Policy_Set &policies
                                          ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_POLICY_BIDIRECTIONAL_GIOP);

  BiDirPolicy::BidirectionalPolicy_var srp =
    BiDirPolicy::BidirectionalPolicy::_narrow (policy.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  BiDirPolicy::BidirectionalPolicyValue val =
    srp->value (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Set the flag in the ORB_Core
  if (val == BiDirPolicy::BOTH)
    orb_core_.bidir_giop_policy (1);
}

void
TAO_BiDirPolicy_Validator::merge_policies_impl (TAO_Policy_Set & /*policies*/
                                                ACE_ENV_ARG_DECL_NOT_USED)
{
  return ;
}

CORBA::Boolean
TAO_BiDirPolicy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  return (type == BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE);
}
