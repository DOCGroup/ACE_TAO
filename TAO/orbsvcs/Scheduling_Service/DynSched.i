// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    DynSched.i
//
// = CREATION DATE
//    23 January 1997
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

// return a pointer to the underlying RT_Info
ACE_INLINE ACE_Scheduler::RT_Info *
ACE_Scheduler::Task_Entry::rt_info ()
{
  return rt_info_;
}

// effective period for the task entry
ACE_INLINE ACE_Scheduler::Period
ACE_Scheduler::Task_Entry::effective_period ()
{
  return effective_period_;
}

ACE_INLINE void 
ACE_Scheduler::Task_Entry::discovered (long l)
{
  discovered_ = l;
  dfs_status_ = VISITED;
}

ACE_INLINE long 
ACE_Scheduler::Task_Entry::discovered ()
{
  return discovered_;
}

ACE_INLINE void 
ACE_Scheduler::Task_Entry::finished (long l)
{
  finished_ = l;
  dfs_status_ = FINISHED;
}

ACE_INLINE long 
ACE_Scheduler::Task_Entry::finished ()
{
  return finished_;
}

ACE_INLINE ACE_Scheduler::Task_Entry::DFS_Status 
ACE_Scheduler::Task_Entry::dfs_status ()
{
  return dfs_status_;
}

ACE_INLINE void
ACE_Scheduler::Task_Entry::dfs_status (ACE_Scheduler::Task_Entry::DFS_Status ds)
{
  dfs_status_ = ds;
}

ACE_INLINE void
ACE_Scheduler::Task_Entry::is_thread_delineator (int i)
{
  is_thread_delineator_ = i;
}

ACE_INLINE int
ACE_Scheduler::Task_Entry::is_thread_delineator ()
{
  return is_thread_delineator_;
}

// access set of Task Entries on which this entry depends
ACE_INLINE ACE_Scheduler::LINK_SET & 
ACE_Scheduler::Task_Entry::calls ()
{
  return calls_;
}

// access set of Task Entries which depend on this entry
ACE_INLINE ACE_Scheduler::LINK_SET & 
ACE_Scheduler::Task_Entry::callers ()
{
  return callers_;
}

ACE_INLINE ACE_Scheduler::Info_Type
ACE_Scheduler::Task_Entry::info_type ()
{
  return rt_info_->info_type;
}

ACE_INLINE double
ACE_Scheduler::Task_Entry::effective_execution_time ()
{
  return (rt_info_->info_type == RtecScheduler::OPERATION)
         ? rt_info_->worst_case_execution_time * arrival_count_
         : 0;
}

// accessor: number of calls of dependency by dependant 
ACE_INLINE CORBA::Long
ACE_Scheduler::Task_Entry_Link::number_of_calls () const 
{
  return number_of_calls_;
}

ACE_INLINE ACE_Scheduler::Dependency_Type 
ACE_Scheduler::Task_Entry_Link::dependency_type () const 
{
  return dependency_type_;
}

// accessor: dependant task entry
ACE_INLINE ACE_Scheduler::Task_Entry &
ACE_Scheduler::Task_Entry_Link::caller () const 
{
  return caller_;
}
    
// accessor: dependency task entry
ACE_INLINE ACE_Scheduler::Task_Entry &
ACE_Scheduler::Task_Entry_Link::called () const 
{
  return called_;
}


//////////////////////////
// Class Dispatch Entry //
//////////////////////////
ACE_INLINE u_long
ACE_Scheduler::Dispatch_Entry::dispatch_id ()
{
  return dispatch_id_;
}

ACE_INLINE ACE_Scheduler::Priority 
ACE_Scheduler::Dispatch_Entry::priority ();
{
  return priority_;
}

ACE_INLINE ACE_Scheduler::Time 
ACE_Scheduler::Dispatch_Entry::arrival ();
{
  return arrival_;
}

ACE_INLINE ACE_Scheduler::Time 
ACE_Scheduler::Dispatch_Entry::deadline ();
{
  return deadline_;
}

ACE_INLINE ACE_Scheduler::Time 
ACE_Scheduler::Dispatch_Entry::execution_time ();
{
  return execution_time_;
}

ACE_INLINE ACE_Scheduler::Task_Entry &
ACE_Scheduler::Dispatch_Entry::task_entry ()
{
  return task_entry_;
}

ACE_INLINE ACE_Scheduler::Dispatch_Entry *
ACE_Scheduler::Dispatch_Entry::previous_instance ()
{
  return previous_instance_;
}

ACE_INLINE void 
ACE_Scheduler::Dispatch_Entry::previous_instance (Dispatch_Entry *p)
{
  previous_instance_ = p;
}



/////////////////////////
// Class ACE_Scheduler //
/////////////////////////

ACE_INLINE u_int
ACE_Scheduler::minimum_priority_queue () const 
{ 
  return minimum_priority_queue_;
}
  // This is intended for use by the Event Channel, so it can determine the
  // number of priority dispatch queues to create.

// = Access the number of tasks.
ACE_INLINE u_int 
ACE_Scheduler::tasks () const 
{
  return tasks_; 
}

  // = Access the number of threads.
ACE_INLINE u_int
ACE_Scheduler::threads () const 
{
  return threads_; 
}

  // = Access the current scheduler status.
ACE_INLINE ACE_Scheduler::status_t 
ACE_Scheduler::status () const 
{  
  return status_; 
}

  // = Access the current output (debugging) level.
ACE_INLINE u_int
ACE_Scheduler::output_level () const {
  return output_level_; 
}
  // Default is 0; set to 1 to print out schedule, by task.  Set
  // to higher than one for debugging info.

  // = Set the scheduler output (debugging) level.
ACE_INLINE void 
ACE_Scheduler::output_level (const u_int level) 
{
  output_level_ = level; 
}
  // the only supported levels are 0 (quiet), 1 (verbose) and 2
  // (debug)

ACE_INLINE void 
ACE_Scheduler::minimum_priority_queue (const u_int minimum_priority_queue_number)
{
  minimum_priority_queue_ = minimum_priority_queue_number; 
}

  // = Set the number of tasks.
ACE_INLINE void 
ACE_Scheduler::tasks (const u_int tasks) 
{
  tasks_ = tasks; 
}

  // = Set the number of threads.
ACE_INLINE void 
ACE_Scheduler::threads (const u_int threads) 
{
  threads_ = threads; 
}

  // = Set the current scheduler status.
ACE_INLINE void 
ACE_Scheduler::status (const status_t new_status) 
{ 
  status_ = new_status; 
}


// EOF
