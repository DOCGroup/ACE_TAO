// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    Scheduler_Internal.cpp
//
// = CREATION DATE
//    23 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#include "math.h"   // for ::pow ()
#include "float.h"  // for DBL_EPSILON

#include "ace/Sched_Params.h"

#include "Scheduler_Internal.h"

#if ! defined (__ACE_INLINE__)
#include "Scheduler_Internal.i"
#endif /* __ACE_INLINE__ */


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// static functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Structure for storing the RT_Info information for each task, per mode.
struct Mode_Entry
{
  RtecScheduler::RT_Info *rt_info_;
  u_long start_time_; // microseconds
  u_long stop_time_;  // microseconds

  Mode_Entry() :
    rt_info_ (0),
    start_time_ (0),
    stop_time_ (0)
  {
  }

  Mode_Entry(RtecScheduler::RT_Info *const rt_info,
             const u_long start_time = 0,
             const u_long stop_time = 0) :
    rt_info_ (rt_info),
    start_time_ (start_time),
    stop_time_ (stop_time)
  {
  }

  ~Mode_Entry () {}

  Mode_Entry &operator= (const Mode_Entry &entry)
  {
    if (this != &entry)
      {
        rt_info_ = entry.rt_info_;
        start_time_ = entry.start_time_;
        stop_time_ = entry.stop_time_;
      }

    return *this;
  }
};


// "Add" an RT_Info to another.
static RtecScheduler::RT_Info&
operator+= (RtecScheduler::RT_Info *rt_info1,
	    const RtecScheduler::RT_Info &rt_info2)
{
  rt_info1->worst_case_execution_time = (rt_info1->worst_case_execution_time
					 + rt_info2.worst_case_execution_time);
  rt_info1->typical_execution_time = (rt_info1->typical_execution_time
				      + rt_info2.typical_execution_time);
  rt_info1->cached_execution_time = (rt_info1->cached_execution_time
				     + rt_info2.cached_execution_time);

  if ((rt_info1->period > rt_info2.period && rt_info2.period > 0)
      || rt_info1->period <= 0)
    {
      rt_info1->period = rt_info2.period;
    }

  return *rt_info1;
}


// Sort the threads into the sorted_rt_info array,
// by decreasing (non-increasing, actually) period.
// Returns the number of tasks in the input set.
static
void
sort (Scheduler_Internal::Thread_Map &threads,
      u_long number_of_threads,
      Mode_Entry sorted_rt_info [],
      u_int output_level)
{
  u_int tasks = 0;

  ACE_OS::memset (sorted_rt_info, 0, (size_t) number_of_threads);

  // Iterate over each of the RT_Info entries that we know about,
  // sorting the entries by decreasing (non-increasing) period.
  Scheduler_Internal::Thread_Map_Entry *entry;
  Scheduler_Internal::Thread_Map_Iterator i (threads);
  while (i.next (entry))
  {
    i.advance ();
    RtecScheduler::RT_Info &rt_info = *entry->int_id_;
    const RtecScheduler::Time entry_time = rt_info.worst_case_execution_time;
    const RtecScheduler::Period entry_period = rt_info.period;

    if (output_level >= 1)
      {
        ACE_OS::printf ("Thread \"%s\"; utilization is %g and period is %ld"
                        " usec (%g Hz)\n",
                        (const char*)rt_info.entry_point,
                        entry_period > 0
#if defined (ACE_WIN32) || defined (ACE_HAS_LONGLONG_T)
                          ? (double) entry_time / entry_period
#else
                          ? (double) entry_time.lo () / entry_period
#endif /* ACE_WIN32 || ACE_HAS_LONGLONG_T */
                          : 0,
                        entry_period / 10 /* usec/100 ns */,
                        entry_period > 0
                          ? 1.0e7 /* 1/100 ns */ / entry_period
                          : 0.0);
      }

    // Use selection sort to sort entries by period.  It's O (n^2), but n
    // shouldn't be very big.
    u_int j;
    for (j = 0; j < tasks; ++j)
      {
        if (entry_period > sorted_rt_info [j].rt_info_->period)
          {
            // Found a smaller period:  since we are sorting by decreasing
            // period, need to insert the current period here.

            if (output_level >= 2)
              {
                ACE_OS::printf ("insert task before task %u with period %ld\n",
                                j + 1,
                                sorted_rt_info [j].rt_info_->period /
                                  10 /* usec/100 ns */);
              }

            // insert this entry into the sort list here
            for (u_int k = tasks; k > j; --k)
              {
                sorted_rt_info [k] = sorted_rt_info [k - 1];
              }
            sorted_rt_info [j].rt_info_ = &rt_info;

            break;
          }
      }
    if (j >= tasks)
      {
        // The entry does not have a lower period than any that we've
        // already seen, so append it to the end of the sorted RT_Info array.
        if (output_level >= 2)
          {
            ACE_OS::printf ("insert task with period %ld at end of sorted "
                              "array\n",
                            entry_period / 10 /* usec/100 ns */);
          }
        sorted_rt_info [j].rt_info_ = &rt_info;
      }

    ++tasks;
  }
}


// Check for harmonically related periods by comparing this entry's period
// with that of each of the other entries for integral division.
// As a side effect, figures out the frame_size, in microsec.
static
int
harmonically_related_periods (Mode_Entry const sorted_rt_info [],
                              const u_int number_of_tasks,
                              u_long &frame_size)
{
  int harmonically_related = 1;
  u_int i;

  if (number_of_tasks > 0)
    {
      // start frame_size with the longest period

      for (i = 0; i < number_of_tasks; ++i)
      frame_size = (u_long) (sorted_rt_info [0].rt_info_->period /
                               10 /* microsec/100 ns */);
    }

  for (i = 0; i < number_of_tasks - 1; ++i)
    {
      // Skip over groups of array entries with the same period.
      while (i > 0  &&  i < number_of_tasks - 1  &&
             sorted_rt_info [i].rt_info_->period ==
             sorted_rt_info [i - 1].rt_info_->period)
        {
          ++i;
        }

      // Compare the current RT_Info entry, at location i in the
      // sorted array, to each of the remoining (with higher array index)
      // entries.
      for (u_int j = i + 1; j < number_of_tasks; ++j)
        {
          // if the period is 0, skip the task
          if (sorted_rt_info [j].rt_info_->period <= 0)
            break;

          // Assumes that the sorted_rt_info array is in decreasing
          // (non-increasing) order.
          double quotient = (double) sorted_rt_info [i].rt_info_->period /
                                     sorted_rt_info [j].rt_info_->period;

          if (quotient - (long) quotient > DBL_EPSILON)
            {
              harmonically_related = 0;

              // and add another factor to the frame_size
              frame_size *= (u_long) (sorted_rt_info [i].rt_info_->period /
                                        10 /* microseconds/100 ns */);
            }
        }
    }

  return harmonically_related;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Scheduler_Internal member functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Scheduler_Internal::Scheduler_Internal () :
  Scheduler (),
  handles_ (0),
  // Set the minimum priority to that for the current platform.  This
  // shouldn't be necessary, but UPSingleProcessorOrb::initialize_reactors
  // creates threads before the Event Channel calls Scheduler::init ().
  minimum_priority_ (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                     ACE_SCOPE_THREAD)),
  increasing_priority_ (-1),
  task_entries_ (),
  ordered_info_ (0),
  visited_ (0),
  dependencies_ (0),
  roots_ (0),
  frame_size_ (0),
  thread_info_ (0),
  leaf_info_ (0),
  timeline_ (0)
{
}


Scheduler_Internal::~Scheduler_Internal ()
{
  reset ();

  delete [] timeline_;
  timeline_ = 0;

  for (u_int current_mode = 0; current_mode < modes (); ++current_mode)
    {
      // Iterate over each of the thread_info_ entries and delete them.
      Thread_Map_Entry *entry;
      Thread_Map_Iterator i (thread_info_ [current_mode]);
      while (i.next (entry))
      {
        i.advance ();
        thread_info_ [current_mode].unbind (entry->ext_id_);
        delete entry->int_id_;
      }
    }

  for (u_int task = 0; task < tasks (); ++task)
    {
      delete [] ordered_info_ [task];
    }

  delete [] thread_info_;
  thread_info_ = 0;

  delete [] ordered_info_;
  ordered_info_ = 0;

  delete [] frame_size_;
  frame_size_ = 0;
}


void
Scheduler_Internal::reset ()
{
  delete dependencies_;
  dependencies_ = 0;

  delete roots_;
  roots_ = 0;

  delete leaf_info_;
  leaf_info_ = 0;
}

Scheduler::status_t
Scheduler_Internal::lookup_rt_info (handle_t handle,
				    RT_Info*& rtinfo)
{
  if (handle < 0 || handle > task_entries_.size ())
    {
      return ST_UNKNOWN_TASK;
    }
  RT_Info*** entry;
  ACE_Unbounded_Set_Iterator <RT_Info **> i (task_entries_);
  while (i.next (entry) != 0)
    {
      i.advance ();
      RT_Info** array = *entry;
      if (array[0]->handle == handle)
	{
	  rtinfo = array[0];
	  return SUCCEEDED;
	}
    }
       
  return ST_UNKNOWN_TASK;
}
     

Scheduler::status_t
Scheduler_Internal::register_task (RT_Info *rt_info [],
                                   const u_int number_of_modes,
                                   handle_t &handle)
{
  status_t ret;

  // try to store the new task's information . . .
  switch (task_entries_.insert (rt_info))
    {
      case 0 : // successfully inserted
        {
          rt_info [0]->handle = (handle = ++handles_);

          // assigned the same handle to the RT_Info for each of its modes
          for (u_int i = 1; i < number_of_modes; ++i)
            {
              if (rt_info [i] != 0)
		rt_info [i]->handle = handle;
            }

          if (number_of_modes > modes ())
            {
              modes (number_of_modes);
            }

          ret = SUCCEEDED;

          if (output_level () >= 5)
            {
              ACE_OS::printf ("registered task \"%s\" with RT_Info starting "
                                "at %X\n",
                              (const char*)rt_info[0]->entry_point,
                              (void *) rt_info[0]);
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


void
Scheduler_Internal::init (const int minimum_priority,
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


Scheduler::status_t
Scheduler_Internal::schedule (void)
{
  ACE_Guard<LOCK> ace_mon (lock_);

  // here goes . . .

  increasing_priority_ = maximum_priority_ >= minimum_priority_;

  status_t status = NOT_SCHEDULED;

  // store number of tasks, based on registrations
  tasks (task_entries_.size ());

  // allocate tables . . .
  ACE_NEW_RETURN (ordered_info_, RT_Info **[tasks ()],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);
  for (u_int task = 0; task < tasks (); ++task)
    {
      ACE_NEW_RETURN (ordered_info_ [task], RT_Info *[modes ()],
                      ST_VIRTUAL_MEMORY_EXHAUSTED);
      ACE_OS::memset (ordered_info_ [task], 0,
                      sizeof (RT_Info *) * modes ());
    }

  ACE_NEW_RETURN (thread_info_, Thread_Map [modes ()],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  ACE_NEW_RETURN (frame_size_, u_long [modes ()],
		  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // set up timeline structure
  if (timeline_filename_ != 0)
    {
      ACE_NEW_RETURN (timeline_,
                      ACE_Unbounded_Queue <Timeline_Entry> [modes()],
                      ST_VIRTUAL_MEMORY_EXHAUSTED);
    }

  for (u_int current_mode = 0; current_mode < modes (); ++current_mode)
    {
      // set the current mode
      mode (current_mode);

      status_t mode_status = NOT_SCHEDULED;

      // Identify threads and store information in convenient form for later
      // use.
      if ((mode_status = identify_threads ()) != SUCCEEDED)
        {
          return mode_status;  // should only happen if virtual memory exceeded
        }

      // Traverse task dependencies to aggregate thread parameters.
      if ((mode_status = aggregate_thread_parameters ()) != SUCCEEDED)
        {
          return mode_status;  // should only happen if virtual memory exceeded
        }

      // only RMS is currently supported
      if ((mode_status = schedule_rms (minimum_priority_,
                                       maximum_priority_)) == SUCCEEDED)
        {
          if (status == NOT_SCHEDULED) status = SUCCEEDED;
        }
      else
        {
          status = mode_status;
        }
    }

  if (output_level () > 0)
    {
      print_schedule ();
    }

  if (runtime_filename_ != 0  &&
      (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
    {
      status = store_schedule (runtime_filename_);
    }

  if (timeline_filename_ != 0  &&
      (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
    {
      status = create_timelines (timeline_filename_);
    }

  if ((status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED)  &&  rt_info_filename_)
    {
      status = store_rt_info (rt_info_filename_);
    }

  // If there was a failure, (try to) remove the output files.
  if (! (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED))
    {
      if (runtime_filename_ && unlink ((char *) runtime_filename_)
	  &&  errno != ENOENT)
        {
          ACE_OS::perror ("Scheduler_Internal::schedule (); "
                          "unable to remove schedule file");
        }
      if (rt_info_filename_  &&  unlink ((char *) rt_info_filename_)  &&
          errno != ENOENT)
        {
          ACE_OS::perror ("Scheduler_Internal::schedule (); "
                          "unable to remove rt_info file");
        }
    }

  return status;
}


Scheduler::status_t
Scheduler_Internal::update_dependencies (RT_Info &rt_info,
                                         Sub_Priority subpriority)
{
  u_int i = 0;

  // Detect cycles in the dependency graph by storing (the address of)
  // each RT_Info instance as it is visited.  Then, on every visit,
  // check to see if the instance had already been visited.  Flag it,
  // if so (and stop the recursion :-).

  if (output_level () >= 4)
    {
      ACE_OS::printf ("update_dependencies; visiting \"%s\"",
                      (const char*)rt_info.entry_point);
    }

  switch (visited_->insert (&rt_info))
    {
      case -1 :
        return ST_VIRTUAL_MEMORY_EXHAUSTED;
      case 0 :
        // successfully inserted
        break;
      case 1 :
      default :
        // oops, had already visited this rt_info!
#if 0 /* Ignore, it seems to work well without this check */
        return CYCLE_IN_DEPENDENCIES;
#else
        break;
#endif /* 0 */
    }

  Scheduler::status_t ret = NOT_SCHEDULED;

  if (rt_info.priority == -1)
    {
      // Just updating the subpriority.

      int *ordered_deps = 0;
      int *marked = 0;

      // Sort the dependencies by importance, lowest first.
      ACE_NEW_RETURN (ordered_deps, int [number_of_dependencies (rt_info)],
                      ST_VIRTUAL_MEMORY_EXHAUSTED);
      ACE_NEW_RETURN (marked, int [number_of_dependencies (rt_info)],
                      ST_VIRTUAL_MEMORY_EXHAUSTED);
      for (i = 0; i < number_of_dependencies (rt_info); ++i)
        {
          ordered_deps [i] = -1;
          marked [i] = 0;
        }

      for (i = 0; i < number_of_dependencies (rt_info); ++i)
        {
          Sub_Priority current_importance = 0x7FFF;
          int current_importance_dep = -1;

          // Find the unmarked dependent with the lowest importances.
          for (u_int j = 0; j < number_of_dependencies (rt_info); ++j)
            {
              ACE_ASSERT (rt_info.dependencies[j].rt_info != -1);
	      RT_Info* info;
	      this->lookup_rt_info (rt_info.handle, info);
              if (! marked [j]  &&
                  info->importance <= current_importance)
                {
                  current_importance = info->importance;
                  current_importance_dep = j;
                }
            }

          ACE_ASSERT (current_importance_dep != -1);
          ordered_deps [i] = current_importance_dep;
          marked [current_importance_dep] = 1;
        }

      if (output_level () >= 5)
        {
          ACE_OS::printf ("\"%s\" has %d dependencies\n",
                          (const char*)rt_info.entry_point,
                          number_of_dependencies(rt_info));
          for (i = 0; i < number_of_dependencies(rt_info); ++i)
            {
	      RT_Info* info;
	      this->lookup_rt_info (rt_info.dependencies[ordered_deps[i]].rt_info, info);
              ACE_OS::printf ("ordered dependency %d: \"%s\" (importance: %d)\n",
                              i,
                              (const char*)info->entry_point,
			      info->importance);
            }
        }

      // Traverse the dependent tasks, in order of importance.
      for (i = 0; i < number_of_dependencies (rt_info); ++i)
        {
	  RT_Info* dep_info;
	  this->lookup_rt_info (rt_info.dependencies[ordered_deps[i]].rt_info,
				dep_info);
          if (i > 0)
	    {
	      RT_Info* previous_info;
	      this->lookup_rt_info (rt_info.dependencies[ordered_deps [i-1]].rt_info,
				    previous_info);
	      if (previous_info->importance < dep_info->importance)
		{
		  // Increment subpriority based on increased importance
		  // of this dependent.
		  ++subpriority;
		}
	    }

              if (output_level () >= 4)
                {
		  ACE_OS::printf ("update_dependencies (); "
				  "incremented subpriority to %u\n",
				  subpriority);
		}

          dep_info->subpriority = subpriority;
          if (output_level () >= 4)
            {
                ACE_OS::printf ("update_dependencies (); "
                                  "set subpriority of \"%s\"o %u\n",
                                (const char*)dep_info->entry_point,
                                subpriority);
            }

          // Recurse until all dependent tasks have been visited.
          if (output_level () >= 4)
            {
              ACE_OS::printf ("update_dependencies (); "
                                "calling recursively for dependent \"%s\"\n",
                              (const char*)dep_info->entry_point);
            }

          if ((ret = update_dependencies (*dep_info,
                                          subpriority + 1)) != NOT_SCHEDULED)
            {
              break;
            }
	}

      delete [] ordered_deps;
      delete [] marked;
    }
  else
    {
      for (i = 0; i < number_of_dependencies (rt_info); ++i)
        {
	  RT_Info* info;
          this->lookup_rt_info (rt_info.dependencies[i].rt_info, info);
	  info->priority = rt_info.priority;
          if (output_level () >= 4)
            {
                ACE_OS::printf ("update_dependencies (); "
				"set priority of \"%s\" to %d "
				"(its subpriority is %u)\n", 
                                (const char*)info->entry_point,
                                info->priority,
                                info->subpriority);
            }

          if (output_level () >= 4)
            {
              ACE_OS::printf ("update_dependencies (); "
			      "calling recursively for dependent \"%s\"\n",
                              (const char*)info->entry_point);
            }

          // Recurse until all dependent tasks have been visited.
          if ((ret = update_dependencies (*info,
                                          0 /* not used when 
                                               setting priorities */)) !=
              NOT_SCHEDULED)
            {
              break;
            }
        }
    }

  return ret;
}


Scheduler::status_t
Scheduler_Internal::schedule_rms (const int minimum_priority,
                                  const int maximum_priority)
{
  double utilization = 0.0;
  u_int i;

  // Allocate an array for the sorted RT_Info.
  Mode_Entry *sorted_rt_info;
  ACE_NEW_RETURN (sorted_rt_info, Mode_Entry [threads ()],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // Sort the threads' thread info entries by period.
  sort (thread_info_ [mode ()], threads (), sorted_rt_info, output_level ());

  // Load priorities by walking the sorted rt_info_array, incrementing
  // current_priority for each new period (rate).  For threads af the same
  // rate, assign subpriorities based on 1) data dependencies and
  // 2) importance.  Also, calculate CPU utilization on the fly.
  ACE_Sched_Priority current_priority = minimum_priority;
  for (i = 0; i < threads (); ++i)
    {
      RT_Info &rt_info = *sorted_rt_info [i].rt_info_;
	  const RtecScheduler::Time entry_time = rt_info.worst_case_execution_time;
      const RtecScheduler::Period entry_period = rt_info.period;

      if (entry_period > DBL_EPSILON)
        {
#if defined (ACE_WIN32) || defined (ACE_HAS_LONGLONG_T)
          utilization += (double) entry_time / entry_period;
#else
          utilization += (double) entry_time.lo () / entry_period;
#endif /* ACE_WIN32 || ACE_HAS_LONGLONG_T */
        }

      if (i != 0  &&  output_level () >= 3)
        {
	  
          ACE_OS::printf ("entry_period: %d (\"%s\"), "
                          "previous thread period: %d\n",
                          entry_period,
                          (const char*)sorted_rt_info [i].rt_info_->entry_point,
                          sorted_rt_info [i - 1].rt_info_->period);
        }

      // If i == 0, we're at the starting point:  there are no other threads
      // to compare to.
      if (i != 0  &&  entry_period != sorted_rt_info [i - 1].rt_info_->period)
        {
          // If the period of this task is the same as the period of the
          // previous task (the threads are sorted by period), then assign
          // it the same ("current") priority.  Otherwise, assign a higher
          // priority (which may have a lower value), via "current_priority".
          if (current_priority == maximum_priority)
            {
              status (ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS);
            }
          else
            {
              if (output_level () >= 3)
                {
                  ACE_OS::printf ("will advance current_priority from its "
                                  "current %d for period of %d\n",
                                  current_priority, entry_period);
                }

              if (increasing_priority_)
                {
                  // This assumes that the target and host are the
                  // same platform!  It's hear to deal with the
                  // non-contiguous OS priorities of Win32 platforms.
                  current_priority =
		    ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
						     current_priority,
						     ACE_SCOPE_PROCESS);
                }
              else
                {
                  // This assumes that the OS priorities on the target
                  // are contiguous, e.g., 255, 254, 253, 252, etc.,
                  // from low to high priority.  The only target we
                  // use with decreasing priorities, VxWorks, does have
                  // contiguous OS priorities.
                  --current_priority;
                }
            }
        }

      if (output_level () >= 3)
        {
          ACE_OS::printf ("thread %s, set priority to %d\n",
                          (const char*)rt_info.entry_point,
			  current_priority);
        }

      rt_info.priority  = current_priority;

      // There should only be one dependency for each
      // RT_Info instance in the (sorted) thread_info_ array.
      if (number_of_dependencies (rt_info) != 1)
	{
	  ACE_ERROR ((LM_ERROR,
		      "On '%s' deps count should be 1, it is %d\n",
		      (const char*)rt_info.entry_point,
		      number_of_dependencies(rt_info)));
	}

      // . . . and it should have a valid RT_Info pointer.
      ACE_ASSERT (rt_info.dependencies[0].rt_info != -1);
      RT_Info* tmp;
      this->lookup_rt_info (rt_info.dependencies[0].rt_info, tmp);
      tmp->priority  = current_priority;
    }


  // Set the priority of every task.
  ACE_Bounded_Set_Iterator <RT_Info *> root_i (*roots_);
  RT_Info **root;
  while (root_i.next (root) != 0)
    {
      root_i.advance ();
      if (output_level () >= 4)
        {
          ACE_OS::printf ("ROOT: %s\n",
			  (const char*)(*root)->entry_point);
        }

      RT_Info *leaf = 0;
      leaf_info_->find ((const char*)(*root)->entry_point, leaf);
      ACE_ASSERT (leaf != 0);

      if (output_level () >= 4)
        {
          ACE_OS::printf ("found leaf_info entry %s\n",
                          (const char*)leaf->entry_point);
        }

      // Find the highest priority leaf corresponding to each root.
      // ???? should do that!

      (*root)->priority = leaf->priority;

      // Update the priority of all dependent tasks.
      // update_dependencies () is recursive, so set up the visited_
      // structure outside of it.
      ACE_NEW_RETURN (visited_,
                      ACE_Bounded_Set <const RT_Info *> (tasks ()),
                      ST_VIRTUAL_MEMORY_EXHAUSTED);

      if (status () == NOT_SCHEDULED  /* first mode */  ||
          status () == SUCCEEDED      /* subsequent modes */ )
        {
          if (output_level () >= 4)
            {
              ACE_OS::printf ("update_dependencies (); call for "
                                "root %s to set priorities\n",
                              (const char*)(*root)->entry_point);
            }

          status (update_dependencies (**root, 0 /* not used when
                                                    setting priorities */));
        }

      delete visited_;
      visited_ = 0;
    }

  // Set minimum_priority, the priority value of the lowest priority
  // (highest numbered) dispatch queue.  This is global, over all modes,
  // so don't overwrite a smaller value.
  if (increasing_priority_)
    {
      if (current_priority - minimum_priority >
            (int) minimum_priority_queue ())
        {
          if (output_level () >= 3)
            {
              ACE_OS::printf ("set minimum_priority_queue to %d = "
                              " %d - %d\n",
                              current_priority - minimum_priority,
                              current_priority, minimum_priority);
            }
          minimum_priority_queue (current_priority - minimum_priority);
        }
    }
  else
    {
      if (minimum_priority - current_priority >
            (int) minimum_priority_queue ())
        {
          if (output_level () >= 3)
            {
              ACE_OS::printf ("set minimum_priority_queue to %d = "
                              " %d - %d\n",
                              current_priority - minimum_priority,
                              current_priority, minimum_priority);
            }
          minimum_priority_queue (minimum_priority - current_priority);
        }
    }

  // Load RT_Info pointers into ordered_info_ array for efficient lookup.
  RT_Info ***entry;
  ACE_Unbounded_Set_Iterator <RT_Info **> task_entries_i (task_entries_);
  while (task_entries_i.next (entry) != 0)
  {
    task_entries_i.advance ();
    if ((*entry) [mode ()] != 0)
      {
        // This mode has a non-zero RT_Info pointer.
        ordered_info_ [(*entry) [mode ()]->handle - 1][mode ()] =
          (*entry) [mode ()];
      }
    // else, just leave the ordered_info_ entry for this [task][mode] at 0.
  }

  const u_int harmonically_related =
    harmonically_related_periods (sorted_rt_info,
                                  threads (),
                                  frame_size_ [mode ()]);

  if (timeline_filename_ != 0)
    {
      status_t timeline_status;
      if ((timeline_status = generate_timeline (sorted_rt_info,
                                                threads (),
                                                frame_size_ [mode ()],
                                                timeline_ [mode()])) !=
          SUCCEEDED)
        {
          status (timeline_status);
        }
    }

  delete [] sorted_rt_info;
  sorted_rt_info = 0;

  // Calculate the utilization bound, based on whether or not the
  // task periods are harmonically related.
  double utilization_bound;

  if (harmonically_related)
    {
      utilization_bound = 1.0;
    }
  else
    {
      utilization_bound = threads () * (::pow ((double) 2, 1./threads ()) - 1);
    }

  // Don't overwrite status if an error had already been noted.
  if (status () == NOT_SCHEDULED)
    {
      // Compare the total utilization to the utilization bound
      // to determine whether or not the threads are schedulable.
      status (utilization <= utilization_bound
	      ?  SUCCEEDED
	      :  ST_UTILIZATION_BOUND_EXCEEDED);
    }

  // ???? if utilization bound is exceeeded, then should try all
  // possible task phasings (RT test)

  if (output_level () >= 1)
    {
      ACE_OS::printf ("mode %u:  utilization bound: %g; "
                        "total CPU utilization: %g: ",
                      mode (),
                      utilization_bound,
                      utilization);
      Scheduler::output (stdout, status ());
      ACE_OS::printf ("\n\n");
    }

  return status ();
}


u_int
Scheduler_Internal::find_number_of_threads (mode_t mode)
{
  u_int number_of_threads = 0;

  // Quickly traverse RT_Info's to determine number of
  // threads, so that the thread_ array can be allocated below.
  RT_Info ***entry;
  ACE_Unbounded_Set_Iterator <RT_Info **> task_entries_i (task_entries_);
  while (task_entries_i.next (entry) != 0)
  {
    task_entries_i.advance ();
    RT_Info *rt_info = (*entry) [mode];

    // This test misses thread delineators that are 0 in a particular
    // mode.
    if (rt_info != 0  &&
        (rt_info->threads > 0 ||
         (number_of_dependencies (rt_info) == 0
	  && rt_info->period > 0)))
      {
        // This mode's RT_Info instance either has more than 0
        // "internal" threads, or delineates a thread because it
        // has no dependencies and has a non-zero period.
	int nt = rt_info->threads > 0 ?  rt_info->threads : 1;
        number_of_threads += nt;
	if (output_level () >= 2)
	  {
	    ACE_OS::printf ("find_number_of_threads (): mode %u; entry %s; "
                            "added %u threads\n",
			    mode,
			    (const char*)rt_info->entry_point,
			    nt);
	  }
      }
  }

  if (output_level () >= 1)
    {
      ACE_OS::printf ("find_number_of_threads (): mode %u; "
                        "identified %lu threads\n",
                      mode, number_of_threads);
    }

  return number_of_threads;
}


Scheduler::status_t
Scheduler_Internal::find_thread_delineators (mode_t mode)
{
  ACE_NEW_RETURN (dependencies_,
                  ACE_Bounded_Set <const RT_Info *> (tasks ()),
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // Fill in the thread_ array.  While doing this, load the
  // dependencies set also.
  RT_Info ***entry;
  ACE_Unbounded_Set_Iterator <RT_Info **> task_entries_i (task_entries_);
  while (task_entries_i.next (entry) != 0)
  {
    task_entries_i.advance ();
    RT_Info *rt_info = (*entry) [mode];

    if (rt_info != 0)
      {
        if ((rt_info->threads > 0 ||
             (number_of_dependencies (rt_info) == 0 &&
              rt_info->period > 0)))
          {
            // This mode's RT_Info instance either has more than 0
            // "internal" threads, or delineates a thread because it
            // has no dependencies and has a non-zero period.

            if (output_level () >= 5)
              {
                ACE_OS::printf ("find_thread_delineators (); %s is a leaf and"
                                " has %u threads\n",
                                (const char*)rt_info->entry_point,
				rt_info->threads);
              }

#if 1
	    // TODO: This seems to break several invariants on the
	    // RT_Info collection, I must talk to David about it.

            // Add one thread for non-active objects, and the number of
            // threads otherwise.
            for (u_int i = 0;
                 i < (rt_info->threads > 0 ? rt_info->threads : 1);
                 ++i)
              {
                RT_Info *thread_info;
                // Set "name" of thread to be that of its root RT_Info.
                ACE_NEW_RETURN (thread_info, RT_Info,
                                ST_VIRTUAL_MEMORY_EXHAUSTED);
		thread_info->entry_point = rt_info->entry_point;
		thread_info->threads = 0;
		thread_info->period = 0;
		Dependency_Info tmp;
		tmp.number_of_calls = 0;
		tmp.rt_info  = rt_info->handle;
                Scheduler::add_dependency (thread_info, tmp);

                if (output_level () >= 4)
                  {
                    ACE_OS::printf ("bind thread delineator %s\n",
                                    (const char*)thread_info->entry_point);
                  }

                thread_info_ [mode].bind ((const char*)rt_info->entry_point,
                                          thread_info);
              }
#endif
          }

        for (u_int dep = 0;
             dep < number_of_dependencies (rt_info);
             ++dep)
          {
	    RT_Info* info;
	    this->lookup_rt_info (rt_info->dependencies[dep].rt_info, info);
            if (output_level () >= 4)
              {
                ACE_OS::printf ("insert dependency %s\n",
                                (const char*)info->entry_point);

              }
            if (dependencies_->insert (info) == -1)
              {
		return ST_VIRTUAL_MEMORY_EXHAUSTED;
	      }
          }

      }
  }

  return SUCCEEDED;
}


Scheduler::status_t
Scheduler_Internal::find_dependency_chain_roots (mode_t mode)
{
  ACE_NEW_RETURN (roots_,
                  ACE_Bounded_Set <RT_Info *> (tasks ()),
                  ST_VIRTUAL_MEMORY_EXHAUSTED);

  // Now do the fun traversal through all RT_Infos to look for roots of
  // dependency trees.  From each of those roots, traverse the dependency
  // tree back to each leaf, which is one of the thread delineators that
  // was found earlier.
  RT_Info ***entry;
  ACE_Unbounded_Set_Iterator <RT_Info **> task_entries_i (task_entries_);
  while (task_entries_i.next (entry) != 0)
  {
    task_entries_i.advance ();
    RT_Info *rt_info = (*entry) [mode];

    if (rt_info != 0  &&  dependencies_->find (rt_info) == -1)
      {
        if (output_level () >= 5)
          {
            ACE_OS::printf ("\"%s\" is not a dependent task (it is a "
                              "dependency-chain root); it has "
                              "%u dependencies\n",
                            (const char*)rt_info->entry_point,
                            number_of_dependencies (rt_info));
          }

        if (output_level () >= 4)
          {
            ACE_OS::printf ("insert root %s\n",
                            (const char*)rt_info->entry_point);
          }
        roots_->insert (rt_info);
      }
  }

  return SUCCEEDED;
}


Scheduler::status_t
Scheduler_Internal::find_dependency_chain_leaves ()
{
  RT_Info **root;
  ACE_Map_Manager <RT_Info *, Sub_Priority, ACE_Null_Mutex>
    root_subpriorities;

  // Search through all of the dependency chain roots and assign
  // subpriorities to them.  In effect, this sorts the roots by
  // importance.

  Sub_Priority current_subpriority = 0;
  ACE_Bounded_Set_Iterator <RT_Info *> roots_i1 (*roots_);
  while (roots_i1.next (root) != 0)
  {
    roots_i1.advance ();
    RT_Info *rt_info = *root;

    if (rt_info == 0) continue; // The task does not run in this mode.

    // If we've seen the root already, ignore it.  Otherwise,
    // consider it a candidate as the lowest-importance root.
    RT_Info **min_importance_root;
	RtecScheduler::Importance root_importance;
    if (root_subpriorities.find (*root) == -1)
      {
        if (output_level () >= 6)
          {
            ACE_OS::printf ("find_dependency_chain_leaves (); root \"%s\" "
                              "has not yet been visited\n",
                            rt_info  ?  (const char*)rt_info->entry_point  :  "NULL");
          }
        min_importance_root = root;
        root_importance = (*root)->importance;
      }
    else
      {
        if (output_level () >= 6)
          {
            ACE_OS::printf ("find_dependency_chain_leaves (); root \"%s\" "
                              "has already been visited\n",
                            rt_info  ?  (const char*)rt_info->entry_point :  "NULL");
          }
        min_importance_root = 0;
        root_importance = (RtecScheduler::Importance) 0xFFFFFFFF;
      }

    RT_Info **root2;
    ACE_Bounded_Set_Iterator <RT_Info *> roots_i2 (*roots_);
    while (roots_i2.next (root2) != 0)
    {
      roots_i2.advance ();
      if (root2 == root  ||  *root2 == 0) continue;

	  RtecScheduler::Importance root2_importance = (*root2)->importance;

      if ((min_importance_root == 0  ||
           root2_importance < root_importance)  &&
          root_subpriorities.find (*root2) == -1)
        {
          min_importance_root = root2;
          root_importance = root2_importance;
        }
    }

    // At this point, min_importance_root contains the root with
    // the lowest importance, among all those that we haven't visited yet.
    if (output_level () >= 6)
      {
        ACE_OS::printf ("lowest importance root is \"%s\"\n",
                        (const char*)(*min_importance_root)->entry_point);
      }

    int result;
    if ((result = root_subpriorities.bind (*min_importance_root,
                                           current_subpriority++)) != 0)
      {
        ACE_OS::fprintf (stderr,
                         "Scheduler_Internal::find_dependency_chain_leaves ();"
                         "bind of %s to %d failed with result %d!!!!\n",
                         (const char*)(*min_importance_root)->entry_point,
                         current_subpriority,
                         result);
        return ST_VIRTUAL_MEMORY_EXHAUSTED;
      }
    else
      {
        if (output_level () >= 6)
          {
            ACE_OS::fprintf (stderr,
                         "Scheduler_Internal::find_dependency_chain_leaves ();"
                         "bind of \"%s\" to %d succeeded.\n",
			 (const char*)(*min_importance_root)->entry_point,
                         current_subpriority);
          }

        Sub_Priority subp;
        root_subpriorities.find (*min_importance_root, subp);
        (*min_importance_root)->subpriority = subp;
        if (output_level () >= 4)
          {
            ACE_OS::printf ("\"%s\" assigned subpriority %d\n\n",
                            (const char*)(*min_importance_root)->entry_point,
                            subp);
          }
      }
  }

  size_t roots = root_subpriorities.current_size ();
  if (output_level () >= 4)
    {
      ACE_OS::printf ("%u roots\n", roots);
    }

  // Sort the roots by importance, least first.
  u_int i;
  RT_Info **sorted_root;
  ACE_NEW_RETURN (sorted_root, RT_Info *[roots], ST_VIRTUAL_MEMORY_EXHAUSTED);

  RT_Info **visited_root;
  ACE_NEW_RETURN (visited_root,
                  RT_Info *[roots],
                  ST_VIRTUAL_MEMORY_EXHAUSTED);
  u_int next_visited_root = 0;
  for (i = 0; i < roots; ++i)
    {
      visited_root[i] = 0;
    }

  Sub_Priority min_subp;
  RT_Info *min_subp_root = 0;
  for (i = 0; i < roots; ++i)
    {
      min_subp = 0x7FFF;

      ACE_Bounded_Set_Iterator <RT_Info *> roots_i3 (*roots_);
      while (roots_i3.next (root) != 0)
      {
        roots_i3.advance ();
        RT_Info *rt_info = *root;

        if (output_level () >= 6)
          {
            ACE_OS::printf ("find_dependency_chain_leaves (); "
                              "sorted root \"%s\"\n",
                            rt_info  ?  (const char*)rt_info->entry_point  :  "NULL");
          }

        if (rt_info == 0) continue; // The task does not run in this mode.
        u_int next_root = 0;
        for (u_int j = 0; j < roots; ++j)
          {
            if (visited_root[j] == rt_info) next_root = 1;
          }
        if (next_root == 1) continue;

        if (min_subp > rt_info->subpriority)
          {
            min_subp = rt_info->subpriority;
            min_subp_root = rt_info;
          }
      }

      sorted_root[i] = min_subp_root;
      visited_root[next_visited_root++] = min_subp_root;
    }

  delete [] visited_root;

  // Now do the fun traversal from each root of a dependency chain.
  // From each of those roots, traverse the dependency chain back to
  // each leaf, which is one of the thread delineators that was found earlier.
  for (i = 0; i < roots; ++i)
  {
    if (sorted_root[i] == 0) break;

    RT_Info *rt_info = sorted_root[i];

    if (rt_info == 0) continue; // The task does not run in this mode.

    // Update the subpriority of this task:  set it to the minimum
    // subpriority if it hadn't already been set.
    if (rt_info->subpriority < 0)
      {
        if (output_level () >= 4)
          {
            ACE_OS::printf ("find_dependency_chain_leaves (); "
                            "set subpriority of %s to %u\n",
                            (const char*)rt_info->entry_point,
                            ACE_Scheduler_MIN_SUB_PRIORITY);
          }
        rt_info->subpriority = ACE_Scheduler_MIN_SUB_PRIORITY;
      }

    // Update the subpriority of all dependent tasks.
    // update_dependencies () is recursive, so set up the visited_
    // structure outside of it.
    ACE_NEW_RETURN (visited_,
                    ACE_Bounded_Set <const RT_Info *> (tasks ()),
                    ST_VIRTUAL_MEMORY_EXHAUSTED);

    if (status () == NOT_SCHEDULED  /* first mode */  ||
        status () == SUCCEEDED      /* subsequent modes */ )
      {
        Sub_Priority subp;
        root_subpriorities.find (rt_info, subp);

        if (output_level () >= 4)
          {
            ACE_OS::printf ("update_dependencies (); call for "
                              "root %s to set subpriorities, starting with "
                            "%d\n",
                            (const char*)rt_info->entry_point,
                            subp);
          }

        status (update_dependencies (*rt_info, subp));
      }

    delete visited_;
    visited_ = 0;
  }

  delete [] sorted_root;

  return SUCCEEDED;
}


Scheduler::status_t
Scheduler_Internal::identify_threads ()
{
  status_t temp_status;

  reset ();

  // Figure out how many threads there are in this mode.
  threads (find_number_of_threads (mode ()));

  // Find and store the thread delineators:  they have no dependencies
  // and have a specified rate, or they have internal threads of their own.
  if ((temp_status = find_thread_delineators (mode ())) != SUCCEEDED)
    {
      return temp_status;
    }

  // Find and store dependency-chain roots.
  if ((temp_status = find_dependency_chain_roots (mode ())) != SUCCEEDED)
    {
      return temp_status;
    }

  // Find and store dependency-chain leaves.
  if ((temp_status = find_dependency_chain_leaves ()) != SUCCEEDED)
    {
      return temp_status;
    }

  return SUCCEEDED;
}


Scheduler::status_t
Scheduler_Internal::aggregate_thread_parameters ()
{
  ACE_NEW_RETURN (leaf_info_, Thread_Map (),
		  ST_VIRTUAL_MEMORY_EXHAUSTED);

  ACE_Bounded_Set_Iterator <RT_Info *> roots_i (*roots_);
  RT_Info **root;
  while (roots_i.next (root))
    {
      roots_i.advance ();
      if (*root != 0)
        {
          RT_Info &rt_info = **root;

          if (output_level () >= 4)
            {
              ACE_OS::printf ("root: %s\n", (const char*)rt_info.entry_point);
            }

          RT_Info leaf_info (rt_info);
          RT_Info *leaf = &rt_info;

          for (u_int i = 0; i < number_of_dependencies (rt_info); ++i)
            {
	      RT_Info* info;
	      this->lookup_rt_info(rt_info.dependencies[i].rt_info, info);
              leaf =
              &calculate_thread_properties (leaf_info,
                                            *info,
                                            rt_info.dependencies[i].
                                              number_of_calls);
               if (output_level () >= 4)
                {
                  ACE_OS::printf ("reached leaf %s\n",
				  (const char*)leaf->entry_point);
                }
            }

          if (output_level () >= 4)
            {
              ACE_OS::printf ("root %s maps to leaf %s\n",
                              (const char*)rt_info.entry_point,
                              (const char*)leaf->entry_point);
            }
          leaf_info_->bind ((const char*)rt_info.entry_point, leaf);

          RT_Info *thread_info = 0;
          thread_info_ [mode ()].find ((const char*)leaf->entry_point,
				       thread_info);
          if (thread_info == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Task \"%s\" depends on a thread identified "
                            "by \"%s\", which does not \n"
                            "have a period or internal threads.  "
                            "Is that dependency backwards?\n",
                          (const char*)rt_info.entry_point,
                          (const char*)leaf->entry_point));
            }
          ACE_ASSERT (thread_info != 0);

          if (output_level () >= 4)
            {
              ACE_OS::printf ("found thread_info entry %s\n",
                              (const char*)thread_info->entry_point);
            }

          // Add the dependency chain's aggregate parameters to that
          // of the thread.
          (void) operator+= (thread_info, leaf_info);
        }
    }

  return SUCCEEDED;
}


RtecScheduler::RT_Info &
Scheduler_Internal::calculate_thread_properties (RT_Info &thread_info,
                                                 RT_Info &rt_info,
                                                 const u_int number_of_calls)
{
  u_int i;

  for (i = 0; i < number_of_calls; ++i)
    {
      (void) operator+= (&thread_info, rt_info);
    }

  if (output_level () >= 4)
    {
      ACE_OS::printf ("calculate_thread_properties (); "
                      "time: %ld, period: %ld usec\n",
#if defined (ACE_WIN32) || defined (ACE_HAS_LONGLONG_T)
                      thread_info.worst_case_execution_time,
#else
                      thread_info.time.lo (),
#endif /* ACE_WIN32 || ACE_HAS_LONGLONG_T */
                      thread_info.period / 10 /* usec/100 ns */);
    }

  if (number_of_dependencies (rt_info) == 0)
    {
      if (output_level () >= 4)
        {
          ACE_OS::printf ("calculate_thread_properties (); "
                          "reached leaf %s\n",
			  (const char*)rt_info.entry_point);
        }
      return rt_info;
    }

#if 0
  for (i = 0; i < number_of_dependencies (rt_info); ++i )
    {
      // TODO:
      if (rt_info.dependencies[i].rt_info.value() != 0)
        {
	  RT_Info info;
	  rt_info.dependencies[i].rt_info >>= &info;
          if (output_level () >= 4)
            {
              ACE_OS::printf ("calculate_thread_properties (): recurse on "
                              "dependency %s\n",
			      (const char*)info.entry_point);
            }

	  RT_Info& ret = 
	    calculate_thread_properties (thread_info,
					 info,
					 rt_info.dependencies[i].
                                         number_of_calls);
          return ret;
        }
    }
#endif /* 0 */

  ACE_ASSERT (! "should never reach this!");  // should never get here!
  return rt_info;  // to avoid compilation warning
}


int
Scheduler_Internal::priority (const handle_t handle,
			      OS_Thread_Priority &priority,
			      Sub_Priority &subpriority,
			      Preemption_Priority &preemption_prio,
                              const mode_t requested_mode) const
{
  // Casting away the const.
  ACE_Guard<LOCK> ace_mon (((Scheduler_Internal *) this)->lock_);

  const mode_t lookup_mode = requested_mode == CURRENT_MODE
                               ?  mode ()
                               :  requested_mode;

  if (!ordered_info_)
    {
      priority = minimum_priority_;
      subpriority = ACE_Scheduler_MIN_SUB_PRIORITY;
      preemption_prio = ACE_Scheduler_MAX_PREEMPTION_PRIORITY;
      return 0;
    }
  else if (lookup_mode < modes ()  &&  handle <= handles_  &&
           ordered_info_ [handle - 1][lookup_mode] != 0)
    {
      priority = ordered_info_ [handle - 1][lookup_mode]->priority;
      subpriority = ordered_info_ [handle - 1][lookup_mode]->subpriority;

      if (increasing_priority_)
        {
#if defined (ACE_WIN32)
          // Find the queue_number by iterating over the OS thread priorities.
          // This allows proper handling of non-contiguous OS priorities.
          // It would be more efficient to store the queue number in the
          // RT_Info instead of computing it here every time.  This is only
          // intended for off-line scheduling, though, so it's not critical.
	  // TODO: // ???? Hard-code knowledge of 5 dispatch queues!!
          preemption_prio = 4; 
          for (ACE_Sched_Priority p =
                 ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                 ACE_SCOPE_PROCESS);
               priority > p;
               p = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                    p,
                                                    ACE_SCOPE_PROCESS))
            {
              --preemption_prio;
            }
#else  /* ! ACE_WIN32 */
          // E.g., Solaris, which has contiguous priorities.
          preemption_prio = minimum_priority_queue () -
                           (priority - minimum_priority_);
#endif /* ! ACE_WIN32 */
        }
      else
        {
          // Assume VxWorks, which has contiguous priorities.
          preemption_prio = minimum_priority_queue () -
                         (minimum_priority_ - priority);
        }

      // TODO: This should be updated when priority and subpriority
      // are set, but I don't know when that happens :(
      ordered_info_ [handle - 1][lookup_mode]->preemption_priority = 
	preemption_prio;
	
      if (output_level () >= 3)
        {
          ACE_OS::printf ("preemption_prio %d: min %d, pri %d, min_pri %d\n",
                          preemption_prio, minimum_priority_queue (),
                          priority, minimum_priority_);
        }

      return 0;
    } else {
      return -1;
    }
}


void
Scheduler_Internal::print_schedule ()
{
  for (mode_t m = 0; m < modes (); ++m)
    {
      ACE_OS::printf ("\nMode %u:\n", m);
      ACE_OS::printf ("Task   Priority Subpri- CPU Time  Period  Rate "
                      "Utilization Name\n"
                      "                 ority  microsec microsec  Hz\n"
                      "----   -------- ------- -------- -------- ---- "
                      "----------- ----\n");

      // at 1 and incrementing by 1.
      for (u_int task = 1; task <= tasks (); ++task)
        {
          RtecScheduler::OS_Priority prio;
		  RtecScheduler::Sub_Priority subpriority;
		  RtecScheduler::Preemption_Priority preemption_prio;
          if (priority (task,
                        prio,
                        subpriority,
                        preemption_prio,
                        m) == -1)
            {
              ACE_OS::printf ("%3u        --     --          --   "
                                "    --   --        --   %s\n",
                              task, 
			      (const char*)ordered_info_[task-1][m]->entry_point);
            }
          else
            {
#if defined (ACE_WIN32) || defined (ACE_HAS_LONGLONG_T)
                const u_long t =
		  (u_long) ordered_info_ [task-1][m]->worst_case_execution_time /
                             10 /* microsec/100 ns */;
#else
                const u_long t = ordered_info_ [task-1][m]->time.lo () /
                             10 /* microsec/100 ns */;
#endif /* ACE_WIN32 || ACE_HAS_LONGLONG_T */

              ACE_OS::printf ("%3u    %6d %6d    %8lu"
                                "%9lu%5lu    %6.4f   %s\n",
                              task, prio, subpriority, t,
                              (u_long) (ordered_info_ [task-1][m]->period /
                                        10 /* microsec/100 ns */),
                              ordered_info_ [task-1][m]->period > 0
                                ?  (u_long) (1.0e7 /
                                               ordered_info_ [task-1][m]->
                                                 period)
                                :  0L,
                              ordered_info_ [task-1][m]->period > 0
                                ?  (double) t /
                                     ordered_info_ [task-1][m]->period
                                :  0.0,
                              (const char*)ordered_info_[task-1][m]->entry_point);
            }
        }
    }
}


Scheduler::status_t
Scheduler_Internal::store_schedule (const char *filename)
{
  u_int i;
  FILE *const file = ACE_OS::fopen (filename, "w");

  if (file)
    {
      ACE_OS::fprintf (file, "\
// Automatically generated \"%s\"\n\
\n\
#include \"Scheduler_Runtime.h\"\n\
\n\
static const unsigned int MODES = %u;\n\
static const unsigned int TASKS = %u;\n\
static const unsigned int THREADS = %u;\n\
static const unsigned int MINIMUM_PRIORITY_QUEUE = %u;\n\
\n\
int\n\
Scheduler_Runtime_registered_tasks_ [TASKS] = { 0 };\n\
\n\
const char *\n\
Scheduler_Runtime_task_names_ [TASKS] =\n\
  {\n\
",                     filename,
                       modes (),
                       tasks (),
                       threads (),
                       minimum_priority_queue ());

      for (i = 0; i < tasks (); ++i)
        {
          ACE_OS::fprintf (file, "    \"%s\"%s\n",
                           (const char*)ordered_info_ [i][0]->entry_point,
                           i == tasks () - 1  ?  ""  :  ",");
        }

      ACE_OS::fprintf (file, "\
  };\n\
\n\
int\n\
Scheduler_Runtime_priorities_ [MODES][TASKS][3] =\n\
  {\n\
");

      for (i = 0; i < modes (); ++i)
        {
          ACE_OS::fprintf (file, "    { /* mode %u */\n", i);
          for (u_int j = 0; j < tasks (); ++j)
            {
			  RtecScheduler::OS_Priority priority;
			  RtecScheduler::Sub_Priority subpriority;
			  RtecScheduler::Preemption_Priority preemption_prio;
              if (ordered_info_ [j][i] == 0)
                {
                  // This task doesn't run in this mode.
                  priority = increasing_priority_ ? minimum_priority_ - 1
                                                  : minimum_priority_ + 1;
                  subpriority = -1;
                  preemption_prio = minimum_priority_queue () + 1;
                }
              else
                {
                  this->priority (j+1, priority, subpriority, preemption_prio, i);
                }

              ACE_OS::fprintf (file, "      { %d, %d, %u }%s\n",
                               priority,
                               subpriority,
                               preemption_prio,
                               j == tasks () - 1  ?  ""  :  ",");
            }
          ACE_OS::fprintf (file, "    }%s /* end mode %u */\n",
                           i == modes () - 1  ?  ""  :  ",", i);
        }

      ACE_OS::fprintf (file, "\
  };\n\
\n\
unsigned int\n\
Scheduler_Runtime::modes ()\n\
{\n\
  return MODES;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::tasks ()\n\
{\n\
  return TASKS;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::threads ()\n\
{\n\
  return THREADS;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::minimum_priority_queue ()\n\
{\n\
  return MINIMUM_PRIORITY_QUEUE;\n\
}\n\
\n\
const char *\n\
Scheduler_Runtime::task_name (const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_task_names_ [handle - 1];\n\
}\n\
\n\
int\n\
Scheduler_Runtime::priority (const unsigned int mode,\n\
                                        const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_priorities_ [mode][handle - 1][0];\n\
}\n\
\n\
int\n\
Scheduler_Runtime::subpriority (const unsigned int mode,\n\
                                           const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_priorities_ [mode][handle - 1][1];\n\
}\n\
\n\
int\n\
Scheduler_Runtime::preemption_prio (const unsigned int mode,\n\
                                            const unsigned int handle)\n\
{\n\
  return Scheduler_Runtime_priorities_ [mode][handle - 1][2];\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::config ()\n\
{\n\
  return 0;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::find (const char *operation_name)\n\
{\n\
  for (unsigned int i = 0; i < TASKS; ++i)\n\
    if (! ACE_OS::strcmp (operation_name,\n\
                          Scheduler_Runtime_task_names_ [i]))\n\
      return i + 1;\n\
\n\
  return 0;\n\
}\n\
\n\
unsigned int\n\
Scheduler_Runtime::register_task (const unsigned int task)\n\
{\n\
  if (Scheduler_Runtime_registered_tasks_ [task - 1] == 1)\n\
    {\n\
      return 0;\n\
    }\n\
  else\n\
    {\n\
      Scheduler_Runtime_registered_tasks_ [task - 1] = 1;\n\
      return task;\n\
    }\n\
}\n\
\n\
");

      if (ACE_OS::fprintf (file, "// EOF\n") > 0  &&
          ACE_OS::fclose (file) == 0)
        {
          return SUCCEEDED;
        }
      else
        {
          return UNABLE_TO_WRITE_SCHEDULE_FILE;
        }
    }
  else
    {
      return UNABLE_TO_OPEN_SCHEDULE_FILE;
    }
}


Scheduler::status_t
Scheduler_Internal::store_rt_info (const char *filename)
{
  FILE *file = ACE_OS::fopen (filename, "w");
  if (file)
    {
      const time_t now = ACE_OS::time (0);

      (void) ACE_OS::fprintf (file,
                              "# RT_Info provided for \"%s\" %s"
                              "# Version 1.1\n"
                              "# Format for each entry:\n"
                              "#   entry name\n"
                              "#   handle\n"
                              "#   worst case execution time\n"
                              "#   typical execution time\n"
                              "#   cached execution time\n"
                              "#   period\n"
                              "#   importance\n"
                              "#   quantum\n"
                              "# begin dependencies\n"
                              "#   number of dependencies\n"
                              "#   entry name, number of calls "
                                "(one of these lines per dependency, if any)\n"
                              "# end dependencies\n"
                              "#   priority\n"
                              "#   order within priority\n\n"
                              "%u modes\n%u operations\n\n",
                              filename, ACE_OS::ctime (&now),
                              modes (), tasks ());

      RT_Info ***entry;
      ACE_Unbounded_Set_Iterator <RT_Info **>
        task_entries_i (task_entries_);
      while (task_entries_i.next (entry) != 0)
        {
          task_entries_i.advance ();
          export( (*entry) [0], file);
        }
    }
  else
    {
      return UNABLE_TO_OPEN_SCHEDULE_FILE;
    }

  if (ACE_OS::fprintf (file, "\n# end of file\n", 1) > 0  &&
      ACE_OS::fclose (file) == 0)
    {
      return SUCCEEDED;
    }
  else
    {
      return UNABLE_TO_WRITE_SCHEDULE_FILE;
    }
}


// This implementation is incredibly ugly, but it sometimes works.  The
// timeline viewer accepts start/stop pairs by task, and figures out the
// preemption.  So, it's not necessary to figure out the preemptions.
Scheduler::status_t
Scheduler_Internal::generate_timeline (
  Mode_Entry const sorted_rt_info [],
  const u_int number_of_tasks,
  u_long frame_size,
  ACE_Unbounded_Queue <Timeline_Entry> &jobs)
{
  if (number_of_tasks > 0)
    {
      u_long start = 0;
      u_long stop = 0;
      u_int i;
      u_long *next_start;
      u_long *leftover;

      ACE_NEW_RETURN (next_start, u_long [number_of_tasks],
                      ST_VIRTUAL_MEMORY_EXHAUSTED);
      ACE_NEW_RETURN (leftover, u_long [number_of_tasks],
                      ST_VIRTUAL_MEMORY_EXHAUSTED);

      for (i = 0; i < number_of_tasks; ++i)
        {
          next_start [i] = 0;
          leftover [i] = 0;
        }

      while (stop <= frame_size)
        {
          for (i = number_of_tasks; i > 0; --i)
            {
              u_long execution_time = 0;  // microseconds

              if (start < next_start [i - 1])
                {
                  if (leftover [i - 1] == 0)
                    {
                      // Not at this task's next start period, and it
                      // has no leftover work in it's current period,
                      // so move on to the next task.

                      if (output_level () >= 3)
                        {
                          ACE_OS::printf ("      continue for task %lu\n",
                            sorted_rt_info [i - 1].rt_info_->handle);
                        }

                      continue;
                    }
                  else
                    {
                      if (output_level () >= 3)
                        {
                          ACE_OS::printf ("      leftover for task %lu\n",
                            sorted_rt_info [i - 1].rt_info_->handle);
                        }

                      execution_time = leftover [i - 1];
                      leftover [i - 1] = 0;
                    }
                }
              else
                {
                  if (output_level () >= 3)
                    {
                      ACE_OS::printf ("      task %s reached next start of"
                                        " %lu (at %lu)\n",
                                      (const char*)sorted_rt_info [i - 1].rt_info_->
                                        entry_point,
                                      next_start [i - 1],
                                      start);
                    }

                  execution_time = (u_long)
                    (sorted_rt_info [i - 1].rt_info_->worst_case_execution_time /
                     10 /* us/100 ns */);

                  next_start [i - 1] +=
                    (u_long) (sorted_rt_info [i - 1].rt_info_->period /
                              10 /* usec/100 ns */);

                  if (output_level () >= 3)
                    {
                      ACE_OS::printf ("    loaded next_start of %lu with "
                                        "%lu\n",
                                      sorted_rt_info [i - 1].rt_info_->
                                        handle,
                                      next_start [i - 1]);
                    }
                }

              if (output_level () >= 3)
                {
                  ACE_OS::printf ("    handle:  %d, "
                                    " execution_time: %lu, "
                                    " leftover: %lu\n",
                                  sorted_rt_info [i - 1].rt_info_->handle,
                                  execution_time,
                                  leftover [i - 1]);
                }

              // Look for a start time of a higher priority task that
              // is before this one can finish.
              int will_finish = 1;
              for (u_int j = number_of_tasks; j > i; --j)
                {
                  // Skip over groups of array entries with the same period.
                  if (j < number_of_tasks  &&
                      sorted_rt_info [j - 1].rt_info_->period ==
                      sorted_rt_info [j].rt_info_->period)
                    {
                      break;
                    }

                  if (next_start [j - 1] > 0  &&
                      next_start [j - 1] <= start + execution_time)
                    {
                      leftover [i - 1] = execution_time -
                                         (next_start [j - 1] - start);
                      execution_time = next_start [j - 1] - start;
                      will_finish = 0;
                      if (output_level () >= 3)
                        {
                          ACE_OS::printf ("    handle:  %d, "
                                          " execution_time: %lu, "
                                          " leftover: %lu\n",
                                          sorted_rt_info [i - 1].rt_info_->
                                            handle,
                                          execution_time,
                                          leftover [i - 1]);
                        }
                    }

                  if (output_level () >= 2)
                    {
                      ACE_OS::printf ("    period:  %ld, time: %lu usec, "
                                      " period ratio: %lu, "
                                      " execution_time: %lu usec\n",
                                      sorted_rt_info [j-1].rt_info_->
                                        period / 10 /* usec/100 ns */,
                                      (u_long) (sorted_rt_info [j-1].rt_info_->
				      worst_case_execution_time / 10 /* usec/100 ns */),
                                      sorted_rt_info [j-1].rt_info_->period
                                          > 0
                                        ? (u_long) (sorted_rt_info [i-1].
                                                      rt_info_->period /
                                                    sorted_rt_info [j-1].
                                                    rt_info_->period)
                                        : 0,
                                      execution_time / 10 /* usec/100 ns */);
                    }
                }

              if (output_level () >= 3)
                {
                  ACE_OS::printf ("  period:  %ld usec, time: %lu usec, "
                                  " execution_time: %lu usec\n",
                                  sorted_rt_info [i - 1].rt_info_->
                                    period / 10 /* usec/100 ns */,
                                  (u_long) (sorted_rt_info [i - 1].rt_info_->
				   worst_case_execution_time / 10 /* usec/100 ns */),
                                  execution_time / 10 /* usec/100 ns */);
                }

              jobs.enqueue_head (Timeline_Entry (sorted_rt_info [i - 1].rt_info_->
                                              handle,
                                            sorted_rt_info [i - 1].rt_info_->
                                              entry_point,
                                            start,
                                            stop = start + execution_time,
                                            next_start [i - 1]));
              start = stop;

              if (! will_finish)
                {
                  // don't bother to visit any other tasks:  restart
                  // with the highest priority task
                  break;
                }
            }

          // figure out next start time based on periods only
          u_long min_start = 0xFFFFFFFF;
          for (i = 0; i < number_of_tasks; ++i)
            {
              if (min_start > next_start [i]) min_start = next_start [i];
            }
          start = min_start;

          // if any tasks haven't finished the current period processing,
          // let them try to finish by overwriting "start" with "stop"
          for (i = 0; i < number_of_tasks; ++i)
            {
              if (leftover [i] > 0)
                {
                  start = stop;
                  break;
                }
            }
        }

      delete [] next_start;
      next_start = 0;
      delete [] leftover;
      leftover = 0;
    }

  return SUCCEEDED;
}


Scheduler::status_t
Scheduler_Internal::create_timelines (const char *filename)
{
  // Store each mode's timeline in a different file.  Append the
  // mode identifier to filename.
  char full_filename [MAXPATHLEN];
  ACE_OS::strcpy (full_filename, filename);
  char *filename_extension = full_filename + ACE_OS::strlen (filename);
  if (filename_extension == 0)
    {
      return UNABLE_TO_OPEN_SCHEDULE_FILE;
    }

  status_t status = SUCCEEDED;

  for (u_int mode = 0;
       (status == SUCCEEDED || status == ST_UTILIZATION_BOUND_EXCEEDED)  &&
         mode < modes ();
       ++mode)
    {
      char mode_id[128];
      ACE_OS::sprintf (mode_id, "-mode_%d.timeline", mode + 1);
      ACE_OS::strcpy (filename_extension, mode_id);

      FILE *const file = ACE_OS::fopen (full_filename, "w");
      if (file)
        {
          u_int i;

          // count the number of tasks in this mode with non-null RT_Info
          u_int nonnull_tasks = 0;
          for (i = 0; i < tasks (); ++i)
            {
              if (ordered_info_ [i][mode] != 0)
                {
                  ++nonnull_tasks;
                }
            }
          ACE_OS::fprintf (file, "%u\n", nonnull_tasks); /* number_of_tasks */


          ACE_OS::fprintf (file, "%lu\n",                /* frame_size */
                           frame_size_ [mode]);
          ACE_OS::fprintf (file, "0 %lu\n",              /* start stop_times */
                           frame_size_ [mode]);

          ACE_Bounded_Set <u_int> emitted_tasks (nonnull_tasks);
          u_int max_priority = 0x7FFFFFFF;
          for (i = 0; i < tasks (); ++i)
            {
              u_int highest_priority = 0x7FFFFFFF;  /* corresponds to lowest
                                                       queue number */
              u_int highest_priority_task = 0xFFFFFFFF;

              for (u_int j = 0; j < tasks (); ++j)
                {
                  if (ordered_info_ [j][mode] != 0)
                    {
					  RtecScheduler::OS_Priority priority;
					  RtecScheduler::Sub_Priority subpriority;
					  RtecScheduler::Preemption_Priority preemption_prio;
                      this->priority (j+1, priority, subpriority, preemption_prio,
                                      mode);

                      if (i == 0)
                        {
                          if (max_priority > preemption_prio)
                            {
                              max_priority = preemption_prio;
                            }
                        }

                      if (! emitted_tasks.find (j)  &&
                          highest_priority > preemption_prio)
                        {
                          highest_priority = preemption_prio;
                          highest_priority_task = j;
                        }
                    }
                }

              /* task_name priority */
              if (highest_priority_task < 0xFFFFFFFF)
                {
                  ACE_OS::fprintf (file, "%s %d\n",
                    /* task name */
		    (const char*)ordered_info_ [highest_priority_task][mode]->
                      entry_point,
                    /* queue number */
                    minimum_priority_queue () -
                      (increasing_priority_
                         ?  ordered_info_ [highest_priority_task][mode]->
                              priority -
                            minimum_priority_
                         :  minimum_priority_ -
                            ordered_info_ [highest_priority_task][mode]->
                              priority));

                  emitted_tasks.insert (highest_priority_task);
                }
            }

          const u_int timeline_entries = (u_int) timeline_ [mode].size ();
          for (i = 0; i < timeline_entries; ++i)
            {
              Timeline_Entry entry;
              if (! timeline_ [mode].dequeue_head (entry))
                {
                  ACE_OS::fprintf (file, "%s %d %s\n%s %d %d\n",
                                   entry.entry_point_,
                                   entry.start_,
                                   "", /* label */
                                   entry.entry_point_,
                                   entry.stop_,
                                   entry.next_start_);
                }
            }

          if (ACE_OS::fclose (file) != 0)
            {
              status = UNABLE_TO_WRITE_SCHEDULE_FILE;
            }
        }
      else
        {
          status = UNABLE_TO_OPEN_SCHEDULE_FILE;
        }
    }

  return status;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Bounded_Set<RtecScheduler::RT_Info *>;
template class ACE_Bounded_Set<const RtecScheduler::RT_Info *>;
template class ACE_Bounded_Set<u_int>;
template class ACE_Bounded_Set_Iterator<RtecScheduler::RT_Info *>;
template class ACE_Node<Scheduler_Internal::Timeline_Entry>;
template class ACE_Node<RtecScheduler::RT_Info **>;
template class ACE_Unbounded_Queue<Scheduler_Internal::Timeline_Entry>;
template class ACE_Unbounded_Queue_Iterator<
  Scheduler_Internal::Timeline_Entry>;
template class ACE_Unbounded_Set<RtecScheduler::RT_Info **>;
template class ACE_Unbounded_Set_Iterator<RtecScheduler::RT_Info **>;

template class ACE_Map_Entry <RtecScheduler::RT_Info *, Scheduler::Sub_Priority>;
template class ACE_Map_Manager <RtecScheduler::RT_Info *, Scheduler::Sub_Priority,
                                ACE_Null_Mutex>;

#if defined (ACE_HAS_THREADS)
  template class ACE_Map_Entry<ACE_CString, RtecScheduler::RT_Info *>;
  template class ACE_Map_Manager<ACE_CString, RtecScheduler::RT_Info *, ACE_Null_Mutex>;
  template class ACE_Map_Iterator<ACE_CString, RtecScheduler::RT_Info *, ACE_Null_Mutex>;
#endif /* ACE_HAS_THREADS */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


// EOF
