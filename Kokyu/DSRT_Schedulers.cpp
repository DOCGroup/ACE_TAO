// $Id$

#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"

#include "DSRT_Schedulers.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Schedulers.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, DSRT_Schedulers, "$Id$")


namespace Kokyu
{

DSRT_Scheduler_Impl::DSRT_Scheduler_Impl (ACE_Sched_Params::Policy sched_policy,
                                          int sched_scope)
  :sched_policy_ (sched_policy),
   sched_scope_ (sched_scope),
   min_prio_ (ACE_Sched_Params::priority_min (sched_policy, sched_scope)),
   max_prio_ (ACE_Sched_Params::priority_max (sched_policy, sched_scope))
{
}

int
DSRT_Scheduler_Impl::schedule (guid_t id, const QoSDescriptor& qos)
{
  return this->schedule_i (id, qos);
}

MIF_Scheduler_Impl::MIF_Scheduler_Impl (ACE_Sched_Params::Policy sched_policy,
                                        int sched_scope)
  : DSRT_Scheduler_Impl (sched_policy, sched_scope)
{
}

Priority_t
MIF_Scheduler_Impl::schedule_i (guid_t id, const QoSDescriptor& qos)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) request for MIF schedule\n"));

  switch (qos.importance_)
    {
    case VERY_LOW_IMPORTANCE:
      return min_prio_;
      break;

    case LOW_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (sched_policy_, min_prio_, sched_scope_);
      break;

    case MEDIUM_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (sched_policy_,
         ACE_Sched_Params::next_priority
         (sched_policy_, min_prio_, sched_scope_),
         sched_scope_);
      break;

    case HIGH_IMPORTANCE:
      return ACE_Sched_Params::previous_priority
        (sched_policy_, max_prio_, sched_scope_);
      break;

    case VERY_HIGH_IMPORTANCE:
      return max_prio_;
      break;

    default:
      break;
    }

  return min_prio_;
}


}
