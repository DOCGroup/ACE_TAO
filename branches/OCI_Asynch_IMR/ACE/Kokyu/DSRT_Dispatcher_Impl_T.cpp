// $Id$

#ifndef DSRT_DISPATCHER_IMPL_T_CPP
#define DSRT_DISPATCHER_IMPL_T_CPP

#include "DSRT_Dispatcher_Impl_T.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl_T.inl"
#endif /* __ACE_INLINE__ */

namespace Kokyu
{

template <class DSRT_Scheduler_Traits>
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::
DSRT_Dispatcher_Impl (ACE_Sched_Params::Policy sched_policy,
                          int sched_scope)
  : sched_policy_ (sched_policy),
    sched_scope_ (sched_scope),
    min_prio_ (ACE_Sched_Params::priority_min
               (sched_policy_,
                sched_scope)),
    max_prio_ (ACE_Sched_Params::priority_max
               (sched_policy_,
                sched_scope)),
    executive_prio_ (max_prio_),
    blocked_prio_ (ACE_Sched_Params::previous_priority
                   (sched_policy_,
                    max_prio_,
                    sched_scope)),
    inactive_prio_ (min_prio_),
    active_prio_ (ACE_Sched_Params::next_priority
                  (sched_policy_,
                   min_prio_)),
    shutdown_flagged_ (0),
    non_rt_thr_flags_ (THR_NEW_LWP | THR_JOINABLE | THR_BOUND),
    curr_scheduled_thr_handle_ (0)
{
  if (sched_policy_ == ACE_SCHED_FIFO)
    rt_thr_flags_ = non_rt_thr_flags_ | THR_SCHED_FIFO;
  else if (sched_policy_ == ACE_SCHED_RR)
    rt_thr_flags_ = non_rt_thr_flags_ | THR_SCHED_RR;
}

//virtual - so don't inline
template <class DSRT_Scheduler_Traits>
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::~DSRT_Dispatcher_Impl ()
{
}

}

#endif /* DSRT_DISPATCHER_IMPL_T_CPP */
