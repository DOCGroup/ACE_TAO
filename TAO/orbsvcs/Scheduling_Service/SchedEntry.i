/* -*- C++ -*- */
//
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
Task_Entry::rt_info ()
{
  return rt_info_;
}

// effective period for the task entry
ACE_INLINE Task_Entry::Period
Task_Entry::effective_period ()
{
  return effective_period_;
}

ACE_INLINE void 
Task_Entry::discovered (long l)
{
  discovered_ = l;
  dfs_status_ = VISITED;
}

ACE_INLINE long 
Task_Entry::discovered ()
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
Task_Entry::finished ()
{
  return finished_;
}

ACE_INLINE Task_Entry::DFS_Status 
Task_Entry::dfs_status ()
{
  return dfs_status_;
}

ACE_INLINE void
Task_Entry::dfs_status (Task_Entry::DFS_Status ds)
{
  dfs_status_ = ds;
}

ACE_INLINE void
Task_Entry::is_thread_delineator (int i)
{
  is_thread_delineator_ = i;
}

ACE_INLINE int
Task_Entry::is_thread_delineator ()
{
  return is_thread_delineator_;
}

// access set of Task Entries on which this entry depends
ACE_INLINE LINK_SET & 
Task_Entry::calls ()
{
  return calls_;
}

// access set of Task Entries which depend on this entry
ACE_INLINE LINK_SET & 
Task_Entry::callers ()
{
  return callers_;
}

ACE_INLINE Task_Entry::Info_Type
Task_Entry::info_type ()
{
  return rt_info_->info_type;
}

ACE_INLINE u_long
Task_Entry::effective_execution_time ()
{
  return (rt_info_->info_type == RtecScheduler::OPERATION)
         ? rt_info_->worst_case_execution_time * arrival_count_
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
Dispatch_Entry::dispatch_id ()
{
  return dispatch_id_;
}

ACE_INLINE Dispatch_Entry::Preemption_Priority
Dispatch_Entry::priority ()
{
  return priority_;
}

ACE_INLINE void
Dispatch_Entry::priority (Dispatch_Entry::Preemption_Priority p)
{
  priority_ = p;
}

ACE_INLINE Dispatch_Entry::OS_Priority
Dispatch_Entry::OS_priority ()
{
  return OS_priority_;
}

ACE_INLINE void
Dispatch_Entry::OS_priority (Dispatch_Entry::OS_Priority p)
{
  OS_priority_ = p;
}

ACE_INLINE Dispatch_Entry::Sub_Priority
Dispatch_Entry::dynamic_subpriority ()
{
  return dynamic_subpriority_;
}

ACE_INLINE void
Dispatch_Entry::dynamic_subpriority (Dispatch_Entry::Sub_Priority p)
{
  dynamic_subpriority_ = p;
}

ACE_INLINE Dispatch_Entry::Sub_Priority
Dispatch_Entry::static_subpriority ()
{
  return static_subpriority_;
}

ACE_INLINE void
Dispatch_Entry::static_subpriority (Dispatch_Entry::Sub_Priority p)
{
  static_subpriority_ = p;
}


ACE_INLINE Dispatch_Entry::Time 
Dispatch_Entry::arrival ()
{
  return arrival_;
}

ACE_INLINE Dispatch_Entry::Time 
Dispatch_Entry::deadline ()
{
  return deadline_;
}

ACE_INLINE Dispatch_Entry::Time 
Dispatch_Entry::execution_time ()
{
  return execution_time_;
}

ACE_INLINE Task_Entry &
Dispatch_Entry::task_entry ()
{
  return task_entry_;
}


///////////////////////////////
// Class Dispatch_Entry_Link //
///////////////////////////////

ACE_INLINE 
Dispatch_Entry_Link::~Dispatch_Entry_Link ()
{
}
  // dtor

Dispatch_Entry_Link::operator < (const Dispatch_Entry_Link &d) const
{
  return (this->dispatch_entry_ < d.dispatch_entry_)
}
  // GT comparator

ACE_INLINE void 
Dispatch_Entry_Link::operator = (const Dispatch_Entry_Link &d)
{
  dispatch_entry_ = d.dispatch_entry_;
}
  // assignment operator

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
Dispatch_Proxy_Iterator::done ()
{
  return iter.done ();
}
  // returns 0 if there are more entries to see, 1 if not


//////////////////////////
// Class TimeLine_Entry //
//////////////////////////

    // dispatch entry accessor
ACE_INLINE Dispatch_Entry &
TimeLine_Entry::dispatch_entry ()
{
  return dispatch_entry_;
}


// accessor for time slice start time (100 nanoseconds)
ACE_INLINE u_long 
TimeLine_Entry::start ()
{
  return start_;
}

// accessor for time slice stop time (100 nanoseconds)
ACE_INLINE u_long 
TimeLine_Entry::stop ()
{
  return stop_;
}

// accessor for next slice for this dispatch
ACE_INLINE TimeLine_Entry *
TimeLine_Entry::next (void)
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
TimeLine_Entry::prev (void)
{
  return prev_;
}

// mutator for previous slice for this dispatch
ACE_INLINE void 
TimeLine_Entry::prev (TimeLine_Entry *t)
{
  prev_ = t;
}
