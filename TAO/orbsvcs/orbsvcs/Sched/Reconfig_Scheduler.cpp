// $Id$

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Reconfig_Scheduler.h"

#if defined (__ACE_INLINE__)
#include "orbsvcs/Reconfig_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Reconfig_Scheduler, "$Id$")

// Default constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_Reconfig_Scheduler::ACE_Reconfig_Scheduler ()
  : next_handle_ (0),
    stability_flags_ (SCHED_NONE_STABLE),
    dependency_count_ (0),
    last_scheduled_priority_ (0)
{
}

// Constructor. Initialize the scheduler from the POD_Config_Info, POD_RT_Info, 
// and POD_Dependency arrays, plus stability flag.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_Reconfig_Scheduler::
ACE_Reconfig_Scheduler (int config_count,
                        ACE_Scheduler_Factory::POD_Config_Info config_infos[],
                        int rt_info_count,
                        ACE_Scheduler_Factory::POD_RT_Info rt_infos[],
                        int dependency_count,
                        ACE_Scheduler_Factory::POD_Dependency_Info dependency_infos[],
                        u_long stability_flags)
  : next_handle_ (0),
    stability_flags_ (SCHED_ALL_STABLE),
    dependency_count_ (0),
    last_scheduled_priority_ (0)
{
  // Declare a CORBA::Environment variable in the current scope.
  ACE_DECLARE_NEW_CORBA_ENV;

  // The init method can throw an exception, which must be caught
  // *inside* the constructor to be portable between compilers that
  // differ in whether they support native C++ exceptions.
  ACE_TRY
    {
      this->init (config_count, config_infos,
                  rt_info_count, rt_infos,
                  dependency_count, dependency_infos,
                  stability_flags);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, corba_sysex)
    {
       ACE_ERROR ((LM_ERROR, "ACE_Reconfig_Scheduler::ACE_Reconfig_Scheduler "
                             "system exception: cannot initialize scheduler.\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK;
}


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
ACE_Reconfig_Scheduler::init (int config_count,
                              ACE_Scheduler_Factory::POD_Config_Info config_info[],
                              int rt_info_count,
                              ACE_Scheduler_Factory::POD_RT_Info rt_info[],
                              int dependency_count,
                              ACE_Scheduler_Factory::POD_Dependency_Info dependency_info[],
                              int stability_flags,
                              CORBA::Environment &ACE_TRY_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::DUPLICATE_NAME, RtecScheduler::UNKNOWN_TASK))
{
  // CDG - TBD - replace all these uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK_RETURN (-1);

  int result = 0;

  // Clear out the previous entries, if any.
  this->close (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // (Re)initialize using the new settings.

  // Add the passed config infos to the scheduler
  for (int config_info_count = 0; config_info_count < config_count; ++config_info_count)
    {
      RtecScheduler::Config_Info* new_config_info;
      ACE_NEW_THROW_EX (new_config_info,
                        RtecScheduler::Config_Info,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (ST_VIRTUAL_MEMORY_EXHAUSTED);

      result = config_info_map_.bind (config_info [config_info_count].preemption_priority,
                                      new_config_info);
      switch (result)
        {
          case -1:
            // Something bad but unknown occurred while trying to bind in map.
            delete new_config_info;
            ACE_THROW_RETURN (CORBA::UNKNOWN (), -1);

          case 1:
            // Tried to bind an operation that was already in the map.
            delete new_config_info;
            ACE_THROW_RETURN (RtecScheduler::DUPLICATE_NAME (), -1);

          default:
            break;
        }

      new_config_info->preemption_priority = 
        config_info [config_info_count].preemption_priority;
      new_config_info->thread_priority =
        config_info [config_info_count].thread_priority;
      new_config_info->dispatching_type = 
        config_info [config_info_count].dispatching_type;

      if (new_config_info->preemption_priority >
          last_scheduled_priority_)
        {
          last_scheduled_priority_ = 
            new_config_info->preemption_priority;
        }
    }

  // Add RT_Infos to scheduler
  RtecScheduler::RT_Info* new_rt_info;
  for (int num_rt_infos = 0; num_rt_infos < rt_info_count; ++num_rt_infos)
    {
      new_rt_info = create_i (rt_info [num_rt_infos].entry_point,
                              rt_info [num_rt_infos].handle,
                              ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      
      // Fill in the portions to which the user has access.
      set_i (new_rt_info,
             RtecScheduler::Criticality_t (rt_info [num_rt_infos].criticality),
             rt_info [num_rt_infos].worst_case_execution_time,
             rt_info [num_rt_infos].typical_execution_time,
             rt_info [num_rt_infos].cached_execution_time,
             rt_info [num_rt_infos].period,
             RtecScheduler::Importance_t (rt_info [num_rt_infos].importance),
             rt_info [num_rt_infos].quantum,
             rt_info [num_rt_infos].threads,
             RtecScheduler::Info_Type_t (rt_info [num_rt_infos].info_type));

      // Fill in the scheduler managed portions.
      new_rt_info->priority = 
        rt_info [num_rt_infos].priority;
      new_rt_info->preemption_subpriority = 
        rt_info [num_rt_infos].preemption_subpriority;
      new_rt_info->preemption_priority = 
        rt_info [num_rt_infos].preemption_priority;
      new_rt_info->volatile_token = 0;

    // Add dependencies between RT_Infos to scheduler.
    for (dependency_count_ = 0; dependency_count_ < dependency_count; ++dependency_count_)
      {
        add_dependency_i (dependency_info [dependency_count_].info_that_depends,
                          dependency_info [dependency_count_].info_depended_on,
                          dependency_info [dependency_count_].dependency_type,
                          dependency_info [dependency_count_].number_of_calls,
                          ACE_TRY_ENV);
        ACE_CHECK_RETURN (-1);
      }

    // Set stability flags after the operations are loaded, as the passed flags
    // should be respected as being the stability state of the passed schedule.
    stability_flags_ = stability_flags;

  return result;
}

// Closes the scheduler, releasing all current resources.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
ACE_Reconfig_Scheduler::close (void)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::UNKNOWN_TASK))
{
  // CDG - TBD - replace all these uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK;

  // Unbind and delete each RT_Info in the map.
  RtecScheduler::RT_Info *rt_info;
  RtecScheduler::handle_t handle;
  while (rt_info_map_.current_size () > 0)
    {
      handle = (*rt_info_map_.begin ()).ext_id_;
      if (rt_info_map_.unbind (handle, rt_info) == 0)
        {
          if (rt_info_tree_.unbind (rt_info->entry_point) == 0)
            {
              delete rt_info;
            }
          else
            {
              ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
            }
         }
      else
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }
    }

  // Delete each Config_Info in the map.
  RtecScheduler::Preemption_Priority_t config_priority;
  RtecScheduler::Config_Info *config_info;
  while (rt_info_map_.current_size () > 0)
    {
      config_priority = (*config_info_map_.begin ()).ext_id_;
      if (config_info_map_.unbind (config_priority, config_info) == 0)
        {
          delete config_info;
        }
      else
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }
    }

  // Finally, start over with the lowest handle number.
  next_handle = 0;    
}

// Create an RT_Info.  If it does not exist, a new RT_Info is
// created and inserted into the schedule, and the handle of the new
// RT_Info is returned.  If the RT_Info already exists, an exception
// is thrown.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::handle_t
ACE_Reconfig_Scheduler::create (const char *entry_point,
                               CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::DUPLICATE_NAME))
{
  // CDG - TBD - replace all such uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK_RETURN (0);

  RtecScheduler::handle_t handle = next_handle_;
  create_i (entry_point, handle, ACE_TRY_ENV);
  ACE_CHECK_RETURN (handle);

  return handle;
}

// Lookup a handle for an RT_Info, and return its handle, or an error
// value if it's not present.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::handle_t
ACE_Reconfig_Scheduler::lookup (const char * entry_point,
                               CORBA::Environment &ACE_TRY_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::UNKNOWN_TASK))
{
  // CDG - TBD - replace all such uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK_RETURN (0);

  RtecScheduler::handle_t handle;
  handle = this->lookup_i (entry_point, ACE_TRY_ENV);
  ACE_CHECK_RETURN (handle);

  return handle;
}


// Return a pointer to the RT_Info corresponding to the passed handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::RT_Info *
ACE_Reconfig_Scheduler::get (RtecScheduler::handle_t handle,
                             CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK))
{
  // CDG - TBD - replace all such uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK_RETURN (0);

  // Find the RT_Info in the hash map.
  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_map_.find (handle, rt_info) != 0)
    {
      ACE_THROW_RETURN (RtecScheduler::UNKNOWN_TASK (), 0)
    }

  // Allocate a new RT_Info
  RtecScheduler::RT_Info* new_info;
  ACE_NEW_THROW_EX (new info,
                    RtecScheduler::RT_Info,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  *new_info = *rt_info;

  return new_info;
}


// Set characteristics of the RT_Info corresponding to the passed handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
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
                             CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  // CDG - TBD - replace all such uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK;

  // Look up the RT_Info by its handle, throw an exception if it's not there.
  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_map_.find (handle, rt_info) != 0)
    {
      ACE_THROW_RETURN (RtecScheduler::UNKNOWN_TASK (), 0)
    }

  // CDG - TBD - set stability flags - check existing values against new ones

  // Call the internal set method.
  this->set_i (rt_info, criticality, time, typical_time,
               cached_time, period, importance, quantum,
               threads, info_type);
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::priority (RtecScheduler::handle_t handle,
                                 RtecScheduler::OS_Priority& o_priority,
                                 RtecScheduler::Preemption_Subpriority_t& subpriority,
                                 RtecScheduler::Preemption_Priority_t& p_priority,
                                 CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
  // CDG - TBD - replace all such uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK;

  // Check stability flags.
  if (stability_flags & SCHED_PRIORITY_NOT_STABLE)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ())
    }
  
  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_tree_.find (entry_point, rt_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ())
    }

  o_priority = rt_info->priority;
  subpriority = rt_info->static_subpriority;
  p_priority = rt_info->preemption_priority;
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its entry point name.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::entry_point_priority (const char * entry_point,
                                             RtecScheduler::OS_Priority& priority,
                                             RtecScheduler::Preemption_Subpriority_t& subpriority,
                                             RtecScheduler::Preemption_Priority_t& p_priority,
                                             CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
  // CDG - TBD - replace all such uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK;

  RtecScheduler::handle_t handle = 
    this->lookup_i (entry_point, ACE_TRY_ENV);
  ACE_CHECK;

  this->priority_i (handle,
                  priority,
                  subpriority,
                  p_priority,
                  ACE_TRY_ENV);
  ACE_CHECK;
}


// This method registers a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::add_dependency (RtecScheduler::handle_t handle /* RT_Info that has the dependency */,
                                       RtecScheduler::handle_t dependency /* RT_Info on which it depends */,
                                       CORBA::Long number_of_calls,
                                       RtecScheduler::Dependency_Type_t dependency_type,
                                       CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  // CDG - TBD - replace all such uses of CORBA system exceptions 
  // with user exceptions (extend RtecScheduler.idl) 
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->lock_, CORBA::UNKNOWN ());
  ACE_CHECK;

  // Delegate to the internal method.
  add_dependency_i (handle, dependency, number_of_calls, 
                    dependency_type, ACE_TRY_ENV);
  ACE_CHECK;

  // Set stability flags and increment the dependency count.
  ++dependency_count_;
  stability_flags |= SCHED_UTILIZATION_NOT_STABLE |
                     SCHED_PRIORITY_NOT_STABLE |
                     SCHED_CONFIG_NOT_STABLE;
}

CDG - TBD - check TAO_THROW_SPECs

// Internal method to create an RT_Info.  If it does not exist, a new RT_Info is
// created and inserted into the schedule, and the handle of the new
// RT_Info is returned.  If the RT_Info already exists, an exception
// is thrown.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::RT_Info *
ACE_Reconfig_Scheduler::create_i (const char *entry_point,
                                  RtecScheduler::handle_t handle,
                                  CORBA::Environment &ACE_TRY_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::DUPLICATE_NAME));
{
  RtecScheduler::RT_Info* new_rt_info;
  int result = 0;

  // Create a new RT_Info.
  ACE_NEW_THROW_EX (new_rt_info,
                    RtecScheduler::RT_Info,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // Bind the new RT_Info to its handle, in the map.
  result = rt_info_map_.bind (handle, new_rt_info);
  switch (result)
    {
      case -1:
        // Something bad but unknown occurred while trying to bind in map.
        delete new_rt_info;
        ACE_THROW_RETURN (CORBA::UNKNOWN (), 0);

      case 1:
        // Tried to bind an operation that was already in the map.
        delete new_rt_info;
        ACE_THROW_RETURN (RtecScheduler::DUPLICATE_NAME (), 0);

      default:
        break;
    }

  // Bind the new RT_Info to its entry point, in the tree.
  result = rt_info_tree_.bind (entry_point, new_rt_info);
  switch (result)
    {
      case -1:
        // Something bad but unknown occurred while trying to bind in tree.
        rt_info_map_.unbind (handle);
        delete new_rt_info;
        ACE_THROW_RETURN (CORBA::UNKNOWN (), -1);

      case 1:
        // Tried to bind an operation that was already in the tree.
        rt_info_map_.unbind (handle);
        delete new_rt_info;
        ACE_THROW_RETURN (RtecScheduler::DUPLICATE_NAME (), -1);

      default:
        break;
        }
    }

  // With everything safely registered in the map and tree,
  // store the passed information, null out the other fields,
  // and update the next handle.
  new_rt_info->entry_point = entry_point;
  new_rt_info->handle = handle;

// CDG - TBD - give these reasonable default values
  rt_info->criticality = ;
  rt_info->worst_case_execution_time = ;
  rt_info->typical_execution_time = ;
  rt_info->cached_execution_time = ;
  rt_info->period = ;
  rt_info->importance = ;
  rt_info->quantum = ;
  rt_info->threads = 0;
  rt_info->info_type = ;
  rt_info->priority = 0;
  rt_info->preemption_subpriority = 0;
  rt_info->preemption_priority = 0;
  rt_info->volatile_token = 0;

  if (handle >= ACE_Reconfig_Scheduler::next_handle_)
    {
      ACE_Reconfig_Scheduler::next_handle_ = handle + 1;
    }

  return new_rt_info;
}


// Internal method to set characteristics of the passed RT_Info.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::set_i (RtecScheduler::RT_Info *rt_info,
                               RtecScheduler::Criticality_t criticality,
                               RtecScheduler::Time time,
                               RtecScheduler::Time typical_time,
                               RtecScheduler::Time cached_time,
                               RtecScheduler::Period_t period,
                               RtecScheduler::Importance_t importance,
                               RtecScheduler::Quantum_t quantum,
                               CORBA::Long threads,
                               RtecScheduler::Info_Type_t info_type)
{
  rt_info->handle = handle;
  rt_info->criticality = criticality;
  rt_info->worst_case_execution_time = time;
  rt_info->typical_execution_time = typical_time;
  rt_info->cached_execution_time = cached_time;
  rt_info->period = period;
  rt_info->importance = importance;
  rt_info->quantum = quantum;
  rt_info->threads = threads;
  rt_info->info_type = info_type;
}


// Internal method to lookup a handle for an RT_Info, and return its
// handle, or an error value if it's not present.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::handle_t
ACE_Reconfig_Scheduler::lookup_i (const char * entry_point,
                                  CORBA::Environment &ACE_TRY_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::UNKNOWN_TASK))
{
  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_tree_.find (entry_point, rt_info) != 0)
    {
      ACE_THROW_RETURN (RtecScheduler::UNKNOWN_TASK (), 0)
    }

  return rt_info->handle;
}

// Internal method that returns the priority and subpriority values
// assigned to an RT_Info, based on its handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::priority_i (RtecScheduler::handle_t handle,
                                    RtecScheduler::OS_Priority& o_priority,
                                    RtecScheduler::Preemption_Subpriority_t& subpriority,
                                    RtecScheduler::Preemption_Priority_t& p_priority,
                                    CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
  // Check stability flags.
  if (stability_flags & SCHED_PRIORITY_NOT_STABLE)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ())
    }
  
  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_tree_.find (entry_point, rt_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ())
    }

  o_priority = rt_info->priority;
  subpriority = rt_info->static_subpriority;
  p_priority = rt_info->preemption_priority;
}


// This method registers a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::add_dependency_i (RtecScheduler::handle_t handle /* RT_Info that has the dependency */,
                                          RtecScheduler::handle_t dependency /* RT_Info on which it depends */,
                                          CORBA::Long number_of_calls,
                                          RtecScheduler::Dependency_Type_t dependency_type,
                                          CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  // Look up the RT_Info in the map
  RtecScheduler::RT_Info *rt_info = 0;

  if (rt_info_map_.find (handle, rt_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

  // Insert unconditionally: there can be multiple copies
  // of the same dependency, if the user so chooses.
  RtecScheduler::Dependency_Set& deps = rt_info->dependencies;
  int prev_length =  deps.length ();
  deps.length (prev_length + 1);
  deps [prev_length].rt_info = dependency;
  deps [prev_length].number_of_calls = number_of_calls;
  deps [prev_length].dependency_type = dependency_type;
}


// If information has been added or changed since the last stable
// schedule was computed, this method causes scheduling information
// to be computed for all registered RT_Infos.  If the schedule is
// already stable, this is a no-op

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::compute_scheduling (CORBA::Long /* minimum_priority */,
                                           CORBA::Long /* maximum_priority */,
                                           RtecScheduler::RT_Info_Set_out /* infos */,
                                           RtecScheduler::Config_Info_Set_out /* configs */,
                                           RtecScheduler::Scheduling_Anomaly_Set_out /* anomalies */,
                                           CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                      RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                      RtecScheduler::TASK_COUNT_MISMATCH))
{
  // CDG - TBD - check stability flags (before)

  // CDG - TBD - add comparable behavior to the dynsched implementation, but be *very* efficient

  // CDG - TBD - set last_scheduled_priority_, etc.

  // CDG - TBD - set stability flags (afterward)

  return;
}


// Provides the thread priority and queue type for the given priority
// level.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
ACE_Reconfig_Scheduler::dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                               RtecScheduler::OS_Priority& t_priority,
                                               RtecScheduler::Dispatching_Type_t & d_type,
                                               CORBA::Environment &ACE_TRY_ENV)
     TAO_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::NOT_SCHEDULED,
                      RtecScheduler::UNKNOWN_PRIORITY_LEVEL))
{
  // Check stability flags
  if (stability_flags_ & SCHED_CONFIG_NOT_STABLE)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ());
    }

  RtecScheduler::Config_Info *config_info;
  if (config_info_map_.find (p_priority, config_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_PRIORITY_LEVEL());
    }
 
  t_priority = config_info->thread_priority;
  d_type = config_info_->dispatching_type;
}


// Returns the last priority number assigned to an operation in the
// schedule.  The number returned is one less than the total number
// of scheduled priorities.  All scheduled priorities range from 0
// to the number returned, inclusive.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::Preemption_Priority_t
ACE_Reconfig_Scheduler::last_scheduled_priority (CORBA::Environment &ACE_TRY_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED))
{
  // Check schedule stability flags.
  if (stability_flags_ & (SCHED_PRIORITY_NOT_STABLE | SCHED_CONFIG_NOT_STABLE))
    {
      ACE_THROW_RETURN (RtecScheduler::NOT_SCHEDULED (),
                        (RtecScheduler::Preemption_Priority_t) -1);
    }

  return last_scheduled_priority_;
}








