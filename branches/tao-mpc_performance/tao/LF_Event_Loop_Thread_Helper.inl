// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_LF_Event_Loop_Thread_Helper::TAO_LF_Event_Loop_Thread_Helper (
  TAO_Leader_Follower &leader_follower,
  TAO_LF_Strategy &lf_strategy,
  ACE_Time_Value *max_wait_time)
  : leader_follower_ (leader_follower)
  , lf_strategy_ (lf_strategy)
{
  this->event_loop_return_ =
    this->lf_strategy_.set_event_loop_thread (max_wait_time, leader_follower_);
}

ACE_INLINE
TAO_LF_Event_Loop_Thread_Helper::~TAO_LF_Event_Loop_Thread_Helper (void)
{
  int const call_reset = (this->event_loop_return_ == 0);
  this->lf_strategy_.reset_event_loop_thread (call_reset,
                                              this->leader_follower_);
}

ACE_INLINE int
TAO_LF_Event_Loop_Thread_Helper::event_loop_return (void) const
{
  return this->event_loop_return_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
