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

#include "math.h"
#include "float.h"

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

  ACE_Scheduler::Task_Entry *first_entry = 
	  * ACE_static_cast (ACE_Scheduler::Task_Entry **, first);
  ACE_Scheduler::Task_Entry *second_entry = 
	  * ACE_static_cast (ACE_Scheduler::Task_Entry **, second);

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

// TBD - move this to the ACE class
// Euclid's greatest common divisor algorithm
u_long gcd (u_long x, u_long y)
{
  if (y = 0)
  {
    return x;
  }
  else
  {
    return gcd (y, x % y);
  }
}


// TBD - move this to the ACE class
// calculate the minimum frame size that 
u_long minimum_frame_size (u_long period1, u_long period2)
{
  // first, find the greatest common divisor of the two periods
  u_long greatest_common_divisor = gcd (period1, period2);

  // explicitly consider cases to reduce risk of possible overflow errors
  if (greatest_common_divisor == 1)
  {
    // periods are relative primes: just multiply them together
    return period1 * period2;
  }
  else if (greatest_common_divisor == period1)
  {
    // the first period divides the second: return the second
    return period2;
  }
  else if (greatest_common_divisor == period2)
  {
    // the second period divides the first: return the first
    return period1;
  }
  else
  {
    // the current frame size and the entry's effective period
    // have a non-trivial greatest common divisor: return the
    // product of factors divided by those in their gcd.
    return (period1 * period2) / greatest_common_divisor;
  }
}

//////////////////////////////////////////
// class ACE_Scheduler member functions //
//////////////////////////////////////////

void
ACE_Scheduler::output (FILE *file, const status_t status)
{
  switch (status)
  {
    case NOT_SCHEDULED :
      ACE_OS::fprintf (file, "NOT_SCHEDULED");
      break;
    case SUCCEEDED :
      ACE_OS::fprintf (file, "SUCCEEDED");
      break;
    case ST_TASK_ALREADY_REGISTERED :
      ACE_OS::fprintf (file, "TASK_ALREADY_REGISTERED");
      break;
    case ST_BAD_DEPENDENCIES_ON_TASK :
      ACE_OS::fprintf (file, "BAD_DEPENDENCIES_ON_TASK");
      break;
    case ST_NULL_DEPENDENCY_POINTER :
      ACE_OS::fprintf (file, "NULL_DEPENDENCY_POINTER");
      break;
    case ST_VIRTUAL_MEMORY_EXHAUSTED :
      ACE_OS::fprintf (file, "VIRTUAL_MEMORY_EXHAUSTED");
      break;
    case ST_UNKNOWN_TASK :
      ACE_OS::fprintf (file, "UNKNOWN_TASK");
      break;
    case TASK_COUNT_MISMATCH :
      ACE_OS::fprintf (file, "TASK_COUNT_MISMATCH");
      break;
    case INVALID_PRIORITY :
      ACE_OS::fprintf (file, "INVALID_PRIORITY");
      break;

    // The following are only used during scheduling (in the case of
    // off-line scheduling, they are only used prior to runtime).
    // To save a little code space (280 bytes on g++ 2.7.2/Solaris 2.5.1),
    // we could conditionally compile them so that they're not in the
    // runtime version.
    case ST_UTILIZATION_BOUND_EXCEEDED :
      ACE_OS::fprintf (file, "UTILIZATION_BOUND_EXCEEDED");
      break;
    case ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS :
      ACE_OS::fprintf (file, "INSUFFICIENT_THREAD_PRIORITY_LEVELS");
      break;
    case ST_CYCLE_IN_DEPENDENCIES :
      ACE_OS::fprintf (file, "CYCLE_IN_DEPENDENCIES");
      break;
    case ST_INVALID_PRIORITY_ORDERING :
      ACE_OS::fprintf (file, "INVALID_PRIORITY_ORDERING");
      break;

    case UNABLE_TO_OPEN_SCHEDULE_FILE :
      ACE_OS::fprintf (file, "UNABLE_TO_OPEN_SCHEDULE_FILE");
      break;
    case UNABLE_TO_WRITE_SCHEDULE_FILE :
      ACE_OS::fprintf (file, "UNABLE_TO_WRITE_SCHEDULE_FILE");
      break;
    // End of config-only status values.

    default:
      ACE_OS::fprintf (file, "UNKNOWN STATUS: %d", status);
  }
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
  , ordered_thread_entries_ (0)
  , dispatch_entries_ (0)
  , ordered_dispatch_entries_ (0)
  , rt_info_entries_ ()
  , lock_ ()
  , handles_ (0)
  , runtime_filename_ (0)
  , rt_info_filename_ (0)
  , timeline_filename_ (0)
  , tasks_ (0)
  , threads_ (0)
  , status_ (NOT_SCHEDULED)
  , output_level_ (0)
  , frame_size_ (0)
  , critical_set_frame_size_ (0)
  , utilization_ (0.0)
  , critical_set_utilization_ (0.0)
  , minimum_priority_queue_ (0)
  , minimum_guaranteed_priority_queue_ (-1)
  , timeline_ ()
  , up_to_date_ (0)
{
}


ACE_Scheduler::~ACE_Scheduler ()
{
  // release all resources used for the most recent schedule
  reset ();
}


void
ACE_Scheduler::init (const int minimum_priority,
                     const int maximum_priority,
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

          // reset the schedule when a new task is registered
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

// CDG - TBD - we'll need to update this to use the information aggregated
//             within the task entry pointed to by the RT_Info's volatile_token 
//             ACT (in fact, there is now more than one
//             priority assignment per RT_Info, w/ disjunction on multiple 
//             priority levels, rates, etc. - iterate through and show each dispatch)

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

    delete [] ordered_thread_entries_;
    ordered_thread_entries_ = 0;

    // free all the 

    delete dispatch_entries_;
    dispatch_entries_ = 0;

    delete [] ordered_dispatch_entries_;
    ordered_dispatch_entries_ = 0;

    delete timeline_;
    timeline_ = 0;

    threads_ = 0;
    status_ = NOT_SCHEDULED;
    frame_size_ = 0;
    critical_set_frame_size_ = 0;
    utilization_ = 0.0;
    critical_set_utilization_ = 0.0;
    minimum_priority_queue_ = 0;
    minimum_guaranteed_priority_queue_ = -1;
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
    // assume this is all going to work unless proven otherwise
    up_to_date_ = 1;

    // save the total number of registered RT_Infos
    tasks (rt_info_entries_.size ());
  }

  // set up the task entry data structures, check for call cycles
  ACE_Scheduler::status_t status = setup_task_entries ();

  if (status == SUCCEEDED)
  {
    // check for cycles in the dependency graph: as a side effect, leaves
    // the ordered_task_entries_ pointer array sorted in topological order, 
    // which is used by propagate_dispatches () to ensure that dispatches
    // are propagated top down in the call graph.
    status = check_dependency_cycles ();
  }

  if (status == SUCCEEDED)
  {
    // task entries are related, now threads can be found
    status = identify_threads ();
  }

  if (status == SUCCEEDED)
  {
    // invokes the internal thread scheduling method of the strategy
    status = schedule_threads ();
  }

  if (status == SUCCEEDED)
  {
    // propagate the dispatch information from the 
    // threads throughout the call graph
    status = propagate_dispatches ();
  }

  if (status == SUCCEEDED)
  {
    // invokes the internal dispatch scheduling method of the strategy
    status = schedule_dispatches ();
  }

  // calculate utilization, frame size, critical set
  if (status == SUCCEEDED)
  {
    status = calculate_utilization_params ();
  }

  // store the timeline to a file if one was given
  if (rt_info_filename_ != 0  &&
      (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
  {
    status = store_rt_infos (rt_info_filename_);
  }
  
  // store the schedule of dispatch entries to a file if one was given
  if (runtime_filename_ != 0  &&
      (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
  {
    status = store_schedule (runtime_filename_ );
  }

  // store the timeline to a file if one was given
  if (timeline_filename_ != 0  &&
      (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
  {
    status = create_timeline (timeline_filename_);
  }


  // if a valid schedule was not generated, clean up from the attempt
  switch (status)
  {
    // these are statuses that indicate a reasonable schedule was generated
    case SUCCEEDED:
    case ST_UTILIZATION_BOUND_EXCEEDED:

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


  return status;
}

ACE_Scheduler::status_t 
ACE_Scheduler::propagate_dispatches ()
{
  // iterate through the ordered_task_entries_ array in order 
  // from highest DFS finishing time to lowest, so that every
  // calling dispatch is accessed before those it causes:
  // the dispatches propagate top down through the call DAG
  for (u_int i = 0; i < tasks (); ++i)
  {
    ordered_task_entries_ [i]->merge_calls (*dispatch_entries_);
  }
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
  critical_set_utilization_ = 0;

  minimum_priority_queue_ = 
    ordered_dispatch_entries_ [0]->priority ();

  minimum_guaranteed_priority_queue_ = -1;

  // iterate through ordered task entries, calculating frame size, utilization
  for (u_int i = 1; i < tasks (); ++i)
  {
    // save the effective period of the task entry to which the dispatch refers
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

    // 
    if (ordered_dispatch_entries_ [i]->task_entry ().info_type () == 
          RtecScheduler::OPERATION)
    {
      utilization_ += ordered_dispatch_entries_ [i]->execution_time () / 
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

// CDG - TBD - walk dependencies and make sure there are no *conjunctive* 
// dependencies by critical set entries on non-guaranteed entries: as long
// as there is *any* path entirely though guaranteed operations to a thread
// delineator, all is well: if not, kick and scream.
//
// Part Deux:
//
// Hmmm... does this have meaning anymore, w/ the notion of priority 
// inheritance? i.e., clients are not assigned priority, but rather
// obtain it from their one-way call dependencies.  We could complain
// if there is a priority/rate/etc specified that doesn't match (or is lower?)

  return status;
}


void
ACE_Scheduler::update_priority_level_params ()
{
  // if we've just finished examining the lowest critical priority level
  if (minimum_priority_queue_ == strategy->minimum_critical_priority ())
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
  ACE_NEW_RETURN (thread_delineators_, ACE_Unbounded_Set <Task_Entry *>,
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // allocate new unbounded set for pointers to dispatch entries
  ACE_NEW_RETURN (dispatch_entries_,
                  ACE_Unbounded_Set <Dispatch_Entry *>,
                  ST_VIRTUAL_MEMORY_EXHAUSTED);


  // set up links between rt_info_entries_, task_entries_,
  // and ordered_task_entries_ tables
  ACE_Unbounded_Set_Iterator <RT_Info *> iter (rt_info_entries_);
  for (u_int i = 0; i <= tasks (); ++i, iter.advance ())
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

  for (u_int i = 0; i <= tasks (); ++i)
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
  // walk array of task entries, picking out thread delineators
  for (u_int i = 0; i <= tasks (); ++i)
  {
    // if entry has exposed threads
    if (task_entries_ [i].rt_info ()->threads > 0)
    {
      task_entries_ [i].is_thread_delineator (1);
      thread_delineators_->insert (& (task_entries_ [i]));
    }
    // or if entry does not have any callers 
    else if (task_entries_ [i].callers ().is_empty ())
    {
      // and period is valued: it's a thread delineator
      if (task_entries_ [i].rt_info ()->period > 0)
      {
        task_entries_ [i].is_thread_delineator (1);
        thread_delineators_->insert (& (task_entries_ [i]));
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
  // allocate an array of pointers to the thread delineators
  ACE_NEW_RETURN (ordered_thread_entries_,
                  Dispatch_Entry * [thread_delineators_.size ()],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // copy pointers to the thread delineators from the set to the array
  ACE_Unbounded_Set_Iterator <Dispatch_Entry *> iter (thread_delineators_);
  for (u_int i = 0; i <= thread_delineators_.size (); ++i, iter.advance ())
  {
    Task_Entry** task_entry;

    if (! iter.next (task_entry))
    {
      return ST_UNKNOWN_TASK;
    }

    ordered_thread_entries_ [i] = *task_entry;
  }

  return schedule_threads_i ();
}
  // thread scheduling method: sets up array of pointers to task 
  // entries that are threads, calls internal thread scheduling method

ACE_Scheduler::status_t
ACE_Scheduler::schedule_dispatches (void)
{
  ACE_NEW_RETURN (ordered_dispatch_entries_,
                  Dispatch_Entry * [dispatch_entries_.size ()],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  ACE_Unbounded_Set_Iterator <Dispatch_Entry *> iter (dispatch_entries_);
  for (u_int i = 0; i <= dispatch_entries_.size (); ++i, iter.advance ())
  {
    Dispatch_Entry** dispatch_entry;

    if (! iter.next (dispatch_entry))
    {
      return ST_UNKNOWN_TASK;
    }

    ordered_dispatch_entries_ [i] = *dispatch_entry;
  }

  return schedule_dispatches_i ();
}
  // dispatch scheduling method: sets up an array of dispatch entries,
  // calls internal dispatch scheduling method.


ACE_Scheduler::Task_Entry::Task_Entry ()
  : rt_info_ (0)
  , effective_execution_time_(0)
  , effective_period_(0)
  , dfs_status_ (NOT_VISITED)
  , discovered_ (-1)
  , finished_ (-1)
  , is_thread_delineator_ (0)
  , calls_ ()
  , callers_ ()
{
}

ACE_Scheduler::Task_Entry::~Task_Entry () 
{
  // zero out the task entry ACT in the corresponding rt_info
  rt_info_->volatile_token = 0;
}

// merge calls according to info type, update
// relevant scheduling characteristics for this entry
void 
ACE_Scheduler::Task_Entry::merge_calls (ACE_Unbounded_Set <Dispatch_Entry *> &dispatches)
{
  switch (dependency_type ())
  {
    case DISJUNCTION:
      disjunctive_merge (ACE_Unbounded_Set <Dispatch_Entry *> &dispatches);
      break;

    default:
      conjunctive_merge (ACE_Unbounded_Set <Dispatch_Entry *> &dispatches);
      break;
  }
}

// CDG - TBD think about this: do previously dispatched 
// operations get subtracted from the frame time remaining ?
// yes!!!

// perform disjunctive merge of arrival times of oneway calls:
// all arrival times of all dependencies are duplicated by the
// multiplier and repetition over the new frame size and merged
void 
ACE_Scheduler::Task_Entry::disjunctive_merge (ACE_Unbounded_Set <Dispatch_Entry *> &dispatches)
{
CDG - TBD - write this
CDG - TBD - add each new dispatch created to the set of dispatches
}

// perform conjunctive merge of arrival times of dependencies:
// all arrival times of all dependencies are duplicated by the
// multiplier and repetition over the new frame size and then
// iteratively merged by choosing the maximal arrival time at
// the current position in each queue (iteration is in lockstep
// over all queues, and ends when any queue ends).
void 
ACE_Scheduler::Task_Entry::conjunctive_merge (ACE_Unbounded_Set <Dispatch_Entry *> &dispatches)
{
CDG - TBD
}

//////////////////////////
// Class Dispatch Entry //
//////////////////////////

u_long ACE_Scheduler::Dispatch_Entry::next_id = 0;

ACE_Scheduler::Dispatch_Entry::Dispatch_Entry (
      Priority priority,
      Time arrival,
      Time deadline,
      Time execution_time,
      Task_Entry &task_entry, 
      Dispatch_Entry *previous_instance)

  : priority_ (priority)
  , arrival_ (arrival)
  , deadline_ (deadline)
  , execution_time_ (execution_time)
  , task_entry_ (task_entry)
  , previous_dispatch_ (0)
{
  // obtain, increment the next id
  dispatch_id_ = next_id_++;
}

ACE_Scheduler::Dispatch_Entry::Dispatch_Entry (const Dispatch_Entry &d)
  : priority_ (d.priority)
  , arrival_ (d.arrival)
  , deadline_ (d.deadline)
  , execution_time_ (d.execution_time)
  , task_entry_ (d.task_entry)
  , previous_dispatch_ (0)
{
  // obtain, increment the next id
  dispatch_id_ = next_id_++;
}


///////////////////////////
// Class Task Entry Link //
///////////////////////////


ACE_Scheduler::Task_Entry_Link::Task_Entry_Link (
  ACE_Scheduler::Task_Entry &caller,
  ACE_Scheduler::Task_Entry &called,
  CORBA::Long number_of_calls,
  RtecScheduler::Dependency_Type dependency_type) 
  : caller_ (caller)
  , called_ (called)
  , dependency_type_ (dependency_type)
  , number_of_calls_ (number_of_calls) 
{
}


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

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// EOF

