// $Id$

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Runtime_Scheduler.h"

#if defined (__ACE_INLINE__)
#include "orbsvcs/Runtime_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Runtime_Scheduler, "$Id$")

  // Constructor. Initialize the data from the POD_RT_Info array.

ACE_Runtime_Scheduler::
ACE_Runtime_Scheduler (int config_count,
                       ACE_Scheduler_Factory::POD_Config_Info config_info[],
                       int entry_count,
                       ACE_Scheduler_Factory::POD_RT_Info rt_info[])
  : config_count_ (config_count),
    config_info_ (config_info),
    entry_count_ (entry_count),
    rt_info_ (rt_info)
{
}

// Create an RT_Info.  In the config run scheduler this actually constructs
// a new RT_Info.  Here, we just return its handle, or an error value if
// it's not present.

RtecScheduler::handle_t
ACE_Runtime_Scheduler::create (const char *entry_point
                               ACE_ENV_ARG_DECL_NOT_USED)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::DUPLICATE_NAME))
{
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
ACE_Runtime_Scheduler::lookup (const char * entry_point
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return create (entry_point ACE_ENV_ARG_PARAMETER);
}


// Return a pointer to the RT_Info corresponding to the passed handle.

RtecScheduler::RT_Info *
ACE_Runtime_Scheduler::get (RtecScheduler::handle_t handle
                            ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK))
{
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
ACE_Runtime_Scheduler::set (RtecScheduler::handle_t handle,
                                 RtecScheduler::Criticality_t criticality,
                                 RtecScheduler::Time time,
                                 RtecScheduler::Time typical_time,
                                 RtecScheduler::Time cached_time,
                                 RtecScheduler::Period_t period,
                                 RtecScheduler::Importance_t importance,
                                 RtecScheduler::Quantum_t quantum,
                                 CORBA::Long threads,
                                 RtecScheduler::Info_Type_t info_type
                                 ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  // We compare the values with the ones stored and print a message on
  // any differences.
  if (handle <= 0 || handle > entry_count_)
    {
      ACE_DEBUG ((LM_DEBUG, "Unknown task: no entry for handle %d\n",
                  handle));
      ACE_THROW (RtecScheduler::UNKNOWN_TASK());
      // NOTREACHED
    }
  if (rt_info_[handle - 1].worst_case_execution_time != time
      || rt_info_[handle - 1].typical_execution_time != typical_time
      || rt_info_[handle - 1].cached_execution_time != cached_time
      || rt_info_[handle - 1].period != period
      || rt_info_[handle - 1].criticality != criticality
      || rt_info_[handle - 1].importance != ACE_static_cast (CORBA::Long, importance)
      || rt_info_[handle - 1].quantum != quantum
      || rt_info_[handle - 1].info_type != info_type
      || rt_info_[handle - 1].threads != ACE_static_cast (CORBA::Long, threads))
    ACE_ERROR ((LM_ERROR,
                "invalid data for RT_Info: %s\n",
                (const char*)rt_info_[handle - 1].entry_point));
  // TODO: throw something here.
}


void
ACE_Runtime_Scheduler::reset (RtecScheduler::handle_t handle,
                              RtecScheduler::Criticality_t criticality,
                              RtecScheduler::Time time,
                              RtecScheduler::Time typical_time,
                              RtecScheduler::Time cached_time,
                              RtecScheduler::Period_t period,
                              RtecScheduler::Importance_t importance,
                              RtecScheduler::Quantum_t quantum,
                              CORBA::Long threads,
                              RtecScheduler::Info_Type_t info_type
                              ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  // Just go ahead and call the set method
  this->set (handle, criticality, time, typical_time, cached_time,
             period, importance, quantum, threads, info_type ACE_ENV_ARG_PARAMETER );
}


void
ACE_Runtime_Scheduler::set_seq (const RtecScheduler::RT_Info_Set& infos
                                ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  for (u_int i = 0; i < infos.length (); ++i)
    {
      // Call the internal set method.
      this->set (infos[i].handle,
                 infos[i].criticality,
                 infos[i].worst_case_execution_time,
                 infos[i].typical_execution_time,
                 infos[i].cached_execution_time,
                 infos[i].period,
                 infos[i].importance,
                 infos[i].quantum,
                 infos[i].threads,
                 infos[i].info_type
                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
ACE_Runtime_Scheduler::replace_seq (const RtecScheduler::RT_Info_Set& infos
                                    ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  for (u_int i = 0; i < infos.length (); ++i)
    {
      // Call the internal set method.
      this->set (infos[i].handle,
                 infos[i].criticality,
                 infos[i].worst_case_execution_time,
                 infos[i].typical_execution_time,
                 infos[i].cached_execution_time,
                 infos[i].period,
                 infos[i].importance,
                 infos[i].quantum,
                 infos[i].threads,
                 infos[i].info_type
                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}


void
ACE_Runtime_Scheduler::reset_seq (const RtecScheduler::RT_Info_Set& infos
                                  ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  // Just call the set sequence method
  this->set_seq (infos ACE_ENV_ARG_PARAMETER);
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its handle.

void
ACE_Runtime_Scheduler::priority (RtecScheduler::handle_t handle,
                                 RtecScheduler::OS_Priority& o_priority,
                                 RtecScheduler::Preemption_Subpriority_t& subpriority,
                                 RtecScheduler::Preemption_Priority_t& p_priority
                                 ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
  if (handle <= 0 || handle > entry_count_)
    ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    // NOTREACHED

  o_priority = rt_info_[handle - 1].priority;
  subpriority = rt_info_[handle - 1].static_subpriority;
  p_priority = rt_info_[handle - 1].preemption_priority;
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its entry point name.

void
ACE_Runtime_Scheduler::entry_point_priority (const char * entry_point,
                                             RtecScheduler::OS_Priority& priority,
                                             RtecScheduler::Preemption_Subpriority_t& subpriority,
                                             RtecScheduler::Preemption_Priority_t& p_priority
                                             ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
  RtecScheduler::handle_t handle = lookup (entry_point ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (handle < -1)
    // The exception was thrown or is in ACE_ENV_SINGLE_ARG_PARAMETER already.
    return;
  this->priority (handle,
                  priority,
                  subpriority,
                  p_priority
                  ACE_ENV_ARG_PARAMETER);
}


// In the config run scheduler, this method registers a dependency between
// two RT_Infos.  In the run time scheduler, this is a no-op.

void
ACE_Runtime_Scheduler::add_dependency (RtecScheduler::handle_t handle,
                                       RtecScheduler::handle_t /* dependency */,
                                       CORBA::Long /* number_of_calls */,
                                       RtecScheduler::Dependency_Type_t /* dependency_type */
                                       ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  if (handle <= 0 || handle > entry_count_)
    ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    // NOTREACHED

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


// In the reconfig scheduler, this method removes a dependency between
// two RT_Infos.  In the run time scheduler, this is a no-op.

void
ACE_Runtime_Scheduler::remove_dependency (RtecScheduler::handle_t handle,
                                          RtecScheduler::handle_t /* dependency */,
                                          CORBA::Long /* number_of_calls */,
                                          RtecScheduler::Dependency_Type_t /* dependency_type */
                                          ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  if (handle <= 0 || handle > entry_count_)
    ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
}


// In the reconfig scheduler, this method enables a dependency between
// two RT_Infos.  In the run time scheduler, this is a no-op.

void
ACE_Runtime_Scheduler::set_dependency_enable_state (RtecScheduler::handle_t handle,
                                                    RtecScheduler::handle_t /* dependency */,
                                                    CORBA::Long /* number_of_calls */,
                                                    RtecScheduler::Dependency_Type_t /* dependency_type */,
                                                    RtecScheduler::Dependency_Enabled_Type_t /* enabled */
                                                    ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  if (handle <= 0 || handle > entry_count_)
    ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
}


// In the reconfig scheduler, this method enables or disables an RT_Info.
// In the run time scheduler, this is a no-op.

void
ACE_Runtime_Scheduler::set_rt_info_enable_state (RtecScheduler::handle_t handle,
                                                 RtecScheduler::RT_Info_Enabled_Type_t /* enabled */
                                                 ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  if (handle <= 0 || handle > entry_count_)
    ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
}


virtual void set_dependency_enable_state_seq (const RtecScheduler::Dependency_Set & dependencies
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RtecScheduler::SYNCHRONIZATION_FAILURE,
                   RtecScheduler::UNKNOWN_TASK))
  // This method sets the enable state of a sequence of dependencies.
{
}

virtual void set_rt_info_enable_state_seq (const RtecScheduler::RT_Info_Enable_State_Pair_Set & pair_set
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RtecScheduler::SYNCHRONIZATION_FAILURE,
                   RtecScheduler::UNKNOWN_TASK));
// This method enables or disables a sequence of RT_Infos.
{
}

// In the config run scheduler, this method causes scheduling information
// to be computed for all registered RT_Infos.  In the run time scheduler,
// this is a no-op.

void
ACE_Runtime_Scheduler::compute_scheduling (CORBA::Long /* minimum_priority */,
                                           CORBA::Long /* maximum_priority */,
                                           RtecScheduler::RT_Info_Set_out /* infos */,
                                           RtecScheduler::Dependency_Set_out /*dependencies */,
                                           RtecScheduler::Config_Info_Set_out /* configs */,
                                           RtecScheduler::Scheduling_Anomaly_Set_out /* anomalies */
                                           ACE_ENV_ARG_DECL_NOT_USED)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                      RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                      RtecScheduler::TASK_COUNT_MISMATCH))
{
  // TODO: Right now just do nothing, later we could validate the
  // priorities (without recomputing).
  // TODO: fill up the infos.
  return;
}

// Recomputes the scheduling priorities, etc.

void
ACE_Runtime_Scheduler::recompute_scheduling (CORBA::Long minimum_priority,
                                             CORBA::Long maximum_priority,
                                             RtecScheduler::Scheduling_Anomaly_Set_out anomalies
                                             ACE_ENV_ARG_DECL)

    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                     RtecScheduler::TASK_COUNT_MISMATCH,
                     RtecScheduler::INTERNAL,
                     RtecScheduler::DUPLICATE_NAME))
{
  // TODO: Right now just do nothing.
  ACE_UNUSED_ARG(minimum_priority);
  ACE_UNUSED_ARG(maximum_priority);
  ACE_UNUSED_ARG(anomalies);

  return;
}


// Returns the set of rt_infos, with their assigned priorities (as
// of the last schedule re-computation).

void
ACE_Runtime_Scheduler::get_rt_info_set (RtecScheduler::RT_Info_Set_out infos
                                        ACE_ENV_ARG_DECL)

    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INTERNAL))
{
  // TODO: Right now just do nothing.
  ACE_UNUSED_ARG(infos);
  return;
}


// Returns the set of rt_infos, with their assigned priorities (as
// of the last schedule re-computation).

void
ACE_Runtime_Scheduler::get_dependency_set (RtecScheduler::Dependency_Set_out dependencies
                                           ACE_ENV_ARG_DECL)

    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INTERNAL))
{
  // TODO: Right now just do nothing.
  ACE_UNUSED_ARG(dependencies);
  return;
}


// Returns the set of config_infos, describing the appropriate
// number, types, and priority levels for the dispatching lanes.

void
ACE_Runtime_Scheduler::get_config_info_set (RtecScheduler::Config_Info_Set_out configs
                                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INTERNAL))
{
  // TODO: Right now just do nothing.
  ACE_UNUSED_ARG(configs);
  return;
}


// Provides the thread priority and queue type for the given priority
// level.

void
ACE_Runtime_Scheduler::dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                               RtecScheduler::OS_Priority& priority,
                                               RtecScheduler::Dispatching_Type_t & d_type
                                               ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::NOT_SCHEDULED,
                      RtecScheduler::UNKNOWN_PRIORITY_LEVEL))
{
  // Throw an exception if a valid schedule has not been loaded
  if (config_count_ <= 0
      || config_info_ [p_priority].preemption_priority != p_priority)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ());
      ACE_NOTREACHED (return);
    }
  // throw an exception if an invalid priority was passed
  else if (p_priority < 0 || p_priority >= config_count_)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_PRIORITY_LEVEL());
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
ACE_Runtime_Scheduler::last_scheduled_priority (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED))
{
  // throw an exception if a valid schedule has not been loaded
  if (config_count_ <= 0)
    ACE_THROW_RETURN (RtecScheduler::NOT_SCHEDULED(),
              (RtecScheduler::Preemption_Priority_t) -1);
  else
    return (RtecScheduler::Preemption_Priority_t) (config_count_ - 1);
}


void
ACE_Runtime_Scheduler::get_config_infos (RtecScheduler::Config_Info_Set_out /*configs */
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                  RtecScheduler::NOT_SCHEDULED))
{
  // throw an exception if a valid schedule has not been loaded
  if (this->config_count_ <= 0)
    ACE_THROW (RtecScheduler::NOT_SCHEDULED());
  //TODO: fill the Config_Info_Set with the runtime Config_Infos
  //for now, this function is unimplemented
  // 
  // @@ If unimplemented we either return a CORBA::NO_IMPL exception 
  // or a proprietary ACE_NOTSUP; -- Bala
  return;
}
