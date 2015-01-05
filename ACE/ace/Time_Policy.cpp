// $Id$

#include "ace/Time_Policy.h"

#if !defined(__ACE_INLINE__)
# include "ace/Time_Policy.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Dynamic_Time_Policy_Base::~ACE_Dynamic_Time_Policy_Base ()
{
}

ACE_Time_Value ACE_Delegating_Time_Policy::NULL_Time_Policy::gettimeofday () const
{
  return ACE_Time_Value::zero;
}

ACE_Delegating_Time_Policy::NULL_Time_Policy ACE_Delegating_Time_Policy::null_policy_;

ACE_END_VERSIONED_NAMESPACE_DECL

