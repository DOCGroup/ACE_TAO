// $Id$

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Reconfig_Scheduler.h"

#if defined (__ACE_INLINE__)
#include "orbsvcs/Reconfig_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Reconfig_Scheduler, "$Id$")

// Constructor. Initialize the scheduler from the POD_Config_Info, POD_RT_Info, 
// and POD_Dependency arrays, plus stability flag.

ACE_Reconfig_Scheduler::
ACE_Reconfig_Scheduler (int config_count,
                       ACE_Scheduler_Factory::POD_Config_Info config_info[],
                       int entry_count,
                       ACE_Scheduler_Factory::POD_RT_Info rt_info[])
  : config_count_ (config_count),
    config_info_ (config_info),
    entry_count_ (entry_count),
    rt_info_ (rt_info),
    dependency_count_ (dependency_count),
    dependency_info_ (dependency_info),
    is_stable_ (is_stable)
{
CDG - TBD - after all the other methods are done, make sure 
the infrastructure they need is appropriately initialized here.
}

// Create an RT_Info.  If the RT_Info already exists, is handle is simply
// returned.  If it does not exist, a new RT_Info is created and inserted
// into the schedule, and the handle of the new RT_Info is returned.

RtecScheduler::handle_t
ACE_Reconfig_Scheduler::create (const char *entry_point,
                               CORBA::Environment &)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::DUPLICATE_NAME))
{
CDG - TBD rework this to be very efficient.

  // Just make sure it's there and return its handle.
  for (int i = 0; i < entry_count_; ++i)
      if (ACE_OS::strcmp (entry_point,
                          rt_info_[i].entry_point) == 0)
        return i + 1;

  // TODO: throw an exception or print an error.
  return -1;
}

// Lookup a handle for an RT_Info, and return its handle, or an error
// value if it's not present.

RtecScheduler::handle_t
ACE_Reconfig_Scheduler::lookup (const char * entry_point,
                               CORBA::Environment &TAO_IN_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException))
{
  return create (entry_point, TAO_IN_ENV);
}


// Return a pointer to the RT_Info corresponding to the passed handle.

RtecScheduler::RT_Info *
ACE_Reconfig_Scheduler::get (RtecScheduler::handle_t handle,
                            CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK))
{
CDG - TBD - redo this

  if (handle <= 0 || handle > entry_count_)
    ACE_THROW_RETURN (RtecScheduler::UNKNOWN_TASK (),
                      0);
  // Note: there is no memory leak here, according to the CORBA spec,
  // we are supposed to allocate an structure and return it, the
  // caller owns it from then on.

  // Allocate a new RT_Info
  RtecScheduler::RT_Info* info;
  ACE_NEW_THROW_EX (info,
                    RtecScheduler::RT_Info,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);


  info->entry_point = rt_info_[handle - 1].entry_point;
  info->handle = rt_info_[handle - 1].handle;
  info->worst_case_execution_time = rt_info_[handle - 1].worst_case_execution_time;
  info->typical_execution_time = rt_info_[handle - 1].typical_execution_time;
  info->cached_execution_time = rt_info_[handle - 1].cached_execution_time;
  info->period = rt_info_[handle - 1].period;
  info->criticality = RtecScheduler::Criticality_t(rt_info_[handle - 1].criticality);
  info->importance = RtecScheduler::Importance_t(rt_info_[handle - 1].importance);
  info->quantum = rt_info_[handle - 1].quantum;
  info->threads = rt_info_[handle - 1].threads;
  info->priority = rt_info_[handle - 1].priority;
  info->preemption_subpriority = rt_info_[handle - 1].static_subpriority;
  info->preemption_priority = rt_info_[handle - 1].preemption_priority;
  info->info_type = RtecScheduler::Info_Type_t(rt_info_[handle - 1].info_type);

  return info;
}


// Set characteristics of the RT_Info corresponding to the passed handle.

void
ACE_Reconfig_Scheduler::set (RtecScheduler::handle_t handle,
                                 RtecScheduler::Criticality_t criticality,
                                 RtecScheduler::Time time,
                                 RtecScheduler::Time typical_time,
                                 RtecScheduler::Time cached_time,
                                 RtecScheduler::Period_t period,
                                 RtecScheduler::Importance_t importance,
                                 RtecScheduler::Quantum_t quantum,
                                 CORBA::Long threads,
                                 RtecScheduler::Info_Type_t info_type,
                                 CORBA::Environment &TAO_IN_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
CDG - TBD - redo this - should actually update, and sset flags accordingly

  // We compare the values with the ones stored and print a message on
  // any differences.
  if (handle <= 0 || handle > entry_count_)
    {
      ACE_DEBUG ((LM_DEBUG, "Unknown task: no entry for handle %d\n",
                  handle));
      TAO_THROW (RtecScheduler::UNKNOWN_TASK());
      // NOTREACHED
    }
  if (rt_info_[handle - 1].worst_case_execution_time != time
      || rt_info_[handle - 1].typical_execution_time != typical_time
      || rt_info_[handle - 1].cached_execution_time != cached_time
      || rt_info_[handle - 1].period != period
      || rt_info_[handle - 1].criticality != criticality
      || rt_info_[handle - 1].importance != importance
      || rt_info_[handle - 1].quantum != quantum
      || rt_info_[handle - 1].info_type != info_type
      || rt_info_[handle - 1].threads != threads)
    ACE_ERROR ((LM_ERROR,
                "invalid data for RT_Info: %s\n",
                (const char*)rt_info_[handle - 1].entry_point));
  // TODO: throw something here.
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its handle.

void
ACE_Reconfig_Scheduler::priority (RtecScheduler::handle_t handle,
                                 RtecScheduler::OS_Priority& o_priority,
                                 RtecScheduler::Preemption_Subpriority_t& subpriority,
                                 RtecScheduler::Preemption_Priority_t& p_priority,
                                 CORBA::Environment &TAO_IN_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
CDG - TBD - redo this

  if (handle <= 0 || handle > entry_count_)
    TAO_THROW (RtecScheduler::UNKNOWN_TASK ());
    // NOTREACHED

  o_priority = rt_info_[handle - 1].priority;
  subpriority = rt_info_[handle - 1].static_subpriority;
  p_priority = rt_info_[handle - 1].preemption_priority;
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its entry point name.

void
ACE_Reconfig_Scheduler::entry_point_priority (const char * entry_point,
                                             RtecScheduler::OS_Priority& priority,
                                             RtecScheduler::Preemption_Subpriority_t& subpriority,
                                             RtecScheduler::Preemption_Priority_t& p_priority,
                                             CORBA::Environment &TAO_IN_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
CDG - TBD - redo this

  RtecScheduler::handle_t handle = lookup (entry_point, TAO_IN_ENV);
  if (handle < -1)
    // The exception was thrown or is in TAO_IN_ENV already.
    return;
  this->priority (handle,
                  priority,
                  subpriority,
                  p_priority,
                  TAO_IN_ENV);
}


// This method registers a dependency between two RT_Infos.

void
ACE_Reconfig_Scheduler::add_dependency (RtecScheduler::handle_t handle,
                                       RtecScheduler::handle_t /* dependency */,
                                       CORBA::Long /* number_of_calls */,
                                       RtecScheduler::Dependency_Type_t /* dependency_type */,
                                       CORBA::Environment &TAO_IN_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  if (handle <= 0 || handle > entry_count_)
    TAO_THROW (RtecScheduler::UNKNOWN_TASK ());
    // NOTREACHED

CDG - TBD - redo this

#if 0
  // Just check that the information is consistent.
  RtecScheduler::Dependency_Set& deps = rt_info_[handle - 1]->dependencies;
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
              (const char*)rt_info_[handle - 1]->entry_point));
#endif
}


// If information has been added or changed since the last stable
// schedule was computed, this method causes scheduling information
// to be computed for all registered RT_Infos.  If the schedule is
// already stable, this is a no-op

void
ACE_Reconfig_Scheduler::compute_scheduling (CORBA::Long /* minimum_priority */,
                                           CORBA::Long /* maximum_priority */,
                                           RtecScheduler::RT_Info_Set_out /* infos */,
                                           RtecScheduler::Config_Info_Set_out /* configs */,
                                           RtecScheduler::Scheduling_Anomaly_Set_out /* anomalies */,
                                           CORBA::Environment &)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                      RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                      RtecScheduler::TASK_COUNT_MISMATCH))
{
CDG - TBD - redo this

  // TODO: Right now just do nothing, later we could validate the
  // priorities (without recomputing).
  // TODO: fill up the infos.
  return;
}


// Provides the thread priority and queue type for the given priority
// level.

void
ACE_Reconfig_Scheduler::dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                               RtecScheduler::OS_Priority& priority,
                                               RtecScheduler::Dispatching_Type_t & d_type,
                                               CORBA::Environment &TAO_IN_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::NOT_SCHEDULED,
                      RtecScheduler::UNKNOWN_PRIORITY_LEVEL))
{
  // Throw an exception if a valid schedule has not been loaded
  if (config_count_ <= 0
      || config_info_ [p_priority].preemption_priority != p_priority)
    {
      TAO_THROW (RtecScheduler::NOT_SCHEDULED ());
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


// Returns the last priority number assigned to an operation in the
// schedule.  The number returned is one less than the total number
// of scheduled priorities.  All scheduled priorities range from 0
// to the number returned, inclusive.

RtecScheduler::Preemption_Priority_t
ACE_Reconfig_Scheduler::last_scheduled_priority (CORBA::Environment &TAO_IN_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED))
{
  // throw an exception if a valid schedule has not been loaded
  if (config_count_ <= 0)
    TAO_THROW_RETURN (RtecScheduler::NOT_SCHEDULED(),
                      (RtecScheduler::Preemption_Priority_t) -1);
  else
    return (RtecScheduler::Preemption_Priority_t) (config_count_ - 1);
}
