// $Id$

#include "DSRT_Dispatcher_Impl.h"
#include "ace/Sched_Params.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Dispatcher_Impl, "$Id$")

namespace Kokyu
{

static Priority_t schedule_MIF (Importance_t imp)
{
  Priority_t min =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);

  Priority_t max =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);

  switch (imp)
    {
    case VERY_LOW_IMPORTANCE:
      return min;
      break;

    case LOW_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (ACE_SCHED_FIFO, min, ACE_SCOPE_THREAD);
      break;

    case MEDIUM_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (ACE_SCHED_FIFO,
         ACE_Sched_Params::next_priority
         (ACE_SCHED_FIFO, min, ACE_SCOPE_THREAD),
         ACE_SCOPE_THREAD);
      break;

    case HIGH_IMPORTANCE:
      return ACE_Sched_Params::previous_priority
        (ACE_SCHED_FIFO, max, ACE_SCOPE_THREAD);
      break;

    case VERY_HIGH_IMPORTANCE:
      return max;
      break;

    default:
      break;
    }

  return 0;
}

int
DSRT_Dispatcher_Impl::init_i (const DSRT_ConfigInfo& config_info)
{
  //create and init the dispatcher tasks here
  ACE_DEBUG ((LM_DEBUG, "entering init_i\n" ));

  //  ACE_NEW_RETURN (this->task_, DSRT_Dispatcher_Task (*config), -1);

  //this->activate ();

  curr_config_info_ = config_info;
  return 0;
}

  /************************
int
DSRT_Dispatcher_Impl::activate ()
{
  long flags = THR_BOUND | THR_SCHED_FIFO;

  Priority_t priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                    ACE_SCOPE_THREAD);

  if (this->task_->activate (flags, 1, 1, priority) == -1)
    {
      flags = THR_BOUND;
      priority = ACE_Sched_Params::priority_max (ACE_SCHED_OTHER,
                                                 ACE_SCOPE_THREAD);
      if (this->task_->activate (flags, 1, 1, priority) == -1)
        ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate queue %d", i));
    }
  return 0;
}
*********************************/

int
DSRT_Dispatcher_Impl::schedule_i (guid_t guid,
                  const QoSDescriptor& qos_info)
{
  ACE_UNUSED_ARG (guid);
  switch (curr_config_info_.scheduler_type_)
    {
    case SCHED_MIF:
      ACE_DEBUG ((LM_DEBUG, "(%t) request for MIF schedule\n"));
      Priority_t prio = schedule_MIF (qos_info.importance_);
      ACE_OS::thr_setprio (ACE_OS::thr_self (), prio);
      break;
    }
  return 0;
}

int
DSRT_Dispatcher_Impl::update_schedule_i (guid_t guid,
                  const QoSDescriptor& qos_info)
{
  ACE_UNUSED_ARG (guid);
  ACE_UNUSED_ARG (qos_info);
  return 0;
}

int
DSRT_Dispatcher_Impl::cancel_schedule_i (guid_t guid,
                  const QoSDescriptor& qos_info)
{
  return schedule_i (guid, qos_info);
}

}
