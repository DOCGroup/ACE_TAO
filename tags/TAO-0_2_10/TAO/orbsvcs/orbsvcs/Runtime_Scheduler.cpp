// $Id$

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Runtime_Scheduler.h"

#if defined (__ACE_INLINE__)
#include "orbsvcs/Runtime_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Runtime_Scheduler, "$Id$")

ACE_Runtime_Scheduler::
ACE_Runtime_Scheduler (int config_count,
					   ACE_Scheduler_Factory::POD_Config_Info config_info[],
					   int entry_count,
                       ACE_Scheduler_Factory::POD_RT_Info rt_info[])
  : config_count_ (config_count)
  , config_info_ (config_info)
  , entry_count_ (entry_count)
  , rt_info_ (rt_info)
{
}

RtecScheduler::handle_t
ACE_Runtime_Scheduler::create (const char * entry_point,
                               CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::DUPLICATE_NAME))
{
  // Just make sure its there and returns its handle (position).
  int i;
  for (i = 0; i < entry_count_; ++i)
    {
      if (strcmp (entry_point, rt_info_[i].entry_point) == 0)
        {
          return i;
        }
    }
  // TODO: throw an exception or print an error.
  return -1;
}

RtecScheduler::handle_t
ACE_Runtime_Scheduler::lookup (const char * entry_point,
                               CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException))
{
  return create (entry_point, _env);
}

RtecScheduler::RT_Info*
ACE_Runtime_Scheduler::get (RtecScheduler::handle_t handle,
                            CORBA::Environment &_env)
     TAO_THROW_SPEC((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK))
{
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW_RETURN (RtecScheduler::UNKNOWN_TASK(), 0);
    }

  // Note: there is no memory leak here, according to the CORBA spec,
  // we are supposed to allocate an structure and return it, the
  // caller owns it from then on.

  RtecScheduler::RT_Info* info = new RtecScheduler::RT_Info;

  info->entry_point = rt_info_[handle].entry_point;
  info->handle = rt_info_[handle].handle;
  info->worst_case_execution_time = rt_info_[handle].worst_case_execution_time;
  info->typical_execution_time = rt_info_[handle].typical_execution_time;
  info->cached_execution_time = rt_info_[handle].cached_execution_time;
  info->period = rt_info_[handle].period;
  info->criticality = RtecScheduler::Criticality(rt_info_[handle].criticality);
  info->importance = RtecScheduler::Importance(rt_info_[handle].importance);
  info->quantum = rt_info_[handle].quantum;
  info->threads = rt_info_[handle].threads;
  info->priority = rt_info_[handle].priority;
  info->preemption_subpriority = rt_info_[handle].static_subpriority;
  info->preemption_priority = rt_info_[handle].preemption_priority;
  info->info_type = RtecScheduler::Info_Type(rt_info_[handle].info_type);

  return info;
}

void ACE_Runtime_Scheduler::set (RtecScheduler::handle_t handle,
                                 RtecScheduler::Criticality criticality,
				 RtecScheduler::Time time,
				 RtecScheduler::Time typical_time,
				 RtecScheduler::Time cached_time,
                                 RtecScheduler::Period period,
                                 RtecScheduler::Importance importance,
                                 RtecScheduler::Quantum quantum,
                                 CORBA::Long threads,
                                 RtecScheduler::Info_Type info_type,
                                 CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  // We compare the values with the ones stored and print a message on
  // any differences.
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW (RtecScheduler::UNKNOWN_TASK());
      // NOTREACHED
    }
  if (rt_info_[handle].worst_case_execution_time != time
      || rt_info_[handle].typical_execution_time != typical_time
      || rt_info_[handle].cached_execution_time != cached_time
      || rt_info_[handle].period != period
      || rt_info_[handle].criticality != criticality
      || rt_info_[handle].importance != importance
      || rt_info_[handle].quantum != quantum
      || rt_info_[handle].info_type != info_type
      || rt_info_[handle].threads != threads)
    {
      ACE_ERROR ((LM_ERROR, "invalid data for RT_Info: %s\n",
                  (const char*)rt_info_[handle].entry_point));
      // TODO: throw something here.
    }
}


void ACE_Runtime_Scheduler::priority (RtecScheduler::handle_t handle,
                                      RtecScheduler::OS_Priority& priority,
                                      RtecScheduler::Preemption_Subpriority& subpriority,
                                      RtecScheduler::Preemption_Priority& p_priority,
                                      CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW (RtecScheduler::UNKNOWN_TASK());
      // NOTREACHED
    }
  priority = rt_info_[handle].priority;
  subpriority = rt_info_[handle].static_subpriority;
  p_priority = rt_info_[handle].preemption_priority;
  // ACE_DEBUG ((LM_DEBUG, "(%t) Returning priority %d\n", priority));
}

void ACE_Runtime_Scheduler::entry_point_priority (const char * entry_point,
                                                  RtecScheduler::OS_Priority& priority,
                                                  RtecScheduler::Preemption_Subpriority& subpriority,
                                                  RtecScheduler::Preemption_Priority& p_priority,
                                                  CORBA::Environment &_env)
     TAO_THROW_SPEC((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED))
{
  RtecScheduler::handle_t handle = lookup (entry_point, _env);
  if (handle < -1)
    {
      // The exception was thrown or is in _env already.
      return;
    }
  this->priority (handle, priority, subpriority, p_priority, _env);
}

void ACE_Runtime_Scheduler::add_dependency (RtecScheduler::handle_t handle,
                                            RtecScheduler::handle_t dependency,
                                            CORBA::Long number_of_calls,
                                            RtecScheduler::Dependency_Type
                                              dependency_type,
                                            CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW (RtecScheduler::UNKNOWN_TASK());
      // NOTREACHED
    }
#if 0
  // Just check that the information is consistent.
  RtecScheduler::Dependency_Set& deps = rt_info_[handle]->dependencies;
  for (CORBA::ULong i = 0; i < deps.length (); ++i)
    {
      if (deps[i].rt_info == dependency
          && deps[i].number_of_calls == number_of_calls
          && deps[i].dependency_type == dependency_type)
        {
          return;
        }
    }
  ACE_ERROR ((LM_ERROR, "unmatched dependency on %s\n",
              (const char*)rt_info_[handle]->entry_point));
#endif
}

void ACE_Runtime_Scheduler::compute_scheduling (CORBA::Long minimum_priority,
                                                CORBA::Long maximum_priority,
                                                RtecScheduler::RT_Info_Set_out infos,
												RtecScheduler::Config_Info_Set_out configs,
                                                CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                      RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                      RtecScheduler::TASK_COUNT_MISMATCH))
{
  // TODO: Right now just do nothing, later we could validate the
  // priorities (without recomputing).
  // TODO: fill up the infos.
  return;
}


void ACE_Runtime_Scheduler::dispatch_configuration(RtecScheduler::Preemption_Priority p_priority,
												   RtecScheduler::OS_Priority& priority,
		   RtecScheduler::Dispatching_Type & d_type,
												   CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::NOT_SCHEDULED,
                      RtecScheduler::UNKNOWN_PRIORITY_LEVEL))
{
  // throw an exception if a valid schedule has not been loaded
  if (config_count_ <= 0 || 
	  config_info_ [p_priority].preemption_priority != p_priority)
  {
      TAO_THROW (RtecScheduler::NOT_SCHEDULED());
      ACE_NOTREACHED (return);
  }
  // throw an exception if an invalid priority was passed
  else if (p_priority < 0 || p_priority >= config_count_)
  {
      TAO_THROW (RtecScheduler::UNKNOWN_PRIORITY_LEVEL());
      ACE_NOTREACHED (return);
  }
  else
  {
    priority = config_info_ [p_priority].thread_priority;
    d_type = config_info_ [p_priority].dispatching_type;
    return;
  }
}
  // provide the thread priority and queue type for the given priority level

RtecScheduler::Preemption_Priority 
ACE_Runtime_Scheduler::last_scheduled_priority (CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED))
{
  // throw an exception if a valid schedule has not been loaded
  if (config_count_ <= 0)
  {
    TAO_THROW_RETURN (RtecScheduler::NOT_SCHEDULED(), 
		              (RtecScheduler::Preemption_Priority) -1);
  }
  else
  {
    return (RtecScheduler::Preemption_Priority) (config_count_ - 1);
  }
}
  // Returns the last priority number assigned to an operation in the schedule.
  // The number returned is one less than the total number of scheduled priorities.
  // All scheduled priorities range from 0 to the number returned, inclusive.









