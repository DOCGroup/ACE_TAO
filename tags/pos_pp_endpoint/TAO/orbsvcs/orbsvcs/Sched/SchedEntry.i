/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    SchedEntry.i
//
// = CREATION DATE
//    7 February 1998
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

//////////////////////
// Class Task_Entry //
//////////////////////

// return a pointer to the underlying RT_Info
ACE_INLINE Task_Entry::RT_Info *
Task_Entry::rt_info () const
{
  return rt_info_;
}

// set the underlying RT_Info pointer
ACE_INLINE void
Task_Entry::rt_info (Task_Entry::RT_Info *info)
{
  rt_info_ = info;
}

// get effective period for the task entry
ACE_INLINE Task_Entry::Period
Task_Entry::effective_period () const
{
  return effective_period_;
}

// set effective period for the task entry
ACE_INLINE void
Task_Entry::effective_period (Task_Entry::Period p)
{
  effective_period_ = p;
}

ACE_INLINE void
Task_Entry::discovered (long l)
{
  discovered_ = l;
  dfs_status_ = VISITED;
}

ACE_INLINE long
Task_Entry::discovered () const
{
  return discovered_;
}

ACE_INLINE void
Task_Entry::finished (long l)
{
  finished_ = l;
  dfs_status_ = FINISHED;
}

ACE_INLINE long
Task_Entry::finished () const
{
  return finished_;
}

ACE_INLINE Task_Entry::DFS_Status
Task_Entry::dfs_status () const
{
  return dfs_status_;
}

ACE_INLINE void
Task_Entry::dfs_status (Task_Entry::DFS_Status ds)
{
  dfs_status_ = ds;
}

// Sets a flag indicating whether node is a thread delineator.

ACE_INLINE void
Task_Entry::is_thread_delineator (int i)
{
  is_thread_delineator_ = i;
}

// Gets the flag indicating whether node is a thread delineator.

ACE_INLINE int
Task_Entry::is_thread_delineator () const
{
  return is_thread_delineator_;
}

// Sets a flag indicating whether node has unresolved remote dependencies.

ACE_INLINE void
Task_Entry::has_unresolved_remote_dependencies (int i)
{
  has_unresolved_remote_dependencies_ = i;
}

// Gets the flag indicating whether node has unresolved remote dependencies.

ACE_INLINE int
Task_Entry::has_unresolved_remote_dependencies () const
{
  return has_unresolved_remote_dependencies_;
}


// Sets a flag indicating whether node has unresolved local dependencies.

ACE_INLINE void
Task_Entry::has_unresolved_local_dependencies (int i)
{
  has_unresolved_local_dependencies_ = i;
}

// Gets the flag indicating whether node has unresolved local dependencies.

ACE_INLINE int
Task_Entry::has_unresolved_local_dependencies () const
{
  return has_unresolved_local_dependencies_;
}

// Gets the set of Task Entries on which this entry depends.

ACE_INLINE ACE_Unbounded_Set <Task_Entry_Link *> &
Task_Entry::calls ()
{
  return calls_;
}

// Gets the set of Task Entries which depend on this entry.

ACE_INLINE ACE_Unbounded_Set <Task_Entry_Link *> &
Task_Entry::callers ()
{
  return callers_;
}

// Gets set of arrivals in the entry's effective period.

ACE_INLINE ACE_Ordered_MultiSet<Dispatch_Entry_Link> &
Task_Entry::dispatches ()
{
  return dispatches_;
}


ACE_INLINE Task_Entry::Info_Type
Task_Entry::info_type () const
{
  return rt_info_->info_type;
}

ACE_INLINE u_long
Task_Entry::effective_execution_time () const
{
  // Just use low 32 bits.  This will have to change when TimeBase.idl
  // is finalized.
  ACE_UINT32 worst_case_execution_time =
    ACE_U64_TO_U32 (rt_info_->worst_case_execution_time);

  return ((rt_info_->info_type == RtecScheduler::OPERATION) ||
          (rt_info_->info_type == RtecScheduler::REMOTE_DEPENDANT))
         ? worst_case_execution_time * dispatches_.size ()
         : 0;
}


///////////////////////////
// Class Task_Entry_Link //
///////////////////////////


// accessor: number of calls of dependency by dependant
ACE_INLINE CORBA::Long
Task_Entry_Link::number_of_calls () const
{
  return number_of_calls_;
}

ACE_INLINE Task_Entry_Link::Dependency_Type
Task_Entry_Link::dependency_type () const
{
  return dependency_type_;
}

// accessor: dependant task entry
ACE_INLINE Task_Entry &
Task_Entry_Link::caller () const
{
  return caller_;
}

// accessor: dependency task entry
ACE_INLINE Task_Entry &
Task_Entry_Link::called () const
{
  return called_;
}

//////////////////////////
// Class Dispatch Entry //
//////////////////////////

ACE_INLINE u_long
Dispatch_Entry::dispatch_id () const
{
  return dispatch_id_;
}

ACE_INLINE Dispatch_Entry::Preemption_Priority
Dispatch_Entry::priority () const
{
  return priority_;
}

ACE_INLINE void
Dispatch_Entry::priority (Dispatch_Entry::Preemption_Priority p)
{
  priority_ = p;
}

ACE_INLINE Dispatch_Entry::OS_Priority
Dispatch_Entry::OS_priority () const
{
  return OS_priority_;
}

ACE_INLINE void
Dispatch_Entry::OS_priority (Dispatch_Entry::OS_Priority p)
{
  OS_priority_ = p;
}

ACE_INLINE Dispatch_Entry::Sub_Priority
Dispatch_Entry::dynamic_subpriority () const
{
  return dynamic_subpriority_;
}

ACE_INLINE void
Dispatch_Entry::dynamic_subpriority (Dispatch_Entry::Sub_Priority p)
{
  dynamic_subpriority_ = p;
}

ACE_INLINE Dispatch_Entry::Sub_Priority
Dispatch_Entry::static_subpriority () const
{
  return static_subpriority_;
}

ACE_INLINE void
Dispatch_Entry::static_subpriority (Dispatch_Entry::Sub_Priority p)
{
  static_subpriority_ = p;
}


ACE_INLINE Dispatch_Entry::Time
Dispatch_Entry::arrival () const
{
  return arrival_;
}

ACE_INLINE Dispatch_Entry::Time
Dispatch_Entry::deadline () const
{
  return deadline_;
}

ACE_INLINE Task_Entry &
Dispatch_Entry::task_entry () const
{
  return task_entry_;
}


// accessor for pointer to original dispatch
ACE_INLINE Dispatch_Entry *
Dispatch_Entry::original_dispatch ()
{
  return original_dispatch_;
}


///////////////////////////////
// Class Dispatch_Entry_Link //
///////////////////////////////

ACE_INLINE
Dispatch_Entry_Link::~Dispatch_Entry_Link ()
{
}
  // dtor

ACE_INLINE int
Dispatch_Entry_Link::operator < (const Dispatch_Entry_Link &d) const
{
  return (this->dispatch_entry_ < d.dispatch_entry_);
}
  // GT comparator


ACE_INLINE Dispatch_Entry &
Dispatch_Entry_Link::dispatch_entry () const
{
  return dispatch_entry_;
}
  // accessor for reference to dispatch entry


///////////////////////////////////
// Class Dispatch_Proxy_Iterator //
///////////////////////////////////

ACE_INLINE int
Dispatch_Proxy_Iterator::done () const
{
  return iter_.done ();
}
  // returns 0 if there are more entries to see, 1 if not


//////////////////////////
// Class TimeLine_Entry //
//////////////////////////

    // dispatch entry accessor
ACE_INLINE Dispatch_Entry &
TimeLine_Entry::dispatch_entry () const
{
  return dispatch_entry_;
}


// accessor for time slice start time (100 nanoseconds)
ACE_INLINE RtecScheduler::Time
TimeLine_Entry::start () const
{
  return start_;
}

// accessor for time slice stop time (100 nanoseconds)
ACE_INLINE RtecScheduler::Time
TimeLine_Entry::stop () const
{
  return stop_;
}

// accessor for time slice stop time (100 nanoseconds)
ACE_INLINE RtecScheduler::Time
TimeLine_Entry::arrival () const
{
  return arrival_;
}

// accessor for time slice stop time (100 nanoseconds)
ACE_INLINE RtecScheduler::Time
TimeLine_Entry::deadline () const
{
  return deadline_;
}


// accessor for next slice for this dispatch
ACE_INLINE TimeLine_Entry *
TimeLine_Entry::next (void) const
{
  return next_;
}

// mutator for next slice for this dispatch
ACE_INLINE void
TimeLine_Entry::next (TimeLine_Entry *t)
{
  next_ = t;
}

// accessor for previous slice for this dispatch
ACE_INLINE TimeLine_Entry *
TimeLine_Entry::prev (void) const
{
  return prev_;
}

// mutator for previous slice for this dispatch
ACE_INLINE void
TimeLine_Entry::prev (TimeLine_Entry *t)
{
  prev_ = t;
}


ACE_INLINE int
TimeLine_Entry::operator < (const TimeLine_Entry &t) const
{
  return (start_ < t.start_) ? 1 : 0;
}
  // comparison operator


///////////////////////////////
// Class TimeLine_Entry_Link //
///////////////////////////////


ACE_INLINE TimeLine_Entry_Link::TimeLine_Entry_Link  (TimeLine_Entry &t)
  : entry_ (t)
{
}
  // ctor

ACE_INLINE TimeLine_Entry &
TimeLine_Entry_Link::entry () const
{
  return entry_;
}
  // accessor for the underlying entry

ACE_INLINE int
TimeLine_Entry_Link::operator < (const TimeLine_Entry_Link &l) const
{
  return (entry_ < l.entry_) ? 1 : 0;
}
  // comparison operator
