// ============================================================================
//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Scheduler_T.cpp
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHEDULER_T_C
#define TAO_RECONFIG_SCHEDULER_T_C

#include "Reconfig_Scheduler_T.h"
#include "orbsvcs/Time_Utilities.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "Reconfig_Scheduler_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Sched, Reconfig_Scheduler_T, "$Id$")

//////////////////////////////////////////////
// Helper function type definition for sort //
//////////////////////////////////////////////

// This is awkward, but it makes MSVC++ happy.
extern "C"
{
typedef int (*COMP_FUNC) (const void*, const void*);
}


// Default constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::TAO_Reconfig_Scheduler (int enforce_schedule_stability)
  : config_info_count_ (0),
    rt_info_count_ (0),
    next_handle_ (1),
    entry_ptr_array_ (0),
    entry_ptr_array_size_ (0),
    stability_flags_ (SCHED_NONE_STABLE),
    enforce_schedule_stability_ (enforce_schedule_stability),
    dependency_count_ (0),
    last_scheduled_priority_ (0)
{
}

// Constructor. Initialize the scheduler from the POD_Config_Info, POD_RT_Info,
// and POD_Dependency arrays, plus stability flag.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_Reconfig_Scheduler (int config_count,
                        ACE_Scheduler_Factory::POD_Config_Info config_infos[],
                        int rt_info_count,
                        ACE_Scheduler_Factory::POD_RT_Info rt_infos[],
                        int dependency_count,
                        ACE_Scheduler_Factory::POD_Dependency_Info dependency_infos[],
                        u_long stability_flags,
                        int enforce_schedule_stability)
  : config_info_count_ (0),
    rt_info_count_ (0),
    next_handle_ (1),
    stability_flags_ (SCHED_ALL_STABLE),
    enforce_schedule_stability_ (enforce_schedule_stability),
    dependency_count_ (0),
    last_scheduled_priority_ (0)
{
  // The init method can throw an exception, which must be caught
  // *inside* the constructor to be portable between compilers that
  // differ in whether they support native C++ exceptions.
  ACE_TRY_NEW_ENV
    {
      this->init (config_count, config_infos,
                  rt_info_count, rt_infos,
                  dependency_count, dependency_infos,
                  stability_flags ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, corba_sysex)
    {
       ACE_ERROR ((LM_ERROR, "TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, "
                             "ACE_LOCK>::TAO_Reconfig_Scheduler "
                             "system exception: cannot init scheduler.\n"));
    }
  ACE_ENDTRY;
}


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
init (int config_count,
      ACE_Scheduler_Factory::POD_Config_Info config_info[],
      int rt_info_count,
      ACE_Scheduler_Factory::POD_RT_Info rt_info[],
      int dependency_count,
      ACE_Scheduler_Factory::POD_Dependency_Info dependency_info[],
      u_long stability_flags
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::DUPLICATE_NAME,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INTERNAL))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (-1);

  int result = 0;

  // Clear out the previous entries, if any.
  this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // (Re)initialize using the new settings.

  // Add the passed config infos to the scheduler
  auto_ptr<RtecScheduler::Config_Info> new_config_info_ptr;
  for (this->config_info_count_ = 0;
       this->config_info_count_ < config_count;
       ++this->config_info_count_)
    {
      RtecScheduler::Config_Info* new_config_info;
      ACE_NEW_THROW_EX (new_config_info,
                        RtecScheduler::Config_Info,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (-1);

      // Make sure the new config info is cleaned up if we exit abruptly.
      ACE_AUTO_PTR_RESET (new_config_info_ptr, new_config_info, RtecScheduler::Config_Info);

      result = config_info_map_.bind (config_info [config_info_count_].preemption_priority,
                                      new_config_info);
      switch (result)
        {
          case -1:
            // Something bad but unknown occurred while trying to bind in map.
            ACE_THROW_RETURN (RtecScheduler::INTERNAL (), -1);

          case 1:
            // Tried to bind an operation that was already in the map.
            ACE_THROW_RETURN (RtecScheduler::DUPLICATE_NAME (), -1);

          default:
            break;
        }

      new_config_info->preemption_priority =
        config_info [config_info_count_].preemption_priority;
      new_config_info->thread_priority =
        config_info [config_info_count_].thread_priority;
      new_config_info->dispatching_type =
        config_info [config_info_count_].dispatching_type;

      if (new_config_info->preemption_priority >
          last_scheduled_priority_)
        {
          this->last_scheduled_priority_ =
            new_config_info->preemption_priority;
        }

      // Release the auto_ptr so it does not clean
      // up the sucessfully bound config info.
      new_config_info_ptr.release ();
    }

  // Add RT_Infos to scheduler
  RtecScheduler::RT_Info* new_rt_info;
  for (int num_rt_infos = 0; num_rt_infos < rt_info_count; ++num_rt_infos)
    {
      new_rt_info = create_i (rt_info [num_rt_infos].entry_point,
                              rt_info [num_rt_infos].handle
                              ACE_ENV_ARG_PARAMETER);
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
        rt_info [num_rt_infos].static_subpriority;
      new_rt_info->preemption_priority =
        rt_info [num_rt_infos].preemption_priority;
      new_rt_info->volatile_token = 0;

      // Add dependencies between RT_Infos to scheduler.
      for (this->dependency_count_ = 0;
           this->dependency_count_ < dependency_count;
           ++this->dependency_count_)
        {
          add_dependency_i (dependency_info [dependency_count_].info_that_depends,
                            dependency_info [dependency_count_].info_depended_on,
                            dependency_info [dependency_count_].number_of_calls,
                            dependency_info [dependency_count_].dependency_type
                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
        }

    }

    // Set stability flags after the operations are loaded, as the passed flags
    // should be respected as being the stability state of the passed schedule.
    this->stability_flags_ = stability_flags;

  return result;
}

// Closes the scheduler, releasing all current resources.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::close (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
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
  while (config_info_map_.current_size () > 0)
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

  // Zero out the scheduling entry pointer array but do not deallocate it.
  if (entry_ptr_array_size_ > 0)
    {
      ACE_OS::memset (entry_ptr_array_, 0,
                      sizeof (TAO_Reconfig_Scheduler_Entry *) *
                        entry_ptr_array_size_);
    }


  // Finally, reset the entry counts and start over with the lowest
  // handle number.
  this->config_info_count_ = 0;
  this->rt_info_count_ = 0;
  this->next_handle_ = 1;
}

// Create an RT_Info.  If it does not exist, a new RT_Info is
// created and inserted into the schedule, and the handle of the new
// RT_Info is returned.  If the RT_Info already exists, an exception
// is thrown.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::handle_t
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
create (const char *entry_point
        ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::DUPLICATE_NAME,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (0);

  RtecScheduler::handle_t handle = next_handle_;
  create_i (entry_point, handle ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (handle);

  // Set affected stability flags.
  this->stability_flags_ |=
    SCHED_UTILIZATION_NOT_STABLE |
    SCHED_PRIORITY_NOT_STABLE;

  return handle;
}

// Lookup a handle for an RT_Info, and return its handle, or an error
// value if it's not present.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::handle_t
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
lookup (const char * entry_point
        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (0);

  RtecScheduler::handle_t handle;
  handle = this->lookup_i (entry_point ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (handle);

  return handle;
}


// Return a pointer to the RT_Info corresponding to the passed handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::RT_Info *
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
get (RtecScheduler::handle_t handle
     ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (0);

  // Find the RT_Info in the hash map.
  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_map_.find (handle, rt_info) != 0)
    {
      ACE_THROW_RETURN (RtecScheduler::UNKNOWN_TASK (), 0);
    }

  // Allocate a new RT_Info
  RtecScheduler::RT_Info* new_info;
  ACE_NEW_THROW_EX (new_info,
                    RtecScheduler::RT_Info,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  *new_info = *rt_info;

  return new_info;
}


// Set characteristics of the RT_Info corresponding to the passed handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set (RtecScheduler::handle_t handle,
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
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Look up the RT_Info by its handle, throw an exception if it's not there.
  RtecScheduler::RT_Info *rt_info_ptr = 0;
  if (rt_info_map_.find (handle, rt_info_ptr) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

  // Call the internal set method.
  this->set_i (rt_info_ptr, criticality, time, typical_time,
               cached_time, period, importance, quantum,
               threads, info_type);

  // Update stability flags, based on changes to operation characteristics.

  // Reference the associated scheduling entry: the double cast is
  // needed to ensure that the size of the pointer and the size of the
  // stored magic cookie are the same (see the definition of
  // ptr_arith_t in ACE to grok how this works portably).
  TAO_Reconfig_Scheduler_Entry *sched_entry_ptr =
    ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                         rt_info_ptr->volatile_token);

  if (0 == sched_entry_ptr)
    {
      ACE_THROW (RtecScheduler::INTERNAL ());
    }

  // Test the utilization difference between the old and new values.
  if ((sched_entry_ptr->orig_rt_info_data ().period != rt_info_ptr->period
       && sched_entry_ptr->orig_rt_info_data ().worst_case_execution_time
          != rt_info_ptr->worst_case_execution_time))
    {
      CORBA::Double orig_time = ACE_static_cast (
        CORBA::Double,
        ACE_UINT64_DBLCAST_ADAPTER (sched_entry_ptr->
                                      orig_rt_info_data ().
                                        worst_case_execution_time));

      CORBA::Double orig_period =
        sched_entry_ptr->orig_rt_info_data ().period;

      CORBA::Double new_time = ACE_static_cast (
        CORBA::Double,
        ACE_UINT64_DBLCAST_ADAPTER (rt_info_ptr->
                                      worst_case_execution_time));

      CORBA::Double new_period = rt_info_ptr->period;

      if ((orig_time / orig_period) - (new_time / new_period) > DBL_EPSILON
          || (orig_time / orig_period) - (new_time / new_period) < DBL_EPSILON)
        {
          this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
        }
    }

  // Test the priority difference between the old and new info.
  if (RECONFIG_SCHED_STRATEGY::priority_diff (sched_entry_ptr->
                                                orig_rt_info_data (),
                                              *rt_info_ptr) != 0)
    {
      this->stability_flags_ |= SCHED_PRIORITY_NOT_STABLE;
    }

  // If the period changed, look up the handle in the calling
  // dependency map and see if there is anything there: if so,
  // the propagation is unstable.
  if (sched_entry_ptr->orig_rt_info_data ().period
      != rt_info_ptr->period)
    {
      // Get the dependency set for the current entry.
      RtecScheduler::Dependency_Set *dependency_set = 0;
      int result = calling_dependency_set_map_.find (rt_info_ptr->handle,
                                                     dependency_set);
      if (result == 0 && dependency_set->length () > 0)
        {
          this->stability_flags_ |= SCHED_PROPAGATION_NOT_STABLE;
        }
    }

  // Update the stored operation characteristics values in the scheduling entry
  sched_entry_ptr->orig_rt_info_data (*rt_info_ptr);
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
priority (RtecScheduler::handle_t handle,
          RtecScheduler::OS_Priority& o_priority,
          RtecScheduler::Preemption_Subpriority_t& subpriority,
          RtecScheduler::Preemption_Priority_t& p_priority
          ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::NOT_SCHEDULED))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Check stability flags.
  if ((this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
      && this->enforce_schedule_stability_)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ());
    }

  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_map_.find (handle, rt_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

  o_priority = rt_info->priority;
  subpriority = rt_info->preemption_subpriority;
  p_priority = rt_info->preemption_priority;
}


// Returns the priority and subpriority values assigned to an RT_Info,
// based on its entry point name.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
entry_point_priority (const char * entry_point,
                      RtecScheduler::OS_Priority& priority,
                      RtecScheduler::Preemption_Subpriority_t& subpriority,
                      RtecScheduler::Preemption_Priority_t& p_priority
                      ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::NOT_SCHEDULED))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  RtecScheduler::handle_t handle =
    this->lookup_i (entry_point ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->priority_i (handle,
                  priority,
                  subpriority,
                  p_priority
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


// This method registers a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
add_dependency (RtecScheduler::handle_t handle /* RT_Info that has the dependency */,
                RtecScheduler::handle_t dependency /* RT_Info on which it depends */,
                CORBA::Long number_of_calls,
                RtecScheduler::Dependency_Type_t dependency_type
                ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::UNKNOWN_TASK))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Delegate to the internal method.
  add_dependency_i (handle, dependency, number_of_calls,
                    dependency_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Since the call graph topology has changed, set *all*
  // stability flags before incrementing the dependency count.
  this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
  ++dependency_count_;
}


// If information has been added or changed since the last stable
// schedule was computed, this method causes scheduling information
// to be computed for all registered RT_Infos.  If the schedule is
// already stable, this is a no-op

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
compute_scheduling (CORBA::Long minimum_priority,
                    CORBA::Long maximum_priority,
                    RtecScheduler::RT_Info_Set_out infos,
                    RtecScheduler::Config_Info_Set_out configs,
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
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // @@ TO DO - use these to establish the bounds of priority assignment.
  ACE_UNUSED_ARG (minimum_priority);
  ACE_UNUSED_ARG (maximum_priority);

  // If everything is already up to date, we're done.
  if (SCHED_ALL_STABLE == stability_flags_)
    {
      return;
    }

  // @@ TO DO - use try/catch blocks to catch exceptions and add anomalies
  //            to scheduling anomaly set, and then perhaps rethrow)

  if (this->stability_flags_ & SCHED_PROPAGATION_NOT_STABLE)
    {
      // Traverse dependency graph, assigning a topological ordering and identifying threads.
      dfs_traverse_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Sort an array of RT_info handles in topological order, check
      // for loops using the strongly connected components algorithm.
      detect_cycles_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Propagate effective execution time and period, set total frame size.
      propagate_characteristics_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
    {
      // Sort operations by urgency (done by strategy), then
      // assign priorities and subpriorities in one pass.
      // Sets last scheduled priority.
      assign_priorities_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (this->stability_flags_ & SCHED_UTILIZATION_NOT_STABLE)
    {
      // Compute utilization, set last feasible priority.
      compute_utilization_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  // @@ TODO: record any scheduling anomalies in a set within the scheduler,
  //          storing the maximum severity level recorded so far.
  if (anomalies.ptr () == 0)
    {
      ACE_NEW_THROW_EX (anomalies,
                        RtecScheduler::Scheduling_Anomaly_Set (0),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }

  // return the set of scheduled RT_Infos
  if (infos.ptr () == 0)
    {
      ACE_NEW_THROW_EX (infos,
                        RtecScheduler::RT_Info_Set (this->rt_info_count_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }
  infos->length (this->rt_info_count_);
  RtecScheduler::RT_Info* rt_info = 0;
  for (ACE_TYPENAME RT_INFO_MAP::iterator info_iter (this->rt_info_map_);
       info_iter.done () == 0;
       ++info_iter)
    {
      rt_info = (*info_iter).int_id_;
      infos[ACE_static_cast (CORBA::ULong, rt_info->handle - 1)] = *rt_info;
    }

  // return the set of scheduled Config_Infos
  if (configs.ptr () == 0)
    {
      ACE_NEW_THROW_EX (configs,
                        RtecScheduler::Config_Info_Set(this->
                                                         config_info_count_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }
  configs->length (this->config_info_count_);
  RtecScheduler::Config_Info* config_info = 0;
  for (ACE_TYPENAME CONFIG_INFO_MAP::iterator config_iter (this->config_info_map_);
       config_iter.done () == 0;
       ++config_iter)
    {
      config_info = (*config_iter).int_id_;
      configs[ACE_static_cast (CORBA::ULong, config_info->preemption_priority)] = *config_info;
    }

  ACE_DEBUG ((LM_DEBUG, "Schedule prepared.\n"));
  ACE_DEBUG ((LM_DEBUG, "Dumping to stdout.\n"));
  ACE_Scheduler_Factory::dump_schedule (*(infos.ptr()), *(configs.ptr()),
                                            *(anomalies.ptr()), 0);
  ACE_DEBUG ((LM_DEBUG, "Dump done.\n"));

  // Set stability flags last.
  this->stability_flags_ = SCHED_ALL_STABLE;
  return;
}


// Provides the thread priority and queue type for the given priority
// level.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                        RtecScheduler::OS_Priority& t_priority,
                        RtecScheduler::Dispatching_Type_t & d_type
                        ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::NOT_SCHEDULED,
                      RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::UNKNOWN_PRIORITY_LEVEL))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Check stability flags
  if ((this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
      && this->enforce_schedule_stability_)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ());
    }

  RtecScheduler::Config_Info *config_info;
  if (config_info_map_.find (p_priority, config_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_PRIORITY_LEVEL());
    }

  t_priority = config_info->thread_priority;
  d_type = config_info->dispatching_type;
}


// Returns the last priority number assigned to an operation in the
// schedule.  The number returned is one less than the total number
// of scheduled priorities.  All scheduled priorities range from 0
// to the number returned, inclusive.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::Preemption_Priority_t
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
last_scheduled_priority (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::NOT_SCHEDULED))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (0);

  // Check schedule stability flags.
  if ((this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
      && this->enforce_schedule_stability_)
    {
      ACE_THROW_RETURN (RtecScheduler::NOT_SCHEDULED (),
                        (RtecScheduler::Preemption_Priority_t) -1);
    }

  return last_scheduled_priority_;
}

// Internal method to create an RT_Info.  If it does not exist, a new RT_Info is
// created and inserted into the schedule, and the handle of the new
// RT_Info is returned.  If the RT_Info already exists, an exception
// is thrown.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
RtecScheduler::RT_Info *
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
create_i (const char *entry_point,
          RtecScheduler::handle_t handle
          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::DUPLICATE_NAME,
                     RtecScheduler::INTERNAL))
{
  RtecScheduler::RT_Info* new_rt_info = 0;
  TAO_Reconfig_Scheduler_Entry* new_sched_entry = 0;
  int result = 0;

  // Create a new scheduling entry for the RT_Info.
  ACE_NEW_THROW_EX (new_rt_info,
                    RtecScheduler::RT_Info,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // Make sure the new scheduling entry is cleaned up if we exit abruptly.
  auto_ptr<RtecScheduler::RT_Info> new_rt_info_ptr (new_rt_info);

  // Set some reasonable default values, and store the passed ones.
  TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::init_rt_info (*new_rt_info);
  new_rt_info->entry_point = CORBA::string_dup(entry_point);
  new_rt_info->handle = handle;

  // Bind the new RT_Info to its handle, in the RT_Info map.
  result = rt_info_map_.bind (handle, new_rt_info);
  switch (result)
    {
      case -1:
        // Something bad but unknown occurred while trying to bind in map.
        ACE_THROW_RETURN (RtecScheduler::INTERNAL (), 0);

      case 1:
        // Tried to bind an operation that was already in the map.
        ACE_THROW_RETURN (RtecScheduler::DUPLICATE_NAME (), 0);

      default:
        break;
    }

  // Bind the new RT_Info to *its* entry point, in the tree.
  result = rt_info_tree_.bind (new_rt_info->entry_point, new_rt_info);
  switch (result)
    {
      case -1:
        // Something bad but unknown occurred while trying to bind in tree.
        rt_info_map_.unbind (handle);
        ACE_THROW_RETURN (RtecScheduler::INTERNAL (), 0);

      case 1:
        // Tried to bind an operation that was already in the tree.
        rt_info_map_.unbind (handle);
        ACE_THROW_RETURN (RtecScheduler::DUPLICATE_NAME (), 0);

      default:
        break;
    }

  // Create a new scheduling entry for the RT_Info.
  ACE_NEW_THROW_EX (new_sched_entry,
                    TAO_Reconfig_Scheduler_Entry (*new_rt_info),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // Make sure the new scheduling entry is cleaned up if we exit abruptly.
  auto_ptr<TAO_Reconfig_Scheduler_Entry> new_sched_entry_ptr (new_sched_entry);

  // Make sure there is room in the scheduling entry pointer array:
  // expand the array eagerly, to minimize memory allocation overhead

  if (this->entry_ptr_array_size_ <= handle)
    {
      if (entry_ptr_array_size_ > 0)
        {
          // Store previous array size.
          long new_size = entry_ptr_array_size_;

          // Double the size of the array until sufficient.
          do
            {
              new_size *= 2;
            }
          while (new_size <= handle);

          // Allocate the new array of the proper size, zero it out.

          TAO_Reconfig_Scheduler_Entry ** new_array;
          ACE_NEW_THROW_EX (new_array,
                    TAO_Reconfig_Scheduler_Entry * [new_size],
                    CORBA::NO_MEMORY ());

          ACE_OS::memset (new_array, 0,
                          sizeof (TAO_Reconfig_Scheduler_Entry *) *
                            new_size);

          // Copy in the previous array.
          ACE_OS::memcpy (new_array, entry_ptr_array_,
                          sizeof (TAO_Reconfig_Scheduler_Entry *) *
                            entry_ptr_array_size_);

          // Free the old array and swap to point to the new one.
          delete [] entry_ptr_array_;
          entry_ptr_array_ = new_array;
          entry_ptr_array_size_ = new_size;
        }
      else
        {
          // For the first allocation, just start with sufficient space
          // for the handle that was given.
          ACE_NEW_THROW_EX (entry_ptr_array_,
                    TAO_Reconfig_Scheduler_Entry * [handle + 1],
                    CORBA::NO_MEMORY ());
          entry_ptr_array_size_ = handle + 1;
        }
    }

  // Atore in the scheduling entry pointer array.
  entry_ptr_array_ [handle - 1] = new_sched_entry;

  // Store a pointer to the scheduling entry in the scheduling entry
  // pointer array and in the RT_Info: the double cast is needed to
  // ensure that the size of the pointer and the size of the stored
  // magic cookie are the same (see the definition of ptr_arith_t in
  // ACE to grok how this works portably).
  new_rt_info->volatile_token =
    ACE_static_cast (CORBA::ULongLong,
                     ACE_reinterpret_cast (ptr_arith_t,
                                           new_sched_entry));

  // Release the auto pointers, so their destruction does not
  // remove the new rt_info that is now in the map and tree,
  // or the scheduling entry attached to the rt_info.
  new_rt_info_ptr.release ();
  new_sched_entry_ptr.release ();

  // With everything safely registered in the map and tree, just
  // update the next handle and info counter and return the new info.
  if (handle >= this->next_handle_)
    {
      this->next_handle_ = handle + 1;
    }
  if (handle > this->rt_info_count_)
    {
      this->rt_info_count_ = handle;
    }

  return new_rt_info;
}


// Internal method to set characteristics of the passed RT_Info.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set_i (RtecScheduler::RT_Info *rt_info,
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
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
lookup_i (const char * entry_point
          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK))
{
  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_tree_.find (entry_point, rt_info) != 0)
    {
      ACE_THROW_RETURN (RtecScheduler::UNKNOWN_TASK (), 0);
    }

  return rt_info->handle;
}

// Internal method that returns the priority and subpriority values
// assigned to an RT_Info, based on its handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
priority_i (RtecScheduler::handle_t handle,
            RtecScheduler::OS_Priority& o_priority,
            RtecScheduler::Preemption_Subpriority_t& subpriority,
            RtecScheduler::Preemption_Priority_t& p_priority
            ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::NOT_SCHEDULED))
{
  // Check stability flags.
  if ((this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
      && this->enforce_schedule_stability_)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ());
    }

  RtecScheduler::RT_Info *rt_info = 0;
  if (rt_info_map_.find (handle, rt_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

  o_priority = rt_info->priority;
  subpriority = rt_info->preemption_subpriority;
  p_priority = rt_info->preemption_priority;
}


// This method registers a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
add_dependency_i (RtecScheduler::handle_t handle /* RT_Info that has the dependency */,
                  RtecScheduler::handle_t dependency /* RT_Info on which it depends */,
                  CORBA::Long number_of_calls,
                  RtecScheduler::Dependency_Type_t dependency_type
                  ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNKNOWN_TASK))
{
  // All dependencies are mapped by both the calling and called
  // operation handles, so that a topological sort can be performed
  // once over both one-way and two-way dependencies.  The dependency
  // specification is in absolute terms, however, so that the calling
  // and called handles are reversed for one-way and two way
  // dependencies.

 switch (dependency_type)
  {
    // In a two-way call, the calling operation depends on the
    // called operation.
    case RtecBase::TWO_WAY_CALL:

      // Add the calling dependency map entry
      map_dependency_i (handle,                        // calling handle
                        dependency,                    // called handle
                        calling_dependency_set_map_,   // calling map
                        number_of_calls,
                        dependency_type
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Add the called dependency map entry
      map_dependency_i (dependency,                    // called handle
                        handle,                        // calling handle
                        called_dependency_set_map_,    // called map
                        number_of_calls,
                        dependency_type
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      break;

    // In a one-way call, the called operation depends on the
    // calling operation.
    case RtecBase::ONE_WAY_CALL:

      // Add the calling dependency map entry
      map_dependency_i (dependency,                    // calling handle
                        handle,                        // called handle
                        calling_dependency_set_map_,   // calling map
                        number_of_calls,
                        dependency_type
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Add the called dependency map entry
      map_dependency_i (handle,                        // called handle
                        dependency,                    // calling handle
                        called_dependency_set_map_,    // called map
                        number_of_calls,
                        dependency_type
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      break;

    default:

      // There should not be any other kinds of dependencies.
      ACE_THROW (RtecScheduler::INTERNAL ());
  }

}

// This method installs a dependency in a dependency set map.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
map_dependency_i (RtecScheduler::handle_t key,
                  RtecScheduler::handle_t handle,
                  ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                          RtecScheduler::Dependency_Set*,
                                          ACE_Hash<RtecScheduler::handle_t>,
                                          ACE_Equal_To<RtecScheduler::handle_t>,
                                          ACE_LOCK> & map,
                  CORBA::Long number_of_calls,
                  RtecScheduler::Dependency_Type_t dependency_type
                  ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK))
{
  RtecScheduler::Dependency_Set *dependency_set;

  // Look up the dependency set in the passed map
  if (map.find (key, dependency_set) != 0)
    {
      // Create a new one
      ACE_NEW_THROW_EX (dependency_set,
                        RtecScheduler::Dependency_Set,
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      if (map.bind (key, dependency_set) != 0)
        {
          delete dependency_set;
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }
    }

  // Insert unconditionally: there can be multiple copies
  // of the same dependency, if the user so chooses.
  int prev_length =  dependency_set->length ();
  dependency_set->length (prev_length + 1);
  (*dependency_set) [prev_length].rt_info = handle;
  (*dependency_set) [prev_length].number_of_calls = number_of_calls;
  (*dependency_set) [prev_length].dependency_type = dependency_type;
}


// Traverses dependency graph, assigning a topological ordering.
// Resets scheduling entries, do DFS traversal, constructs DFS map.
// Fills in: dfs_status_, discovered_, finished_, is_thread_delineator_,
// has_unresolved_remote_dependencies_, has_unresolved_local_dependencies_,
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
dfs_traverse_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL))
{
  int i;  // index into array of scheduling entry pointers

  // Reset registered RT_Infos.
  TAO_RSE_Reset_Visitor reset_visitor;
  for (i = 0; i < this->rt_info_count_; ++i)
    {
      if (reset_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Traverse registered RT_Infos, assigning DFS start, finish order.
  TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
    dfs_visitor (this->calling_dependency_set_map_,
                 this->rt_info_map_);
  for (i = 0; i < this->rt_info_count_; ++i)
    {
      if (dfs_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }
}



// Sorts an array of RT_info handles in topological order, then
// checks for loops, marks unresolved remote dependencies.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
detect_cycles_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::CYCLIC_DEPENDENCIES))
{
  // Sort the pointers to entries in order of descending forward
  // finish times, which produces a topological ordering, with
  // callers ahead of called nodes.
  ACE_OS::qsort (ACE_reinterpret_cast (void *, entry_ptr_array_),
                 this->rt_info_count_,
                 sizeof (TAO_Reconfig_Scheduler_Entry *),
                 ACE_reinterpret_cast (COMP_FUNC,
                                       RECONFIG_SCHED_STRATEGY::comp_entry_finish_times));

  // Traverse entries in reverse topological order,
  // looking for strongly connected components (cycles).
  TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
    scc_visitor (this->called_dependency_set_map_,
                   this->rt_info_map_);
  for (int i = 0; i < this->rt_info_count_; ++i)
    {
      // Each new top level entry marks a potential new cycle.
      scc_visitor.in_a_cycle (0);

      if (scc_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Check whether any cycles were detected.
  if (scc_visitor.number_of_cycles () > 0)
    {
      ACE_THROW (RtecScheduler::CYCLIC_DEPENDENCIES ());
    }
}

// Propagates effective execution time and period, sets total frame size.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
propagate_characteristics_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNRESOLVED_LOCAL_DEPENDENCIES,
                      RtecScheduler::THREAD_SPECIFICATION))
{
  // Traverse entries in topological (DFS finish) order,
  // propagating period and effective execution time from
  // calling node to called node at each step.
  TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
    prop_visitor (this->calling_dependency_set_map_,
                  this->rt_info_map_);
  for (int i = 0; i < this->rt_info_count_; ++i)
    {
      if (prop_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Check whether any unresolved local dependencies were detected.
  if (prop_visitor.unresolved_locals () > 0)
    {
      ACE_THROW (RtecScheduler::UNRESOLVED_LOCAL_DEPENDENCIES ());
    }

  // Check whether any thread specification errors were detected.
  if (prop_visitor.thread_specification_errors () > 0)
    {
      ACE_THROW (RtecScheduler::THREAD_SPECIFICATION ());
    }
}

// Sort operations by urgency (done by strategy), then
// assign priorities and subpriorities in one pass.
// Sets last scheduled priority.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
assign_priorities_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::DUPLICATE_NAME))
{
  // Sort the pointers to entries in descending order
  // of static priority and static subpriority, according
  // to our given scheduling strategy.
  ACE_OS::qsort (ACE_reinterpret_cast (void *, entry_ptr_array_),
                 this->rt_info_count_,
                 sizeof (TAO_Reconfig_Scheduler_Entry *),
                 ACE_reinterpret_cast (COMP_FUNC,
                                       RECONFIG_SCHED_STRATEGY::total_priority_comp));

  // Empty out the previously stored configuration infos, if any.
  RtecScheduler::Preemption_Priority_t config_priority;
  RtecScheduler::Config_Info *config_info_temp;
  while (config_info_map_.current_size () > 0)
    {
      config_priority = (*config_info_map_.begin ()).ext_id_;
      if (config_info_map_.unbind (config_priority, config_info_temp) == 0)
        {
          delete config_info_temp;
        }
      else
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Traverse using a priority assignment visitor, which uses a
  // strategy to decide when a new priority or subpriority is reached.
  TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY>
    prio_visitor (this->rt_info_count_, this->entry_ptr_array_);
  auto_ptr<RtecScheduler::Config_Info> new_config_info_ptr;
  for (int i = 0; i <= this->rt_info_count_; ++i)
    {
      int result;

      if (i < this->rt_info_count_)
        {
          // Visit the next entry in the array.
          result = prio_visitor.visit (* (this->entry_ptr_array_ [i]));
        }
      else
        {
          // Finish up after all enties in the array have been visited.
          result = prio_visitor.finish ();
        }

      if (result < 0)
        {
          // Something bad happened with the internal data structures.
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
      else if (result == 1)
        {
          RtecScheduler::Config_Info* new_config_info;
          ACE_NEW_THROW_EX (new_config_info,
                            RtecScheduler::Config_Info,
                            CORBA::NO_MEMORY ());
          ACE_CHECK;

          // Make sure the new config info is cleaned up if we exit abruptly.
          ACE_AUTO_PTR_RESET (new_config_info_ptr, new_config_info, RtecScheduler::Config_Info);

          // Have the strategy fill in the new config info for that
          // priority level, using the representative scheduling entry.
          if  (RECONFIG_SCHED_STRATEGY::assign_config (*new_config_info,
                                                       *(entry_ptr_array_ [i])) < 0)
            {
              ACE_THROW (RtecScheduler::INTERNAL ());
            }

          if (new_config_info->preemption_priority >
              last_scheduled_priority_)
            {
              this->last_scheduled_priority_ =
                new_config_info->preemption_priority;
            }

          result = config_info_map_.bind (new_config_info->preemption_priority,
                                          new_config_info);
          switch (result)
            {
              case -1:
                // Something bad but unknown occurred while trying to bind in map.
                ACE_THROW (RtecScheduler::INTERNAL ());

              case 1:
                // Tried to bind an operation that was already in the map.
                ACE_THROW (RtecScheduler::DUPLICATE_NAME ());

              default:
                ++this->config_info_count_;
                break;
            }

          // Release the auto_ptr so it does not clean
          // up the sucessfully bound config info.
          new_config_info_ptr.release ();
        }
    }
}

// Compute utilization, set last feasible priority.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
compute_utilization_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL))
{
  TAO_RSE_Utilization_Visitor<RECONFIG_SCHED_STRATEGY> util_visitor;
  for (int i = 0; i < this->rt_info_count_; ++i)
    {
      if (util_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Store the values accumulated by the visitor.
  this->noncritical_utilization_ =
    util_visitor.noncritical_utilization ();
  this->critical_utilization_ =
    util_visitor.critical_utilization ();
}


// Static helper method to give an RT_Info some reasonable default values.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::init_rt_info (RtecScheduler::RT_Info &rt_info)
     ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Set some reasonable default values.
  rt_info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  rt_info.worst_case_execution_time = 0;
  rt_info.typical_execution_time = 0;
  rt_info.cached_execution_time = 0;
  rt_info.period = 1;
  rt_info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  rt_info.quantum = 0;
  rt_info.threads = 0;
  rt_info.info_type = RtecScheduler::OPERATION;
  rt_info.priority = 0;
  rt_info.preemption_subpriority = 0;
  rt_info.preemption_priority = 0;
  rt_info.volatile_token = 0;
}


// Accesses scheduling strategy for the reconfig scheduler.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> RECONFIG_SCHED_STRATEGY &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::sched_strategy ()
{
  return this->sched_strategy_;
}


// Accesses map for O(1) lookup of Config_Infos by priority level.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_Hash_Map_Manager_Ex<RtecScheduler::Preemption_Priority_t,
                                  RtecScheduler::Config_Info*,
                                  ACE_Hash<RtecScheduler::Preemption_Priority_t>,
                                  ACE_Equal_To<RtecScheduler::Preemption_Priority_t>,
                                  ACE_LOCK> &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::config_info_map ()
{
  return this->config_info_map_;
}


// Returns the number of config infos, which is also the number of
// assigned priority levels.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> long
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::config_info_count ()
{
  return this->config_info_count_;
}


// Accesses map for O(1) lookup of RT_Infos by handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                      RtecScheduler::RT_Info*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::rt_info_map ()
{
  return this->rt_info_map_;
}


// Returns the number of registered RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> long
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::rt_info_count ()
{
  return this->rt_info_count_;
}


// Accesses tree for O(log n) lookup of RT_Infos by name.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_RB_Tree<const char *,
                      RtecScheduler::RT_Info*,
                      ACE_Less_Than<const char *>,
                      ACE_LOCK> &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::rt_info_tree ()
{
  return this->rt_info_tree_;
}


// Accesses map for O(1) lookup of RT_Info dependency
// set by the caller operation's handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                        RtecScheduler::Dependency_Set*,
                        ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::calling_dependency_set_map ()
{
  return this->calling_dependency_set_map_;
}


// Accesses map for O(1) lookup of RT_Info dependency
// set by the called operation's handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::called_dependency_set_map ()
{
  return this->called_dependency_set_map_;
}


// Returns the number of dependencies in the dependency lists of all RT_Infos.
// This is used when traversing the dependency graph.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::dependency_count ()
{
  return this->dependency_count_;
}



#endif /* TAO_RECONFIG_SCHEDULER_T_C */
