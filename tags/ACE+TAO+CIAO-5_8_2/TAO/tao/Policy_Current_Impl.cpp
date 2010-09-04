// -*- C++ -*-
// $Id$

#include "tao/Policy_Current_Impl.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Current_Impl.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Policy_Current_Impl::set_policy_overrides (
    const CORBA::PolicyList & policies,
    CORBA::SetOverrideType set_add)
{
  this->manager_impl_.set_policy_overrides (policies, set_add);
}

CORBA::PolicyList *
TAO_Policy_Current_Impl::get_policy_overrides (const CORBA::PolicyTypeSeq & ts)
{
  return this->manager_impl_.get_policy_overrides (ts);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
