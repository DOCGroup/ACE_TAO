#include "ORB_Time_Policy.h"

#if !defined (__ACE_INLINE__)
# include "ORB_Time_Policy.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_Time_Policy_T<ACE_Default_Time_Policy> ORB_Time_Policy::default_policy_;
  ACE_Dynamic_Time_Policy_Base const * ORB_Time_Policy::active_policy_ = &ORB_Time_Policy::default_policy_;

  void ORB_Time_Policy::set_time_policy (ACE_Dynamic_Time_Policy_Base const * pol)
  {
    if (pol != nullptr )
      {
        active_policy_ = pol;
      }
  }

  void ORB_Time_Policy::reset_time_policy ()
  {
    active_policy_ = &default_policy_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
