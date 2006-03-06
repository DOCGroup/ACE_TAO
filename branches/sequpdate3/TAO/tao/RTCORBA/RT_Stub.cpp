// $Id$

#include "RT_Stub.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "RT_Policy_i.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_Set.h"
#include "tao/Policy_Manager.h"


ACE_RCSID (RTCORBA,
           RT_Stub,
           "$Id$")


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
    are_policies_parsed_ (0)
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
TAO_RT_Stub::parse_policies (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::PolicyList_var policy_list
    = this->base_profiles_.policy_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = policy_list->length ();

  // Cache away the policies that we'll need later.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      if (policy_list[i]->policy_type () ==
           RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
        this->exposed_priority_model (policy_list[i]);

      else if (policy_list[i]->policy_type () ==
                RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
        this->exposed_priority_banded_connection (policy_list[i]);

      else if (policy_list[i]->policy_type () ==
                RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
        this->exposed_client_protocol (policy_list[i]);
    }

  this->are_policies_parsed_ = 1;
}

CORBA::Policy *
TAO_RT_Stub::exposed_priority_model (ACE_ENV_SINGLE_ARG_DECL)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return CORBA::Policy::_duplicate (this->priority_model_policy_.in ());
}

void
TAO_RT_Stub::exposed_priority_model (CORBA::Policy_ptr policy)
{
  this->priority_model_policy_ = CORBA::Policy::_duplicate (policy);
}

CORBA::Policy *
TAO_RT_Stub::exposed_priority_banded_connection (ACE_ENV_SINGLE_ARG_DECL)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return CORBA::Policy::_duplicate (this->priority_banded_connection_policy_.in ());
}

void
TAO_RT_Stub::exposed_priority_banded_connection (CORBA::Policy_ptr policy)
{
  this->priority_banded_connection_policy_ =
    CORBA::Policy::_duplicate (policy);
}

CORBA::Policy *
TAO_RT_Stub::exposed_client_protocol (ACE_ENV_SINGLE_ARG_DECL)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return CORBA::Policy::_duplicate (this->client_protocol_policy_.in ());
}

void
TAO_RT_Stub::exposed_client_protocol (CORBA::Policy_ptr policy)
{
  this->client_protocol_policy_ = CORBA::Policy::_duplicate (policy);
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
TAO_RT_Stub::get_policy (CORBA::PolicyType type
                         ACE_ENV_ARG_DECL)
{
  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
    return this->exposed_priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);

  if (type == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
    return this->effective_priority_banded_connection (ACE_ENV_SINGLE_ARG_PARAMETER);

  if (type == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
    return this->effective_client_protocol (ACE_ENV_SINGLE_ARG_PARAMETER);

  return this->TAO_Stub::get_policy (type ACE_ENV_ARG_PARAMETER);
}

CORBA::Policy_ptr
TAO_RT_Stub::get_cached_policy (TAO_Cached_Policy_Type type
                                ACE_ENV_ARG_DECL)
{
  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  if (type == TAO_CACHED_POLICY_PRIORITY_MODEL)
    return this->exposed_priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);

  if (type == TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION)
    return this->effective_priority_banded_connection (ACE_ENV_SINGLE_ARG_PARAMETER);

  if (type == TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL)
    return this->effective_client_protocol (ACE_ENV_SINGLE_ARG_PARAMETER);

  return this->TAO_Stub::get_cached_policy (type ACE_ENV_ARG_PARAMETER);
}


TAO_Stub *
TAO_RT_Stub::set_policy_overrides (const CORBA::PolicyList & policies,
                                   CORBA::SetOverrideType set_add
                                   ACE_ENV_ARG_DECL)
{
  // Validity check.  Make sure requested policies are allowed to be
  // set at this scope.
  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::PolicyType type = policy->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (type == RTCORBA::PRIORITY_MODEL_POLICY_TYPE ||
          type == RTCORBA::THREADPOOL_POLICY_TYPE ||
          type == RTCORBA::SERVER_PROTOCOL_POLICY_TYPE)
        ACE_THROW_RETURN (CORBA::NO_PERMISSION (), 0);

    }

  // We are not required to check for consistency of <policies> with
  // overrides at other levels or with policies exported in the IOR.
  return this->TAO_Stub::set_policy_overrides(policies, set_add ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_HAS_CORBA_MESSAGING */

CORBA::Policy *
TAO_RT_Stub::effective_priority_banded_connection (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->TAO_Stub::get_cached_policy (TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Get the value from the ior.
  CORBA::Policy_var exposed =
    this->exposed_priority_banded_connection (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  RTCORBA::PriorityBandedConnectionPolicy_var override_policy_var =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (override.in ()
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_PriorityBandedConnectionPolicy *override_policy =
    static_cast<TAO_PriorityBandedConnectionPolicy *> (override_policy_var.in ());

  RTCORBA::PriorityBandedConnectionPolicy_var exposed_policy_var =
    RTCORBA::PriorityBandedConnectionPolicy::_narrow (exposed.in ()
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_PriorityBandedConnectionPolicy *exposed_policy =
    static_cast<TAO_PriorityBandedConnectionPolicy *> (exposed_policy_var.in ());

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  if (exposed_policy->priority_bands_rep ().length () == 0)
    return override._retn ();

  if (override_policy->priority_bands_rep ().length () == 0)
    return exposed._retn ();

  // Both override and exposed have been set and neither has empty
  // priority bands.  This is illegal (ptc/99-05-03, sec. 4.12.1).
  ACE_THROW_RETURN (CORBA::INV_POLICY (),
                    0);
}

CORBA::Policy *
TAO_RT_Stub::effective_client_protocol (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->TAO_Stub::get_cached_policy (TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Get the value from the ior.
  CORBA::Policy_var exposed =
    this->exposed_client_protocol (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  RTCORBA::ClientProtocolPolicy_var override_policy_var =
    RTCORBA::ClientProtocolPolicy::_narrow (override.in ()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_ClientProtocolPolicy *override_policy =
    static_cast<TAO_ClientProtocolPolicy *> (override_policy_var.in ());

  RTCORBA::ClientProtocolPolicy_var exposed_policy_var =
    RTCORBA::ClientProtocolPolicy::_narrow (exposed.in ()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  TAO_ClientProtocolPolicy *exposed_policy =
    static_cast<TAO_ClientProtocolPolicy *> (exposed_policy_var.in ());

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
  ACE_THROW_RETURN (CORBA::INV_POLICY (),
                    0);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING  && TAO_HAS_CORBA_MESSAGING != 0 */
