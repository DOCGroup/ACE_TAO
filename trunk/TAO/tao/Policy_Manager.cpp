// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING)

#include "tao/Policy_Manager.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Manager.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Policy_Manager, "$Id$")

void
TAO_Policy_Manager_Impl::set_policy_overrides (
    const CORBA::PolicyList & policies,
    CORBA::SetOverrideType set_add,
    CORBA::Environment &ACE_TRY_ENV)
{
  // @@ The spec does not say what to do on this case.
  if (set_add != CORBA::SET_OVERRIDE
      && set_add != CORBA::ADD_OVERRIDE)
    ACE_THROW (CORBA::BAD_PARAM (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));

  CORBA::InvalidPolicies invalid_policies;
  invalid_policies.indices.length (policies.length ());
  int n = 0;
  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::ULong index = policy->policy_type (ACE_TRY_ENV);
      ACE_CHECK;

      if (TAO_MIN_PROPIETARY_POLICY <= index
          && index < TAO_MAX_PROPIETARY_POLICY)
        index -= TAO_MIN_PROPIETARY_POLICY;

      if (index >= TAO_MAX_POLICIES)
        invalid_policies.indices[n++] = CORBA::UShort (i);
    }
  if (n != 0)
    {
      invalid_policies.indices.length (n);
      ACE_THROW (CORBA::InvalidPolicies (invalid_policies));
    }

  if (set_add == CORBA::SET_OVERRIDE)
    {
      for (int i = 0; i < TAO_MAX_POLICIES; ++i)
        {
          this->policies_[i] = CORBA::Policy::_nil ();
        }
      this->count_ = 0;
    }

  for (CORBA::ULong j = 0; j < policies.length ();  ++j)
    {
      CORBA::Policy_ptr policy = policies[j];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::ULong index = policy->policy_type (ACE_TRY_ENV);
      ACE_CHECK;

      if (TAO_MIN_PROPIETARY_POLICY <= index
          && index < TAO_MAX_PROPIETARY_POLICY)
        index -= TAO_MIN_PROPIETARY_POLICY;

      // No need to validate the index...
      if (CORBA::is_nil (this->policies_[index].in ()))
        this->count_++;

      this->policies_[index] =
        CORBA::Policy::_duplicate (policy);
    }
}

CORBA::PolicyList *
TAO_Policy_Manager_Impl::get_policy_overrides (
    const CORBA::PolicyTypeSeq & types,
    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong l = types.length ();

  CORBA::ULong slots = l;
  if (slots == 0)
    slots = this->count_;

  CORBA::PolicyList_var policy_list =
    new CORBA::PolicyList (slots);
  policy_list->length (slots);

  int n = 0;
  if (types.length () == 0)
    {
      for (int i = 0; i < TAO_MAX_POLICIES; ++i)
        {
          if (CORBA::is_nil (this->policies_[i].in ()))
            continue;
          policy_list[n++] =
            CORBA::Policy::_duplicate (this->policies_[i].in ());
        }
    }
  else
    {
      for (CORBA::ULong j = 0; j < types.length ();  ++j)
        {
          CORBA::ULong index = types[j];

          if (TAO_MIN_PROPIETARY_POLICY <= index
              && index < TAO_MAX_PROPIETARY_POLICY)
            index -= TAO_MIN_PROPIETARY_POLICY;

          if (index < TAO_MAX_POLICIES)
            policy_list[n++] =
              CORBA::Policy::_duplicate (this->policies_[index].in ());
        }
    }

  policy_list->length (n);
  return policy_list._retn ();
}

CORBA::Policy_ptr
TAO_Policy_Manager_Impl::get_policy (
    CORBA::PolicyType type,
    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong index = type;

  if (TAO_MIN_PROPIETARY_POLICY <= index
      && index < TAO_MAX_PROPIETARY_POLICY)
    index -= TAO_MIN_PROPIETARY_POLICY;

  if (index >= TAO_MAX_POLICIES)
    return CORBA::Policy::_nil ();

  return CORBA::Policy::_duplicate (this->policies_[index].in ());
}

// ****************************************************************

CORBA::PolicyList *
TAO_Policy_Manager::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV)
{
  return this->impl_.get_policy_overrides (ts, ACE_TRY_ENV);
}

void
TAO_Policy_Manager::set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV)
{
  this->impl_.set_policy_overrides (policies, set_add, ACE_TRY_ENV);
}

// ****************************************************************

CORBA::PolicyList *
TAO_Policy_Current::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV)
{
  return this->impl_.get_policy_overrides (ts, ACE_TRY_ENV);
}

void
TAO_Policy_Current::set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV)
{
  this->impl_.set_policy_overrides (policies, set_add, ACE_TRY_ENV);
}

#endif /* TAO_HAS_CORBA_MESSAGING */
