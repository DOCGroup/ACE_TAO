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

#include <math.h>
#include <float.h>

#include "ace/Sched_Params.h"
#include "DynSched.h"

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
// class ACE_Scheduler member functions //
//////////////////////////////////////////

const char *
ACE_Scheduler::status_message (ACE_Scheduler::status_t status)
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


ACE_Scheduler::ACE_Scheduler () 
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
  , ordered_dispatch_entries_ (0)
  , dispatch_entry_count_ (0)
  , threads_ (0)
  , rt_info_entries_ ()
  , lock_ ()
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
  , timeline_ (0)
  , up_to_date_ (0)

{
}


ACE_Scheduler::~ACE_Scheduler ()
{
  // release all resources used for the most recent schedule
  reset ();
}


void
ACE_Scheduler::init (const OS_Priority minimum_priority,
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

ACE_Scheduler::status_t
ACE_Scheduler::register_task (RT_Info *rt_info, handle_t &handle)
{
  ACE_Scheduler::status_t ret;

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



ACE_Scheduler::status_t
ACE_Scheduler::get_rt_info (Object_Name name,
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



int ACE_Scheduler::number_of_dependencies(RT_Info* rt_info)
{
  return rt_info->dependencies.length();
}

int ACE_Scheduler::number_of_dependencies(RT_Info& rt_info)
{
  return rt_info.dependencies.length();
}

int ACE_Scheduler::add_dependency(RT_Info* rt_info,
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
                  d.dependency_type, rt_info->entry_point));
      return -1;      
      break;
  }
  
  ACE_DEBUG ((LM_DEBUG, "adding dependecy to: %s\n",
	      (const char*)temp_info->entry_point));

  RtecScheduler::Dependency_Set& set = temp_info->dependencies;
  int l = set.length();
  set.length(l + 1);
  set[l] = d;
  return 0;
}

void ACE_Scheduler::export(RT_Info* info, FILE* file)
{
  export(*info, file);
}

void ACE_Scheduler::export(RT_Info& info, FILE* file)
{
  // The divide-by-1 is for ACE_U_LongLong support.
  (void) ACE_OS::fprintf (file,
                          "%s\n%d\n%ld\n%ld\n%ld\n%ld\n%d\n%d\n%ld\n%u\n"
                          "# begin calls\n%d\n",
                          (const char*)info.entry_point,
                          info.handle,
                          info.worst_case_execution_time / 1,
                          info.typical_execution_time / 1,
                          info.cached_execution_time / 1,
                          info.period,
                          info.criticality,
                          info.importance,
                          info.quantum / 1,
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


ACE_Scheduler::status_t
ACE_Scheduler::lookup_rt_info (handle_t handle,
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
ACE_Scheduler::reset ()
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

ACE_Scheduler::status_t
ACE_Scheduler::schedule (void)
{
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
    status_ = create_timeline ();
  }

  // store the timeline to a file if one was given
  if ((timeline_filename_ != 0)  &&
      ((status_ == SUCCEEDED) || (status_ == ST_UTILIZATION_BOUND_EXCEEDED)))
  {
    status_ = output_timeline (timeline_filename_, 0);
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
      if (timeline_filename_ && unlink ((char *) timeline_filename_) &&
          errno != ENOENT)
      {
        ACE_OS::perror ("ACE_Scheduler::schedule (); "
                        "unable to remove timeline file");
      }
      if (runtime_filename_ && unlink ((char *) runtime_filename_) &&
          errno != ENOENT)
      {
        ACE_OS::perror ("ACE_Scheduler::schedule (); "
                        "unable to remove schedule file");
      }
      if (rt_info_filename_  &&  unlink ((char *) rt_info_filename_)  &&
          errno != ENOENT)
      {
        ACE_OS::perror ("ACE_Scheduler::schedule (); "
                        "unable to remove rt_info file");
      }
      
      // free resources and remove "up_to_date" mark
      reset ();

      break;
  }


  return status_;
}

ACE_Scheduler::status_t 
ACE_Scheduler::propagate_dispatches ()
{
  // iterate through the ordered_task_entries_ array in order 
  // from highest DFS finishing time to lowest, so that every
  // calling dispatch is accessed before those it calls:
  // the dispatches propagate top down through the call DAG
  for (u_int i = 0; i < tasks (); ++i)
  {
    if (ordered_task_entries_ [i]->merge_dispatches (*dispatch_entries_) < 0)
    {
      return ST_UNKNOWN_TASK;
    }
  }

  return SUCCEEDED;
}
// propagate the dispatch information from the
// threads throughout the call graph



ACE_Scheduler::status_t
ACE_Scheduler::calculate_utilization_params (void)
{
  status_t status = SUCCEEDED;  

  frame_size_ = 
    ordered_dispatch_entries_ [0]->task_entry ().effective_period ();

  critical_set_frame_size_ = 0;
  utilization_ = 0.0;
  critical_set_utilization_ = 0.0;

  minimum_priority_queue_ = 
    ordered_dispatch_entries_ [0]->priority ();

  minimum_guaranteed_priority_queue_ = -1;

  // iterate through ordered task entries, calculating frame size, utilization
  for (u_int i = 1; i < dispatch_entry_count_; ++i)
  {
    // hold the effective period of the task entry to which the dispatch refers
    Period period =       
      ordered_dispatch_entries_ [i]->task_entry ().effective_period ();

    // if we've just finished examining another priority level
    if (minimum_priority_queue_ != ordered_dispatch_entries_ [i]->priority ())
    {
      // update parameters for the previous priority level
      update_priority_level_params ();
    
      // update the minimum priority queue
      minimum_priority_queue_ = ordered_dispatch_entries_ [i]->priority ();
    }

    // only consider computation times of dispatches on OPERATION descriptors
    if (ordered_dispatch_entries_ [i]->task_entry ().info_type () == 
          RtecScheduler::OPERATION)
    {
      utilization_ += 
        ACE_static_cast(double, 
                        ordered_dispatch_entries_ [i]->
						  task_entry ().rt_info ()->
						    worst_case_execution_time) /
        ACE_static_cast(double, period);

	 }

    if (period)
    {
      // grow frame size by new factors that are not in gcd of the 
      // frame size and the newly considered entry's effective period
      frame_size_ = 
        minimum_frame_size (frame_size_, period);
	 }
  }

  // update parameters for the lowest priority level
  update_priority_level_params ();

  return status;
}


void
ACE_Scheduler::update_priority_level_params ()
{
  // if we've just finished examining the lowest critical priority level
  if (minimum_priority_queue_ == minimum_critical_priority ())
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

ACE_Scheduler::status_t
ACE_Scheduler::setup_task_entries (void)
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

ACE_Scheduler::status_t
ACE_Scheduler::relate_task_entries (void)
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

ACE_Scheduler::status_t
ACE_Scheduler::relate_task_entries_recurse (long &time, Task_Entry &entry)
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

ACE_Scheduler::status_t
ACE_Scheduler::identify_threads (void)
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
        for (j = 0; j < thread_count; j++)
        {
          Dispatch_Entry *dispatch_ptr;
          ACE_NEW_RETURN(dispatch_ptr,
                         Dispatch_Entry (0, task_entries_ [i].effective_period (),
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

ACE_Scheduler::status_t
ACE_Scheduler::check_dependency_cycles (void)
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


ACE_Scheduler::status_t
ACE_Scheduler::check_dependency_cycles_recurse (Task_Entry &entry)
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
                  (*calling_entry_link)->caller ().rt_info ()->entry_point,
                  entry.rt_info ()->entry_point));

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


ACE_Scheduler::status_t 
ACE_Scheduler::schedule_threads (void) 
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

ACE_Scheduler::status_t
ACE_Scheduler::schedule_dispatches (void)
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

ACE_Scheduler::status_t
ACE_Scheduler::create_timeline ()
{

  ACE_NEW_RETURN(timeline_, ACE_Ordered_MultiSet <TimeLine_Entry_Link>,
	             ST_VIRTUAL_MEMORY_EXHAUSTED);

  for (u_int i = 0; i < dispatch_entry_count_; ++i)
  {
    // only put OPERATION dispatches into the timeline.
    if (ordered_dispatch_entries_[i]->task_entry().info_type () != 
		RtecScheduler::OPERATION)
    {
      continue;
    }
    

    // timeline entries cover the execution time of the dispatch
    u_long remaining_time = 
      ordered_dispatch_entries_[i]->task_entry().rt_info ()->
        worst_case_execution_time;

    // initialize last stop time to beginning of frame 
    u_long last_stop = 0;

    TimeLine_Entry *last_entry = 0;
    TimeLine_Entry *current_entry = 0;
    ACE_Ordered_MultiSet_Iterator <TimeLine_Entry_Link> iter (*timeline_);
    for (iter.first (); (remaining_time > 0) && (iter.done () == 0); 
         iter.advance ())
    {
      TimeLine_Entry_Link *link;
      if ((iter.next (link) == 0) || (! link))
      {
        return ST_BAD_INTERNAL_POINTER;
      }

      // if there's room, schedule a new timeline entry for the dispatch
      if (link->entry ().start() > last_stop)
      {
        ACE_NEW_RETURN (
          current_entry,
          TimeLine_Entry (
            *(ordered_dispatch_entries_[i]),
            last_stop,
            (((remaining_time + last_stop) < link->entry ().start())
               ? (remaining_time + last_stop) : link->entry ().start()),
            0, last_entry),
          ST_VIRTUAL_MEMORY_EXHAUSTED);
  
        // patch up the pointers within the list of entries for this dispatch
        if (last_entry)
        {
          last_entry->next (current_entry); 
        }
        last_entry = current_entry; 

        timeline_->insert(TimeLine_Entry_Link(*current_entry));

        // update the remaining time and last stop values
        remaining_time -= ((remaining_time < (link->entry ().start() - last_stop))
                            ? remaining_time : (link->entry ().start() - last_stop));
		}

      // update the last stop time 
      last_stop = link->entry ().stop ();
    }    

    // if there is still dispatch time remaining, and we've 
    // reached the end of the list, insert what's left
    if (remaining_time > 0)
    {
      ACE_NEW_RETURN (
        current_entry,
        TimeLine_Entry (
          *(ordered_dispatch_entries_[i]),
          last_stop,
          remaining_time + last_stop,
          0, last_entry),
        ST_VIRTUAL_MEMORY_EXHAUSTED);
  
      // patch up the pointers within the list of entries for this dispatch
      if (last_entry)
      {
        last_entry->next (current_entry); 
      }

      timeline_->insert(TimeLine_Entry_Link(*current_entry));
    }

  }

  return SUCCEEDED;
}
  // Create a timeline.

ACE_Scheduler::status_t
ACE_Scheduler::output_dispatch_timeline (const char *filename)
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

ACE_Scheduler::status_t
ACE_Scheduler::output_dispatch_timeline (FILE *file)
{
  if (ACE_OS::fprintf (
      file, "\n\nSCHEDULING RESULTS:\n\n"
            "Number of dispatches:              %u\n"
            "Number of threads:                 %u\n"
            "Number of tasks:                   %u\n"
            "Scheduler Status:                  [%d] %s\n"
            "Total Frame Size:                  %lu nsec (%lf Hz)\n"
            "Critical Set Frame Size:           %lu nsec (%lf Hz)\n"
            "Utilization:                       %lf\n"
            "Critical Set Utilization:          %lf\n"
            "Minimum Priority Queue:            %ld\n"
            "Minimum Guaranteed Priority Queue: %ld\n\n\n"
            "DISPATCH TIMELINE:\n\n"
            "           dispatch            dynamic      static       arrival  deadline     start      stop    execution\n"
            "operation  ID        priority  subpriority  subpriority  (nsec)   (nsec)       (nsec)     (nsec)  time (nsec)\n"
            "---------  --------  --------  -----------  -----------  -------  --------     -----      ------  -----------\n\n",
      dispatch_entry_count_, threads_, tasks_, status_, 
      status_message(status_), frame_size_, (double) (10000000.0 / ((double) frame_size_)), 
      critical_set_frame_size_, (double) (10000000.0 / ((double) critical_set_frame_size_)),
      utilization_, critical_set_utilization_, minimum_priority_queue_,
      minimum_guaranteed_priority_queue_) < 0)
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

    // for each timeline entry that starts a new dispatch
    if (link->entry ().prev () == 0)
    {
      // find the last time slice for the dispatch
      TimeLine_Entry *last_entry = &(link->entry ());
      while (last_entry->next ())
      {
        last_entry = last_entry->next ();
		}

      if (ACE_OS::fprintf (
            file, "%-9s  %8lu  %8lu  %11lu  %11lu  %7lu  %8lu  %8lu  %8lu  %10lu\n",
            link->entry ().dispatch_entry ().task_entry ().rt_info ()->
              entry_point,
            link->entry ().dispatch_entry ().dispatch_id (), 
            link->entry ().dispatch_entry ().priority (), 
            link->entry ().dispatch_entry ().dynamic_subpriority (), 
            link->entry ().dispatch_entry ().static_subpriority (), 
            link->entry ().dispatch_entry ().arrival (), 
            link->entry ().dispatch_entry ().deadline (), 
            link->entry ().start (), last_entry->stop (),
            link->entry ().dispatch_entry ().task_entry ().rt_info ()->
              worst_case_execution_time) < 0)

      {
        return UNABLE_TO_WRITE_SCHEDULE_FILE;
      }
    }
  }

  return SUCCEEDED;
}
  // this prints the entire set of timeline outputs to the specified file

ACE_Scheduler::status_t
ACE_Scheduler::output_preemption_timeline (const char *filename)
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

ACE_Scheduler::status_t
ACE_Scheduler::output_preemption_timeline (FILE *file)
{
  if (ACE_OS::fprintf (
        file, "\n\nPREEMPTION TIMELINE:\n\n"
        "                          start     stop\n"
        "operation  dispatch id    (nsec)    (nsec)\n"
        "---------  -----------    ------    ------\n\n") < 0)
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

    if (ACE_OS::fprintf (
          file, "%-9s  %11lu  %8lu  %8lu\n", 
          link->entry ().dispatch_entry ().task_entry ().rt_info ()->
            entry_point,
          link->entry ().dispatch_entry ().dispatch_id (), 
          link->entry ().start (),
          link->entry ().stop ()) < 0)
    {
      return UNABLE_TO_WRITE_SCHEDULE_FILE;
    }
  }

  return SUCCEEDED;
}
  // this prints the entire set of timeline outputs to the specified file

ACE_Scheduler::status_t 
ACE_Scheduler::output_timeline (const char *filename, const char *heading)
{
  status_t status = SUCCEEDED;
  FILE *file;

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
    status = output_dispatch_timeline (file);
  }

  if (status == SUCCEEDED)
  {
    status = output_preemption_timeline (file);
  }

  if (file)
  {
    fclose (file);
  }

  return status;
}
  // this prints the entire set of timeline outputs to the specified file



// CDG - TBD - need to compile on Sun OS5 and make sure
// the correct template instantiations are present

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX>;
template class ACE_Map_Entry<ACE_CString, ACE_Scheduler::RT_Info **>;

  template class ACE_Lock_Adapter<ACE_SYNCH_RW_MUTEX>;
  template class ACE_Lock_Adapter<ACE_SYNCH_MUTEX>;
  template class ACE_Map_Manager<ACE_CString,
				 ACE_Scheduler::RT_Info **,
                                 ACE_SYNCH_MUTEX>;
  template class ACE_Map_Iterator_Base<ACE_CString, ACE_Scheduler::RT_Info **,
                                       ACE_SYNCH_MUTEX>;
  template class ACE_Map_Iterator<ACE_CString, ACE_Scheduler::RT_Info **,
                                  ACE_SYNCH_MUTEX>;
  template class ACE_Map_Reverse_Iterator<ACE_CString, ACE_Scheduler::RT_Info **,
                                  ACE_SYNCH_MUTEX>;
  template class ACE_Read_Guard<ACE_SYNCH_MUTEX>;
  template class ACE_Write_Guard<ACE_SYNCH_MUTEX>;

  template class ACE_Ordered_MultiSet<Dispatch_Entry_Link>;
  template class ACE_Ordered_MultiSet_Iterator<Dispatch_Entry_Link>;
  template class ACE_DNode<Dispatch_Entry_Link>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_CString, ACE_Scheduler::RT_Info **>

#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_CString, ACE_Scheduler::RT_Info **, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Read_Guard<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Write_Guard<ACE_SYNCH_MUTEX>

#pragma instantiate ACE_Ordered_MultiSet<Dispatch_Entry_Link>;
#pragma instantiate ACE_Ordered_MultiSet_Iterator<Dispatch_Entry_Link>;
#pragma instantiate ACE_DNode<Dispatch_Entry_Link>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// EOF

