#include "tao/ZIOP/ZIOP_Policy_Validator.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

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
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

  if (policy.in () == 0)
    return;

  ZIOP::CompressionEnablingPolicy_var srp =
    ZIOP::CompressionEnablingPolicy::_narrow (policy.in ());

  if (srp.in () == 0)
    return;

  // Set the flag in the ORB_Core
  orb_core_.ziop_enabled (srp->compression_enabled ());
}

void
TAO_ZIOPPolicy_Validator::merge_policies_impl (TAO_Policy_Set &policies)
{
  // Check if the user has specified the priority model policy.
  CORBA::Policy_var priority_model =
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

  if (CORBA::is_nil (priority_model.in ()))
    {
      // If not, check if the priority model policy has been specified
      // at the ORB level.
      priority_model =
        this->orb_core_.get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

      if (!CORBA::is_nil (priority_model.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (priority_model.in ());
        }
    }

  // Check if the user has specified the server protocol policy.
  CORBA::Policy_var server_protocol =
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY);

  if (CORBA::is_nil (server_protocol.in ()))
    {
      // If not, check if the server protocol policy has been
      // specified at the ORB level.
      server_protocol =
        this->orb_core_.get_cached_policy (TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY);

      if (!CORBA::is_nil (server_protocol.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (server_protocol.in ());
        }
    }

  // Check if the user has specified the server protocol policy.
  CORBA::Policy_var x =
    policies.get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);

  if (CORBA::is_nil (x.in ()))
    {
      // If not, check if the server protocol policy has been
      // specified at the ORB level.
      x =
        this->orb_core_.get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);

      if (!CORBA::is_nil (x.in ()))
        {
          // If so, we'll use that policy.
          policies.set_policy (x.in ());
        }
    }
}

CORBA::Boolean
TAO_ZIOPPolicy_Validator::legal_policy_impl (CORBA::PolicyType type)
{
  return (type == ZIOP::COMPRESSION_ENABLING_POLICY_ID ||
          type == ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID ||
          type == ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
