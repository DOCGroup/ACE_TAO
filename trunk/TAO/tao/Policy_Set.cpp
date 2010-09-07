// -*- C++ -*-
// $Id$

#include "tao/Policy_Set.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "ace/CORBA_macros.h"

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Set.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Policy_Set::TAO_Policy_Set (TAO_Policy_Scope scope)
  : scope_ (scope)
{
  for (unsigned int i = 0; i < TAO_CACHED_POLICY_MAX_CACHED; ++i)
    this->cached_policies_[i] = 0;
}

TAO_Policy_Set::~TAO_Policy_Set (void)
{
  try
    {
      this->cleanup_i ();
    }
  catch (const ::CORBA::Exception&)
    {
      // Ignore exceptions...
    }
}

TAO_Policy_Set::TAO_Policy_Set (const TAO_Policy_Set &rhs)
  : scope_ (rhs.scope_)
{
  // Initialize the cache.
  for (int i = 0; i < TAO_CACHED_POLICY_MAX_CACHED; ++i)
    {
      this->cached_policies_[i] = 0;
    }

  // Copy over the policy list.
  this->policy_list_.length (rhs.policy_list_.length ());

  try
    {
      for (CORBA::ULong i = 0; i < rhs.policy_list_.length (); ++i)
        {
          CORBA::Policy_ptr policy = rhs.policy_list_[i];

          if (CORBA::is_nil (policy))
            {
              continue;
            }

          CORBA::Policy_var copy = policy->copy ();

          TAO_Cached_Policy_Type const cached_type = copy->_tao_cached_type ();

          // Add the "cacheable" policies into the cache.
          if (cached_type != TAO_CACHED_POLICY_UNCACHED && cached_type >= 0)
            {
              this->cached_policies_[cached_type] = copy.ptr ();
            }

          this->policy_list_[i] = copy._retn ();
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      if (TAO_debug_level > 4)
        ex._tao_print_exception ("TAO_Policy_Set::TAO_Policy_Set");

      // "Try" to make this recoverable as we must have run out of memory.
      this->policy_list_.length (0);
    }
}

void
TAO_Policy_Set::copy_from (TAO_Policy_Set *source)
{
  if (source == 0)
    {
      return;
    }

  this->cleanup_i ();

  for (CORBA::ULong i = 0; i < source->policy_list_.length (); ++i)
    {
      CORBA::Policy_ptr policy = source->policy_list_[i];

      if (CORBA::is_nil (policy))
        {
          continue;
        }

      if (! this->compatible_scope (policy->_tao_scope()))
        {
          throw ::CORBA::NO_PERMISSION ();
        }

      CORBA::Policy_var copy = policy->copy ();

      CORBA::ULong const length = this->policy_list_.length ();
      this->policy_list_.length (length + 1);

      TAO_Cached_Policy_Type const cached_type =
        copy->_tao_cached_type ();

      // Add the "cacheable" policies into the cache.
      if (cached_type != TAO_CACHED_POLICY_UNCACHED
          && cached_type >= 0)
        {
          this->cached_policies_[cached_type] = copy.ptr ();
        }

      this->policy_list_[length] = copy._retn ();
    }
}

void
TAO_Policy_Set::cleanup_i (void)
{
  CORBA::ULong const len = this->policy_list_.length ();
  // Cleanup the policy list.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      this->policy_list_[i]->destroy ();
      this->policy_list_[i] = CORBA::Policy::_nil ();
    }

  this->policy_list_.length (0);

  // Cleanup the cache.
  for (CORBA::ULong j = 0; j < TAO_CACHED_POLICY_MAX_CACHED; ++j)
    {
      this->cached_policies_[j] = 0;
    }
}

  // @@ !!! Add comments regarding Policy lifetimes, etc.
void
TAO_Policy_Set::set_policy_overrides (const CORBA::PolicyList &policies,
                                      CORBA::SetOverrideType set_add)
{
  // @@ The spec does not say what to do on this case.
  if (set_add != CORBA::SET_OVERRIDE && set_add != CORBA::ADD_OVERRIDE)
    {
      throw ::CORBA::BAD_PARAM ();
    }

  if (set_add == CORBA::SET_OVERRIDE)
    {
      this->cleanup_i ();
    }

  // Flag, indicating whether we have already overridden
  // RTCORBA::ServerProtocolPolicy during this call.
  bool server_protocol_set = false;

  CORBA::ULong const plen = policies.length ();

  for (CORBA::ULong i = 0; i < plen; ++i)
    {
      CORBA::Policy_ptr policy = policies[i];

      if (CORBA::is_nil (policy))
        {
          continue;
        }

      CORBA::PolicyType const policy_type = policy->policy_type ();

      if (policy_type == TAO_RT_SERVER_PROTOCOL_POLICY_TYPE)
        {
          // Only one ServerProtocolPolicy should be included in a
          // given PolicyList (section 4.15.2 of RTCORBA 1.0, i.e.,
          // ptc/99-05-03).
          // User-caused exceptional conditions can leave the Policy
          // Manager in an inconsistent state.  It is the
          // responsibility of the user to return it to consistent state.
          if (server_protocol_set)
            {
              throw ::CORBA::INV_POLICY ();
            }

          server_protocol_set = true;
        }

      this->set_policy (policy);
    }
}

void
TAO_Policy_Set::set_policy (const CORBA::Policy_ptr policy)
{
  if (! this->compatible_scope (policy->_tao_scope()))
    {
      throw ::CORBA::NO_PERMISSION ();
    }

  CORBA::PolicyType const policy_type = policy->policy_type ();

  CORBA::Policy_var copy = policy->copy ();

  CORBA::ULong j = 0;
  CORBA::ULong const length = this->policy_list_.length ();

  while (j != length)
    {
      CORBA::ULong const current =
        this->policy_list_[j]->policy_type ();

      if (current == policy_type)
        {
          this->policy_list_[j]->destroy ();

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
  TAO_Cached_Policy_Type const cached_policy_type =
    policy->_tao_cached_type ();

  if (cached_policy_type != TAO_CACHED_POLICY_UNCACHED
      && cached_policy_type >= 0)
    {
      this->cached_policies_[cached_policy_type] = copy.ptr ();
    }

  // Transfer ownership to the policy list.
  (void) copy._retn ();
}

CORBA::PolicyList *
TAO_Policy_Set::get_policy_overrides (const CORBA::PolicyTypeSeq &types)
{
  CORBA::ULong const slots = types.length ();
  CORBA::PolicyList *policy_list_ptr = 0;

  if (slots == 0)
    {
      // Copy our own policy list.
      ACE_NEW_THROW_EX (policy_list_ptr,
                        CORBA::PolicyList (this->policy_list_),
                        CORBA::NO_MEMORY ());

      return policy_list_ptr;
    }

  ACE_NEW_THROW_EX (policy_list_ptr,
                    CORBA::PolicyList (slots),
                    CORBA::NO_MEMORY ());

  CORBA::PolicyList_var policy_list (policy_list_ptr);
  policy_list->length (slots);
  CORBA::ULong n = 0;

  for (CORBA::ULong j = 0; j < slots; ++j)
    {
      CORBA::ULong const slot = types[j];
      CORBA::ULong const length = this->policy_list_.length ();

      for (CORBA::ULong i = 0; i < length; ++i)
        {
          CORBA::ULong const current =
            this->policy_list_[i]->policy_type ();

          if (current != slot)
            {
              continue;
            }

          policy_list[n++] =
            CORBA::Policy::_duplicate (this->policy_list_[i]);
          break;
        }
    }

  policy_list->length (n);  // Truncate buffer if necessary.

  return policy_list._retn ();
}

CORBA::Policy_ptr
TAO_Policy_Set::get_policy (CORBA::PolicyType type)
{
  CORBA::ULong const length = this->policy_list_.length ();

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::PolicyType const current = this->policy_list_[i]->policy_type ();

      if (current == type)
        {
          return CORBA::Policy::_duplicate (this->policy_list_[i]);
        }
    }

  return CORBA::Policy::_nil ();
}

CORBA::Policy_ptr
TAO_Policy_Set::get_cached_const_policy (TAO_Cached_Policy_Type type) const
{
  if (type != TAO_CACHED_POLICY_UNCACHED && type < TAO_CACHED_POLICY_MAX_CACHED)
    {
      return this->cached_policies_[type];
    }

  return CORBA::Policy::_nil ();
}

CORBA::Policy_ptr
TAO_Policy_Set::get_cached_policy (TAO_Cached_Policy_Type type)
{
  if (type != TAO_CACHED_POLICY_UNCACHED && type < TAO_CACHED_POLICY_MAX_CACHED)
    {
      return CORBA::Policy::_duplicate (this->cached_policies_[type]);
    }

  return CORBA::Policy::_nil ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
