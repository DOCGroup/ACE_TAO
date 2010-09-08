//=============================================================================
/**
 *  @file    DynSched.cpp
 *
 *  $Id$
 *
 *  $Id$
 *
 *  @author Chris Gill
 */
//=============================================================================


#include "orbsvcs/Sched/DynSched.h"
#include "ace/Basic_Types.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Null_Mutex.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Sched/DynSched.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//////////////////////
// Helper functions //
//////////////////////

// compare the DFS finish times of two task entries, order higher time *first*
#if defined (ACE_HAS_WINCE)
int _cdecl compare_entry_finish_times (const void *first, const void *second)
#else
extern "C" int compare_entry_finish_times (const void *first, const void *second)
#endif  // ACE_HAS_WINCE
{
  // sort blank entries to the end
  if (! first)
  {
    return (second) ? 1 : 0;
  }
  else if (! second)
  {
    return -1;
  }

  const Task_Entry *first_entry =
          * static_cast<const Task_Entry *const *> (first);
  const Task_Entry *second_entry =
          * static_cast<const Task_Entry *const *> (second);

  // sort blank entries to the end
  if (! first_entry)
  {
    return (second_entry) ? 1 : 0;
  }
  else if (! second_entry)
  {
    return -1;
  }

  if (first_entry->finished () > second_entry->finished ())
  {
    return -1;
  }
  else if (first_entry->finished () < second_entry->finished ())
  {
    return 1;
  }

  return 0;
}

//////////////////////////////////////////
// class ACE_DynScheduler member functions //
//////////////////////////////////////////

const char *
ACE_DynScheduler::status_message (ACE_DynScheduler::status_t status)
{
  switch (status)
  {
    case NOT_SCHEDULED :
      return "NOT_SCHEDULED";
    case SUCCEEDED :
      return "SUCCEEDED";
    case ST_NO_TASKS_REGISTERED :
      return "ST_NO_TASKS_REGISTERED";
    case ST_TASK_ALREADY_REGISTERED :
      return "TASK_ALREADY_REGISTERED";
    case ST_BAD_INTERNAL_POINTER :
      return "BAD_INTERNAL_POINTER";
    case ST_VIRTUAL_MEMORY_EXHAUSTED :
      return "VIRTUAL_MEMORY_EXHAUSTED";
    case ST_UNKNOWN_TASK :
      return "UNKNOWN_TASK";
    case TASK_COUNT_MISMATCH :
      return "TASK_COUNT_MISMATCH";
    case THREAD_COUNT_MISMATCH :
      return "THREAD_COUNT_MISMATCH";
    case INVALID_PRIORITY :
      return "INVALID_PRIORITY";
    case TWO_WAY_DISJUNCTION :
      return "TWO_WAY_DISJUNCTION (IGNORED)";
    case TWO_WAY_CONJUNCTION :
      return "TWO_WAY_CONJUNCTION (IGNORED)";
    case UNRECOGNIZED_INFO_TYPE :
      return "UNRECOGNIZED_INFO_TYPE (IGNORED)";

    // The following are only used during scheduling (in the case of
    // off-line scheduling, they are only used prior to runtime).
    // To save a little code space (280 bytes on g++ 2.7.2/Solaris 2.5.1),
    // we could conditionally compile them so that they're not in the
    // runtime version.
    case ST_UTILIZATION_BOUND_EXCEEDED :
      return "UTILIZATION_BOUND_EXCEEDED";
    case ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS :
      return "INSUFFICIENT_THREAD_PRIORITY_LEVELS";
    case ST_CYCLE_IN_DEPENDENCIES :
      return "CYCLE_IN_DEPENDENCIES";
    case ST_UNRESOLVED_REMOTE_DEPENDENCIES :
      return "ST_UNRESOLVED_REMOTE_DEPENDENCIES";
    case ST_UNRESOLVED_LOCAL_DEPENDENCIES :
      return "ST_UNRESOLVED_LOCAL_DEPENDENCIES";
    case ST_INVALID_PRIORITY_ORDERING :
      return "INVALID_PRIORITY_ORDERING";
    case UNABLE_TO_OPEN_SCHEDULE_FILE :
      return "UNABLE_TO_OPEN_SCHEDULE_FILE";
    case UNABLE_TO_WRITE_SCHEDULE_FILE :
      return "UNABLE_TO_WRITE_SCHEDULE_FILE";
    // End of config-only status values.

    default:
      break;
  }

  return "UNKNOWN STATUS";
}

// = Utility function for creating an entry for determining
//   the severity of an anomaly detected during scheduling.
ACE_DynScheduler::Anomaly_Severity
ACE_DynScheduler::anomaly_severity (ACE_DynScheduler::status_t status)
{
  // Determine severity of the anomaly
  switch (status)
  {
    // Fatal anomalies reflect unrecoverable internal scheduler errors
    case ST_BAD_INTERNAL_POINTER :
    case ST_VIRTUAL_MEMORY_EXHAUSTED :
    case THREAD_COUNT_MISMATCH :
    case TASK_COUNT_MISMATCH :
      return RtecScheduler::ANOMALY_FATAL;

    // Errors reflect severe problems with given scheduling information
    case UNABLE_TO_OPEN_SCHEDULE_FILE :
    case UNABLE_TO_WRITE_SCHEDULE_FILE :
    case NOT_SCHEDULED :
    case ST_UNRESOLVED_LOCAL_DEPENDENCIES :
    case ST_UNKNOWN_TASK :
    case ST_CYCLE_IN_DEPENDENCIES :
    case ST_INVALID_PRIORITY_ORDERING :
      return RtecScheduler::ANOMALY_ERROR;

    // Warnings reflect serious problems with given scheduling information
    case ST_TASK_ALREADY_REGISTERED :
    case ST_UNRESOLVED_REMOTE_DEPENDENCIES :
    case ST_UTILIZATION_BOUND_EXCEEDED :
    case ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS :
    case TWO_WAY_DISJUNCTION :
    case TWO_WAY_CONJUNCTION :
    case UNRECOGNIZED_INFO_TYPE :
    case ST_NO_TASKS_REGISTERED :
      return RtecScheduler::ANOMALY_WARNING;

    // Produce a lowest severity anomaly for any unknown status value
    default:
      return RtecScheduler::ANOMALY_NONE;
  }
}


// = Utility function for creating an entry for the
//   log of anomalies detected during scheduling.
ACE_DynScheduler::Scheduling_Anomaly *
ACE_DynScheduler::create_anomaly (ACE_DynScheduler::status_t status)
{
  ACE_DynScheduler::Scheduling_Anomaly * anomaly;
  ACE_NEW_RETURN (anomaly, ACE_DynScheduler::Scheduling_Anomaly, 0);

  anomaly->severity = anomaly_severity (status);
  anomaly->description = status_message (status);

  return anomaly;
}


ACE_DynScheduler::ACE_DynScheduler ()
  // Set the minimum and maximum priority to those for the current platform.
  // This shouldn't be necessary, but UPSingleProcessorOrb::initialize_reactors
  // creates threads before the Event Channel calls Scheduler::init ().
  : minimum_priority_ (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                       ACE_SCOPE_PROCESS))
  , maximum_priority_ (ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                                       ACE_SCOPE_PROCESS))
  , task_entries_ (0)
  , ordered_task_entries_ (0)
  , thread_delineators_ (0)
  , ordered_thread_dispatch_entries_ (0)
  , dispatch_entries_ (0)
  , config_info_entries_ (0)
  , expanded_dispatches_ (0)
  , ordered_dispatch_entries_ (0)
  , dispatch_entry_count_ (0)
  , threads_ (0)
  , timeline_ (0)
  , lock_ ()
  , rt_info_entries_ ()
  , handles_ (0)
  , runtime_filename_ (0)
  , rt_info_filename_ (0)
  , timeline_filename_ (0)
  , tasks_ (0)
  , status_ (NOT_SCHEDULED)
  , output_level_ (0)
  , frame_size_ (1)
  , critical_set_frame_size_ (0)
  , utilization_ (0.0)
  , critical_set_utilization_ (0.0)
  , minimum_priority_queue_ (0)
  , minimum_guaranteed_priority_queue_ (-1)
  , up_to_date_ (0)
  , min_dispatch_id_ (0)
  , max_dispatch_id_ (0)
{
}


ACE_DynScheduler::~ACE_DynScheduler ()
{
  // release all resources used for the most recent schedule
  reset ();
}


void
ACE_DynScheduler::init (const OS_Priority minimum_priority,
                     const OS_Priority maximum_priority,
                     const char *runtime_filename,
                     const char *rt_info_filename,
                     const char *timeline_filename)
{
  minimum_priority_ = minimum_priority;
  maximum_priority_ = maximum_priority;
  runtime_filename_ = runtime_filename;
  rt_info_filename_ = rt_info_filename;
  timeline_filename_ = timeline_filename;
}
  // = initializes the scheduler.

ACE_DynScheduler::status_t
ACE_DynScheduler::register_task (RT_Info *rt_info, handle_t &handle)
{
  ACE_DynScheduler::status_t ret = ST_UNKNOWN_TASK;

  // check the pointer we were passed
  if (! rt_info)
  {
    handle = 0;
    return ret;
  }

  // try to store the new task's information . . .
  switch (rt_info_entries_.insert (rt_info))
    {
      case 0 : // successfully inserted
        {
          rt_info->handle = (handle = ++handles_);
          ret = SUCCEEDED;

          // zero out the task entry ACT used by the scheduler
          rt_info->volatile_token = 0;

          // make sure the schedule is reset when a new task is registered
          reset ();

          if (output_level () >= 5)
            {
              ACE_OS::printf ("registered task \"%s\" with RT_Info at %p\n",
                              (const char*)(rt_info->entry_point), rt_info);
            }
        }
        break;

      case 1 : // the entry had already been inserted
        handle = 0;
        ret = ST_TASK_ALREADY_REGISTERED;
        break;

      default :
        // case -1 : insert failed, probably because virtual memory exhaused
        handle = 0;
        ret = ST_VIRTUAL_MEMORY_EXHAUSTED;
        break;
    }

  return ret;
}
  // = registers a task.



ACE_DynScheduler::status_t
ACE_DynScheduler::get_rt_info (Object_Name name,
                               RT_Info* &rtinfo)
{
  handle_t handle;

  // This makes a copy.  We can optimize this with our own string
  // class.
  ACE_CString lookup (name);
  // Search the map for the <name>.  If found, return the RT_Info.
  if (info_collection_.find (lookup, rtinfo) >= 0)
    {
      // If we find it, return.
      return SUCCEEDED;
    }
  else
    // Otherwise, make one, bind it, and register it.
    {
      rtinfo = new RT_Info;
      rtinfo->info_type = RtecScheduler::OPERATION;
      rtinfo->entry_point = CORBA::string_dup(name);
      // Bind the rtinfo to the name.
      if (info_collection_.bind (lookup, rtinfo) != 0)
        {
          delete rtinfo;
          rtinfo = 0;
          return FAILED; // Error!
        }
      else
        {
          // Register the task
          status_t result = this->register_task (rtinfo, handle);
          if (result == SUCCEEDED)
            {
              rtinfo->handle = handle;
              return ST_UNKNOWN_TASK; // Didn't find it, but made one!
            }
          else
            {
              rtinfo->handle = 0;
              return FAILED;
            }
        }
    }
}




int ACE_DynScheduler::priority (
  const RtecScheduler::handle_t handle,
  RtecScheduler::OS_Priority &priority,
  RtecScheduler::Preemption_Subpriority_t &subpriority,
  RtecScheduler::Preemption_Priority_t &preemption_prio)
{
  // look up the RT_Info that has the given handle
  RT_Info *rt_info = 0;
  if (lookup_rt_info (handle, rt_info) == SUCCEEDED)
  {
    // copy the priority values from the RT_Info
    priority = rt_info->priority;
    subpriority = rt_info->preemption_subpriority;
    preemption_prio = rt_info->preemption_priority;

    return 0;
  }
  else

  {
    // RT_Info not found: assign default priority values
    priority = minimum_priority_;
    subpriority = ACE_Scheduler_MIN_SUB_PRIORITY;
    preemption_prio = ACE_Scheduler_MAX_PREEMPTION_PRIORITY;

    if (output_level () >= 3)
    {
      ACE_OS::printf ("preemption_prio %d: min %d, pri %d, min_pri %d\n",
                      preemption_prio, minimum_priority_queue (),
                      priority, minimum_priority_);
    }

    return -1;
  }
}
  // "priority" is the OS thread priority that was assigned to the Task that
  // was assigned "handle".  "subpriority" combines the dynamic and static
  // subpriorities of the Task that was assigned handle.  "preemption_prio"
  // is a platform-independent priority queue number, ranging from a
  // highest priority value of 0 to the lowest priority value, which is
  // returned by "minimum_priority_queue ()".  The current and deadline times
  // supplied are used to compute the operation's dynamic subpriority
  // Returns 0 on success, or -1 if an invalid handle was supplied.


int ACE_DynScheduler::number_of_dependencies(RT_Info* rt_info)
{
  return rt_info->dependencies.length();
}

int ACE_DynScheduler::number_of_dependencies(RT_Info& rt_info)
{
  return rt_info.dependencies.length();
}

int ACE_DynScheduler::add_dependency(RT_Info* rt_info,
                                     Dependency_Info& d)
{
  RT_Info *temp_info = 0; // temporary pointer to the caller's RT_Info

  switch (d.dependency_type)
  {
    case RtecBase::TWO_WAY_CALL:

      temp_info = rt_info;
      break;

    case RtecBase::ONE_WAY_CALL:

      // swap the handles and point to the caller instead of the called operation
      if (lookup_rt_info (d.rt_info, temp_info) != SUCCEEDED)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("cannot find %d to add dependency\n"), d.rt_info));
        return -1;
      }

      d.rt_info = rt_info->handle;
      break;

    default:

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("unrecognized dependency type %d for %C\n"),
                  d.dependency_type, rt_info->entry_point.in ()));
      return -1;
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("Sched (%t) adding %s dependency to caller: %C\n"),
              (const ACE_TCHAR *) ((d.dependency_type == RtecBase::TWO_WAY_CALL)
                              ? ACE_TEXT("TWO_WAY") : ACE_TEXT("ONE_WAY")),
              temp_info->entry_point.in ()));

  RtecScheduler::Dependency_Set& set = temp_info->dependencies;
  int l = set.length();
  set.length(l + 1);
  set[l] = d;
  return 0;
}

void ACE_DynScheduler::export_to_file (RT_Info* info, FILE* file)
{
  ACE_DynScheduler::export_to_file (*info, file);
}

void ACE_DynScheduler::export_to_file (RT_Info& info, FILE* file)
{
  (void) ACE_OS::fprintf (file,
                          "%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%u\n"
                          "# begin calls\n%d\n",
                          info.entry_point.in (),
                          info.handle,
                          ACE_U64_TO_U32 (info.worst_case_execution_time),
                          ACE_U64_TO_U32 (info.typical_execution_time),
                          ACE_U64_TO_U32 (info.cached_execution_time),
                          int(info.period),
                          info.criticality,
                          info.importance,
                          ACE_U64_TO_U32 (info.quantum),
                          info.threads,
                          number_of_dependencies(info));

  for (int i = 0; i < number_of_dependencies(info); ++i)
    {
      RT_Info tmp;
      (void) ACE_OS::fprintf (file, "%s, %d\n",
                              (const char*)tmp.entry_point,
                              info.dependencies[i].number_of_calls);

    }

  (void) ACE_OS::fprintf (file, "# end calls\n%d\n%d\n\n",
                          info.priority,
                          info.preemption_subpriority);


}


int
ACE_DynScheduler::dispatch_configuration (const Preemption_Priority & p_priority,
                                          OS_Priority & priority,
                                          Dispatching_Type & d_type)
{
  // look up the stored configuration info for the given priority level
  Config_Info *config_info;
  if (lookup_config_info (p_priority, config_info) != SUCCEEDED)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("Config info for priority %lu could not be found\n"),
                       p_priority),
                      -1);
  }

  priority = config_info->thread_priority;
  d_type = config_info->dispatching_type;

  return 0;
}
  // provide the thread priority and queue type for the given priority level


ACE_DynScheduler::status_t
ACE_DynScheduler::lookup_rt_info (handle_t handle,
                                  RT_Info*& rtinfo)
{
  if (handle < 0 || (size_t) handle > rt_info_entries_.size ())
  {
    return ST_UNKNOWN_TASK;
  }

  RT_Info** entry;
  ACE_Unbounded_Set_Iterator <RT_Info *> i (rt_info_entries_);
  while (i.next (entry) != 0)
    {
      i.advance ();
      RT_Info* info_ptr = *entry;
      if (info_ptr->handle == handle)
        {
          rtinfo = info_ptr;
          return SUCCEEDED;
        }
    }

  return ST_UNKNOWN_TASK;
}
  // obtains an RT_Info based on its "handle".

ACE_DynScheduler::status_t
ACE_DynScheduler::lookup_config_info (Preemption_Priority priority,
                                      Config_Info* &config_info)
{
  if (config_info_entries_ == 0)
  {
    return NOT_SCHEDULED;
  }

  if (priority < 0 || (size_t) priority > config_info_entries_->size ())
  {
    return ST_UNKNOWN_PRIORITY;
  }

  Config_Info** entry;
  ACE_Unbounded_Set_Iterator <Config_Info *> i (*config_info_entries_);
  while (i.next (entry) != 0)
    {
      i.advance ();
      Config_Info* config_ptr = *entry;
      if (config_ptr->preemption_priority == priority)
        {
          config_info = config_ptr;
          return SUCCEEDED;
        }
    }

  return ST_UNKNOWN_PRIORITY;
}
  // Obtains a Config_Info based on its priority.


void
ACE_DynScheduler::reset ()
{
  // if the schedule is up to date, free resources
  // and mark schedule as not being up to date
  if (up_to_date_)
  {
    delete [] task_entries_;
    task_entries_ = 0;

    delete [] ordered_task_entries_;
    ordered_task_entries_ = 0;

    delete thread_delineators_;
    thread_delineators_ = 0;

    delete [] ordered_thread_dispatch_entries_;
    ordered_thread_dispatch_entries_ = 0;

    if (dispatch_entries_)
    {
      // free all the dispatch entries in the list, then the list itself
      ACE_Unbounded_Set_Iterator <Dispatch_Entry *> iter (*dispatch_entries_);
      Dispatch_Entry **entry = 0;
      for (iter.first (); ! iter.done (); iter.advance (), entry = 0)
      {
        if ((iter.next (entry) != 0) && (entry) && (*entry))
        {
          delete (*entry);
        }
      }
      delete dispatch_entries_;
      dispatch_entries_ = 0;
    }

    if (config_info_entries_)
    {
      // free all the config info entries in the list, then the list itself
      ACE_Unbounded_Set_Iterator <Config_Info *> iter (*config_info_entries_);
      Config_Info **entry = 0;
      for (iter.first (); ! iter.done (); iter.advance (), entry = 0)
      {
        if ((iter.next (entry) != 0) && (entry) && (*entry))
        {
          delete (*entry);
        }
      }
      delete config_info_entries_;
      config_info_entries_ = 0;
    }


    if (expanded_dispatches_)
    {
      // free all the dispatch entries in the list, then the list itself
      ACE_Unbounded_Set_Iterator <Dispatch_Entry *> expanded_iter (*expanded_dispatches_);
      Dispatch_Entry **expanded_entry = 0;
      for (expanded_iter.first (); ! expanded_iter.done ();
           expanded_iter.advance (), expanded_entry = 0)
      {
        if ((expanded_iter.next (expanded_entry) != 0) &&
            (expanded_entry) && (*expanded_entry))
        {
          delete (*expanded_entry);
        }
      }
      delete expanded_dispatches_;
      expanded_dispatches_ = 0;
    }

    delete [] ordered_dispatch_entries_;
    ordered_dispatch_entries_ = 0;

    dispatch_entry_count_ = 0;
    threads_ = 0;

    status_ = NOT_SCHEDULED;

    frame_size_ = 1;
    critical_set_frame_size_ = 0;
    utilization_ = 0.0;
    critical_set_utilization_ = 0.0;
    minimum_priority_queue_ = 0;
    minimum_guaranteed_priority_queue_ = -1;

    if (timeline_)
    {
      // iterate over and delete the set of timeline entries
      ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> t_iter (*timeline_);
      TimeLine_Entry_Link *t_entry = 0;
      for (t_iter.first (); ! t_iter.done (); t_iter.advance (), t_entry = 0)
      {
        if ((t_iter.next (t_entry) != 0) && (t_entry))
        {
          delete &(t_entry->entry ());
        }
      }
      delete timeline_;
      timeline_ = 0;
         }

    up_to_date_ = 0;
  }
}

ACE_DynScheduler::status_t
ACE_DynScheduler::schedule (
  ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set)
{
  ACE_GUARD_RETURN (LOCK, ace_mon, lock_, ACE_DynScheduler::FAILED);

  RtecScheduler::Anomaly_Severity severity = RtecScheduler::ANOMALY_NONE;
  RtecScheduler::Anomaly_Severity temp_severity = RtecScheduler::ANOMALY_NONE;
  status_t temp_status = SUCCEEDED;
  Scheduling_Anomaly *anomaly = 0;
  ACE_CString unresolved_locals (""), unresolved_remotes ("");

  if (up_to_date_)
  {
    // do nothing if the RT_Infos have not changed
    // since the last valid schedule was generated
    return SUCCEEDED;
  }
  else
  {
    // save the total number of registered RT_Infos
    tasks (static_cast<u_int> (rt_info_entries_.size ()));
  }

  // set up the task entry data structures
  status_ = setup_task_entries ();
  if (status_ != SUCCEEDED)
    {
      // Create an anomaly, add it to anomaly set
      anomaly = create_anomaly (status_);
      if (anomaly)
        {
          anomaly_set.insert (anomaly);
        }
      else
        {
          return ST_VIRTUAL_MEMORY_EXHAUSTED;
        }

      switch (anomaly->severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            return status_;

          default:
            severity = anomaly->severity;
            break;
        }
    }

  // check for cycles in the dependency graph: as a side effect, leaves
  // the ordered_task_entries_ pointer array sorted in topological order,
  // which is used by propagate_dispatches () to ensure that dispatches
  // are propagated top down in the call graph.
  temp_status = check_dependency_cycles ();
  if (temp_status != SUCCEEDED)
    {
      // Create an anomaly, add it to anomaly set
      anomaly = create_anomaly (temp_status);
      if (anomaly)
        {
          anomaly_set.insert (anomaly);
        }
      else
        {
          return ST_VIRTUAL_MEMORY_EXHAUSTED;
        }

      switch (anomaly->severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            status_ = temp_status;
            return status_;

          case RtecScheduler::ANOMALY_ERROR :
            severity = anomaly->severity;
            status_ = temp_status;
            break;

          case RtecScheduler::ANOMALY_WARNING :
            if (severity == RtecScheduler::ANOMALY_NONE)
              {
                severity = anomaly->severity;
                status_ = temp_status;
              }
            break;

          default:
            break;
        }
    }

  // task entries are related, now threads can be found
  temp_status = identify_threads (unresolved_locals,
                                  unresolved_remotes);
  if (temp_status != SUCCEEDED)
    {
      temp_severity = anomaly_severity (temp_status);
      switch (temp_severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            status_ = temp_status;
            return status_;

          case RtecScheduler::ANOMALY_ERROR :
            severity = temp_severity;
            status_ = temp_status;
            break;

          case RtecScheduler::ANOMALY_WARNING :
            if (severity == RtecScheduler::ANOMALY_NONE)
              {
                severity = temp_severity;
                status_ = temp_status;
              }
            break;

          default:
            break;
        }
    }

  // invoke the internal thread scheduling method of the strategy
  temp_status = schedule_threads (anomaly_set);
  if (temp_status != SUCCEEDED)
    {
      temp_severity = anomaly_severity (temp_status);
      switch (temp_severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            status_ = temp_status;
            return status_;

          case RtecScheduler::ANOMALY_ERROR :
            severity = temp_severity;
            status_ = temp_status;
            break;

          case RtecScheduler::ANOMALY_WARNING :
            if (severity == RtecScheduler::ANOMALY_NONE)
              {
                severity = temp_severity;
                status_ = temp_status;
              }
            break;

          default:
            break;
        }
    }

  // propagate the dispatch information from the
  // threads throughout the call graph
  temp_status = propagate_dispatches (anomaly_set,
                                      unresolved_locals,
                                      unresolved_remotes);
  if (temp_status != SUCCEEDED)
    {
      temp_severity = anomaly_severity (temp_status);
      switch (temp_severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            status_ = temp_status;
            return status_;

          case RtecScheduler::ANOMALY_ERROR :
            severity = temp_severity;
            status_ = temp_status;
            break;

          case RtecScheduler::ANOMALY_WARNING :
            if (severity == RtecScheduler::ANOMALY_NONE)
              {
                severity = temp_severity;
                status_ = temp_status;
              }
            break;

          default:
            break;
        }
    }

    // log anomalies for unresolved local dependencies
    if (unresolved_locals.length () > 0)
      {
        // Create an anomaly, add it to anomaly set
        anomaly = create_anomaly (ST_UNRESOLVED_LOCAL_DEPENDENCIES);
        if (anomaly)
          {
            anomaly_set.insert (anomaly);
          }
        else
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }

        ACE_NEW_RETURN (anomaly, ACE_DynScheduler::Scheduling_Anomaly,
                        ST_VIRTUAL_MEMORY_EXHAUSTED);

        ACE_CString temp_str ("The following entry points have "
                            "unresolved local dependencies:\n");
        temp_str += unresolved_locals;

        anomaly->severity =
          anomaly_severity (ST_UNRESOLVED_LOCAL_DEPENDENCIES);
        anomaly->description = temp_str.c_str ();
        anomaly_set.insert (anomaly);
      }

    // log anomalies for unresolved remote dependencies
    if (unresolved_remotes.length () > 0)
      {
        // Create an anomaly, add it to anomaly set
        anomaly = create_anomaly (ST_UNRESOLVED_REMOTE_DEPENDENCIES);
        if (anomaly)
          {
            anomaly_set.insert (anomaly);
          }
        else
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }

        ACE_NEW_RETURN (anomaly, ACE_DynScheduler::Scheduling_Anomaly,
                        ST_VIRTUAL_MEMORY_EXHAUSTED);

        ACE_CString temp_str ("The following entry points have "
                              "unresolved remote dependencies:\n");
        temp_str += unresolved_remotes;

        anomaly->severity =
          anomaly_severity (ST_UNRESOLVED_REMOTE_DEPENDENCIES);
        anomaly->description = temp_str.c_str ();
        anomaly_set.insert (anomaly);
      }

  // invoke the internal dispatch scheduling method of the strategy
  temp_status = schedule_dispatches (anomaly_set);
  if (temp_status != SUCCEEDED)
    {
      temp_severity = anomaly_severity (temp_status);
      switch (temp_severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            status_ = temp_status;
            return status_;

          case RtecScheduler::ANOMALY_ERROR :
            severity = temp_severity;
            status_ = temp_status;
            break;

          case RtecScheduler::ANOMALY_WARNING :
            if (severity == RtecScheduler::ANOMALY_NONE)
              {
                severity = temp_severity;
                status_ = temp_status;
              }
            break;

          default:
            break;
        }
    }

  // calculate utilization, total frame size, critical set
  temp_status = calculate_utilization_params ();
  if (temp_status != SUCCEEDED)
    {
      // Create an anomaly, add it to anomaly set
      anomaly = create_anomaly (temp_status);
      if (anomaly)
        {
          anomaly_set.insert (anomaly);
        }
      else
        {
          return ST_VIRTUAL_MEMORY_EXHAUSTED;
        }

      switch (anomaly->severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            status_ = temp_status;
            return status_;

          case RtecScheduler::ANOMALY_ERROR :
            severity = anomaly->severity;
            status_ = temp_status;
            break;

          case RtecScheduler::ANOMALY_WARNING :
            if (severity == RtecScheduler::ANOMALY_NONE)
              {
                severity = anomaly->severity;
                status_ = temp_status;
              }
            break;

          default:
            break;
        }
    }

  // calculate utilization, total frame size, critical set
  temp_status = store_assigned_info ();
  if (temp_status != SUCCEEDED)
    {
      // Create an anomaly, add it to anomaly set
      anomaly = create_anomaly (temp_status);
      if (anomaly)
        {
          anomaly_set.insert (anomaly);
        }
      else
        {
          return ST_VIRTUAL_MEMORY_EXHAUSTED;
        }

      switch (anomaly->severity)
        {
          case RtecScheduler::ANOMALY_FATAL :
            status_ = temp_status;
            return status_;

          case RtecScheduler::ANOMALY_ERROR :
            severity = anomaly->severity;
            status_ = temp_status;
            break;

          case RtecScheduler::ANOMALY_WARNING :
            if (severity == RtecScheduler::ANOMALY_NONE)
              {
                severity = anomaly->severity;
                status_ = temp_status;
              }
            break;

          default:
            break;
        }
    }

  // generate, store the timeline to a file if file was given
  if (timeline_filename_ != 0)
    {
      // generate the scheduling timeline over the total frame size
      temp_status = create_timeline ();
      if (temp_status != SUCCEEDED)
        {
          // Create an anomaly, add it to anomaly set
          anomaly = create_anomaly (temp_status);
          if (anomaly)
            {
              anomaly_set.insert (anomaly);
            }
          else
            {
              return ST_VIRTUAL_MEMORY_EXHAUSTED;
            }

          switch (anomaly->severity)
            {
              case RtecScheduler::ANOMALY_FATAL :
                status_ = temp_status;
                return status_;

              case RtecScheduler::ANOMALY_ERROR :
                severity = anomaly->severity;
                status_ = temp_status;
                break;

              case RtecScheduler::ANOMALY_WARNING :
                if (severity == RtecScheduler::ANOMALY_NONE)
                  {
                    severity = anomaly->severity;
                    status_ = temp_status;
                  }
                break;

              default:
                break;
            }
        }


      temp_status = output_timeline (timeline_filename_, 0);
      if (temp_status != SUCCEEDED)
        {
          // Create an anomaly, add it to anomaly set
          anomaly = create_anomaly (temp_status);
          if (anomaly)
            {
              anomaly_set.insert (anomaly);
            }
          else
            {
              return ST_VIRTUAL_MEMORY_EXHAUSTED;
            }

          switch (anomaly->severity)
            {
              case RtecScheduler::ANOMALY_FATAL :
                status_ = temp_status;
                return status_;

              case RtecScheduler::ANOMALY_ERROR :
                severity = anomaly->severity;
                status_ = temp_status;
                break;

              case RtecScheduler::ANOMALY_WARNING :
                if (severity == RtecScheduler::ANOMALY_NONE)
                  {
                    severity = anomaly->severity;
                    status_ = temp_status;
                  }
                break;

              default:
                break;
            }
        }
    }

  // if a valid schedule was generated, mark it as up to date
  switch (status_)
  {
    // These are statuses that indicate a reasonable schedule was generated.
    case SUCCEEDED:
    case ST_TASK_ALREADY_REGISTERED :
    case ST_UNRESOLVED_REMOTE_DEPENDENCIES :
    case ST_UTILIZATION_BOUND_EXCEEDED :
    case ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS :

        // if we made it here, the schedule is done
        up_to_date_ = 1;

      break;

    default:
      break;
  }


  return status_;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::propagate_dispatches (
  ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set,
  ACE_CString & unresolved_locals,
  ACE_CString & unresolved_remotes)
{
  u_long i;
  frame_size_ = 1;
  status_t status = SUCCEEDED;
  Scheduling_Anomaly * anomaly = 0;

  // iterate through the ordered_task_entries_ array in order
  // from highest DFS finishing time to lowest, so that every
  // calling dispatch is accessed before those it calls:
  // the dispatches propagate top down through the call DAG
  for (i = 0; i < tasks (); ++i)
  {
    switch (ordered_task_entries_ [i]->merge_dispatches (*dispatch_entries_,
                                                         unresolved_locals,
                                                         unresolved_remotes))
    {
      case Task_Entry::INTERNAL_ERROR :
        // Create an anomaly, add it to anomaly set
        anomaly = create_anomaly (ST_BAD_INTERNAL_POINTER);
        if (anomaly)
          {
            anomaly_set.insert (anomaly);
          }
        else
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }

        return ST_BAD_INTERNAL_POINTER;

      case Task_Entry::TWO_WAY_DISJUNCTION :
        if (status == SUCCEEDED)
          {
            status = TWO_WAY_DISJUNCTION;
          }
        anomaly = create_anomaly (TWO_WAY_DISJUNCTION);
        if (anomaly)
          {
            anomaly_set.insert (anomaly);
          }
        else
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }
        break;

      case Task_Entry::TWO_WAY_CONJUNCTION :
        if (status == SUCCEEDED)
          {
            status = TWO_WAY_CONJUNCTION;
          }
        anomaly = create_anomaly (TWO_WAY_CONJUNCTION);
        if (anomaly)
          {
            anomaly_set.insert (anomaly);
          }
        else
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }
        break;

      case Task_Entry::UNRECOGNIZED_INFO_TYPE :
        if (status == SUCCEEDED)
          {
            status = UNRECOGNIZED_INFO_TYPE;
          }
        anomaly = create_anomaly (UNRECOGNIZED_INFO_TYPE);
        if (anomaly)
          {
            anomaly_set.insert (anomaly);
          }
        else
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }
        break;

      case Task_Entry::SUCCEEDED :
      default:
        break;
    }

    if (ordered_task_entries_ [i]->effective_period () > 0)
    {
      frame_size_ =
        ACE::minimum_frame_size (frame_size_,
                                 ordered_task_entries_ [i]->
                                   effective_period ());
    }
  }

  return status;
}
// propagate the dispatch information from the
// threads throughout the call graph


ACE_DynScheduler::status_t
ACE_DynScheduler::calculate_utilization_params (void)
{
  critical_set_frame_size_ = 0;
  utilization_ = 0.0;
  critical_set_utilization_ = 0.0;

  minimum_priority_queue_ =
    ordered_dispatch_entries_ [0]->priority ();

  minimum_guaranteed_priority_queue_ = -1;

  // iterate through ordered task entries, calculating frame size, utilization
  for (u_int i = 0; i < dispatch_entry_count_; ++i)
  {
    // if we've just finished examining another priority level
    if (minimum_priority_queue_ != ordered_dispatch_entries_ [i]->priority ())
    {
      // update parameters for the previous priority level
      update_priority_level_params ();

      // update the minimum priority queue
      minimum_priority_queue_ = ordered_dispatch_entries_ [i]->priority ();
    }

    // Only consider computation times of dispatches of
    // OPERATION and REMOTE_DEPENDANT descriptors.
    if (((ordered_dispatch_entries_ [i]->task_entry ().info_type () ==
           RtecScheduler::OPERATION) ||
         (ordered_dispatch_entries_ [i]->task_entry ().info_type () ==
           RtecScheduler::REMOTE_DEPENDANT)) &&
        (ordered_dispatch_entries_ [i]->task_entry ().effective_period () > 0))
    {
      utilization_ +=
        static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER (ordered_dispatch_entries_ [i]->
            task_entry ().rt_info ()->worst_case_execution_time)) /
        static_cast<double> (ordered_dispatch_entries_ [i]->
            task_entry ().effective_period ());
    }
  }

  // update parameters for the lowest priority level
  update_priority_level_params ();

  // if the critical set is schedulable, return success
  return (1.0 - critical_set_utilization_ > DBL_EPSILON)
    ? SUCCEEDED : ST_UTILIZATION_BOUND_EXCEEDED;
}



void
ACE_DynScheduler::update_priority_level_params ()
{
  // if we've just finished examining a critical priority level
  if (minimum_priority_queue_ <= minimum_critical_priority ())
  {
    // update the information about the critical set
    critical_set_frame_size_ = frame_size_;
    critical_set_utilization_ = utilization_;
  }

  // if the lowest priority level considered is schedulable
  if (1.0 - utilization_ > DBL_EPSILON)
  {
    // the minimum guaranteed priority queue is the minimum considered so far
    minimum_guaranteed_priority_queue_ = minimum_priority_queue_;
  }
}

ACE_DynScheduler::status_t
ACE_DynScheduler::setup_task_entries (void)
{
  // store number of tasks, based on registrations
  tasks (static_cast<u_int> (rt_info_entries_.size ()));

  // bail out if there are no tasks registered
  if (tasks () <= 0)
  {
    return ST_NO_TASKS_REGISTERED;
  }

  // clear the decks of any previous scheduling information
  reset ();

  // allocate new table of task entries (wrappers for rt_infos)
  size_t task_count = tasks ();
  ACE_NEW_RETURN (task_entries_, Task_Entry [task_count],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // allocate new table of pointers to task entries (for sorting)
  ACE_NEW_RETURN (ordered_task_entries_, Task_Entry *[task_count],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);
  // @@ TODO: this is completely bogus code, the bit-wise
  //    representation of a null pointer is not always a string of
  //    zeroes.  The correct way to intialize this array is with a for
  //    loop.
  // ACE_OS::memset (ordered_task_entries_, 0,
  //                 sizeof (Task_Entry *) * task_count);
  for (size_t j = 0; j != task_count; ++j)
    ordered_task_entries_[j] = 0;

  // allocate new unbounded set for pointers to
  // task entries that delineate threads
  ACE_NEW_RETURN (thread_delineators_, ACE_Unbounded_Set <Dispatch_Entry *>,
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // allocate new unbounded set for pointers to dispatch entries
  ACE_NEW_RETURN (dispatch_entries_,
                  ACE_Unbounded_Set <Dispatch_Entry *>,
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // allocate new unbounded set for pointers to config info entries
  ACE_NEW_RETURN (config_info_entries_,
                  ACE_Unbounded_Set <Config_Info *>,
                  ST_VIRTUAL_MEMORY_EXHAUSTED);


  // set up links between rt_info_entries_, task_entries_,
  // and ordered_task_entries_ tables
  ACE_Unbounded_Set_Iterator <RT_Info *> iter (rt_info_entries_);
  for (u_int i = 0; i < tasks (); ++i, iter.advance ())
  {
    RT_Info** info_entry;

    // tie task entry to corresponding rt_info
    if (! iter.next (info_entry))
    {
      return ST_BAD_INTERNAL_POINTER;
    }
    task_entries_ [i].rt_info (*info_entry);

    // Tie rt_info to corresponding task entry: the double cast is
    // needed to ensure that the size of the pointer and the size of the
    // stored magic cookie are the same (see the definition of
    // ptrdiff_t in ACE to grok how this works portably).
    task_entries_ [i].rt_info ()->volatile_token =
      static_cast<CORBA::ULongLong> (reinterpret_cast<ptrdiff_t> (&(task_entries_ [i])));

    // tie ordered task entry pointer to corresponding task entry
    ordered_task_entries_ [i] = &(task_entries_ [i]);
  }

  // set up bidirectional links between task entries
  return relate_task_entries ();
}

ACE_DynScheduler::status_t
ACE_DynScheduler::relate_task_entries (void)
{
  status_t status = SUCCEEDED;

  // do DFS traversal of the entire RT_Info handle dependency DAG, replicating
  // the handle dependency DAG as a calls DAG of pointers between task
  // entries (and creating its transpose, the callers DAG).  This is done
  // to avoid the O(n) cost of handle lookups in the RT_Infos for further
  // traversal of the graph during schedule sorting.  One useful side effect
  // of this traversal is that is produces a topological ordering of dependencies
  // in the traversal finishing times, which can be used to detect call cycles.
  long time = 0;

  for (u_int i = 0; i < tasks (); ++i)
  {
    if ((status = relate_task_entries_recurse (time, task_entries_[i]))
        != SUCCEEDED)
    {
      break;
    }
  }

  return status;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::relate_task_entries_recurse (long &time, Task_Entry &entry)
{

  // may have entered at a non-root node previously, so this does
  //  not necessarily indicate a cycle in the dependency graph
        if (entry.dfs_status () != Task_Entry::NOT_VISITED)
  {
    return SUCCEEDED;
  }

  // when a node is discovered, mark it as visited, increment "time" and
  // store as the entry's discovery time.  This is not currently used in
  // the scheduling algorithms, but is left in for possible future use
  // as it shows full parenthetization of entry discovery/finishing.
  entry.dfs_status (Task_Entry::VISITED);
  entry.discovered (++time);

  u_int dependency_count = number_of_dependencies (*entry.rt_info ());
  if (dependency_count > 0)
  {
    // traverse dependencies of underlying RT_Info
    for (u_int i = 0; i < dependency_count; ++i)
    {
      // obtain a pointer to the corresponding Task_Entry for each dependency

      RT_Info* dependency_info = 0;
      lookup_rt_info(entry.rt_info ()->dependencies[i].rt_info, dependency_info);

      if (! dependency_info)
      {
        return ST_BAD_INTERNAL_POINTER;
      }

      // Obtain a pointer to the Task_Entry from the dependency
      // RT_Info: the double cast is needed to ensure that the size of
      // the pointer and the size of the stored magic cookie are the
      // same (see the definition of ptrdiff_t in ACE to grok how
      // this works portably).
      Task_Entry *dependency_entry_ptr =
        ACE_LONGLONG_TO_PTR (Task_Entry *, dependency_info->volatile_token);

      if (! dependency_entry_ptr)
      {
        return ST_BAD_INTERNAL_POINTER;
      }

      // relate the entries according to the direction of the dependency
      Task_Entry_Link *link;
      ACE_NEW_RETURN (link,
                      Task_Entry_Link (entry,
                                       *dependency_entry_ptr,
                                       entry.rt_info ()->dependencies[i].number_of_calls,
                                       entry.rt_info ()->dependencies[i].dependency_type),
                      ST_VIRTUAL_MEMORY_EXHAUSTED);

      dependency_entry_ptr->callers ().insert (link);
      entry.calls ().insert (link);

      // depth first recursion on the newly identified entry
      relate_task_entries_recurse (time, *dependency_entry_ptr);
    }
  }

  // when a node is finished, mark it as finished, increment "time" and
  // store as the entry's finish time.  This produces a topological ordering
  // based on dependencies, which is used to check for call cycles.
  entry.dfs_status (Task_Entry::FINISHED);
  entry.finished (++time);

  return SUCCEEDED;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::identify_threads (ACE_CString & unresolved_locals,
                                    ACE_CString & unresolved_remotes)
{
  u_int i, j;
  ACE_DynScheduler::status_t result = SUCCEEDED;
  char string_buffer [BUFSIZ];

  // walk array of task entries, picking out thread delineators
  for (i = 0; i < tasks_; i++)
  {
    // if entry has exposed threads or no callers, it may be a thread
    if ((task_entries_ [i].rt_info ()->threads > 0) ||
        (task_entries_ [i].callers ().is_empty ()))
    {
      // if its period is valued, it's a thread delineator
      if (task_entries_ [i].rt_info ()->period > 0)
      {
        task_entries_ [i].effective_period (task_entries_ [i].rt_info ()->period);
        task_entries_ [i].is_thread_delineator (1);

        // create a Dispatch_Entry for each thread of the delimiting Task_Entry
        u_int thread_count = (task_entries_ [i].rt_info ()->threads > 0)
                             ? task_entries_ [i].rt_info ()->threads : 1;
        // Just use low 32 bits of effective_period.  This will
        // have to change when TimeBase.idl is finalized.
        const TimeBase::TimeT zero = 0;
        for (j = 0; j < thread_count; j++)
        {
          Dispatch_Entry *dispatch_ptr;
          const TimeBase::TimeT effective_period =
            task_entries_ [i].effective_period ();
          ACE_NEW_RETURN(dispatch_ptr,
                         Dispatch_Entry (zero,
                                         effective_period,
                                         task_entries_ [i].rt_info ()->preemption_priority,
                                         task_entries_ [i].rt_info ()->priority,
                                         task_entries_ [i]),
                         ST_VIRTUAL_MEMORY_EXHAUSTED);

          if ((task_entries_ [i].dispatches ().insert (Dispatch_Entry_Link (*dispatch_ptr)) < 0) ||
              (dispatch_entries_->insert (dispatch_ptr) < 0) ||
                          (thread_delineators_->insert (dispatch_ptr) < 0))
          {
            return ST_VIRTUAL_MEMORY_EXHAUSTED;
          }

                  // increase the count of thread dispatches
          ++ threads_;
        }
      }
      else if (task_entries_ [i].rt_info ()->info_type == RtecScheduler::REMOTE_DEPENDANT)
        {
          // Warn about unresolved remote dependencies, mark the task entry

          result = (result == SUCCEEDED)
                   ? ST_UNRESOLVED_REMOTE_DEPENDENCIES
                   : result;

          task_entries_ [i].has_unresolved_remote_dependencies (1);

          ACE_DEBUG (
             (LM_DEBUG,
              ACE_TEXT("Warning: an operation identified by ")
              ACE_TEXT("\"%C\" has unresolved remote dependencies.\n"),
              (const char*)task_entries_ [i].rt_info ()->entry_point));

          // Record entry point in list of unresolved remote dependencies
          ACE_OS::sprintf (string_buffer, "// %s\n",
                           (const char*) task_entries_ [i].rt_info ()->
                                                               entry_point);
          unresolved_remotes +=
            ACE_CString (string_buffer);
        }
      else
        {
          // Local node that no one calls and has neither rate nor threads is suspect
          ACE_DEBUG (
             (LM_DEBUG,
              ACE_TEXT("Error: operation \"%C\" does not specify a period or\n")
              ACE_TEXT("visible threads, and is not called by any other operation.\n")
              ACE_TEXT("Are there backwards dependencies.\n"),
              (const char*)task_entries_ [i].rt_info ()->entry_point));

          result = ST_UNRESOLVED_LOCAL_DEPENDENCIES;

          task_entries_ [i].has_unresolved_local_dependencies (1);

          // Record entry point in list of unresolved local dependencies
          ACE_OS::sprintf (string_buffer, "// %s\n",
                           (const char*) task_entries_ [i].rt_info ()->
                                                               entry_point);
          unresolved_locals +=
            ACE_CString (string_buffer);
        }
    }
  }

  return result;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::check_dependency_cycles (void)
{
  status_t return_status = SUCCEEDED;

  // sort the pointers to entries in order of descending finish time
  ACE_OS::qsort ((void *) ordered_task_entries_,
                 tasks (),
                 sizeof (Task_Entry *),
                 compare_entry_finish_times);

  // set all the dfs_status indicators to NOT_VISITED
  u_int i;
  for (i = 0; i < tasks (); ++i)
  {
          ordered_task_entries_ [i]->dfs_status (Task_Entry::NOT_VISITED);
  }

  // recurse on each entry, saving most recent status if it is not SUCCEEDED
  for (i = 0; i < tasks (); ++i)
  {
    status_t status =
      check_dependency_cycles_recurse (*ordered_task_entries_ [i]);

    if (status != SUCCEEDED)
    {
      return_status = status;
         }
  }

  return return_status;
}
  // uses strongly connected components algorithm: consider entries
  // in order of finishing time from dependency DAG traversal,
  // but traverse transpose graph: any entry that has a dependant
  // that was not previously visited in this traversal is part
  // of a dependency cycle


ACE_DynScheduler::status_t
ACE_DynScheduler::check_dependency_cycles_recurse (Task_Entry &entry)
{
  status_t return_status = SUCCEEDED;

  // halt DFS recursion on callers graph if entry has already been visited
  if (entry.dfs_status () != Task_Entry::NOT_VISITED)
  {
    return return_status;
  }

  // mark the entry as visited
  entry.dfs_status (Task_Entry::VISITED);

  // check all the calling operations: if there is one that has not already been
  // visited, mark the return status indicating there is a cycle, print
  // an error message to that effect, and recurse on that dependant
  Task_Entry_Link **calling_entry_link;
  ACE_Unbounded_Set_Iterator <Task_Entry_Link *> i (entry.callers ());
  while (i.next (calling_entry_link) != 0)
  {
    i.advance ();
    if ((*calling_entry_link)->caller ().dfs_status () == Task_Entry::NOT_VISITED)
    {
      // indicate the two tasks are in (the same) dependency cycle
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Tasks \"%C\" and \"%C\" are part of a call cycle.\n"),
                  (*calling_entry_link)->caller ().rt_info ()->entry_point.in (),
                  entry.rt_info ()->entry_point.in ()));

      // set return status, ignore status returned by recursive call:
      // we already know there are cycles in the dependencies
      return_status = ST_CYCLE_IN_DEPENDENCIES;
      check_dependency_cycles_recurse ((*calling_entry_link)->caller ());
    }
  }

  // mark the entry as finished
  entry.dfs_status (Task_Entry::FINISHED);

  return return_status;
}


ACE_DynScheduler::status_t
ACE_DynScheduler::schedule_threads (ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set)
{
  // make sure there are as many thread delineator
  // entries in the set as the counter indicates
  if (threads_ != thread_delineators_->size ())
  {
    return THREAD_COUNT_MISMATCH;
  }

  // allocate an array of pointers to the thread delineators
  ACE_NEW_RETURN (ordered_thread_dispatch_entries_,
                  Dispatch_Entry * [threads_],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);
  ACE_OS::memset (ordered_thread_dispatch_entries_, 0,
                  sizeof (Dispatch_Entry *) * threads_);


  // copy pointers to the thread delineators from the set to the array
  ACE_Unbounded_Set_Iterator <Dispatch_Entry *> iter (*thread_delineators_);
  for (u_int i = 0; i < threads_; ++i, iter.advance ())
  {
    Dispatch_Entry** dispatch_entry;

    if (! iter.next (dispatch_entry))
    {
      return ST_BAD_INTERNAL_POINTER;
    }

    ordered_thread_dispatch_entries_ [i] = *dispatch_entry;
  }

  // sort the thread dispatch entries into priority order
  status_t status = sort_dispatches (ordered_thread_dispatch_entries_, threads_);

  if (status == SUCCEEDED)
  {
    // assign priorities to the thread dispatch entries
    status = assign_priorities (ordered_thread_dispatch_entries_,
                                threads_, anomaly_set);
  }

  return status;
}
  // thread scheduling method: sets up array of pointers to task
  // entries that are threads, calls internal thread scheduling method

ACE_DynScheduler::status_t
ACE_DynScheduler::schedule_dispatches (ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set)
{
  dispatch_entry_count_ = static_cast<u_int> (dispatch_entries_->size ());

  ACE_NEW_RETURN (ordered_dispatch_entries_,
                  Dispatch_Entry * [dispatch_entry_count_],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);
  ACE_OS::memset (ordered_dispatch_entries_, 0,
                  sizeof (Dispatch_Entry *) * dispatch_entry_count_);

  ACE_Unbounded_Set_Iterator <Dispatch_Entry *> iter (*dispatch_entries_);
  for (u_int i = 0; i < dispatch_entry_count_; ++i, iter.advance ())
  {
    Dispatch_Entry** dispatch_entry;

    if (! iter.next (dispatch_entry))
    {
      return ST_BAD_INTERNAL_POINTER;
    }

    ordered_dispatch_entries_ [i] = *dispatch_entry;
  }

  // sort the entries in order of priority and subpriority
  sort_dispatches (ordered_dispatch_entries_, dispatch_entry_count_);

  // assign dynamic and static subpriorities to the thread dispatch entries
  return assign_subpriorities (ordered_dispatch_entries_,
                               dispatch_entry_count_, anomaly_set);
}
  // dispatch scheduling method: sets up an array of dispatch entries,
  // calls internal dispatch scheduling method.

ACE_DynScheduler::status_t
ACE_DynScheduler::store_assigned_info (void)
{
  for  (u_int i = 0; i < dispatch_entry_count_; ++i)
    {
      if ((! ordered_dispatch_entries_) || (! (ordered_dispatch_entries_[i])) ||
          (! (ordered_dispatch_entries_[i]->task_entry ().rt_info ())))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("ACE_DynScheduler::store_assigned_info () could not store ")
                             ACE_TEXT("priority information (error in internal representation)")),
                             ST_BAD_INTERNAL_POINTER);
        }

  // set OS priority and Scheduler preemption priority and static
  // preemption subpriority in underlying RT_Info
  ordered_dispatch_entries_ [i]->task_entry ().rt_info ()->priority =
    ordered_dispatch_entries_ [i]->OS_priority ();
  ordered_dispatch_entries_ [i]->task_entry ().rt_info ()->preemption_priority =
    ordered_dispatch_entries_ [i]->priority ();
  ordered_dispatch_entries_ [i]->task_entry ().rt_info ()->preemption_subpriority =
    ordered_dispatch_entries_ [i]->static_subpriority ();
  }

  return SUCCEEDED;
}
  // = store assigned information back into the RT_Infos


ACE_DynScheduler::status_t
ACE_DynScheduler::create_timeline ()
{
  // queue of previously scheduled entries that need to be rescheduled
  ACE_Unbounded_Queue <Dispatch_Entry *> reschedule_queue;

  status_t status = SUCCEEDED;

  ACE_NEW_RETURN(timeline_, ACE_Ordered_MultiSet <TimeLine_Entry_Link>,
                 ST_VIRTUAL_MEMORY_EXHAUSTED);

  ACE_NEW_RETURN(expanded_dispatches_, ACE_Unbounded_Set <Dispatch_Entry *>,
                     ST_VIRTUAL_MEMORY_EXHAUSTED);

  // start with the id of the first entry in the array
  min_dispatch_id_ = ordered_dispatch_entries_[0]->dispatch_id ();
  max_dispatch_id_ = ordered_dispatch_entries_[0]->dispatch_id ();

  for (u_long i = 0; i < dispatch_entry_count_; ++i)
  {
    // update the minimal and maximal id values for the schedule
    if (ordered_dispatch_entries_[i]->dispatch_id () < min_dispatch_id_)
    {
      min_dispatch_id_ = ordered_dispatch_entries_[i]->dispatch_id ();
    }
    if (ordered_dispatch_entries_[i]->dispatch_id () > max_dispatch_id_)
    {
      max_dispatch_id_ = ordered_dispatch_entries_[i]->dispatch_id ();
    }

    // only put OPERATION and REMOTE_DEPENDANT dispatches into the timeline.
    if ((ordered_dispatch_entries_[i]->task_entry().info_type () !=
                RtecScheduler::OPERATION) &&
        (ordered_dispatch_entries_[i]->task_entry().info_type () !=
                RtecScheduler::REMOTE_DEPENDANT))
    {
      continue;
    }

    // schedule the current dispatch entry into the timeline
    status = schedule_timeline_entry (*(ordered_dispatch_entries_[i]),
                                      reschedule_queue);
    if (status != SUCCEEDED)
    {
      break;
    }

    // iterate through the set of dispatch entries that need to be rescheduled
    Dispatch_Entry *rescheduled_entry;
    while (reschedule_queue.is_empty () == 0)
    {

      if (reschedule_queue.dequeue_head (rescheduled_entry) < 0)
      {
        status = ST_BAD_INTERNAL_POINTER;
        break;
      }

      status = schedule_timeline_entry (*rescheduled_entry, reschedule_queue);
      if (status != SUCCEEDED)
      {
        break;
      }
    }
    if (status != SUCCEEDED)
    {
      break;
    }

    // Schedule additional dispatches of the entry
    // over the total frame size into the timeline.
    u_long current_frame_offset = 0;
    u_long task_period =
      ordered_dispatch_entries_[i]->task_entry ().effective_period ();
    for (current_frame_offset = task_period;
         current_frame_offset < frame_size_;
                        current_frame_offset += task_period)
    {
      Dispatch_Entry *new_dispatch_entry;

      // create a new dispatch entry at the current sub-frame offset
      // Just use low 32 bits of arrival and deadline.  This will
      // have to change when TimeBase.idl is finalized.
      const TimeBase::TimeT arrival =
        ordered_dispatch_entries_[i]->arrival () +
        static_cast<ACE_UINT32> (current_frame_offset);
      const TimeBase::TimeT deadline=
        ordered_dispatch_entries_[i]->deadline () +
        static_cast<ACE_UINT32> (current_frame_offset);

      ACE_NEW_RETURN (
        new_dispatch_entry,
        Dispatch_Entry (arrival,
                        deadline,
                        ordered_dispatch_entries_[i]->priority (),
                        ordered_dispatch_entries_[i]->OS_priority (),
                        ordered_dispatch_entries_[i]->task_entry (),
                        ordered_dispatch_entries_[i]),
        ST_VIRTUAL_MEMORY_EXHAUSTED);

      // add the new dispatch entry to the set of expanded dispatches
      expanded_dispatches_->insert (new_dispatch_entry);

      // schedule the new dispatch entry into the timeline
      status = schedule_timeline_entry (*new_dispatch_entry, reschedule_queue);
      if (status != SUCCEEDED)
      {
        break;
      }

      while (reschedule_queue.is_empty () == 0)
      {
        if (reschedule_queue.dequeue_head (rescheduled_entry) < 0)
        {
          status = ST_BAD_INTERNAL_POINTER;
          break;
        }
        status = schedule_timeline_entry (*rescheduled_entry, reschedule_queue);
        if (status != SUCCEEDED)
        {
          break;
        }
      }
      if (status != SUCCEEDED)
      {
        break;
      }
    }

    if (status != SUCCEEDED)
    {
      break;
    }
  }

  return status;
}
  // Create a timeline.



ACE_DynScheduler::status_t
ACE_DynScheduler::output_dispatch_priorities (const char *filename)
{
  status_t status = UNABLE_TO_OPEN_SCHEDULE_FILE;

  // open the file
  FILE *file = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(filename), ACE_TEXT("w"));
  if (file)
  {
    status = output_dispatch_priorities (file);
    ACE_OS::fclose (file);
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT("ACE_DynScheduler::output_dispatch_priorities: ")
                ACE_TEXT("Could not open schedule file (\"%C\")"),
                filename));
  }

  return status;
}


ACE_DynScheduler::status_t
ACE_DynScheduler::output_dispatch_priorities (FILE *file)
{

  u_long dispatch_count = 0;
  u_long i = 0;
  for (i = 0; i < dispatch_entry_count_; ++i)
  {
    dispatch_count +=
     frame_size_
     / ordered_dispatch_entries_[i]->task_entry ().effective_period ();
  }

  if (ACE_OS::fprintf (
      file, "\n\nSCHEDULING RESULTS:\n\n"
            "Number of dispatches:              %3lu\n"
            "Number of threads:                 %3u\n"
            "Number of tasks:                   %3u\n"
            "Scheduler Status:                    [%d] %s\n"
            "Total Frame Size:                    %lu nsec (%f Hz)\n"
            "Critical Set Frame Size:             %lu nsec (%f Hz)\n"
            "Utilization:                         %f\n"
            "Critical Set Utilization:            %f\n"
            "Minimum Priority Queue:            %3d\n"
            "Minimum Guaranteed Priority Queue: %3d\n"
            "Minimum Critical Priority:         %3d\n\n\n"

            "DISPATCH PRIORITIES:\n\n"
            "                                  (critical             \n"
            "                                   instant)             \n"
            "             dispatch              dynamic      static  \n"
            "operation          ID  priority  subpriority  subpriority\n"
            "---------    --------  --------  -----------  -----------\n",
      dispatch_count, threads_, tasks_, status_,
      status_message(status_), frame_size_,
      (double) (10000000.0 / ((double) frame_size_)),
      critical_set_frame_size_,
      (double) (10000000.0 / ((double) critical_set_frame_size_)),
      utilization_, critical_set_utilization_,
      int(minimum_priority_queue_),
      int(minimum_guaranteed_priority_queue_),
      int(minimum_critical_priority ())) < 0)

  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("ACE_DynScheduler::output_dispatch_priorities: ")
                       ACE_TEXT("Could not write to schedule file\n")),
                      UNABLE_TO_WRITE_SCHEDULE_FILE);
  }

  for (i = 0; i < dispatch_entry_count_; ++i)
  {
    if (ACE_OS::fprintf (file, "%-11s  %8lu  %8u  %11u  %11u\n",
        ordered_dispatch_entries_[i]->task_entry ().rt_info ()->
          entry_point.in (),
        ordered_dispatch_entries_[i]->dispatch_id (),
        ordered_dispatch_entries_[i]->priority (),
        ordered_dispatch_entries_[i]->dynamic_subpriority (),
        ordered_dispatch_entries_[i]->static_subpriority ()) < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE_DynScheduler::output_dispatch_priorities: ")
                         ACE_TEXT("Could not write to schedule file\n")),
                        UNABLE_TO_WRITE_SCHEDULE_FILE);
    }
  }

  return SUCCEEDED;
}


ACE_DynScheduler::status_t
ACE_DynScheduler::output_dispatch_timeline (const char *filename)
{
  status_t status = UNABLE_TO_OPEN_SCHEDULE_FILE;

  // open the file
  FILE *file = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(filename), ACE_TEXT("w"));
  if (file)
  {
    status = output_dispatch_timeline (file);
    ACE_OS::fclose (file);
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT("ACE_DynScheduler::output_dispatch_timeline: ")
                ACE_TEXT("Could not open schedule file (\"%s\")"),
                filename));
  }

  return status;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::output_dispatch_timeline (FILE *file)
{
  //FUZZ: disable check_for_lack_ACE_OS
  if (ACE_OS::fprintf (
      file, "\n\nDISPATCH TIMELINE:\n\n"
            "                dispatch  arrival  deadline     start        stop    execution     latency        laxity\n"
            "operation             ID   (nsec)    (nsec)    (nsec)      (nsec)  time (nsec)      (nsec)        (nsec)\n"
            "---------    -----------  -------  --------     -----      ------  -----------      -------       ------\n") < 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("ACE_DynScheduler::output_dispatch_timeline: ")
                       ACE_TEXT("Could not write to schedule file")),
                      UNABLE_TO_WRITE_SCHEDULE_FILE);
  }
  //FUZZ: enable check_for_lack_ACE_OS

  // iterate through timeline, picking out entries whose prev_ pointer
  // is null (i.e. those representing the start of a dispatch), find end
  // of dispatch, output the operation, dispatch, priority, and time info
  ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> iter (*timeline_);
  for (iter.first (); iter.done () == 0; iter.advance ())
  {
    TimeLine_Entry_Link *link;
    if ((iter.next (link) == 0) || (! link))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE_DynScheduler::output_dispatch_timeline: ")
                         ACE_TEXT("Bad internal pointer\n")),
                        ST_BAD_INTERNAL_POINTER);
    }

    // for each timeline entry that starts a dispatch
    if (link->entry ().prev () == 0)
    {
      // find the last time slice for the dispatch
      TimeLine_Entry *last_entry = &(link->entry ());
      while (last_entry->next ())
      {
        last_entry = last_entry->next ();
      }

      Time tmp = last_entry->stop () - link->entry ().arrival () -
                 link->entry ().dispatch_entry ().task_entry ().rt_info ()->
                   worst_case_execution_time;
      if (link->entry ().dispatch_entry ().original_dispatch ())
      {
        if (ACE_OS::fprintf (
              file,
              "%-11s  [%4lu] %4lu  %7u  %8u  %8u  "
              "%10u  %11u   %10d   %10d\n",
              link->entry ().dispatch_entry ().task_entry ().rt_info ()->
                entry_point.in (),
              link->entry ().dispatch_entry ().original_dispatch ()->dispatch_id (),
              link->entry ().dispatch_entry ().dispatch_id (),
              ACE_U64_TO_U32 (link->entry ().arrival ()),
              ACE_U64_TO_U32 (link->entry ().deadline ()),
              ACE_U64_TO_U32 (link->entry ().start ()),
              ACE_U64_TO_U32 (last_entry->stop ()),
              ACE_U64_TO_U32 (link->entry ().dispatch_entry ().task_entry ().
                rt_info ()->worst_case_execution_time),
              ACE_U64_TO_U32 (tmp),
              ACE_U64_TO_U32 (link->entry ().deadline () -
                              last_entry->stop ())) < 0)

        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("ACE_DynScheduler::output_dispatch_timeline: ")
                             ACE_TEXT("Unable to write to schedule file\n")),
                            UNABLE_TO_WRITE_SCHEDULE_FILE);
        }
      }
      else
      {
        if (ACE_OS::fprintf (
              file,
              "%-11s  %11lu  %7u  %8u  %8u  %10u  %11u   %10d   %10d\n",
              link->entry ().dispatch_entry ().task_entry ().rt_info ()->
                entry_point.in (),
              link->entry ().dispatch_entry ().dispatch_id (),
              ACE_U64_TO_U32 (link->entry ().arrival ()),
              ACE_U64_TO_U32 (link->entry ().deadline ()),
              ACE_U64_TO_U32 (link->entry ().start ()),
              ACE_U64_TO_U32 (last_entry->stop ()),
              ACE_U64_TO_U32 (link->entry ().dispatch_entry ().task_entry ().
                rt_info ()->worst_case_execution_time),
              ACE_U64_TO_U32 (tmp),
              ACE_U64_TO_U32 (link->entry ().deadline () -
                              last_entry->stop ())) < 0)

        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("ACE_DynScheduler::output_dispatch_timeline: ")
                             ACE_TEXT("Unable to write to schedule file\n")),
                            UNABLE_TO_WRITE_SCHEDULE_FILE);
        }
      }
    }
  }

  return SUCCEEDED;
}
  // this prints the entire set of timeline outputs to the specified file

ACE_DynScheduler::status_t
ACE_DynScheduler::output_preemption_timeline (const char *filename)
{
  status_t status = UNABLE_TO_OPEN_SCHEDULE_FILE;

  // open the file
  FILE *file = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(filename), ACE_TEXT("w"));
  if (file)
  {
    status = output_preemption_timeline (file);
    ACE_OS::fclose (file);
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT("ACE_DynScheduler::output_preemption_timeline: ")
                ACE_TEXT("Cannot open timeline file (\"%C\")\n"),
                filename));
  }

  return status;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::output_preemption_timeline (FILE *file)
{
  if (ACE_OS::fprintf (
        file, "\n\nPREEMPTION TIMELINE:\n\n"
        "              dispatch     start      stop \n"
        "operation           ID    (nsec)    (nsec)\n"
        "---------  -----------    ------    ------\n") < 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("ACE_DynScheduler::output_preemption_timeline: ")
                       ACE_TEXT("Cannot write to timeline file\n")),
                      UNABLE_TO_WRITE_SCHEDULE_FILE);
  }

  ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> iter (*timeline_);

  TimeLine_Entry_Link *link;
  for (iter.first (); iter.done () == 0; iter.advance ())
  {
    if ((iter.next (link) == 0) || (! link))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("ACE_DynScheduler::output_preemption_timeline: ")
                         ACE_TEXT("Bad internal pointer\n")),
                         ST_BAD_INTERNAL_POINTER);
    }

    if (link->entry ().dispatch_entry ().original_dispatch ())
    {
      if (ACE_OS::fprintf (
            file, "%-9s  [%4lu] %4lu  %8u  %8u\n",
            link->entry ().dispatch_entry ().task_entry ().rt_info ()->
              entry_point.in (),
            link->entry ().dispatch_entry ().original_dispatch ()->dispatch_id (),
            link->entry ().dispatch_entry ().dispatch_id (),
            ACE_U64_TO_U32 (link->entry ().start ()),
            ACE_U64_TO_U32 (link->entry ().stop ())) < 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("ACE_DynScheduler::output_preemption_timeline: ")
                           ACE_TEXT("Cannot write to timeline file\n")),
                          UNABLE_TO_WRITE_SCHEDULE_FILE);
      }
    }
    else
    {
      if (ACE_OS::fprintf (
            file, "%-9s  %11lu  %8u  %8u\n",
            link->entry ().dispatch_entry ().task_entry ().rt_info ()->
              entry_point.in (),
            link->entry ().dispatch_entry ().dispatch_id (),
            ACE_U64_TO_U32 (link->entry ().start ()),
            ACE_U64_TO_U32 (link->entry ().stop ())) < 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("ACE_DynScheduler::output_preemption_timeline: ")
                           ACE_TEXT("Cannot write to timeline file\n")),
                          UNABLE_TO_WRITE_SCHEDULE_FILE);
      }
    }
  }

  return SUCCEEDED;
}


ACE_DynScheduler::status_t
ACE_DynScheduler::output_viewer_timeline (const char *filename)
{
  status_t status = UNABLE_TO_OPEN_SCHEDULE_FILE;

  // open the file
  FILE *file = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(filename), ACE_TEXT("w"));
  if (file)
  {
    status = output_dispatch_timeline (file);
    ACE_OS::fclose (file);
  }

  return status;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::output_viewer_timeline (FILE *file)
{
  //FUZZ: disable check_for_lack_ACE_OS
  if (ACE_OS::fprintf (
      file, "\n\nVIEWER TIMELINE:\n\n"
            "                                    arrival  deadline   completion    execution \n"
            "operation  utilization   overhead    (nsec)    (nsec)  time (nsec)  time (nsec)\n"
            "---------  -----------   --------   -------  --------  -----------  -----------\n") < 0)
  {
    return UNABLE_TO_WRITE_SCHEDULE_FILE;
  }
  //FUZZ: enable check_for_lack_ACE_OS

  // iterate through timeline, picking out dispatches in chronological
  // order of operation completion time
  int entries_remain = 1;
  Time accumulated_execution = 0;
  Time current_accumulated_execution = 0;
  Time last_completion = 0;
  Time current_completion = 0;
  TimeLine_Entry *current_entry = 0;
  TimeLine_Entry *current_last_entry = 0;

  while (entries_remain)
  {
    last_completion = current_completion;

    accumulated_execution = 0;
    current_accumulated_execution = 0;
    current_completion = 0;
    current_entry = 0;
    current_last_entry = 0;

    ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> iter (*timeline_);
    for (iter.first (); iter.done () == 0; iter.advance ())
    {
      TimeLine_Entry_Link *link;
      if ((iter.next (link) == 0) || (! link))
      {
        return ST_BAD_INTERNAL_POINTER;
      }

      accumulated_execution += link->entry ().stop () -
                               link->entry ().start ();

      // for each timeline entry that starts a dispatch
      if (link->entry ().prev () == 0)
      {
        // find the last time slice for the dispatch
        TimeLine_Entry *last_entry = &(link->entry ());
        while (last_entry->next ())
        {
          last_entry = last_entry->next ();
        }

        if ((last_entry->stop () > last_completion) &&
            ((last_entry->stop () < current_completion) ||
             (current_completion == 0U)))
        {
          current_completion = last_entry->stop ();
          current_entry = &(link->entry ());
          current_last_entry = last_entry;
        }
      }

      // save the accumulated execution if we're at
      // the last entry for the current dispatch
      if (current_last_entry == &(link->entry ()))
      {
        current_accumulated_execution = accumulated_execution;
      }
    }

    // if we found another entry, print it (otherwise we're done)
    if (current_entry)
    {
      if (ACE_OS::fprintf (
            file, "%-11s  %9f  %9f  %8u  %8u  %11u  %11u\n",
            current_entry->dispatch_entry ().task_entry ().rt_info ()->
              entry_point.in (),
                        static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER(current_accumulated_execution)) /
                        static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER(current_completion)),
            0.0,
            ACE_U64_TO_U32 (current_entry->arrival ()),
            ACE_U64_TO_U32 (current_entry->deadline ()),
            ACE_U64_TO_U32 (current_last_entry->stop ()),
            ACE_U64_TO_U32 (current_entry->dispatch_entry ().task_entry ().
              rt_info ()->worst_case_execution_time)) < 0)
      {
        return UNABLE_TO_WRITE_SCHEDULE_FILE;
      }
    }
    else
    {
      entries_remain = 0;
    }
  }

  return SUCCEEDED;
}


ACE_DynScheduler::status_t
ACE_DynScheduler::output_timeline (const char *filename, const char *heading)
{
  status_t status = SUCCEEDED;
  FILE *file = 0;

  // bail out if we're not up to date or there is no timeline
  if ((! up_to_date_) || (! timeline_))
  {
    status = NOT_SCHEDULED;
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT("ACE_DynScheduler::output_timeline: ")
                ACE_TEXT("Schedule not generated")));
  }

  if (status == SUCCEEDED)
  {
    // open the file
    file = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(filename), ACE_TEXT("w"));
    if (! file)
    {
      status = UNABLE_TO_OPEN_SCHEDULE_FILE;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("ACE_DynScheduler::output_timeline: ")
                  ACE_TEXT("Could not open schedule file")));
    }
  }

  if ((status == SUCCEEDED) && (heading))
  {
    if (ACE_OS::fprintf (file, "%s\n\n", heading) < 0)
    {
      status = UNABLE_TO_WRITE_SCHEDULE_FILE;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("ACE_DynScheduler::output_timeline: ")
                  ACE_TEXT("Could not write to schedule file")));
    }
  }

  if (status == SUCCEEDED)
  {
    status = output_dispatch_priorities (file);
  }

  if (status == SUCCEEDED)
  {
    status = output_dispatch_timeline (file);
  }

  if (status == SUCCEEDED)
  {
    status = output_preemption_timeline (file);
  }

  if (status == SUCCEEDED)
  {
    status = output_viewer_timeline (file);
  }

  if (file)
  {
    ACE_OS::fclose (file);
  }

  return status;
}
  // this prints the entire set of timeline outputs to the specified file

TAO_END_VERSIONED_NAMESPACE_DECL
