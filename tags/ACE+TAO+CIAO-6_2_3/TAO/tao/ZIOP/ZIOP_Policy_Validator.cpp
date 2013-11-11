// $Id$

#include "tao/ZIOP/ZIOP_Policy_Validator.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ZIOP/ZIOP_Policy_i.h"
#include "tao/Policy_Set.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ZIOPPolicy_Validator::TAO_ZIOPPolicy_Validator (TAO_ORB_Core &orb_core)
  : TAO_Policy_Validator (orb_core)
{
}


void
TAO_ZIOPPolicy_Validator::validate_impl (TAO_Policy_Set &policies)
{
  CORBA::Policy_var policy =
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

  if (policy.in () == 0)
    return;

  ZIOP::CompressionEnablingPolicy_var srp =
    ZIOP::CompressionEnablingPolicy::_narrow (policy.in ());

  if (srp.in () == 0)
    return;
}

void
TAO_ZIOPPolicy_Validator::merge_policies_impl (TAO_Policy_Set &policies)
{
  // Check if the user has specified the compression enabled policy.
  CORBA::Policy_var compression_enabled =
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

  if (CORBA::is_nil (compression_enabled.in ()))
    {
      // If not, check if the compression enabled policy has been specified
      // at the ORB level.
      compression_enabled =
        this->orb_core_.get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

      if (!CORBA::is_nil (compression_enabled.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (compression_enabled.in ());
        }
    }

  // Check if the user has specified the compression low value policy.
  CORBA::Policy_var low_value_policy =
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY);

  if (CORBA::is_nil (low_value_policy.in ()))
    {
      // If not, check if the compression low value policy has been
      // specified at the ORB level.
      low_value_policy =
        this->orb_core_.get_cached_policy (TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY);

      if (!CORBA::is_nil (low_value_policy.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (low_value_policy.in ());
        }
    }

  // Check if the user has specified the minimum compression ratio policy.
  CORBA::Policy_var min_ratio_policy =
    policies.get_cached_policy (TAO_CACHED_MIN_COMPRESSION_RATIO_POLICY);

  if (CORBA::is_nil (min_ratio_policy.in ()))
    {
      // If not, check if the minimum compression ratio policy has been
      // specified at the ORB level.
      min_ratio_policy =
        this->orb_core_.get_cached_policy (TAO_CACHED_MIN_COMPRESSION_RATIO_POLICY);

      if (!CORBA::is_nil (min_ratio_policy.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (min_ratio_policy.in ());
        }
    }

  // Check if the user has specified the compression list policy.
  CORBA::Policy_var compressior_list_policy =
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);

  if (CORBA::is_nil (compressior_list_policy.in ()))
    {
      // If not, check if the compression list policy has been
      // specified at the ORB level.
      compressior_list_policy =
        this->orb_core_.get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);

      if (!CORBA::is_nil (compressior_list_policy.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (compressior_list_policy.in ());
        }
    }
}

CORBA::Boolean
TAO_ZIOPPolicy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  return (type == ZIOP::COMPRESSION_ENABLING_POLICY_ID ||
          type == ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID ||
          type == ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID ||
          type == ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
