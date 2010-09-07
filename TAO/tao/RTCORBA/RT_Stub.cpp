// $Id$

#include "tao/RTCORBA/RT_Stub.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_Set.h"
#include "tao/Policy_Manager.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Stub::TAO_RT_Stub (const char *repository_id,
                          const TAO_MProfile &profiles,
                          TAO_ORB_Core *orb_core)
  : TAO_Stub (repository_id,
              profiles,
              orb_core),
    priority_model_policy_ (0),
    priority_banded_connection_policy_ (0),
    client_protocol_policy_ (0),
    are_policies_parsed_ (false)
{
}

TAO_RT_Stub::~TAO_RT_Stub (void)
{
  if (this->priority_model_policy_.in ())
    this->priority_model_policy_->destroy ();

  if (this->priority_banded_connection_policy_.in ())
    this->priority_banded_connection_policy_->destroy ();

  if (this->client_protocol_policy_.in ())
    this->client_protocol_policy_->destroy ();
}

void
TAO_RT_Stub::parse_policies (void)
{
  CORBA::PolicyList_var policy_list
    = this->base_profiles_.policy_list ();

  CORBA::ULong const length = policy_list->length ();

  // Cache away the policies that we'll need later.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      switch (policy_list[i]->policy_type ())
        {
          case RTCORBA::PRIORITY_MODEL_POLICY_TYPE:
            {
              this->exposed_priority_model (policy_list[i]);            }
            break;
          case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE :
            {
              this->exposed_priority_banded_connection (policy_list[i]);
            }
            break;
          case RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE :
            {
              this->exposed_client_protocol (policy_list[i]);
            }
            break;
        }
    }

  this->are_policies_parsed_ = true;
}

CORBA::Policy *
TAO_RT_Stub::exposed_priority_model (void)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies ();
    }

  return CORBA::Policy::_duplicate (this->priority_model_policy_.in ());
}

void
TAO_RT_Stub::exposed_priority_model (CORBA::Policy_ptr policy)
{
  this->priority_model_policy_ = CORBA::Policy::_duplicate (policy);
}

CORBA::Policy *
TAO_RT_Stub::exposed_priority_banded_connection (void)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies ();
    }

  return CORBA::Policy::_duplicate (this->priority_banded_connection_policy_.in ());
}

void
TAO_RT_Stub::exposed_priority_banded_connection (CORBA::Policy_ptr policy)
{
  this->priority_banded_connection_policy_ = CORBA::Policy::_duplicate (policy);
}

CORBA::Policy *
TAO_RT_Stub::exposed_client_protocol (void)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies ();
    }

  return CORBA::Policy::_duplicate (this->client_protocol_policy_.in ());
}

void
TAO_RT_Stub::exposed_client_protocol (CORBA::Policy_ptr policy)
{
  this->client_protocol_policy_ = CORBA::Policy::_duplicate (policy);
}

CORBA::Policy_ptr
TAO_RT_Stub::get_policy (CORBA::PolicyType type)
{
  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  switch (type)
    {
      case RTCORBA::PRIORITY_MODEL_POLICY_TYPE:
        {
          return this->exposed_priority_model ();
        }
      case RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE :
        {
          return this->effective_priority_banded_connection ();
        }
      case RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE :
        {
          return this->effective_client_protocol ();
        }
    }

  return this->TAO_Stub::get_policy (type);
}

CORBA::Policy_ptr
TAO_RT_Stub::get_cached_policy (TAO_Cached_Policy_Type type)
{
  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  switch (type)
    {
      case TAO_CACHED_POLICY_PRIORITY_MODEL:
        {
          return this->exposed_priority_model ();
        }
      case TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION :
        {
          return this->effective_priority_banded_connection ();
        }
      case TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL :
        {
          return this->effective_client_protocol ();
        }
      default:
        break;
    }

  return this->TAO_Stub::get_cached_policy (type);
}


TAO_Stub *
TAO_RT_Stub::set_policy_overrides (const CORBA::PolicyList & policies,
                                   CORBA::SetOverrideType set_add)
{
  // Validity check.  Make sure requested policies are allowed to be
  // set at this scope.
  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      if (!CORBA::is_nil (policy))
        {
          CORBA::PolicyType const type = policy->policy_type ();

          if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE ||
              type == RTCORBA::THREADPOOL_POLICY_TYPE ||
              type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE)
            throw ::CORBA::NO_PERMISSION ();
        }
    }

  // We are not required to check for consistency of <policies> with
  // overrides at other levels or with policies exported in the IOR.
  return this->TAO_Stub::set_policy_overrides(policies, set_add);
}

CORBA::Policy_ptr
TAO_RT_Stub::effective_priority_banded_connection (void)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->TAO_Stub::get_cached_policy (
      TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION);

  // Get the value from the ior.
  CORBA::Policy_var exposed = this->exposed_priority_banded_connection ();

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  RTCORBA::PriorityBandedConnectionPolicy_var override_policy_var =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (override.in ());

  TAO_PriorityBandedConnectionPolicy *override_policy =
    dynamic_cast<TAO_PriorityBandedConnectionPolicy *> (override_policy_var.in ());

  RTCORBA::PriorityBandedConnectionPolicy_var exposed_policy_var =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (exposed.in ());

  TAO_PriorityBandedConnectionPolicy *exposed_policy =
    dynamic_cast<TAO_PriorityBandedConnectionPolicy *> (exposed_policy_var.in ());

  if (!override_policy || !exposed_policy)
    {
      throw ::CORBA::INV_POLICY ();
    }

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  if (exposed_policy->priority_bands_rep ().length () == 0)
    return override._retn ();

  if (override_policy->priority_bands_rep ().length () == 0)
    return exposed._retn ();

  // Both override and exposed have been set and neither has empty
  // priority bands.  This is illegal (ptc/99-05-03, sec. 4.12.1).
  throw ::CORBA::INV_POLICY ();
}

CORBA::Policy_ptr
TAO_RT_Stub::effective_client_protocol (void)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->TAO_Stub::get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL);

  // Get the value from the ior.
  CORBA::Policy_var exposed =
    this->exposed_client_protocol ();

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  RTCORBA::ClientProtocolPolicy_var override_policy_var =
    RTCORBA::ClientProtocolPolicy::_narrow (override.in ());

  TAO_ClientProtocolPolicy *override_policy =
    dynamic_cast<TAO_ClientProtocolPolicy *> (override_policy_var.in ());

  RTCORBA::ClientProtocolPolicy_var exposed_policy_var =
    RTCORBA::ClientProtocolPolicy::_narrow (exposed.in ());

  TAO_ClientProtocolPolicy *exposed_policy =
    dynamic_cast<TAO_ClientProtocolPolicy *> (exposed_policy_var.in ());

  if (!override_policy || !exposed_policy)
    {
      throw ::CORBA::INV_POLICY ();
    }

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  RTCORBA::ProtocolList &protocols_rep_var =
    exposed_policy->protocols_rep ();

  if (protocols_rep_var.length () == 0)
    return override._retn ();

  if (override_policy->protocols_rep ().length () == 0)
    return exposed._retn ();

  // Both override and exposed have been set and neither has empty
  // protocols.  This is illegal (ptc/99-05-03, sec. 4.15.4).
  throw ::CORBA::INV_POLICY ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

 #endif /* TAO_HAS_CORBA_MESSAGING  && TAO_HAS_CORBA_MESSAGING != 0 */
