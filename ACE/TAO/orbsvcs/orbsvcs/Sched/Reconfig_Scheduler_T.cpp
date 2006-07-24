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

#include "orbsvcs/Sched/Reconfig_Scheduler_T.h"
#include "orbsvcs/Time_Utilities.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//#ifdef _DEBUG
//#define SCHEDULER_LOGGING 1
//#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_Reconfig_Scheduler (int enforce_schedule_stability,
                        const CORBA::Double & critical_utilization_threshold,
                        const CORBA::Double & noncritical_utilization_threshold)
  : config_info_count_ (0),
    rt_info_count_ (0),
    rt_info_tuple_count_ (0),
    next_handle_ (1),
    entry_ptr_array_ (0),
    entry_ptr_array_size_ (0),
    tuple_ptr_array_ (0),
    tuple_ptr_array_size_ (0),
    stability_flags_ (SCHED_NONE_STABLE),
    enforce_schedule_stability_ (enforce_schedule_stability),
    dependency_count_ (0),
    last_scheduled_priority_ (0),
    noncritical_utilization_ (0.0),
    critical_utilization_ (0.0),
    noncritical_utilization_threshold_ (noncritical_utilization_threshold),
    critical_utilization_threshold_ (critical_utilization_threshold)
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler default ctor.\n"));
#endif /* SCHEDULER_LOGGING */
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
                        int enforce_schedule_stability,
                        const CORBA::Double & critical_utilization_threshold,
                        const CORBA::Double & noncritical_utilization_threshold)
  : config_info_count_ (0),
    rt_info_count_ (0),
    rt_info_tuple_count_ (0),
    next_handle_ (1),
    stability_flags_ (SCHED_ALL_STABLE),
    enforce_schedule_stability_ (enforce_schedule_stability),
    dependency_count_ (0),
    last_scheduled_priority_ (0),
    noncritical_utilization_ (0.0),
    critical_utilization_ (0.0),
    noncritical_utilization_threshold_ (noncritical_utilization_threshold),
    critical_utilization_threshold_ (critical_utilization_threshold)
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler alternative ctor.\n"));
#endif /* SCHEDULER_LOGGING */

  // @ TODO - think about what it means to emit all the tuples as
  // well as the established RT_Infos.  State is more complex now.

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


// Destructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
~TAO_Reconfig_Scheduler ()
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler dtor.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_TRY_NEW_ENV
    {
      this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, corba_sysex)
    {
      ACE_ERROR ((LM_ERROR, "TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, "
                            "ACE_LOCK>::~TAO_Reconfig_Scheduler "
                            "exception: cannot close scheduler.\n"));
    }
  ACE_ENDTRY;

  // Delete the entry and tuple pointer arrays.
  delete [] entry_ptr_array_;
  delete [] tuple_ptr_array_;
}

// Additive initialization: can be called multiple times, with
// new sets of operation, dependency, and config information.

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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::init.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (-1);

  int result = 0;
  int i = 0;

/* WSOA merge - commented out
  // Clear out the previous entries, if any.
  this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
*/

  // Re-map the RT_Info and dependency handle values if necessary.
  // Assumes that dependencies only refer to handles within the
  // current set: changing that assumption would require us to use
  // operation names, and the equivalent of a symbol table and
  // relocating linker for RT_Infos to do this correctly in the
  // general case.
  if (this->next_handle_ > 1)
    {
      for (i = 0; i < rt_info_count; ++i)
        {
          rt_info [i].handle += this->next_handle_ - 1;
        }
      for (i = 0; i < dependency_count; ++i)
        {
          dependency_info [i].info_that_depends += this->next_handle_ - 1;
          dependency_info [i].info_depended_on += this->next_handle_ - 1;
        }
    }

  // (Re)initialize using the new settings.

  // Add the passed config infos to the scheduler
  auto_ptr<RtecScheduler::Config_Info> new_config_info_ptr;
  for (i = 0; i < config_count; ++i)
    {
      RtecScheduler::Config_Info* new_config_info;
      ACE_NEW_THROW_EX (new_config_info,
                        RtecScheduler::Config_Info,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (-1);

      // Make sure the new config info is cleaned up if we exit abruptly.
      ACE_AUTO_PTR_RESET (new_config_info_ptr, new_config_info, RtecScheduler::Config_Info);

      result = config_info_map_.bind (config_info [i].preemption_priority,
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
        config_info [i].preemption_priority;
      new_config_info->thread_priority =
        config_info [i].thread_priority;
      new_config_info->dispatching_type =
        config_info [i].dispatching_type;

      if (new_config_info->preemption_priority >
          last_scheduled_priority_)
        {
          this->last_scheduled_priority_ =
            new_config_info->preemption_priority;
        }

      // Release the auto_ptr so it does not clean
      // up the sucessfully bound config info.
      new_config_info_ptr.release ();

      // Increase the count of successfully bound config infos.
      ++this->config_info_count_;
    }

  // Add RT_Infos to scheduler
  TAO_RT_Info_Ex* new_rt_info;
  for (int num_rt_infos = 0; num_rt_infos < rt_info_count; ++num_rt_infos)
    {
      new_rt_info = create_i (rt_info [num_rt_infos].entry_point,
                              rt_info [num_rt_infos].handle, 1
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (new_rt_info == 0)
        {
          ACE_THROW_RETURN (RtecScheduler::INTERNAL (), -1);
        }

      // Set the new info's enabled state
      new_rt_info->enabled_state (rt_info [num_rt_infos].enabled);

      // Fill in the portions to which the user has access.
      this->set_i (new_rt_info,
             RtecScheduler::Criticality_t (rt_info [num_rt_infos].criticality),
             rt_info [num_rt_infos].worst_case_execution_time,
             rt_info [num_rt_infos].typical_execution_time,
             rt_info [num_rt_infos].cached_execution_time,
             rt_info [num_rt_infos].period,
             RtecScheduler::Importance_t (rt_info [num_rt_infos].importance),
             rt_info [num_rt_infos].quantum,
             rt_info [num_rt_infos].threads,
             RtecScheduler::Info_Type_t (rt_info [num_rt_infos].info_type)
             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      // Fill in the scheduler managed portions.
      new_rt_info->priority =
        rt_info [num_rt_infos].priority;
      new_rt_info->preemption_subpriority =
        rt_info [num_rt_infos].static_subpriority;
      new_rt_info->preemption_priority =
        rt_info [num_rt_infos].preemption_priority;
      new_rt_info->volatile_token = 0;

      // Add dependencies between RT_Infos to scheduler.
      for (i = 0; i < dependency_count; ++i)
        {
          add_dependency_i (dependency_info [dependency_count_].info_that_depends,
                            dependency_info [dependency_count_].info_depended_on,
                            dependency_info [dependency_count_].number_of_calls,
                            dependency_info [dependency_count_].dependency_type,
                            dependency_info [dependency_count_].enabled
                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          ++this->dependency_count_;
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
                     RtecScheduler::INTERNAL,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::SYNCHRONIZATION_FAILURE))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::close.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Unbind and delete each RT_Info in the map: this also cleans up
  // all the entries and tuples associated with each RT_Info.
  TAO_RT_Info_Ex *rt_info = 0;
  RtecScheduler::handle_t handle;
  while (rt_info_map_.current_size () > 0)
    {
      handle = (*rt_info_map_.begin ()).ext_id_;
      if (rt_info_map_.unbind (handle, rt_info) == 0)
        {
          if (rt_info_tree_.unbind (rt_info->entry_point) == 0)
            {
              // Delete the entry associated with the RT_Info, then
              // the RT_Info itself.
              delete ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                                          rt_info->volatile_token);
              delete rt_info;
            }
          else
            {
              ACE_THROW (RtecScheduler::INTERNAL ());
            }
         }
      else
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }
    }

  // Delete each Config_Info in the map.
  RtecScheduler::Preemption_Priority_t config_priority;
  RtecScheduler::Config_Info *config_info = 0;
  while (config_info_map_.current_size () > 0)
    {
      config_priority = (*config_info_map_.begin ()).ext_id_;
      if (config_info_map_.unbind (config_priority, config_info) == 0)
        {
          delete config_info;
        }
      else
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Delete each dependency set in the caller map
  RtecScheduler::Dependency_Set *dependency_set = 0;
  while (calling_dependency_set_map_.current_size () > 0)
    {
      handle = (*calling_dependency_set_map_.begin ()).ext_id_;
      if (calling_dependency_set_map_.unbind (handle, dependency_set) == 0)
        {
          delete dependency_set;
        }
      else
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Delete each dependency set in the called map
  while (called_dependency_set_map_.current_size () > 0)
    {
      handle = (*called_dependency_set_map_.begin ()).ext_id_;
      if (called_dependency_set_map_.unbind (handle, dependency_set) == 0)
        {
          delete dependency_set;
        }
      else
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Zero out the scheduling entry pointer array but do not deallocate it.
  if (entry_ptr_array_size_ > 0)
    {
      ACE_OS::memset (this->entry_ptr_array_, 0,
                      sizeof (TAO_Reconfig_Scheduler_Entry *)
                      * this->entry_ptr_array_size_);
    }

  // Zero out the scheduling entry pointer array but do not deallocate it.
  if (tuple_ptr_array_size_ > 0)
    {
      ACE_OS::memset (this->tuple_ptr_array_, 0,
                      sizeof (TAO_RT_Info_Tuple *)
                      * this->tuple_ptr_array_size_);
    }

  // Finally, reset the entry counts and start over with the lowest
  // handle number.
  this->config_info_count_ = 0;
  this->rt_info_count_ = 0;
  this->rt_info_tuple_count_ = 0;
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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::create.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (0);

  RtecScheduler::handle_t handle = next_handle_;
  create_i (entry_point, handle, 0 ACE_ENV_ARG_PARAMETER);
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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::lookup.\n"));
#endif /* SCHEDULER_LOGGING */

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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::get.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK_RETURN (0);

  // Find the RT_Info in the hash map.
  TAO_RT_Info_Ex *rt_info = 0;
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
set (::RtecScheduler::handle_t handle,
     ::RtecScheduler::Criticality_t criticality,
     ::RtecScheduler::Time time,
     ::RtecScheduler::Time typical_time,
     ::RtecScheduler::Time cached_time,
     ::RtecScheduler::Period_t period,
     ::RtecScheduler::Importance_t importance,
     ::RtecScheduler::Quantum_t quantum,
     ::RtecScheduler::Threads_t threads,
     ::RtecScheduler::Info_Type_t info_type
     ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Look up the RT_Info by its handle, throw an exception if it's not there.
  TAO_RT_Info_Ex *rt_info_ptr = 0;
  if (rt_info_map_.find (handle, rt_info_ptr) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

  if (rt_info_ptr == 0)
    {
      ACE_THROW (RtecScheduler::INTERNAL ());
    }

  // Enable the RT_Info if it was disabled.  Does not modify NON_VOLATILE ops.
  if (rt_info_ptr->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    {
      rt_info_ptr->enabled_state (RtecScheduler::RT_INFO_ENABLED);
    }

  // Call the internal set method.
  this->set_i (rt_info_ptr, criticality, time, typical_time,
               cached_time, period, importance, quantum,
               threads, info_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;


  // Update stability flags.  For now, just mark everything as unstable.
  // @@ TODO - revisit this and see if we can efficiently detect when
  //           changes do not affect stability of various aspects.
  this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
  this->stability_flags_ |= SCHED_PRIORITY_NOT_STABLE;
  this->stability_flags_ |= SCHED_PROPAGATION_NOT_STABLE;

  return;
}

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
reset (RtecScheduler::handle_t handle,
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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::reset.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Look up the RT_Info by its handle, throw an exception if it's not there.
  TAO_RT_Info_Ex *rt_info_ptr = 0;
  if (rt_info_map_.find (handle, rt_info_ptr) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }
  if (rt_info_ptr == 0)
    {
      ACE_THROW (RtecScheduler::INTERNAL ());
    }

  // Enable the RT_Info if it was disabled.  Does not modify NON_VOLATILE ops.
  if (rt_info_ptr->enabled_state () == RtecScheduler::RT_INFO_NON_VOLATILE)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }
  else
    {
      // Reset the RT_Info.
      rt_info_ptr->reset (TAO_Reconfig_Scheduler_Entry::ORIGINAL
                          | TAO_Reconfig_Scheduler_Entry::PROPAGATED);

      rt_info_ptr->enabled_state (RtecScheduler::RT_INFO_ENABLED);
    }

  // Refresh the internal tuple pointer array.
  this->refresh_tuple_ptr_array_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Then call the internal set method.
  this->set_i (rt_info_ptr, criticality, time, typical_time,
               cached_time, period, importance, quantum,
               threads, info_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;


  // Update stability flags.  For now, just mark everything as unstable.
  // @@ TODO - revisit this and see if we can efficiently detect when
  //           changes do not affect stability of various aspects.
  this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
  this->stability_flags_ |= SCHED_PRIORITY_NOT_STABLE;
  this->stability_flags_ |= SCHED_PROPAGATION_NOT_STABLE;

  return;
}



template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set_seq (const RtecScheduler::RT_Info_Set& infos
         ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set_seq.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  for (u_int i = 0; i < infos.length (); ++i)
    {
      // Look up the RT_Info by its handle, throw an exception if it's not there.
      TAO_RT_Info_Ex *rt_info_ptr = 0;
      if (rt_info_map_.find (infos[i].handle, rt_info_ptr) != 0)
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }

      if (rt_info_ptr == 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }

      // Enable the RT_Info if it was disabled.  Does not modify NON_VOLATILE ops.
      if (rt_info_ptr->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
        {
          rt_info_ptr->enabled_state (RtecScheduler::RT_INFO_ENABLED);
        }

#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x582) && defined (NDEBUG)
      // BCB2006 and BCB6 get an internal backend error when building this
      // code in release mode, reported to Borland as QC27961
      // Call the internal set method.
      RtecScheduler::RT_Info info = infos[i];

      this->set_i (rt_info_ptr,
                   info.criticality,
                   info.worst_case_execution_time,
                   info.typical_execution_time,
                   info.cached_execution_time,
                   info.period,
                   info.importance,
                   info.quantum,
                   info.threads,
                   info.info_type
                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
#else
      this->set_i (rt_info_ptr,
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
#endif
    }

  // Update stability flags.  For now, just mark everything as unstable.
  // @@ TODO - revisit this and see if we can efficiently detect when
  //           changes do not affect stability of various aspects.
          this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
  this->stability_flags_ |= SCHED_PRIORITY_NOT_STABLE;
  this->stability_flags_ |= SCHED_PROPAGATION_NOT_STABLE;

  return;
}


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
reset_seq (const RtecScheduler::RT_Info_Set& infos
           ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::reset_seq.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  TAO_RT_Info_Ex *rt_info_ptr = 0;
  u_int i;

  for (i = 0; i < infos.length (); ++i)
    {
      // Look up the RT_Info by its handle, throw an exception if it's not there.
      if (rt_info_map_.find (infos[i].handle, rt_info_ptr) != 0)
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }

      // Enable the RT_Info.  Does not modify NON_VOLATILE ops.
      if (rt_info_ptr->enabled_state () == RtecScheduler::RT_INFO_NON_VOLATILE)
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }
      else
        {
          // Reset the RT_Info.
          rt_info_ptr->reset (TAO_Reconfig_Scheduler_Entry::ORIGINAL
                              | TAO_Reconfig_Scheduler_Entry::PROPAGATED);

          rt_info_ptr->enabled_state (RtecScheduler::RT_INFO_ENABLED);
        }
    }

  // Refresh the internal tuple pointer array.
  this->refresh_tuple_ptr_array_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (i = 0; i < infos.length (); ++i)
    {
      // Look up the RT_Info by its handle, throw an exception if it's not there.
      if (rt_info_map_.find (infos[i].handle, rt_info_ptr) != 0)
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }

      if (rt_info_ptr == 0)
    {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }

#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x582) && defined (NDEBUG)
      // BCB2006 and BCB6 get an internal backend error when building this
      // code in release mode, reported to Borland as QC27961
      // Call the internal set method.
      RtecScheduler::RT_Info info = infos[i];

      // Call the internal set method.
      this->set_i (rt_info_ptr,
                   info.criticality,
                   info.worst_case_execution_time,
                   info.typical_execution_time,
                   info.cached_execution_time,
                   info.period,
                   info.importance,
                   info.quantum,
                   info.threads,
                   info.info_type
                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
#else
      // Call the internal set method.
      this->set_i (rt_info_ptr,
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
#endif
    }

  // Update stability flags.  For now, just mark everything as unstable.
  // @@ TODO - revisit this and see if we can efficiently detect when
  //           changes do not affect stability of various aspects.
  this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
      this->stability_flags_ |= SCHED_PRIORITY_NOT_STABLE;
  this->stability_flags_ |= SCHED_PROPAGATION_NOT_STABLE;

  return;
}

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
replace_seq (const RtecScheduler::RT_Info_Set& infos
             ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::UNKNOWN_TASK,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::SYNCHRONIZATION_FAILURE))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::replace_seq.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  TAO_RT_Info_Ex *rt_info_ptr = 0;

  for (ACE_TYPENAME RT_INFO_MAP::iterator info_iter (this->rt_info_map_);
       info_iter.done () == 0;
       ++info_iter)
    {
      // Get a pointer to each registered RT_Info.
      rt_info_ptr = (*info_iter).int_id_;
      if (! rt_info_ptr)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }

      switch (rt_info_ptr->enabled_state ())
        {
          case RtecScheduler::RT_INFO_ENABLED:

            // Disable enabled RT_Infos.
            rt_info_ptr->enabled_state (RtecScheduler::RT_INFO_DISABLED);

            // Reset Enabled and Non-Volatile RT_Infos.
            rt_info_ptr->reset (TAO_Reconfig_Scheduler_Entry::ORIGINAL
                                | TAO_Reconfig_Scheduler_Entry::PROPAGATED);
            break;

            // Intentional fall-through to ignore non-volatile RT_Infos
          case RtecScheduler::RT_INFO_NON_VOLATILE:

          default:                             // Ignore disabled RT_Infos.
            break;
        }
    }

  // Zero out the tuple pointer array, set count to zero
  ACE_OS::memset (this->tuple_ptr_array_, 0,
                  sizeof (TAO_RT_Info_Tuple *)
                  * this->tuple_ptr_array_size_);
  this->rt_info_tuple_count_ = 0;

  for (u_int i = 0; i < infos.length (); ++i)
    {
      // Look up the RT_Info by its handle, throw an exception if it's not there.
      if (rt_info_map_.find (infos[i].handle, rt_info_ptr) != 0)
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

      if (rt_info_ptr == 0)
    {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }

      // Enable the RT_Info if it was disabled.  Does not modify NON_VOLATILE ops.
      if (rt_info_ptr->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
        {
          rt_info_ptr->enabled_state (RtecScheduler::RT_INFO_ENABLED);
        }

#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x582) && defined (NDEBUG)
      // BCB2006 and BCB6 get an internal backend error when building this
      // code in release mode, reported to Borland as QC27961
      // Call the internal set method.
      RtecScheduler::RT_Info info = infos[i];

      // Call the internal set method.
      this->set_i (rt_info_ptr,
                   info.criticality,
                   info.worst_case_execution_time,
                   info.typical_execution_time,
                   info.cached_execution_time,
                   info.period,
                   info.importance,
                   info.quantum,
                   info.threads,
                   info.info_type
                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
#else
      // Call the internal set method.
      this->set_i (rt_info_ptr,
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
#endif
    }

  // Update stability flags.  For now, just mark everything as unstable.
  // @@ TODO - revisit this and see if we can efficiently detect when
  //           changes do not affect stability of various aspects.
  this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
  this->stability_flags_ |= SCHED_PRIORITY_NOT_STABLE;
  this->stability_flags_ |= SCHED_PROPAGATION_NOT_STABLE;

  return;
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

  // CDG - TBD - address priority "generations" i.e., after an
  // adaptive transition.  For now, go ahead and return whatever
  // priority is there, even if the RT_Info_Ex is disabled.

  TAO_RT_Info_Ex *rt_info = 0;
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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::entry_point_priority.\n"));
#endif /* SCHEDULER_LOGGING */

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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::add_dependency.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Delegate to the internal method.
  add_dependency_i (handle, dependency, number_of_calls, dependency_type,
                    RtecBase::DEPENDENCY_ENABLED ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Since the call graph topology has changed, set *all*
  // stability flags before incrementing the dependency count.
  this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
  ++dependency_count_;
}


// This method removes a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
remove_dependency (RtecScheduler::handle_t handle,
                   RtecScheduler::handle_t dependency,
                   CORBA::Long number_of_calls,
                   RtecScheduler::Dependency_Type_t dependency_type
                   ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::remove_dependency.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Delegate to the internal method.
  remove_dependency_i (handle, dependency, number_of_calls,
                       dependency_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Since the call graph topology has changed, set *all*
  // stability flags before incrementing the dependency count.
  this->stability_flags_ |= SCHED_UTILIZATION_NOT_STABLE;
  --dependency_count_;
}

// This method sets the enable state for a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set_dependency_enable_state (RtecScheduler::handle_t handle,
                             RtecScheduler::handle_t dependency,
                             CORBA::Long number_of_calls,
                             RtecScheduler::Dependency_Type_t dependency_type,
                             RtecScheduler::Dependency_Enabled_Type_t enabled
                             ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set_dependency_enable_state.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Delegate to the internal method.
  set_dependency_enable_state_i (handle, dependency, number_of_calls,
                                 dependency_type, enabled ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


// This method sets the enable state of a sequence of dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set_dependency_enable_state_seq (const RtecScheduler::Dependency_Set & dependencies
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set_dependency_enable_state_seq.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Delegate to the internal method for each dependency in the sequence.
  for (u_int i = 0; i < dependencies.length (); ++i)
    {
      set_dependency_enable_state_i (dependencies[i].rt_info,
                                     dependencies[i].rt_info_depended_on,
                                     dependencies[i].number_of_calls,
                                     dependencies[i].dependency_type,
                                     dependencies[i].enabled
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}


// This method enables or disables an RT_Info.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set_rt_info_enable_state (RtecScheduler::handle_t handle,
                          RtecScheduler::RT_Info_Enabled_Type_t enabled
                          ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set_rt_info_enable_state.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Look up the RT_Info by its handle, throw an exception if it's not there.
  TAO_RT_Info_Ex *rt_info_ptr = 0;
  if (rt_info_map_.find (handle, rt_info_ptr) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

  if (rt_info_ptr == 0)
    {
      ACE_THROW (RtecScheduler::INTERNAL ());
    }

  // Enable the RT_Info.
  rt_info_ptr->enabled_state (enabled);
}


// This method enables or disables a sequence of RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set_rt_info_enable_state_seq (const RtecScheduler::RT_Info_Enable_State_Pair_Set & pair_set
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::INTERNAL,
                     RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set_rt_info_enable_state_seq.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  for (u_int i = 0; i < pair_set.length (); ++i)
    {
      // Look up the RT_Info by its handle, throw an exception if it's not there.
      TAO_RT_Info_Ex *rt_info_ptr = 0;
      if (rt_info_map_.find (pair_set[i].handle, rt_info_ptr) != 0)
        {
          ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
        }

      if (rt_info_ptr == 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }

      // Enable the RT_Info.
      rt_info_ptr->enabled_state (pair_set[i].enabled);
    }
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
                    RtecScheduler::Dependency_Set_out dependencies,
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
  // Delegates to recompute_scheduling and the respective accessors.
  this->recompute_scheduling (minimum_priority, maximum_priority,
                              anomalies ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->get_rt_info_set (infos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->get_dependency_set (dependencies ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->get_config_info_set (configs ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#if defined (SCHEDULER_DUMP)
  ACE_DEBUG ((LM_TRACE, "Schedule prepared.\n"));
  ACE_DEBUG ((LM_TRACE, "Dumping to stdout.\n"));
  ACE_Scheduler_Factory::dump_schedule (*(infos.ptr()), *(dependencies.ptr()), *(configs.ptr()),
                                            *(anomalies.ptr()), 0);
  ACE_DEBUG ((LM_TRACE, "Dump done.\n"));
#endif // SCHEDULER_DUMP

  return;
}

// Recomputes the scheduling priorities, etc.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
recompute_scheduling (CORBA::Long /* minimum_priority */,
                      CORBA::Long /* maximum_priority */,
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
#if defined (SCHEDULER_LOGGING)
   ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::compute_scheduling.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // @@ TO DO - use these to establish the bounds of priority assignment.
  //      minimum_priority
  //      maximum_priority

  // If everything is already up to date, we're done.
  if (SCHED_ALL_STABLE == stability_flags_)
    {

      // Must always provide a value for an out parameter
      ACE_NEW_THROW_EX (anomalies,
         RtecScheduler::Scheduling_Anomaly_Set (0),
         CORBA::NO_MEMORY ());
      ACE_CHECK;
      return;
    }

  // @@ TO DO - use try/catch blocks to catch exceptions and add anomalies
  //            to scheduling anomaly set, and then perhaps rethrow)

  if ((this->stability_flags_ & SCHED_PROPAGATION_NOT_STABLE)
      || (this->stability_flags_ & SCHED_UTILIZATION_NOT_STABLE))
    {

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "1_pre_crit_traverse.txt");
#endif

      // Traverse criticality dependency graph, assigning a
      // topological ordering and identifying threads.
      crit_dfs_traverse_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "2_crit_dfs_traverse_i.txt");
#endif

      // Propagate criticalities.
      propagate_criticalities_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "3_propagate_criticalities_i.txt");
#endif

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "4_pre_traverse.txt");
#endif

      // Traverse dependency graph, assigning a topological ordering and identifying threads.
      dfs_traverse_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "5_dfs_traverse_i.txt");
#endif

      // Sort an array of RT_info handles in topological order, check
      // for loops using the strongly connected components algorithm.
      detect_cycles_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "6_detect_cycles_i.txt");
#endif

      // Perform admission control for task delineator rate tuples.
      perform_admission_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "7_perform_admission_i.txt");
#endif

      // Propagate effective execution time and period, set total frame size.
      propagate_characteristics_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "8_propagate_characteristics_i.txt");
#endif

    }

  if (this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
    {
      // Sort operations by urgency, then assign priorities and
      // subpriorities in one pass.  Sets last scheduled priority and
      // last feasible priority.
      assign_priorities_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

#if defined (SCHEDULER_LOGGING)
      ACE_Scheduler_Factory::log_scheduling_entries(entry_ptr_array_,
                                                    this->rt_info_count_,
                                                    "9_assign_priorities_i.txt");
#endif

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

  ACE_DEBUG ((LM_DEBUG,
              "cutil = %f, ncutil = %f\n",
              this->critical_utilization_,
              this->noncritical_utilization_));

  if (this->critical_utilization_ > critical_utilization_threshold_ ||
      this->noncritical_utilization_ > noncritical_utilization_threshold_)
    {
      CORBA::ULong len = anomalies->length ();
      anomalies->length (len + 1);
      anomalies[len].description = CORBA::string_dup("Utilization Bound exceeded");
      anomalies[len].severity =  RtecScheduler::ANOMALY_ERROR;
    }

  // Set stability flags last.
  this->stability_flags_ = SCHED_ALL_STABLE;
  return;
}


// Returns the set of rt_infos, with their assigned priorities (as
// of the last schedule re-computation).

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
get_rt_info_set (RtecScheduler::RT_Info_Set_out infos
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INTERNAL))
{
  // return the set of scheduled RT_Infos

  if (infos.ptr () == 0)
    {
      ACE_NEW_THROW_EX (infos,
                        RtecScheduler::RT_Info_Set (this->rt_info_count_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }

  infos->length (this->rt_info_count_);
  TAO_RT_Info_Ex* rt_info = 0;
  for (ACE_TYPENAME RT_INFO_MAP::iterator info_iter (this->rt_info_map_);
       info_iter.done () == 0;
       ++info_iter)
    {
      // TODO - rethink this: is it more useful to only return the *enabled* RT_Infos?
      rt_info = (*info_iter).int_id_;
      infos[static_cast<CORBA::ULong> (rt_info->handle - 1)] = *rt_info;
    }

  return;
}


// Returns the set of rt_infos, with their assigned priorities (as
// of the last schedule re-computation).

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
get_dependency_set (RtecScheduler::Dependency_Set_out dependencies
                    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INTERNAL))
{
  // Return the set of dependencies: just need to iterate over one of the maps.

  if (dependencies.ptr () == 0)
    {
      dependencies = new RtecScheduler::Dependency_Set (this->dependency_count_);
    }
  dependencies->length (this->dependency_count_);
  RtecScheduler::Dependency_Set *dependency_set = 0;
  int i = 0;
  for (ACE_TYPENAME DEPENDENCY_SET_MAP::iterator
         dependency_iter (this->called_dependency_set_map_);
       dependency_iter.done () == 0 && i < this->dependency_count_;
       ++dependency_iter)
    {
      dependency_set = (*dependency_iter).int_id_;
      for (u_int j = 0;
           j < dependency_set->length () && i < this->dependency_count_;
           ++i, ++j)
        {
          (* dependencies) [i] =  (*dependency_set) [j];
          // For two-way calls, swap the handles (stored in reverse order in the called map)
          if ((* dependencies) [i].dependency_type == RtecBase::TWO_WAY_CALL)
            {
              (* dependencies) [i].rt_info =  (* dependency_set) [j].rt_info_depended_on;
              (* dependencies) [i].rt_info_depended_on =  (* dependency_set) [j].rt_info;
            }
        }
    }

  return;
}


// Returns the set of config_infos, describing the appropriate
// number, types, and priority levels for the dispatching lanes.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
get_config_info_set (RtecScheduler::Config_Info_Set_out configs
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                     RtecScheduler::INTERNAL))
{
  // Return the set of scheduled Config_Infos.

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
      configs[static_cast<CORBA::ULong> (config_info->preemption_priority)] = *config_info;
    }

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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::dispatch_configuration.\n"));
#endif /* SCHEDULER_LOGGING */

  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Check stability flags
  if ((this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
      && this->enforce_schedule_stability_)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ());
    }

  RtecScheduler::Config_Info *config_info = 0;
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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::last_scheduled_priority.\n"));
#endif /* SCHEDULER_LOGGING */

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

// Provides the set of Config_Infos associated with the current schedule.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
get_config_infos (RtecScheduler::Config_Info_Set_out configs
		  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   RtecScheduler::SYNCHRONIZATION_FAILURE,
		   RtecScheduler::NOT_SCHEDULED))
{
  ACE_GUARD_THROW_EX (ACE_LOCK, ace_mon, this->mutex_,
                      RtecScheduler::SYNCHRONIZATION_FAILURE ());
  ACE_CHECK;

  // Check schedule stability flags.
  if ((this->stability_flags_ & SCHED_PRIORITY_NOT_STABLE)
      && this->enforce_schedule_stability_)
    {
      ACE_THROW (RtecScheduler::NOT_SCHEDULED ());
    }

  // return the set of Config_Infos
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
      configs[static_cast<CORBA::ULong> (config_info->preemption_priority)] = *config_info;
    }
}


// Internal method to create an RT_Info.  If it does not exist, a new
// RT_Info is created and inserted into the schedule, and the handle
// of the new RT_Info is returned.  If the RT_Info already exists,
// then if the ignore_duplicates flag is set, the handle is simply
// returned; otherwise, an exception is thrown.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RT_Info_Ex *
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
create_i (const char *entry_point,
          RtecScheduler::handle_t handle,
          int ignore_duplicates
          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::DUPLICATE_NAME,
                     RtecScheduler::INTERNAL))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::create_i.\n"));
#endif /* SCHEDULER_LOGGING */

  TAO_RT_Info_Ex* new_rt_info = 0;
  TAO_Reconfig_Scheduler_Entry* new_sched_entry = 0;
  int result = 0;

  // If we're ignoring duplicates, check for and return the existing
  // entry if there is one.
  if (ignore_duplicates
      && rt_info_map_.find (handle, new_rt_info) == 0)
    {
      return new_rt_info;
    }

  // Create a new scheduling entry for the RT_Info.
  ACE_NEW_THROW_EX (new_rt_info,
                    TAO_RT_Info_Ex,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // Make sure the new scheduling entry is cleaned up if we exit abruptly.
  auto_ptr<TAO_RT_Info_Ex> new_rt_info_ptr (new_rt_info);

  // Set some reasonable default values, and store the passed ones.
  new_rt_info->entry_point = CORBA::string_dup (entry_point);
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
        if (ignore_duplicates)
          {
            // Should never get here unless something is badly awry.
            ACE_THROW_RETURN (RtecScheduler::INTERNAL (), 0);
          }
        else
          {
            // Already bound, and we're not ignoring duplicates.
        ACE_THROW_RETURN (RtecScheduler::DUPLICATE_NAME (), 0);
          }

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

  // Maintain the size of the entry pointer array.
  maintain_scheduling_array (entry_ptr_array_, entry_ptr_array_size_,
                             handle ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);


  // Store the new entry in the scheduling entry pointer array.
  entry_ptr_array_ [handle - 1] = new_sched_entry;

  // Release the auto pointers, so their destruction does not
  // remove the new rt_info that is now in the map and tree,
  // or the scheduling entry attached to the rt_info.
  new_rt_info_ptr.release ();
  new_sched_entry_ptr.release ();

  // Connect the entry to the RT_Info.
  new_rt_info->volatile_token =
    static_cast<CORBA::ULongLong> (reinterpret_cast<ptrdiff_t> (new_sched_entry));

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
set_i (TAO_RT_Info_Ex *rt_info,
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
                     RtecScheduler::INTERNAL))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set_i.\n"));
#endif /* SCHEDULER_LOGGING */

  // Do not allow conjunction nodes for now.
  if (info_type == RtecScheduler::CONJUNCTION)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Conjunction Nodes are not supported currently.")));
      ACE_THROW (RtecScheduler::INTERNAL ());
    }


  // Set the base RT_Info to have the attributes of the latest values.
  rt_info->criticality = criticality;
  rt_info->worst_case_execution_time = time;
  rt_info->typical_execution_time = typical_time;
  rt_info->cached_execution_time = cached_time;
  rt_info->period = period;
  rt_info->importance = importance;
  rt_info->quantum = quantum;
  rt_info->threads = threads;
  rt_info->info_type = info_type;

  // If a rate is advertised, create a separate tuple for that rate.
  if (period > 0)
    {
      TAO_Reconfig_Scheduler_Entry * rse_ptr =
        ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                             rt_info->volatile_token);
//      ACE_DEBUG((LM_DEBUG, "Updating or inserting tuple for RT_Info: %d, entry_ptr: %x\n", rt_info->handle, rse_ptr));
      if (rse_ptr == 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }

      int result = rse_ptr->update_tuple (*rt_info);
      if (result < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
      else if (result == 0)  // We did not find an existing tuple.
        {
          // Create a new RT_Info tuple from the passed RT_Info.
          TAO_RT_Info_Tuple *tuple_ptr = 0;
          ACE_NEW_THROW_EX (tuple_ptr,
                            TAO_RT_Info_Tuple (*rt_info),
                            CORBA::NO_MEMORY ());
          ACE_CHECK;


          // Make sure the new tuple is cleaned up if we exit abruptly.
          auto_ptr<TAO_RT_Info_Tuple> tuple_auto_ptr (tuple_ptr);

//          ACE_DEBUG((LM_DEBUG, "Tuple not found.  Inserting new tuple for RT_Info: %d, entry_ptr: 0x%x, tuple_ptr: 0x%x\n",
//                     rt_info->handle,
//                     rse_ptr,
//                     tuple_ptr));
          // Add the tuple to the entry's original tuple set
          result = rse_ptr->insert_tuple (*tuple_ptr);
          if (result < 0)
            {
              ACE_THROW (RtecScheduler::INTERNAL ());
            }

          // Maintain the size of the entry pointer array.
          maintain_scheduling_array (tuple_ptr_array_,
                                     tuple_ptr_array_size_,
                                     rt_info_tuple_count_
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Store the new tuple in the tuple pointer array.
          tuple_ptr_array_ [this->rt_info_tuple_count_] = tuple_ptr;

          ++this->rt_info_tuple_count_;

          // All is well: release the auto pointer's hold on the tuple.
          tuple_auto_ptr.release ();
        }
    }
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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::lookup_i.\n"));
#endif /* SCHEDULER_LOGGING */

  TAO_RT_Info_Ex *rt_info = 0;
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

  TAO_RT_Info_Ex *rt_info = 0;
  if (rt_info_map_.find (handle, rt_info) != 0)
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }

  o_priority = rt_info->priority;
  subpriority = rt_info->preemption_subpriority;
  p_priority = rt_info->preemption_priority;
}


// This internal method registers a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
add_dependency_i (RtecScheduler::handle_t handle /* RT_Info that has the dependency */,
                  RtecScheduler::handle_t dependency /* RT_Info on which it depends */,
                  CORBA::Long number_of_calls,
                  RtecScheduler::Dependency_Type_t dependency_type,
                  RtecScheduler::Dependency_Enabled_Type_t enabled
                  ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::add_dependency_i.\n"));
#endif /* SCHEDULER_LOGGING */

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
                        dependency_type,
                        enabled
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Add the called dependency map entry
      map_dependency_i (dependency,                    // called handle
                        handle,                        // calling handle
                        called_dependency_set_map_,    // called map
                        number_of_calls,
                        dependency_type,
                        enabled
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
                        dependency_type,
                        enabled
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Add the called dependency map entry
      map_dependency_i (handle,                        // called handle
                        dependency,                    // calling handle
                        called_dependency_set_map_,    // called map
                        number_of_calls,
                        dependency_type,
                        enabled
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    default:

      // There should not be any other kinds of dependencies.
      ACE_THROW (RtecScheduler::INTERNAL ());
  }

 // Add the criticality dependency map entry.
 // If A---TW--->B and C---OW--->D, the add_dependency_calls
 // would look like this
 // add_dependency (A, B, TW)
 // add_dependency (D, C, OW)
 // Neither of the other two maps capture these dependencies
 // A depends on B and D depends on C.
 // The calling dependency map captures
 // A calls B and C calls D.
 // The called dependency map captures
 // B called by A and D called by C.

 map_dependency_i (handle,                    // calling handle
                   dependency,                // called handle
                   crit_dependency_set_map_,// crit dependency map
                   number_of_calls,
                   dependency_type,
                   enabled
                   ACE_ENV_ARG_PARAMETER);
 ACE_CHECK;

}


// This internal method removes a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
remove_dependency_i (RtecScheduler::handle_t handle /* RT_Info that has the dependency */,
                     RtecScheduler::handle_t dependency /* RT_Info on which it depends */,
                     CORBA::Long number_of_calls,
                     RtecScheduler::Dependency_Type_t dependency_type
                     ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::remove_dependency_i.\n"));
#endif /* SCHEDULER_LOGGING */

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

      // Remove the calling dependency map entry
      unmap_dependency_i (handle,                        // calling handle
                          dependency,                    // called handle
                          calling_dependency_set_map_,   // calling map
                          number_of_calls,
                          dependency_type
                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Remove the called dependency map entry
      unmap_dependency_i (dependency,                    // called handle
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

      // Remove the calling dependency map entry
      unmap_dependency_i (dependency,                    // calling handle
                          handle,                        // called handle
                          calling_dependency_set_map_,   // calling map
                          number_of_calls,
                          dependency_type
                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Remove the called dependency map entry
      unmap_dependency_i (handle,                        // called handle
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


// This method sets the enable state for a dependency between two RT_Infos.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
set_dependency_enable_state_i (RtecScheduler::handle_t handle,
                               RtecScheduler::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type,
                               RtecScheduler::Dependency_Enabled_Type_t enabled
                               ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::SYNCHRONIZATION_FAILURE,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::set_dependency_enable_state_i.\n"));
#endif /* SCHEDULER_LOGGING */

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

      // Update the calling dependency map entry
      map_dependency_enable_state_i (handle,                        // calling handle
                                     dependency,                    // called handle
                                     calling_dependency_set_map_,   // calling map
                                     number_of_calls,
                                     dependency_type,
                                     enabled
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Update the called dependency map entry
      map_dependency_enable_state_i (dependency,                    // called handle
                                     handle,                        // calling handle
                                     called_dependency_set_map_,    // called map
                                     number_of_calls,
                                     dependency_type,
                                     enabled
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      break;

    // In a one-way call, the called operation depends on the
    // calling operation.
    case RtecBase::ONE_WAY_CALL:

      // Update the calling dependency map entry
      map_dependency_enable_state_i (dependency,                    // calling handle
                                     handle,                        // called handle
                                     calling_dependency_set_map_,   // calling map
                                     number_of_calls,
                                     dependency_type,
                                     enabled
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Update the called dependency map entry
      map_dependency_enable_state_i (handle,                        // called handle
                                     dependency,                    // calling handle
                                     called_dependency_set_map_,    // called map
                                     number_of_calls,
                                     dependency_type,
                                     enabled
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
                  ACE_TYPENAME TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &dependency_map,
                  CORBA::Long number_of_calls,
                  RtecScheduler::Dependency_Type_t dependency_type,
                  RtecScheduler::Dependency_Enabled_Type_t enabled
                  ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::map_dependency_i.\n"));
#endif /* SCHEDULER_LOGGING */

  RtecScheduler::Dependency_Set *dependency_set;

  // Look up the dependency set in the passed map
  if (dependency_map.find (key, dependency_set) != 0)
    {
      // Create a new one
      ACE_NEW_THROW_EX (dependency_set,
                        RtecScheduler::Dependency_Set,
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      if (dependency_map.bind (key, dependency_set) != 0)
        {
          delete dependency_set;
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Insert unconditionally: there can be multiple copies
  // of the same dependency, if the user so chooses.
  int prev_length =  dependency_set->length ();
  dependency_set->length (prev_length + 1);
  (*dependency_set) [prev_length].rt_info = handle;
  (*dependency_set) [prev_length].rt_info_depended_on = key;  // may actually be the other way around
  (*dependency_set) [prev_length].number_of_calls = number_of_calls;
  (*dependency_set) [prev_length].dependency_type = dependency_type;
  (*dependency_set) [prev_length].enabled = enabled;
}


// This method removes a dependency from a dependency set map.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unmap_dependency_i (RtecScheduler::handle_t key,
                    RtecScheduler::handle_t handle,
                    ACE_TYPENAME TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &dependency_map,
                    CORBA::Long number_of_calls,
                    RtecScheduler::Dependency_Type_t dependency_type
                    ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::unmap_dependency_i.\n"));
#endif /* SCHEDULER_LOGGING */

  RtecScheduler::Dependency_Set *dependency_set = 0;

  // Try to unbind the matching dependency set from the passed map
  // and if successful, destroy the
  if (dependency_map.find (key, dependency_set) == 0)
    {
      if (dependency_set)
        {
          int prev_length =  dependency_set->length ();
          int found = 0;
          for (int i = 0; i < prev_length; ++i)
            {
              if ((*dependency_set) [i].rt_info == handle
                  &&  (*dependency_set) [i].number_of_calls == number_of_calls
                  &&  (*dependency_set) [i].dependency_type == dependency_type)
                {
                  // we found the dependency to delete: set our
                  // found flag to true and compact the array
                  found = 1;
                  for (int j = i+1; j < prev_length; ++j)
                    {
                      (*dependency_set) [j - 1].rt_info = (*dependency_set) [j].rt_info;
                      (*dependency_set) [j - 1].number_of_calls = (*dependency_set) [j].number_of_calls;
                      (*dependency_set) [j - 1].dependency_type = (*dependency_set) [j].dependency_type;
                      (*dependency_set) [j - 1].enabled = (*dependency_set) [j].enabled;
                    }
                  dependency_set->length (prev_length - 1);
                  break;
                }
            }

          if (!found)
            {
              ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
            }
        }
      else
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }
  else
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }
}

// Internal method that enables or disables a dependency between two RT_Infos.
// Assumes it is being called with all locks held, and does *not*
// set any schedule stability flags.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
map_dependency_enable_state_i (RtecScheduler::handle_t key,
                               RtecScheduler::handle_t handle,
                               ACE_TYPENAME
                                 TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &dependency_map,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type,
                               RtecScheduler::Dependency_Enabled_Type_t enabled
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::INTERNAL,
                     RtecScheduler::UNKNOWN_TASK))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::map_dependency_enable_state_i.\n"));
#endif /* SCHEDULER_LOGGING */

  RtecScheduler::Dependency_Set *dependency_set = 0;

  // Try to unbind the matching dependency set from the passed map
  // and if successful, destroy the
  if (dependency_map.find (key, dependency_set) == 0)
    {
      if (dependency_set)
        {
          int set_length =  dependency_set->length ();
          int found = 0;
          for (int i = 0; i < set_length; ++i)
            {
              if ((*dependency_set) [i].rt_info == handle
                  &&  (*dependency_set) [i].number_of_calls == number_of_calls
                  &&  (*dependency_set) [i].dependency_type == dependency_type)
                {
                  // we found the dependency to update
                  found = 1;
                  (*dependency_set) [i].enabled = enabled;
                  break;
                }
            }

          if (!found)
            {
              ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
            }
        }
      else
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }
  else
    {
      ACE_THROW (RtecScheduler::UNKNOWN_TASK ());
    }
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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::dfs_traverse_i.\n"));
#endif /* SCHEDULER_LOGGING */

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
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::detect_cycles_i.\n"));
#endif /* SCHEDULER_LOGGING */

  // Sort the pointers to entries in order of descending forward
  // finish times, which produces a reverse topological ordering,
  // with callers ahead of called nodes.
  ACE_OS::qsort (reinterpret_cast<void *> (entry_ptr_array_),
                 this->rt_info_count_,
                 sizeof (TAO_Reconfig_Scheduler_Entry *),
                 reinterpret_cast<COMP_FUNC> (RECONFIG_SCHED_STRATEGY::comp_entry_finish_times));

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


// Propagates aggregate execution times, then performs admission over
// rate tuples.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
perform_admission_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                      CORBA::SystemException,
                      RtecScheduler::INTERNAL))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::perform_admission_i.\n"));
#endif /* SCHEDULER_LOGGING */

  // Traverse entries in topological (ascending forward DFS
  // finish time) order, propagating aggregate execution
  // time from called nodes to calling node at each step.

  TAO_RSE_Reverse_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
    prop_visitor (this->calling_dependency_set_map_,
                  this->rt_info_map_);
  int i;
  for (i = this->rt_info_count_ - 1; i >= 0; --i)
    {
      if (prop_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }

  // Sort the pointers to original tuples in ascending admission
  // order, according to the scheduling strategy's admission policy.
  ACE_OS::qsort (reinterpret_cast<void *> (tuple_ptr_array_),
           this->rt_info_tuple_count_,
           sizeof (TAO_RT_Info_Tuple *),
           reinterpret_cast<COMP_FUNC> (RECONFIG_SCHED_STRATEGY::total_admission_comp));

  // Traverse tuples in admission order, updating the associate tuple
  // for each thread delineator.
#if defined (SCHEDULER_LOGGING)
  ACE_Scheduler_Factory::log_scheduling_tuples(tuple_ptr_array_,
                                               this->rt_info_tuple_count_,
                                               "sorted_admit_tuples.txt");
#endif

  TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>
    admit_visitor (critical_utilization_threshold_,
                   noncritical_utilization_threshold_);

  for (i = 0; i < this->rt_info_tuple_count_; ++i)
    {
      if (admit_visitor.visit (* (tuple_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }


  // Store the values accumulated by the visitor.
  this->noncritical_utilization_ =
    admit_visitor.total_noncritical_utilization ();
  this->critical_utilization_ =
    admit_visitor.total_critical_utilization ();
}

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
crit_dfs_traverse_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::crit_dfs_traverse_i.\n"));
#endif /* SCHEDULER_LOGGING */

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
    dfs_visitor (this->crit_dependency_set_map_,
                 this->rt_info_map_);
  for (i = 0; i < this->rt_info_count_; ++i)
    {
      if (dfs_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }
}

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
propagate_criticalities_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::propagate_criticalities_i.\n"));
#endif /* SCHEDULER_LOGGING */

  // Sort the pointers to original tuples in ascending admission
  // order, according to the scheduling strategy's admission policy.

  ACE_OS::qsort (reinterpret_cast<void *> (tuple_ptr_array_),
                 this->rt_info_tuple_count_,
                 sizeof (TAO_RT_Info_Tuple *),
                 reinterpret_cast<COMP_FUNC> (RECONFIG_SCHED_STRATEGY::comp_tuple_finish_times ));

  // Traverse entries in topological (ascending forward DFS
  // finish time) order, propagating aggregate execution
  // time from called nodes to calling node at each step.

  TAO_RSE_Criticality_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
    crit_prop_visitor (this->crit_dependency_set_map_,
                  this->rt_info_map_);
  int i;
  for (i = 0; i<this->rt_info_count_; ++i)
    {
      if (crit_prop_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }
  for (i = this->rt_info_count_-1; i>=0; --i)
    {
      if (crit_prop_visitor.visit (* (entry_ptr_array_ [i])) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }
}

// Propagates periods, sets total frame size.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
propagate_characteristics_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL,
                      RtecScheduler::UNRESOLVED_LOCAL_DEPENDENCIES,
                      RtecScheduler::THREAD_SPECIFICATION))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::propagate_characteristics_i.\n"));
#endif /* SCHEDULER_LOGGING */

  // Traverse entries in reverse topological (descending forward DFS
  // finish time) order, propagating period and effective execution
  // time from calling node to called node at each step.
  TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
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
  int i;
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::assign_priorities_i.\n"));

  ACE_DEBUG ((LM_DEBUG, "Scheduler::entry ptr array before sorting is\n"));
  for (i = 0; i < this->rt_info_count_; ++i)
    {
      RtecScheduler::RT_Info* rt_info_ptr =
        this->entry_ptr_array_[i]->actual_rt_info ();
      ACE_DEBUG ((LM_DEBUG,
                  " %s [%d] crit=%d,prio=%d,preemption_prio=%d,subprio=%d\n ",
                  rt_info_ptr->entry_point.in (),
                  rt_info_ptr->handle,
                  rt_info_ptr->criticality,
                  rt_info_ptr->priority,
                  rt_info_ptr->preemption_priority,
                  rt_info_ptr->preemption_subpriority));
    }
#endif /* SCHEDULER_LOGGING */

  // Sort the pointers to entries in descending order
  // of static priority and static subpriority, according
  // to our given scheduling strategy.
  ACE_OS::qsort (reinterpret_cast<void *> (entry_ptr_array_),
                 this->rt_info_count_,
                 sizeof (TAO_Reconfig_Scheduler_Entry *),
                 reinterpret_cast<COMP_FUNC> (RECONFIG_SCHED_STRATEGY::total_priority_comp));

#ifdef SCHEDULER_LOGGING
  ACE_DEBUG ((LM_DEBUG, "Scheduler::qsorted array is\n"));
  for (i = 0; i < this->rt_info_count_; ++i)
    {
      RtecScheduler::RT_Info* rt_info_ptr =
        this->entry_ptr_array_[i]->actual_rt_info ();
      ACE_DEBUG ((LM_DEBUG,
                  " %s [%d] crit=%d,prio=%d,preemption_prio=%d,subprio=%d\n ",
                  rt_info_ptr->entry_point.in (),
                  rt_info_ptr->handle,
                  rt_info_ptr->criticality,
                  rt_info_ptr->priority,
                  rt_info_ptr->preemption_priority,
                  rt_info_ptr->preemption_subpriority));
    }
#endif

  // Empty out the previously stored configuration infos, if any.
  RtecScheduler::Preemption_Priority_t config_priority;
  RtecScheduler::Config_Info *config_info_temp = 0;
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
  this->config_info_count_ = 0;

  // Traverse using a priority assignment visitor, which uses a
  // strategy to decide when a new priority or subpriority is reached.
  TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
    prio_visitor (this->rt_info_count_, this->entry_ptr_array_);
  auto_ptr<RtecScheduler::Config_Info> new_config_info_ptr;
  for (i = 0; i <= this->rt_info_count_; ++i)
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
              this->last_scheduled_priority_)
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


// Refreshes the array of tuple pointers, tuple pointer count.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
refresh_tuple_ptr_array_i (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RtecScheduler::INTERNAL))
{
#if defined (SCHEDULER_LOGGING)
  ACE_DEBUG ((LM_TRACE,
              " TAO_Reconfig_Scheduler::refresh_tuple_ptr_array_i.\n"));
#endif /* SCHEDULER_LOGGING */

  // Zero out the tuple pointer array, set count to zero
  ACE_OS::memset (this->tuple_ptr_array_, 0,
                  sizeof (TAO_RT_Info_Tuple *)
                  * this->tuple_ptr_array_size_);
  this->rt_info_tuple_count_ = 0;

  for (int i = 0; i < this->rt_info_count_; ++i)
    {
      if (entry_ptr_array_ [i]->register_tuples (this->tuple_ptr_array_,
                                                 this->rt_info_tuple_count_) < 0)
        {
          ACE_THROW (RtecScheduler::INTERNAL ());
        }
    }
}

// Accesses scheduling strategy for the reconfig scheduler.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> RECONFIG_SCHED_STRATEGY &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::sched_strategy ()
{
  return this->sched_strategy_;
}


// Accesses map for O(1) lookup of Config_Infos by priority level.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_TYPENAME TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::CONFIG_INFO_MAP &
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
ACE_TYPENAME TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP &
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

// Returns the number of registered RT_Info tuples.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> long
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::rt_info_tuple_count ()
{
  return this->rt_info_tuple_count_;
}


// Accesses tree for O(log n) lookup of RT_Infos by name.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_TYPENAME TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_TREE &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::rt_info_tree ()
{
  return this->rt_info_tree_;
}


// Accesses map for O(1) lookup of RT_Info dependency
// set by the caller operation's handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_TYPENAME TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::calling_dependency_set_map ()
{
  return this->calling_dependency_set_map_;
}


// Accesses map for O(1) lookup of RT_Info dependency
// set by the called operation's handle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
ACE_TYPENAME TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &
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

// Accessor for utilization by noncritical tasks.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
CORBA::Double
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
noncritical_utilization ()
{
  return noncritical_utilization_;
}

// Accessor for utilization by critical tasks.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
CORBA::Double
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
critical_utilization ()
{
  return critical_utilization_;
}

// Accessor for noncritical task utilization threshold.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
CORBA::Double
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
noncritical_utilization_threshold ()
{
  return noncritical_utilization_threshold_;
}

// Mutator for noncritical task utilization threshold.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
noncritical_utilization_threshold (const CORBA::Double &d)
{
  noncritical_utilization_threshold_ = d;
}

// Accessor for critical task utilization threshold.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
CORBA::Double
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
critical_utilization_threshold ()
{
  return critical_utilization_threshold_;
}

// Mutator for critical task utilization threshold.
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
void
TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
critical_utilization_threshold (const CORBA::Double &d)
{
  critical_utilization_threshold_ = d;
}


// Helper function: makes sure there is room in the scheduling pointer
// arrays.  This function expands the array eagerly, to minimize
// memory allocation overhead.

template <class ARRAY_ELEMENT_TYPE> void
maintain_scheduling_array (ARRAY_ELEMENT_TYPE ** & current_ptr_array,
                           long & current_ptr_array_size,
                           RtecScheduler::handle_t handle
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (current_ptr_array_size <= handle)
    {
      long new_size = handle + 1;
      ARRAY_ELEMENT_TYPE ** new_array;

      if (current_ptr_array_size > 0)
        {
          // Store previous array size.
          for (new_size = 2 * current_ptr_array_size;
               new_size <= handle;
               new_size *= 2);
        }

      // Allocate the new array of the proper size, zero it out.

      ACE_NEW_THROW_EX (new_array,
                ARRAY_ELEMENT_TYPE * [new_size],
                CORBA::NO_MEMORY ());

      ACE_OS::memset (new_array, 0,
                      sizeof (ARRAY_ELEMENT_TYPE *) *
                      new_size);

      if (current_ptr_array_size > 0)
        {
          // Copy in the previous array.
          ACE_OS::memcpy (new_array, current_ptr_array,
                          sizeof (ARRAY_ELEMENT_TYPE *) *
                            current_ptr_array_size);

          // Free the old array and swap to point to the new one.
          delete [] current_ptr_array;
        }

      current_ptr_array = new_array;
      current_ptr_array_size = new_size;
    }
}

/* WSOA merge - commented out
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

*/

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_RECONFIG_SCHEDULER_T_C */
