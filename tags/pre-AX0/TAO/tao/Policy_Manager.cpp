// $Id$

#include "tao/Policy_Manager.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/Environment.h"
#include "tao/ORB_Core.h"

#include "tao/Buffering_Constraint_Policy.h"
#include "tao/Messaging_Policy_i.h"

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Manager.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Policy_Manager, "$Id$")


TAO_Policy_Current_Impl &
TAO_Policy_Current::implementation (TAO_Policy_Current_Impl &current)
{
  TAO_TSS_Resources *tss =
    TAO_TSS_RESOURCES::instance ();

  TAO_Policy_Current_Impl *old = tss->policy_current_;
  tss->policy_current_ = &current;
  return *old;
}

TAO_Policy_Current_Impl &
TAO_Policy_Current::implementation (void) const
{
  return *TAO_TSS_RESOURCES::instance ()->policy_current_;
}

void
TAO_Policy_Current_Impl::set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add
        ACE_ENV_ARG_DECL)
{
  this->manager_impl_.set_policy_overrides (policies, set_add ACE_ENV_ARG_PARAMETER);
}

CORBA::PolicyList *
TAO_Policy_Current_Impl::get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts
        ACE_ENV_ARG_DECL)
{
  return this->manager_impl_.get_policy_overrides (ts ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */






