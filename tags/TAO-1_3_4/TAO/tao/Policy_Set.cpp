// $Id$

#include "tao/Policy_Set.h"
#include "tao/Environment.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Set.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Policy_Set, "$Id$")

TAO_Policy_Set::~TAO_Policy_Set (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->cleanup_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions...
    }
  ACE_ENDTRY;
}

TAO_Policy_Set::TAO_Policy_Set (const TAO_Policy_Set &rhs)
  :
    scope_ (rhs.scope_)
{
  // Initialize the cache.
  for (int i = 0; i < TAO_CACHED_POLICY_MAX_CACHED; i++)
    this->cached_policies_[i] = 0;

  // Copy over the policy list.
  this->policy_list_.length (rhs.policy_list_.length ());

  ACE_TRY_NEW_ENV
    {
      for (CORBA::ULong i = 0;
           i < rhs.policy_list_.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = rhs.policy_list_[i];
          if (CORBA::is_nil (policy))
            continue;

          CORBA::Policy_var copy = policy->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          // Add the "cacheable" policies into the cache.
          if (copy->_tao_cached_type () != TAO_CACHED_POLICY_UNCACHED)
            this->cached_policies_[copy->_tao_cached_type ()] = copy.ptr ();

          this->policy_list_[i] = copy._retn ();
        }
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 4)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "TAO_Policy_Set::TAO_Policy_Set");

      // "Try" to make this recoverable as we must have run out of memory.
      this->policy_list_.length (0);
    }
  ACE_ENDTRY;
}

void
TAO_Policy_Set::copy_from (TAO_Policy_Set *source
                                    ACE_ENV_ARG_DECL)
{
  if (source == 0)
    return;

  this->cleanup_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0;
       i < source->policy_list_.length ();
       ++i)
    {
      CORBA::Policy_ptr policy = source->policy_list_[i];
      if (CORBA::is_nil (policy))
        continue;

      if (! this->compatible_scope (policy->_tao_scope()))
        ACE_THROW (CORBA::NO_PERMISSION ());

      CORBA::Policy_var copy = policy->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::ULong length = this->policy_list_.length ();
      this->policy_list_.length (length + 1);

      // Add the "cacheable" policies into the cache.
      if (copy->_tao_cached_type () != TAO_CACHED_POLICY_UNCACHED)
        this->cached_policies_[copy->_tao_cached_type ()] = copy.ptr ();

      this->policy_list_[length] = copy._retn ();
    }
}

void
TAO_Policy_Set::cleanup_i (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ULong i;

  // Cleanup the policy list.
  for (i = 0;
       i < this->policy_list_.length ();
       ++i)
    {
      this->policy_list_[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      this->policy_list_[i] = CORBA::Policy::_nil ();
    }
  this->policy_list_.length (0);

  // Cleanup the cache.
  for (i = 0; i < TAO_CACHED_POLICY_MAX_CACHED; i++)
    this->cached_policies_[i] = 0;
}

  // @@ !!! Add comments regarding Policy lifetimes, etc.
void
TAO_Policy_Set::set_policy_overrides (
    const CORBA::PolicyList &policies,
    CORBA::SetOverrideType set_add
    ACE_ENV_ARG_DECL)
{
  // @@ The spec does not say what to do on this case.
  if (set_add != CORBA::SET_OVERRIDE
      && set_add != CORBA::ADD_OVERRIDE)
    ACE_THROW (CORBA::BAD_PARAM ());

  if (set_add == CORBA::SET_OVERRIDE)
    {
      this->cleanup_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  // Flag, indicating whether we have already overridden
  // RTCORBA::ServerProtocolPolicy during this call.
  int server_protocol_set = 0;

  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      // Because MSVC 5 has a bug, we had to replace
      // a simple CORBA::Policy_var policy = policies[i];
      // with the following.
      CORBA::Policy_ptr temp = policies[i];
      CORBA::Policy_var policy = CORBA::Policy::_duplicate (temp);

      if (CORBA::is_nil (policy.in ()))
        continue;

      CORBA::PolicyType policy_type = policy->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (policy_type == TAO_RT_SERVER_PROTOCOL_POLICY_TYPE)
        {
          // Only one ServerProtocolPolicy should be included in a
          // given PolicyList (section 4.15.2 of RTCORBA 1.0, i.e.,
          // ptc/99-05-03).
          // User-caused exceptional conditions can leave the Policy
          // Manager in an inconsistent state.  It is the
          // responsibility of the user to return it to consistent state.
          if (server_protocol_set != 0)
            ACE_THROW (CORBA::INV_POLICY ());
          server_protocol_set = 1;
        }

      this->set_policy (policy.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_Policy_Set::set_policy (const CORBA::Policy_ptr policy
                                     ACE_ENV_ARG_DECL)
{
  if (! this->compatible_scope (policy->_tao_scope()))
    ACE_THROW (CORBA::NO_PERMISSION ());

  CORBA::PolicyType policy_type = policy->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Policy_var copy = policy->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;


  CORBA::ULong j = 0;
  CORBA::ULong length = this->policy_list_.length ();
  while (j != length)
    {
      CORBA::ULong current =
        this->policy_list_[j]->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (current == policy_type)
        {
          this->policy_list_[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
          this->policy_list_[j] = copy.ptr ();
          break;
        }
      ++j;
    }
  if (j == length)
    {
      this->policy_list_.length (length + 1);
      this->policy_list_[j] = copy.ptr ();
    }

  // If this is a policy that gets accessed on the critical path,
  // save a pointer to it in the cache.
  TAO_Cached_Policy_Type cached_policy_type = policy->_tao_cached_type ();
  if (cached_policy_type != TAO_CACHED_POLICY_UNCACHED)
    this->cached_policies_[cached_policy_type] = copy.ptr ();

  // Transfer ownership to the policy list.
  (void) copy._retn ();
}

CORBA::PolicyList *
TAO_Policy_Set::get_policy_overrides (
    const CORBA::PolicyTypeSeq &types
    ACE_ENV_ARG_DECL)
{
  CORBA::ULong types_length = types.length ();

  CORBA::ULong slots = types_length;
  CORBA::PolicyList *policy_list_ptr;

  if (slots == 0)
    {
      slots = this->policy_list_.length ();
      ACE_NEW_THROW_EX (policy_list_ptr,
                        CORBA::PolicyList (slots),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);
      *policy_list_ptr = this->policy_list_;
      return policy_list_ptr;
    }

  ACE_NEW_THROW_EX (policy_list_ptr,
                    CORBA::PolicyList (slots),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::PolicyList_var policy_list (policy_list_ptr);

  policy_list->length (slots);

  CORBA::ULong n = 0;

  for (CORBA::ULong j = 0; j < types.length ();  ++j)
    {
      CORBA::ULong slot = types[j];

      CORBA::ULong length = this->policy_list_.length ();
      for (CORBA::ULong i = 0;
           i != length;
           ++i)
        {
          CORBA::ULong current =
            this->policy_list_[i]->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (current != slot)
            continue;

          policy_list[n++] =
            CORBA::Policy::_duplicate (this->policy_list_[i].in ());
          break;
        }
    }
  policy_list->length (n);
  return policy_list._retn ();
}

CORBA::Policy_ptr
TAO_Policy_Set::get_policy (CORBA::PolicyType type
                                     ACE_ENV_ARG_DECL)
{
  CORBA::ULong length = this->policy_list_.length ();
  for (CORBA::ULong i = 0;
       i != length;
       ++i)
    {
      CORBA::ULong current =
        this->policy_list_[i]->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (current != type)
        continue;

      return CORBA::Policy::_duplicate (this->policy_list_[i].in ());
    }

  return CORBA::Policy::_nil ();
}

CORBA::Policy_ptr
TAO_Policy_Set::get_cached_const_policy (TAO_Cached_Policy_Type type) const
{
  if (type != TAO_CACHED_POLICY_UNCACHED && type < TAO_CACHED_POLICY_MAX_CACHED)
    return this->cached_policies_[type];

  return CORBA::Policy::_nil ();
}

CORBA::Policy_ptr
TAO_Policy_Set::get_cached_policy (TAO_Cached_Policy_Type type)
{
  if (type != TAO_CACHED_POLICY_UNCACHED && type < TAO_CACHED_POLICY_MAX_CACHED)
    return CORBA::Policy::_duplicate (this->cached_policies_[type]);

  return CORBA::Policy::_nil ();
}
