// $Id$
#include "RT_Stub.h"
#include "RT_Policy_i.h"

#if (TAO_HAS_RT_CORBA == 1)

ACE_RCSID(tao, TAO_RT_Stub, "$Id$")

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
  if (this->priority_model_policy_)
    this->priority_model_policy_->destroy ();

  if (this->priority_banded_connection_policy_)
    this->priority_banded_connection_policy_->destroy ();

  if (this->client_protocol_policy_)
    this->client_protocol_policy_->destroy ();
}

void
TAO_RT_Stub::parse_policies (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::PolicyList_var policy_list
    = this->base_profiles_.policy_list (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::ULong length = policy_list->length ();

  // @@ Priyanka The code implemented in this methods doesn't do what
  //    it is supposed to. I reverted your change for the time being.
  // this->orb_core ()->get_protocols_hooks ()->call_policy_type_hook (policy_list,
  //   policy_type);

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      if (policy_list[i]->policy_type () ==
           RTCORBA::PRIORITY_MODEL_POLICY_TYPE)
        this->exposed_priority_model (policy_list[i].in ());

      else if (policy_list[i]->policy_type () ==
                RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE)
        this->exposed_priority_banded_connection (policy_list[i].in ());

      else if (policy_list[i]->policy_type () ==
                RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
        this->exposed_client_protocol (policy_list[i].in ());
    }

  this->are_policies_parsed_ = 1;
}

CORBA::Policy *
TAO_RT_Stub::exposed_priority_model (CORBA::Environment &ACE_TRY_ENV)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return CORBA::Policy::_duplicate (this->priority_model_policy_);
}

void
TAO_RT_Stub::exposed_priority_model (CORBA::Policy_ptr policy)
{
  this->priority_model_policy_ = CORBA::Policy::_duplicate (policy);
}

CORBA::Policy *
TAO_RT_Stub::exposed_priority_banded_connection (CORBA::Environment &ACE_TRY_ENV)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return CORBA::Policy::_duplicate (this->priority_banded_connection_policy_);
}

void
TAO_RT_Stub::exposed_priority_banded_connection (CORBA::Policy_ptr policy)
{
  this->priority_banded_connection_policy_ =
    CORBA::Policy::_duplicate (policy);
}

CORBA::Policy *
TAO_RT_Stub::exposed_client_protocol (CORBA::Environment &ACE_TRY_ENV)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies (ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return CORBA::Policy::_duplicate (this->client_protocol_policy_);
}

void
TAO_RT_Stub::exposed_client_protocol (CORBA::Policy_ptr policy)
{
  this->client_protocol_policy_ = CORBA::Policy::_duplicate (policy);
}



#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
TAO_RT_Stub::get_policy (CORBA::PolicyType type,
                      CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  CORBA::ULong type_value = 0;

  // Validity check.  Make sure requested policy type is appropriate
  // for this scope.
  this->orb_core_->get_protocols_hooks ()->validate_policy_type (type,
                                                                 type_value,
                                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  if (type_value == 1)
    return this->exposed_priority_model (ACE_TRY_ENV);

  if (type_value == 2)
    return this->effective_priority_banded_connection (ACE_TRY_ENV);

  if (type_value == 3)
    return this->effective_client_protocol (ACE_TRY_ENV);

  return this->TAO_Stub::get_policy (type, ACE_TRY_ENV);
}

CORBA::Policy_ptr
TAO_RT_Stub::get_client_policy (CORBA::PolicyType type,
                                CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  CORBA::ULong type_value = 0;

  // Validity check.  Make sure requested policy type is appropriate
  // for this scope.
  this->orb_core_->get_protocols_hooks ()->validate_policy_type (type,
                                                                 type_value,
                                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  if (type_value == 1)
    return this->exposed_priority_model (ACE_TRY_ENV);

  if (type_value == 2)
    return this->effective_priority_banded_connection (ACE_TRY_ENV);

  if (type_value == 3)
    return this->effective_client_protocol (ACE_TRY_ENV);

  return this->TAO_Stub::get_client_policy (type, ACE_TRY_ENV);
}

TAO_Stub *
TAO_RT_Stub::set_policy_overrides (const CORBA::PolicyList & policies,
                                   CORBA::SetOverrideType set_add,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  // Validity check.  Make sure requested policies are allowed to be
  // set at this scope.
  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::ULong slot = policy->policy_type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::ULong type_value = 0;

      // @@ Not sure if this is right.
      //    This method was throwing CORBA::NO_PERMISSION when the
      //    if statements (which are now in the validate_policy_type
      //    hook) were true. I am using the same hook method
      //    for two other methods which need to throw
      //    CORBA::INV_Policy (). So, in here, I am not checking the
      //    CORBA::Environment variable, but checking on the value of
      //    type_value to throw the right exception. - Priyanka
      this->orb_core_->get_protocols_hooks ()->validate_policy_type (
         slot,
         type_value,
         ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (type_value == 1 || type_value == 4)
        ACE_THROW_RETURN (CORBA::NO_PERMISSION (), 0);

    }

  // We are not required to check for consistency of <policies> with
  // overrides at other levels or with policies exported in the IOR.


  return this->TAO_Stub::set_policy_overrides(policies, set_add, ACE_TRY_ENV);
}

#endif /* TAO_HAS_CORBA_MESSAGING */

CORBA::Policy *
TAO_RT_Stub::private_connection (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->private_connection ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.private_connection ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->private_connection ();
    }

  if (result == 0)
    result = this->orb_core_->default_private_connection ();

  return result;
}

CORBA::Policy *
TAO_RT_Stub::client_protocol (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->client_protocol ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.client_protocol ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->client_protocol ();
    }

  // No default is used for client priority policy (default creates
  // conflict in case the policy is also set for the object on the
  // server side).

  return result;
}

CORBA::Policy *
TAO_RT_Stub::priority_banded_connection (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->priority_banded_connection ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.priority_banded_connection ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->priority_banded_connection ();
    }

  if (result == 0)
    result = this->orb_core_->default_priority_banded_connection ();

  return result;
}

CORBA::Policy *
TAO_RT_Stub::effective_priority_banded_connection (CORBA::Environment &ACE_TRY_ENV)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->priority_banded_connection ();

  // Get the value from the ior.
  CORBA::Policy_var exposed =
    this->exposed_priority_banded_connection (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  CORBA::Policy_var policy =
    this->orb_core_->get_protocols_hooks ()->
    effective_priority_banded_connection_hook (override.in (),
                                               exposed.in (),
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return policy._retn ();
}

CORBA::Policy *
TAO_RT_Stub::effective_client_protocol (CORBA::Environment &ACE_TRY_ENV)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->client_protocol ();

  // Get the value from the ior.
  CORBA::Policy_var exposed =
    this->exposed_client_protocol (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  CORBA::Policy_var policy =
    this->orb_core_->get_protocols_hooks ()->effective_client_protocol_hook (
                                                override.in (),
                                                exposed.in (),
                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  /*if (
  ACE_CHECK_RETURN (CORBA::INV_POLICY (),
                    CORBA::Policy::_nil ());
  */

  return policy._retn ();
}

#endif /* TAO_HAS_RT_CORBA == 1 */
