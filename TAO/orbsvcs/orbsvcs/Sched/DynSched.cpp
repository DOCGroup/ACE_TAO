// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    DynSched.cpp
//
// = CREATION DATE
//    23 January 1997
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

#include "DynSched.h"
#include "ace/Sched_Params.h"
#include "math.h"

#if ! defined (__ACE_INLINE__)
#include "DynSched.i"
#endif /* __ACE_INLINE__ */

//////////////////////
// Helper functions //
//////////////////////

// compare the DFS finish times of two task entries, order higher time *first*
extern "C" int compare_entry_finish_times (const void *first, const void *second)
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

  Task_Entry *first_entry =
          * ACE_static_cast (Task_Entry **, first);
  Task_Entry *second_entry =
          * ACE_static_cast (Task_Entry **, second);

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
    case ST_TASK_ALREADY_REGISTERED :
      return "TASK_ALREADY_REGISTERED";
    case ST_BAD_DEPENDENCIES_ON_TASK :
      return "BAD_DEPENDENCIES_ON_TASK";
    case ST_BAD_INTERNAL_POINTER :
      return "BAD_INTERNAL_POINTER";
    case ST_VIRTUAL_MEMORY_EXHAUSTED :
      return "VIRTUAL_MEMORY_EXHAUSTED";
    case ST_UNKNOWN_TASK :
      return "UNKNOWN_TASK";
    case TASK_COUNT_MISMATCH :
      return "TASK_COUNT_MISMATCH";
    case INVALID_PRIORITY :
      return "INVALID_PRIORITY";

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
  ACE_DynScheduler::status_t ret;

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
              ACE_OS::printf ("registered task \"%s\" with RT_Info at %X\n",
                              (const char*)rt_info->entry_point,
                              (void *) rt_info);
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
      rtinfo->entry_point = name;
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
    case RtecScheduler::TWO_WAY_CALL:

      temp_info = rt_info;
      break;

    case RtecScheduler::ONE_WAY_CALL:

      // swap the handles and point to the caller instead of the called operation
      if (lookup_rt_info (d.rt_info, temp_info) != SUCCEEDED)
      {
        ACE_ERROR ((LM_ERROR, "cannot find %d to add dependency\n", d.rt_info));
        return -1;
      }

      d.rt_info = temp_info->handle;

    default:

      ACE_ERROR ((LM_ERROR, "unrecognized dependency type %d for %s\n",
                  d.dependency_type, rt_info->entry_point.in ()));
      return -1;
  }

  ACE_DEBUG ((LM_DEBUG, "adding dependecy to: %s\n",
              (const char*)temp_info->entry_point));

  RtecScheduler::Dependency_Set& set = temp_info->dependencies;
  int l = set.length();
  set.length(l + 1);
  set[l] = d;
  return 0;
}

void ACE_DynScheduler::export(RT_Info* info, FILE* file)
{
  export(*info, file);
}

void ACE_DynScheduler::export(RT_Info& info, FILE* file)
{
  // The .low selection is for CosTimeBase::ulonglong support.  It
  // should change after that is finalized to support 64 bit integers
  // on all platforms.
  (void) ACE_OS::fprintf (file,
                          "%s\n%d\n%ld\n%ld\n%ld\n%ld\n%d\n%d\n%ld\n%u\n"
                          "# begin calls\n%d\n",
                          info.entry_point.in (),
                          info.handle,
                          info.worst_case_execution_time.low,
                          info.typical_execution_time.low,
                          info.cached_execution_time.low,
                          info.period,
                          info.criticality,
                          info.importance,
                          info.quantum.low,
                          info.threads,
                          number_of_dependencies(info));

  for (int i = 0; i < number_of_dependencies(info); ++i)
    {
      RT_Info tmp;
      (void) ACE_OS::fprintf (file, "%s, %d\n",
                              (const char*)tmp.entry_point,
                              info.dependencies[i].number_of_calls);

    }

// TBD - we'll need to update this to use the information aggregated
//       within the task entry pointed to by the RT_Info's volatile_token
//       ACT (in fact, there is now more than one
//       priority assignment per RT_Info, w/ disjunction on multiple
//       priority levels, rates, etc. - iterate through and show each dispatch)

  (void) ACE_OS::fprintf (file, "# end calls\n%d\n%d\n%d\n\n",
                          info.priority,
                          info.dynamic_subpriority,
                          info.static_subpriority);


}


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
ACE_DynScheduler::schedule (void)
{
  status_t temp_status = SUCCEEDED;

  ACE_Guard<LOCK> ace_mon (lock_);

  if (up_to_date_)
  {
    // do nothing if the RT_Infos have not changed
    // since the last valid schedule was generated
    return SUCCEEDED;
  }
  else
  {
    // save the total number of registered RT_Infos
    tasks (rt_info_entries_.size ());
  }

  // set up the task entry data structures, check for call cycles
  status_ = setup_task_entries ();

  if (status_ == SUCCEEDED)
  {
    // check for cycles in the dependency graph: as a side effect, leaves
    // the ordered_task_entries_ pointer array sorted in topological order,
    // which is used by propagate_dispatches () to ensure that dispatches
    // are propagated top down in the call graph.
    status_ = check_dependency_cycles ();
  }

  if (status_ == SUCCEEDED)
  {
    // task entries are related, now threads can be found
    status_ = identify_threads ();
  }

  if (status_ == SUCCEEDED)
  {
    // invokes the internal thread scheduling method of the strategy
    status_ = schedule_threads ();
  }

  if (status_ == SUCCEEDED)
  {
    // propagate the dispatch information from the
    // threads throughout the call graph
    status_ = propagate_dispatches ();
  }

  if (status_ == SUCCEEDED)
  {
    // invokes the internal dispatch scheduling method of the strategy
    status_ = schedule_dispatches ();
  }

  // calculate utilization, total frame size, critical set
  if (status_ == SUCCEEDED)
  {
    status_ = calculate_utilization_params ();
  }

  // generate the scheduling timeline over the total frame size
  if ((status_ == SUCCEEDED) || (status_ == ST_UTILIZATION_BOUND_EXCEEDED))
  {
    temp_status = create_timeline ();
  }

  if (temp_status != SUCCEEDED)
  {
    status_ = temp_status;
  }

  // store the timeline to a file if one was given
  if ((timeline_filename_ != 0)  &&
      ((status_ == SUCCEEDED) || (status_ == ST_UTILIZATION_BOUND_EXCEEDED)))
  {
    temp_status = output_timeline (timeline_filename_, 0);
  }

  if (temp_status != SUCCEEDED)
  {
    status_ = temp_status;
  }

  // if a valid schedule was not generated, clean up from the attempt
  switch (status_)
  {
    // these are statuses that indicate a reasonable schedule was generated
    case SUCCEEDED:
    case ST_UTILIZATION_BOUND_EXCEEDED:

        // if we made it here, the schedule is done
        up_to_date_ = 1;


      break;

    default:

      // (try to) remove the output files
      if (timeline_filename_ && ACE_OS::unlink (timeline_filename_) &&
          errno != ENOENT)
      {
        ACE_OS::perror ("ACE_DynScheduler::schedule (); "
                        "unable to remove timeline file");
      }
      if (runtime_filename_ && ACE_OS::unlink (runtime_filename_) &&
          errno != ENOENT)
      {
        ACE_OS::perror ("ACE_DynScheduler::schedule (); "
                        "unable to remove schedule file");
      }
      if (rt_info_filename_  &&  ACE_OS::unlink (rt_info_filename_)  &&
          errno != ENOENT)
      {
        ACE_OS::perror ("ACE_DynScheduler::schedule (); "
                        "unable to remove rt_info file");
      }

      // free resources and remove "up_to_date" mark
      reset ();

      break;
  }


  return status_;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::propagate_dispatches ()
{
  u_long i;
  frame_size_ = 1;

  // iterate through the ordered_task_entries_ array in order
  // from highest DFS finishing time to lowest, so that every
  // calling dispatch is accessed before those it calls:
  // the dispatches propagate top down through the call DAG
  for (i = 0; i < tasks (); ++i)
  {
    if (ordered_task_entries_ [i]->merge_dispatches (*dispatch_entries_) < 0)
    {
      return ST_UNKNOWN_TASK;
    }

    if (ordered_task_entries_ [i]->effective_period () > 0)
    {
      frame_size_ =
        minimum_frame_size (frame_size_,
                            ordered_task_entries_ [i]->effective_period ());
    }
    else
    {
      return ST_UNKNOWN_TASK;
    }

  }

  return SUCCEEDED;
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

    // only consider computation times of dispatches on OPERATION descriptors
    if ((ordered_dispatch_entries_ [i]->task_entry ().info_type () ==
          RtecScheduler::OPERATION) &&
        (ordered_dispatch_entries_ [i]->task_entry ().effective_period () > 0))
    {
      // Just use low 32 bits of worst_case_execution_time.  This will
      // have to change when CosTimeBase.idl is finalized.
      utilization_ +=
        ACE_static_cast(double,
                        ordered_dispatch_entries_ [i]->
                                                  task_entry ().rt_info ()->
                                                    worst_case_execution_time.low) /
        ACE_static_cast(double,
                        ordered_dispatch_entries_ [i]->
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
  tasks (rt_info_entries_.size ());

  // clear the decks of any previous scheduling information
  reset ();

  // allocate new table of task entries (wrappers for rt_infos)
  ACE_NEW_RETURN (task_entries_, Task_Entry [tasks ()],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // allocate new table of pointers to task entries (for sorting)
  ACE_NEW_RETURN (ordered_task_entries_, Task_Entry *[tasks ()],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);
  ACE_OS::memset (ordered_task_entries_, 0,
                  sizeof (Task_Entry *) * tasks ());

  // allocate new unbounded set for pointers to
  // task entries that delineate threads
  ACE_NEW_RETURN (thread_delineators_, ACE_Unbounded_Set <Dispatch_Entry *>,
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // allocate new unbounded set for pointers to dispatch entries
  ACE_NEW_RETURN (dispatch_entries_,
                  ACE_Unbounded_Set <Dispatch_Entry *>,
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
      return ST_UNKNOWN_TASK;
    }
    task_entries_ [i].rt_info (*info_entry);

    // tie rt_info to corresponding task entry
    task_entries_ [i].rt_info ()->volatile_token = (u_long) &(task_entries_ [i]);

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
        return ST_UNKNOWN_TASK;
      }

      // obtain a pointer to the Task_Entry from the dependency RT_Info
      Task_Entry *dependency_entry_ptr =
        (Task_Entry *) dependency_info->volatile_token;

      if (! dependency_entry_ptr)
      {
        return ST_UNKNOWN_TASK;
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
ACE_DynScheduler::identify_threads (void)
{
  u_int i, j;

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
        // have to change when CosTimeBase.idl is finalized.
        const TimeBase::ulonglong zero = {0, 0};
        for (j = 0; j < thread_count; j++)
        {
          Dispatch_Entry *dispatch_ptr;
          const TimeBase::ulonglong effective_period =
            {task_entries_ [i].effective_period (), 0};
          ACE_NEW_RETURN(dispatch_ptr,
                         Dispatch_Entry (zero,
                                         effective_period,
                                         task_entries_ [i].rt_info ()->preemption_priority,
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
      else
      {
        // node that no one calls and has neither rate nor threads is suspect
        ACE_ERROR_RETURN (
           (LM_ERROR,
            "An operation identified by \"%s\" does not specify a period or\n"
            "visible threads, and is not called by any other operation.  "
            "Are there backwards dependencies?\n",
            (const char*) task_entries_ [i].rt_info ()->entry_point),
            ST_BAD_DEPENDENCIES_ON_TASK);
      }
    }
  }

  return SUCCEEDED;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::check_dependency_cycles (void)
{
  status_t return_status = SUCCEEDED;

  // sort the pointers to entries in order of descending finish time
  ::qsort ((void *) ordered_task_entries_,
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
                  "Tasks \"%s\" and \"%s\" are part of a call cycle.\n",
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
ACE_DynScheduler::schedule_threads (void)
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
      return ST_UNKNOWN_TASK;
    }

    ordered_thread_dispatch_entries_ [i] = *dispatch_entry;
  }

  // sort the thread dispatch entries into priority order
  status_t status = sort_dispatches (ordered_thread_dispatch_entries_, threads_);

  if (status == SUCCEEDED)
  {
    // assign priorities to the thread dispatch entries
    status == assign_priorities (ordered_thread_dispatch_entries_, threads_);
  }

  return status;
}
  // thread scheduling method: sets up array of pointers to task
  // entries that are threads, calls internal thread scheduling method

ACE_DynScheduler::status_t
ACE_DynScheduler::schedule_dispatches (void)
{
  dispatch_entry_count_ = dispatch_entries_->size ();

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
      return ST_UNKNOWN_TASK;
    }

    ordered_dispatch_entries_ [i] = *dispatch_entry;
  }

  // sort the entries in order of priority and subpriority
  sort_dispatches (ordered_dispatch_entries_, dispatch_entry_count_);

  // assign dynamic and static subpriorities to the thread dispatch entries
  return assign_subpriorities (ordered_dispatch_entries_, dispatch_entry_count_);
}
  // dispatch scheduling method: sets up an array of dispatch entries,
  // calls internal dispatch scheduling method.

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

    // only put OPERATION dispatches into the timeline.
    if (ordered_dispatch_entries_[i]->task_entry().info_type () !=
                RtecScheduler::OPERATION)
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
        status = ST_UNKNOWN_TASK;
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


    // schedule additional dispatcheds of the entry
    // over the total frame size into the timeline
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
      // have to change when CosTimeBase.idl is finalized.
      const TimeBase::ulonglong arrival =
        {ordered_dispatch_entries_[i]->arrival ().low + current_frame_offset,
         0};
      const TimeBase::ulonglong deadline=
        {ordered_dispatch_entries_[i]->deadline ().low + current_frame_offset,
         0};

      ACE_NEW_RETURN (
        new_dispatch_entry,
        Dispatch_Entry (arrival,
                        deadline,
                        ordered_dispatch_entries_[i]->priority (),
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
          status = ST_UNKNOWN_TASK;
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
  FILE *file = ACE_OS::fopen (filename, "w");
  if (file)
  {
    status = output_dispatch_priorities (file);
    fclose (file);
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
    dispatch_count += frame_size_ / ordered_dispatch_entries_[i]->task_entry ().effective_period ();
  }

  if (ACE_OS::fprintf (
      file, "\n\nSCHEDULING RESULTS:\n\n"
            "Number of dispatches:              %3u\n"
            "Number of threads:                 %3u\n"
            "Number of tasks:                   %3u\n"
            "Scheduler Status:                    [%d] %s\n"
            "Total Frame Size:                    %lu nsec (%lf Hz)\n"
            "Critical Set Frame Size:             %lu nsec (%lf Hz)\n"
            "Utilization:                         %lf\n"
            "Critical Set Utilization:            %lf\n"
            "Minimum Priority Queue:            %3ld\n"
            "Minimum Guaranteed Priority Queue: %3ld\n"
            "Minimum Critical Priority:         %3ld\n\n\n"

            "DISPATCH PRIORITIES:\n\n"
            "                                  (critical              \n"
            "                                   instant)              \n"
            "             dispatch              dynamic      static   \n"
            "operation          ID  priority  subpriority  subpriority\n"
            "---------    --------  --------  -----------  -----------\n",
      dispatch_count, threads_, tasks_, status_,
      status_message(status_), frame_size_, (double) (10000000.0 / ((double) frame_size_)),
      critical_set_frame_size_, (double) (10000000.0 / ((double) critical_set_frame_size_)),
      utilization_, critical_set_utilization_, minimum_priority_queue_,
      minimum_guaranteed_priority_queue_, minimum_critical_priority ()) < 0)

  {
    return UNABLE_TO_WRITE_SCHEDULE_FILE;
  }

  for (i = 0; i < dispatch_entry_count_; ++i)
  {
    if (ACE_OS::fprintf (file, "%-11s  %8lu  %8lu  %11lu  %11lu\n",
              ordered_dispatch_entries_[i]->task_entry ().rt_info ()->entry_point.in (),
              ordered_dispatch_entries_[i]->dispatch_id (),
              ordered_dispatch_entries_[i]->priority (),
              ordered_dispatch_entries_[i]->dynamic_subpriority (),
              ordered_dispatch_entries_[i]->static_subpriority ()) < 0)
    {
      return UNABLE_TO_WRITE_SCHEDULE_FILE;
    }
  }

  return SUCCEEDED;
}


ACE_DynScheduler::status_t
ACE_DynScheduler::output_dispatch_timeline (const char *filename)
{
  status_t status = UNABLE_TO_OPEN_SCHEDULE_FILE;

  // open the file
  FILE *file = ACE_OS::fopen (filename, "w");
  if (file)
  {
    status = output_dispatch_timeline (file);
    fclose (file);
  }

  return status;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::output_dispatch_timeline (FILE *file)
{
  if (ACE_OS::fprintf (
      file, "\n\nDISPATCH TIMELINE:\n\n"
            "                dispatch  arrival  deadline     start        stop    execution     latency        laxity\n"
            "operation             ID   (nsec)    (nsec)    (nsec)      (nsec)  time (nsec)      (nsec)        (nsec)\n"
            "---------    -----------  -------  --------     -----      ------  -----------      -------       ------\n") < 0)
  {
    return UNABLE_TO_WRITE_SCHEDULE_FILE;
  }

  // iterate through timeline, picking out entries whose prev_ pointer
  // is null (i.e. those representing the start of a dispatch), find end
  // of dispatch, output the operation, dispatch, priority, and time info
  ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> iter (*timeline_);
  for (iter.first (); iter.done () == 0; iter.advance ())
  {
    TimeLine_Entry_Link *link;
    if ((iter.next (link) == 0) || (! link))
    {
      return ST_BAD_INTERNAL_POINTER;
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

      // Just use low 32 bits of worst_case_execution_time.  This will
      // have to change when CosTimeBase.idl is finalized.
      const ACE_UINT32 tmp =
        last_entry->stop () - link->entry ().arrival () -
          link->entry ().dispatch_entry ().task_entry ().rt_info ()->
            worst_case_execution_time.low;
      if (link->entry ().dispatch_entry ().original_dispatch ())
      {
        if (ACE_OS::fprintf (
              file, "%-11s  [%4lu] %4lu  %7lu  %8lu  %8lu  %10lu  %11lu   %10ld   %10ld\n",
              link->entry ().dispatch_entry ().task_entry ().rt_info ()->
                entry_point.in (),
              link->entry ().dispatch_entry ().original_dispatch ()->dispatch_id (),
              link->entry ().dispatch_entry ().dispatch_id (),
              link->entry ().arrival (),
              link->entry ().deadline (),
              link->entry ().start (), last_entry->stop (),
              link->entry ().dispatch_entry ().task_entry ().rt_info ()->
                worst_case_execution_time,
              tmp,
              link->entry ().deadline () - last_entry->stop ()) < 0)

        {
          return UNABLE_TO_WRITE_SCHEDULE_FILE;
        }
      }
      else
      {
        if (ACE_OS::fprintf (
              file, "%-11s  %11lu  %7lu  %8lu  %8lu  %10lu  %11lu   %10ld   %10ld\n",
              link->entry ().dispatch_entry ().task_entry ().rt_info ()->
                entry_point.in (),
              link->entry ().dispatch_entry ().dispatch_id (),
              link->entry ().arrival (),
              link->entry ().deadline (),
              link->entry ().start (), last_entry->stop (),
              link->entry ().dispatch_entry ().task_entry ().rt_info ()->
                worst_case_execution_time,
              tmp,
              link->entry ().deadline () - last_entry->stop ()) < 0)

        {
          return UNABLE_TO_WRITE_SCHEDULE_FILE;
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
  FILE *file = ACE_OS::fopen (filename, "w");
  if (file)
  {
    status = output_preemption_timeline (file);
    fclose (file);
  }

  return status;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::output_preemption_timeline (FILE *file)
{
  if (ACE_OS::fprintf (
        file, "\n\nPREEMPTION TIMELINE:\n\n"
        "              dispatch     start      stop  \n"
        "operation           ID    (nsec)    (nsec)\n"
        "---------  -----------    ------    ------\n") < 0)
  {
    return UNABLE_TO_WRITE_SCHEDULE_FILE;
  }

  ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> iter (*timeline_);

  TimeLine_Entry_Link *link;
  for (iter.first (); iter.done () == 0; iter.advance ())
  {
    if ((iter.next (link) == 0) || (! link))
    {
      return ST_BAD_INTERNAL_POINTER;
    }

    if (link->entry ().dispatch_entry ().original_dispatch ())
    {
      if (ACE_OS::fprintf (
            file, "%-9s  [%4lu] %4lu  %8lu  %8lu\n",
            link->entry ().dispatch_entry ().task_entry ().rt_info ()->
              entry_point.in (),
            link->entry ().dispatch_entry ().original_dispatch ()->dispatch_id (),
            link->entry ().dispatch_entry ().dispatch_id (),
            link->entry ().start (),
            link->entry ().stop ()) < 0)
      {
        return UNABLE_TO_WRITE_SCHEDULE_FILE;
      }
    }
    else
    {
      if (ACE_OS::fprintf (
            file, "%-9s  %11lu  %8lu  %8lu\n",
            link->entry ().dispatch_entry ().task_entry ().rt_info ()->
              entry_point.in (),
            link->entry ().dispatch_entry ().dispatch_id (),
            link->entry ().start (),
            link->entry ().stop ()) < 0)
      {
        return UNABLE_TO_WRITE_SCHEDULE_FILE;
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
  FILE *file = ACE_OS::fopen (filename, "w");
  if (file)
  {
    status = output_dispatch_timeline (file);
    fclose (file);
  }

  return status;
}

ACE_DynScheduler::status_t
ACE_DynScheduler::output_viewer_timeline (FILE *file)
{
  if (ACE_OS::fprintf (
      file, "\n\nVIEWER TIMELINE:\n\n"
            "                                    arrival  deadline   completion    execution  \n"
            "operation  utilization   overhead    (nsec)    (nsec)  time (nsec)  time (nsec)\n"
            "---------  -----------   --------   -------  --------  -----------  -----------\n") < 0)
  {
    return UNABLE_TO_WRITE_SCHEDULE_FILE;
  }

  // iterate through timeline, picking out dispatches in chronological
  // order of operation completion time
  int entries_remain = 1;
  u_long accumulated_execution = 0;
  u_long current_accumulated_execution = 0;
  u_long last_completion = 0;
  u_long current_completion = 0;
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
             (current_completion == 0)))
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
            file, "%-11s  %9lf  %9lf  %8lu  %8lu  %11lu  %11lu\n",
            current_entry->dispatch_entry ().task_entry ().rt_info ()->
              entry_point.in (),
            (double) (((double) current_accumulated_execution) /
                      ((double) current_completion)),
            0.0,
            current_entry->arrival (),
            current_entry->deadline (),
            current_last_entry->stop (),
            current_entry->dispatch_entry ().task_entry ().rt_info ()->
              worst_case_execution_time) < 0)
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
  }

  if (status == SUCCEEDED)
  {
    // open the file
    file = ACE_OS::fopen (filename, "w");
    if (! file)
    {
      status = UNABLE_TO_OPEN_SCHEDULE_FILE;
    }
  }

  if ((status == SUCCEEDED) && (heading))
  {
    if (ACE_OS::fprintf (file, "%s\n\n", heading) < 0)
    {
      status = UNABLE_TO_WRITE_SCHEDULE_FILE;
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
    fclose (file);
  }

  return status;
}
  // this prints the entire set of timeline outputs to the specified file

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Map_Entry<ACE_CString, RtecScheduler::RT_Info *>;
template class ACE_Map_Manager<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>;
template class ACE_Map_Iterator_Base<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>;
template class ACE_Map_Iterator<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>;
template class ACE_DNode<Dispatch_Entry_Link>;
template class ACE_DNode<Dispatch_Proxy_Iterator *>;
template class ACE_DNode<TimeLine_Entry_Link>;
template class ACE_Node<RtecScheduler::RT_Info *>;
template class ACE_Node<Task_Entry_Link *>;
template class ACE_Ordered_MultiSet<Dispatch_Entry_Link>;
template class ACE_Ordered_MultiSet<Dispatch_Proxy_Iterator *>;
template class ACE_Ordered_MultiSet<TimeLine_Entry_Link>;
template class ACE_Ordered_MultiSet_Iterator<Dispatch_Entry_Link>;
template class ACE_Ordered_MultiSet_Iterator<Dispatch_Proxy_Iterator *>;
template class ACE_Ordered_MultiSet_Iterator<TimeLine_Entry_Link>;
template class ACE_Unbounded_Queue<Dispatch_Entry *>;
template class ACE_Unbounded_Queue_Iterator<Dispatch_Entry *>;
template class ACE_Unbounded_Set<RtecScheduler::RT_Info *>;
template class ACE_Unbounded_Set<Task_Entry_Link *>;
template class ACE_Unbounded_Set_Iterator<RtecScheduler::RT_Info *>;
template class ACE_Unbounded_Set_Iterator<Task_Entry_Link *>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Map_Entry<ACE_CString, RtecScheduler::RT_Info *>
#pragma instantiate ACE_Map_Manager<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_CString, RtecScheduler::RT_Info *, ACE_Thread_Mutex>
#pragma instantiate ACE_DNode<Dispatch_Entry_Link>
#pragma instantiate ACE_DNode<Dispatch_Proxy_Iterator *>
#pragma instantiate ACE_DNode<TimeLine_Entry_Link>
#pragma instantiate ACE_Node<RtecScheduler::RT_Info *>
#pragma instantiate ACE_Node<Task_Entry_Link *>
#pragma instantiate ACE_Ordered_MultiSet<Dispatch_Entry_Link>
#pragma instantiate ACE_Ordered_MultiSet<Dispatch_Proxy_Iterator *>
#pragma instantiate ACE_Ordered_MultiSet<TimeLine_Entry_Link>
#pragma instantiate ACE_Ordered_MultiSet_Iterator<Dispatch_Entry_Link>
#pragma instantiate ACE_Ordered_MultiSet_Iterator<Dispatch_Proxy_Iterator *>
#pragma instantiate ACE_Ordered_MultiSet_Iterator<TimeLine_Entry_Link>
#pragma instantiate ACE_Unbounded_Queue<Dispatch_Entry *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<Dispatch_Entry *>
#pragma instantiate ACE_Unbounded_Set<RtecScheduler::RT_Info *>
#pragma instantiate ACE_Unbounded_Set<Task_Entry_Link *>
#pragma instantiate ACE_Unbounded_Set_Iterator<RtecScheduler::RT_Info *>
#pragma instantiate ACE_Unbounded_Set_Iterator<Task_Entry_Link *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// EOF
