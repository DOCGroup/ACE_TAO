// $Id$

#include "tao/Policy_Current_Impl.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Current_Impl.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           Policy_Manager,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Policy_Current_Impl::set_policy_overrides (
    const CORBA::PolicyList & policies,
    CORBA::SetOverrideType set_add
    ACE_ENV_ARG_DECL
  )
{
  this->manager_impl_.set_policy_overrides (policies,
                                            set_add
                                            ACE_ENV_ARG_PARAMETER);
}

CORBA::PolicyList *
TAO_Policy_Current_Impl::get_policy_overrides (
    const CORBA::PolicyTypeSeq & ts
    ACE_ENV_ARG_DECL
  )
{
  return this->manager_impl_.get_policy_overrides (ts
                                                   ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
