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



/////////////////////////
// Class ACE_Scheduler //
/////////////////////////

ACE_INLINE ACE_Scheduler::Preemption_Priority
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
ACE_Scheduler::minimum_priority_queue (const Preemption_Priority minimum_priority_queue_number)
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
// TBD - remove this - allowing the application to modify this is *not* good
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

ACE_INLINE u_long
ACE_Scheduler::min_dispatch_id () const
{ 
  return min_dispatch_id_; 
}

ACE_INLINE u_long
ACE_Scheduler::max_dispatch_id () const
{ 
  return max_dispatch_id_; 
}


// EOF
