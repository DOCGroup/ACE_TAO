// $Id$

#include "tao/Policy_Manager.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#if !defined (__ACE_INLINE__)
# include "tao/Policy_Manager.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::PolicyList *
TAO_Policy_Manager::get_policy_overrides (const CORBA::PolicyTypeSeq & ts)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return this->impl_.get_policy_overrides (ts);
}

void
TAO_Policy_Manager::set_policy_overrides (const CORBA::PolicyList & policies,
                                          CORBA::SetOverrideType set_add)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->impl_.set_policy_overrides (policies, set_add);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
