// $Id$

#include "tao/Policy_Current.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/TSS_Resources.h"

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Current.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Policy_Current,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Policy_Current_Impl &
TAO_Policy_Current::implementation (TAO_Policy_Current_Impl &current)
{
  TAO_TSS_Resources * const tss =
    TAO_TSS_Resources::instance ();

  TAO_Policy_Current_Impl *old = tss->policy_current_;
  tss->policy_current_ = &current;
  return *old;
}

TAO_Policy_Current_Impl &
TAO_Policy_Current::implementation (void) const
{
  return *TAO_TSS_Resources::instance ()->policy_current_;
}

CORBA::Policy_ptr
TAO_Policy_Current::get_policy (
      CORBA::PolicyType policy
      ACE_ENV_ARG_DECL)
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.get_policy (policy ACE_ENV_ARG_PARAMETER);
}

CORBA::PolicyList *
TAO_Policy_Current::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts
        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.get_policy_overrides (ts ACE_ENV_ARG_PARAMETER);
}

void
TAO_Policy_Current::set_policy_overrides (const CORBA::PolicyList & policies,
                                          CORBA::SetOverrideType set_add
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::InvalidPolicies))
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  impl.set_policy_overrides (policies, set_add ACE_ENV_ARG_PARAMETER);
}

CORBA::Policy_ptr
TAO_Policy_Current::get_cached_policy (TAO_Cached_Policy_Type type
                                       ACE_ENV_ARG_DECL)
{
  TAO_Policy_Current_Impl &impl = this->implementation ();

  return impl.get_cached_policy (type
                                 ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
