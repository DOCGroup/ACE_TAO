// $Id$

#include "tao/Strategies/LF_Strategy_Null.h"

#if !defined (__ACE_INLINE__)
# include "tao/Strategies/LF_Strategy_Null.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_Strategy_Null::~TAO_LF_Strategy_Null (void)
{
}

void
TAO_LF_Strategy_Null::set_upcall_thread (TAO_Leader_Follower &)
{
}

int
TAO_LF_Strategy_Null::set_event_loop_thread (ACE_Time_Value *,
                                             TAO_Leader_Follower &)
{
  return 0;
}

void
TAO_LF_Strategy_Null::reset_event_loop_thread (int,
                                               TAO_Leader_Follower &)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
