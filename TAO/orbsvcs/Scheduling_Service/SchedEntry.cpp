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
//    SchedEntry.cpp
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

Task_Entry::Task_Entry ()
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

Task_Entry::~Task_Entry () 
{
  // zero out the task entry ACT in the corresponding rt_info
  rt_info_->volatile_token = 0;

  // iterate through the "calls" set of Task Entry Links and free each one
  ACE_Unbounded_Set_Iterator <Task_Entry_Link *> iter(calls_);
  Task_Entry_Link *link = 0;
  for (iter.first (); ! iter.done (); iter.advance (), link = 0)
  {
    if ((iter.next (link) != 0) && (link))
    {
      // remove the link object pointer from the calling
      // entry's "callers" set and destroy the link object
      link->called ().callers_.remove (link);
      delete link;
    }
  }
}

// merge dispatches according to info type and type of call,
// update relevant scheduling characteristics for this entry
int
Task_Entry::merge_dispatches (ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries)
{
  int result = 0;
  switch (dependency_type ())
  {
    case DISJUNCTION:

      // prohibit two-way dispatches of a disjunction group,
      // and disjunctively merge its one-way dispatches.
      result = prohibit_dispatches (TWO_WAY_CALL);
      if (result == 0)
      {
        result = disjunctive_merge (ONE_WAY_CALL, dispatch_entries);
      }

      break;

    case CONJUNCTION:

      // prohibit two-way dispatches of a conjunction group,
      // and conjunctively merge its one-way dispatches.
      result = prohibit_dispatches (TWO_WAY_CALL);
      if (result == 0)
      {
        result = conjunctive_merge (ONE_WAY_CALL, dispatch_entries);
      }

      break;

    case OPERATION:

      // disjunctively merge the operation's two-way dispatches,
      // and conjunctively merge its one-way dispatches.
      result = disjunctive_merge (TWO_WAY_CALL, dispatch_entries);
      if (result == 0)
      {
        result = conjunctive_merge (ONE_WAY_CALL, dispatch_entries);
      }

      break;


    default:

      // there should not be any other kind of RT_Info, or if
      // there is, the above switch logic is in need of repair.
      result -1;
      break;
  }

  return result;
}


// prohibit calls of the given type: currently used to enforce
// the notion that two-way calls to disjunctive or conjunctive
// RT_Infos do not have any defined meaning, and thus should be
// considered dependency specification errors: if these constraints
// are removed in the future, this method should be removed as well
// Returns 0 if all is well, or -1 if an error has occurred.
int 
Task_Entry::prohibit_dispatches (Dependency_Type dt)
{
  // iterate over the set of dependencies, ensuring
  // none of them has the given dependency type
  ACE_Unbounded_Set_Iterator <Task_Entry_Link *> iter (callers_);
  while (! iter.done ())
  {
    Task_Entry_Link *link;
    if ((iter.next (link) == 0) || (link == 0) || 
        (link->dependency_type () == dt))
    {
      return -1;
    }

    iter.advance ();
  }

  return 0;
}


// perform disjunctive merge of arrival times of oneway calls:
// all arrival times of all dependencies are duplicated by the
// multiplier and repetition over the new frame size and merged
void 
Task_Entry::disjunctive_merge (
  Dependency_Type dt, 
  ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries)
{
  // iterate over the set of dependencies, ensuring
  // none of them has the given dependency type
  ACE_Unbounded_Set_Iterator <Task_Entry_Link *> iter (callers_);
  while (! iter.done ())
  {
    Task_Entry_Link *link;
    if ((iter.next (link) == 0) || (link == 0))
    {
      return -1;
    }

    // the link matches the dependency type given
    if (link->dependency_type () == dt)
    {
      // merge the caller's dispatches into the current set
      if (merge_dispatches (dispatches_, link->caller ().dispatches_,
                            effective_period_, 
                            link->caller ().effective_period_, 
                            link->number_of_calls ()) < 0)
      {
        return -1;
      }
    }

    iter.advance ();
  }

  return 0;
}

// perform conjunctive merge of arrival times of calls:
// all arrival times of all dependencies are duplicated by the
// multiplier and repetition over the new frame size and then
// iteratively merged by choosing the maximal arrival time at
// the current position in each queue (iteration is in lockstep
// over all queues, and ends when any queue ends).
void 
Task_Entry::conjunctive_merge (
  Dependency_Type dt, 
  ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries)
{
  int result = 0;

  // iterate over the dependencies, and determine the total frame size 
  u_long frame_size = 1;
  for (ACE_Unbounded_Set_Iterator <Task_Entry_Link *> dep_iter (callers_);
       dep_iter.done () == 0; dep_iter.advance ())
  {
    Task_Entry_Link *link;
    if ((dep_iter.next (link) == 0) || (link == 0))
    {
      return -1;
    }

    // the link matches the dependency type given
    if (link->dependency_type () == dt)
    {
      frame_size = minimum_frame_size (frame_size, link->caller ().effective_period_);
    }
  }

  // reframe dispatches in the set to the new frame size
  // (expands the set's effective period to be the new enclosing frame)
  if (reframe (dispatches_, effective_period_, frame_size) < 0)
  {
    return -1;
  }

  // A set and iterator for virtual dispatch sets 
  // over which the conjunction will iterate
  ACE_Ordered_MultiSet <Dispatch_Proxy_Iterator *> conj_set;
  ACE_Ordered_MultiSet_Iterator <Dispatch_Proxy_Iterator *> conj_set_iter (conj_set);

  // iterate over the dependencies, and for each of the given call type, 
  // create a Dispatch_Proxy_Iterator for the caller's dispatch set, using
  // the caller's period, the total frame size, and the number of calls:
  // if any of the sets is empty, just return 0;
  for (dep_iter.first (); dep_iter.done () == 0; dep_iter.advance ())
  {
    Task_Entry_Link *link;
    if ((iter.next (link) == 0) || (link == 0))
    {
      return -1;
    }

    // the link matches the dependency type given
    if (link->dependency_type () == dt)
    {
      Dispatch_Proxy_Iterator *proxy_ptr;
      ACE_NEW_RETURN (proxy_ptr, 
                      Dispatch_Proxy_Iterator src_iter (
                        link->caller ().dispatches_, 
                        link->caller ().effective_period_, 
                        frame_size, link->number_of_calls ())
                      -1);

      // if there are no entries in the virtual set, we're done
      if (proxy_ptr->done ())
      {  
        return 0;
      }
      if (conj_set.insert (proxy_ptr, conj_set_iter) < 0)
      {
        return -1;
      }
    }
  }
  
  // loop, adding conjunctive dispatches, until one of the conjunctive
  // dispatch sources runs out of entries over the total frame
  conj_set_iter.first ();
  int more_dispatches = (conj_set_iter.done ()) ? 0 : 1;
  while (more_dispatches)
  {
    for (conj_set_iter.first ();
         conj_set_iter.done () == 0; 
         conj_set_iter.advance ())
	 {
      // initialize to earliest arrival and deadline, and highest priority
      u_long arrival = 0;
      u_long deadline = 0;
      u_long priority = 0;

      // Policy: conjunctively dispatched operations get the latest deadline of any
      //         of the dispatches in the conjunction at the time they were dispatched
      //          - when and if it is useful to change any of the merge policies, this
      //         should be one of the decisions factored out into the conjunctive merge
      //         strategy class.

      // Policy: conjunctively dispatched operations get the lowest priority of any
      //         of the dispatches in the conjunction at the time they were dispatched
      //          - when and if it is useful to change any of the merge policies, this
      //         should be one of the decisions factored out into the conjunctive merge
      //         strategy class.

      // obtain a pointer to the current dispatch proxy iterator
      Dispatch_Proxy_Iterator *proxy_iter;
      if ((conj_set_iter.next (proxy_iter) == 0) || (! proxy_iter)
      {
        return -1;
      }

      // use latest arrival, latest deadline, lowest priority (0 is highest)
      arrival = (arrival < proxy_iter->arrival ()) 
        ? arrival : proxy_iter->arrival ();
      deadline = (deadline < proxy_iter->deadline ()) 
        ? deadline : proxy_iter->deadline ();
      priority = (priority < proxy_iter->priority ()) 
        ? priority : proxy_iter->priority ();

      proxy_iter->advance ();         
      if (proxy_iter->done ())
      {
        more_dispatches = 0;
      }
    }

    // use iterator for efficient insertion into the dispatch set
    ACE_Ordered_MultiSet_Iterator <Dispatch_Entry_Link> insert_iter (dispatches_);

    Dispatch_Entry *entry_ptr;
    ACE_NEW_RETURN (entry_ptr, 
                    Dispatch_Entry (arrival, deadline, priority, *this),
                    -1);

    // if even one new dispatch was inserted, result is "something happened".
    result = 1;

    // add the new dispatch entry to the set of all dispatches, and
    //  a link to it to the dispatch links for this task entry
    if (dispatch_entries_.insert (entry_ptr) < 0)
    {
      return -1;
    }            
    if (dispatches_.insert (Dispatch_Entry_Link (*entry_ptr), insert_iter) < 0)
    {
      return -1;
    }    

    // TBD - Clients are not assigned priority, but rather obtain it from
    // their call dependencies.  We could complain here if there is a
    // priority specified that doesn't match (or is lower QoS?)
  }

  return result;
}

// this static method is used to reframe an existing dispatch set
// to the given new period multiplier, creating new instances of
// each existing dispatch (with adjusted arrival and deadline)
// in each successive sub-frame.  Returns 1 if the set was reframed
// to a new period, 0 if the set was not changed (the new period
// was not a multiple of the old one), or -1 if an error occurred.
int 
Task_Entry::reframe (
  ACE_Ordered_MultiSet <Dispatch_Entry_Link> &set,
  u_long &set_period, u_long new_period)
{
  // make sure the new period is greater than the current
  // set period, and that they are harmonically related
  if (new_period <= set_period)
  {  
    // return an error if they're not harmonically related,
    // do nothing if set's frame is a multiple of the new frame
    return (set_period % new_period) ? -1 : 0;
  }
  else if (new_period % set_period)
  {
    return -1;
  }

  // use an empty ordered multiset for subsequent sub-frame dispatches
  // (the set already holds all dispatches the 0th sub-frame)
  ACE_Ordered_MultiSet <Dispatch_Entry_Link> new_set;

  // merge the old set with its old period into the new (empty)
  // set with the new period, starting after the 0th sub-frame:
  // this puts all dispatches after the 0th sub-frame of the new period
  // in the new set, and leaves all dispatches in the 0th sub-frame of
  // the new period in the old set.
  u_long temp_period = new_period;
  int result = merge_frames (new_set, set, temp_period, set_period, 1, 1);

  // if the period changed during the merge, or an error was returned, bail out
  if ((temp_period != new_period) || result == -1)
  {
    return -1;
  }

  // now, update the period for the set, and merge the 
  // new set into the old set, over the same period
  set_period = new_period;
  int result = merge_frames (set, new_set, set_period, set_period);

  // if the period changed during the merge, return an error
  if (set_period != new_period)
  {
    result = -1;
  }

  return result;
}


// this static method is used to merge an existing dispatch set,
// multiplied by the given multipliers for the period and number of
// instances in each period of each existing dispatch, into the
// given "into" set, without affecting the "from set".
int 
Task_Entry::merge_frames (
  ACE_Ordered_MultiSet <Dispatch_Entry_Link> &dest,
  ACE_Ordered_MultiSet <Dispatch_Entry_Link> &src,
  u_long &dest_period,
  u_long src_period,
  u_long number_of_calls,
  u_long starting_dest_sub_frame)
{
  int status = 0;

  // reframe dispatches in the destination set to the new frame size
  // (expands the destination set's period to be the new enclosing frame)
  if (reframe (dest, dest_period, 
               minimum_frame_size (dest_period, src_period)) < 0)
  {
    return -1;
  }

  // use iterator for efficient insertion into the destination set
  ACE_Ordered_MultiSet_Iterator <Dispatch_Entry_Link> dest_iter (dest);

  // do virutal iteration over the source set in the new frame,
  // adding adjusted dispatch entries to the destination

  for (Dispatch_Proxy_Iterator src_iter (src, src_period, dest_period, 
                                         number_of_calls, 
                                         starting_dest_sub_frame); 
       src_iter.done () == 0; src_iter.advance ())
  {

    // Policy: disjunctively dispatched operations get their deadline and 
    //         priority from the original dispatch - when and if it is useful
    //         to change any of the merge policies, this should be one of the
    //         decisions factored out into the disjunctive merge strategy 
    //         class.

    Dispatch_Entry *entry_ptr;
    ACE_NEW_RETURN (entry_ptr, 
                    Dispatch_Entry (src_iter.arrival (), 
                                    src_iter.deadline (),
                                    src_iter.priority (), *this),
                    -1);

    // if even one new dispatch was inserted, status is "something happened".
    status = 1;

    // add the new dispatch entry to the set of all dispatches, and
    //  a link to it to the dispatch links for this task entry
    if (dispatch_entries_.insert (entry_ptr) < 0)
    {
      return -1;
    }            
    if (dest.insert (Dispatch_Entry_Link (*entry_ptr), dest_iter) < 0)
    {
      return -1;
    }        

    // TBD - Clients are not assigned priority, but rather obtain it from
    // their call dependencies.  We could complain here if there is a 
    // priority specified that doesn't match (or is lower QoS?)    
  }

  return status;
}


///////////////////////////
// Class Task_Entry_Link //
///////////////////////////


Task_Entry_Link::Task_Entry_Link (
  Task_Entry &caller,
  Task_Entry &called,
  CORBA::Long number_of_calls,
  RtecScheduler::Dependency_Type dependency_type) 
  : caller_ (caller)
  , called_ (called)
  , dependency_type_ (dependency_type)
  , number_of_calls_ (number_of_calls) 
{
}


//////////////////////////
// Class Dispatch_Entry //
//////////////////////////

Dispatch_Entry::next_id = 0;

Dispatch_Entry::Dispatch_Entry (
      Preemption_Priority priority,
      Time arrival,
      Time deadline,
      Task_Entry &task_entry)

  : priority_ (priority)
  , OS_priority_ (0)
  , dynamic_subpriority_ (0)
  , static_subpriority_ (0)
  , arrival_ (arrival)
  , deadline_ (deadline)
  , task_entry_ (task_entry)
{
  // obtain, increment the next id
  dispatch_id_ = next_id_++;
}

Dispatch_Entry::Dispatch_Entry (const Dispatch_Entry &d)
  : priority_ (d.priority)
  , OS_priority_ (d.OS_priority_)
  , dynamic_subpriority_ (d.dynamic_subpriority_)
  , static_subpriority_ (d.static_subpriority_)
  , arrival_ (d.arrival)
  , deadline_ (d.deadline)
  , task_entry_ (d.task_entry)
{
  // obtain, increment the next id
  dispatch_id_ = next_id_++;
}


ACE_INLINE int 
Dispatch_Entry::operator < (const Dispatch_Entry &d) const
{
  // for positioning in the ordered dispatch multiset

  // lowest arrival time first
  if (this->arrival_ != d.arrival_)
  {
    return (this->arrival_ < d.arrival_) ? 1 : 0;
  }

  // highest priority second
  if (this->priority_ != d.priority_)
  {
    return (this->priority_ > d.priority_) ? 1 : 0;
  }

  // lowest laxity (highest dynamic sub-priority) third
  Time this_laxity = deadline_ - 
                     task_entry ().rt_info ()->worst_case_execution_time;
  Time that_laxity = d.deadline_ - 
                     d.task_entry ().rt_info ()->worst_case_execution_time;
  if (this_laxity != that_laxity)
  {
    return (this_laxity < that_laxity) ? 1 : 0;
  }

  // finally, by higher importance
  return (task_entry ().rt_info ()->importance >
          d.task_entry ().rt_info ()->importance) ? 1 : 0;  
}


///////////////////////////////
// Class Dispatch_Entry_Link //
///////////////////////////////


Dispatch_Entry_Link::Dispatch_Entry_Link (Dispatch_Entry &d)
  : dispatch_entry_ (d)
{
}
  // ctor

Dispatch_Entry_Link::Dispatch_Entry_Link (
  const Dispatch_Entry_Link &d)
  : dispatch_entry_ (d.dispatch_entry_)
{
}
  // copy ctor


///////////////////////////////////
// Class Dispatch_Proxy_Iterator //
///////////////////////////////////

Dispatch_Proxy_Iterator::Dispatch_Proxy_Iterator 
  (ACE_Ordered_MultiSet <Dispatch_Entry_Link> set,
   u_long actual_frame_size,
   u_long virtual_frame_size,
   u_long number_of_calls,
   u_long starting_sub_frame)
  : number_of_calls_ (number_of_calls)
  , current_call_ (current_call)
  , actual_frame_size_ (actual_frame_size)
  , virtual_frame_size_ (virtual_frame_size)
  , current_frame_offset_ (actual_frame_size * starting_sub_frame)
  , iter_ (set)
{
  first ();
}
      // ctor

int 
Dispatch_Proxy_Iterator::first (u_int sub_frame = 0)
{
  if (actual_frame_size_ * (sub_frame + 1) >= virtual_frame_size_)
  {
    // can not position the virtual iterator
    // in the given range: do nothing
    return 0;
  }

  // restart the call counter
  current_call_ = 0;

  // use the given sub-frame offset if it's valid
  current_frame_offset_ = actual_frame_size_ * sub_frame;

  // restart the iterator
  return iter_.first ();
}
  // positions the iterator at the first entry of the passed 
  // sub-frame, returns 1 if it could position the iterator
  // correctly, 0 if not, and -1 if an error occurred.

int
Dispatch_Proxy_Iterator::last ()
{
  // use the last call
  current_call_ = number_of_calls_ - 1;

  // use the last sub-frame
  current_frame_offset_ = virtual_frame_size_ - actual_frame_size_;

  // position the iterator at the last dispatch
  return iter_.first ();
}
  // positions the iterator at the last entry of the total
  // frame, returns 1 if it could position the iterator
  // correctly, 0 if not, and -1 if an error occurred.

int 
Dispatch_Proxy_Iterator::advance ()
{
  int result = 1;

  if (iter_.done ())
  {
    result = 0; // cannot retreat if we're out of bounds
  }
  else if (current_call_ < number_of_calls_ - 1)
  {
    // if we're still in the same set of calls, increment the call counter
    ++current_call_;
  }
  else
  {
    // roll over the call counter
    current_call_ = 0;

    // advance the iterator in the current sub-frame
    if (! iter_.advance ())
    {
      // if we're not already in the last sub_frame
      if (current_frame_offset_ + actual_frame_size_ < virtual_frame_size_)
      {
        // increment the sub-frame offset
        current_frame_offset_ += actual_frame_size_;

        // restart the iterator at the front of the sub-frame
        result = iter_.first ();
      }
      else
      {
        result = 0; // cannot advance if we're already at the end
      }
    }
  }

  return result;
}
  // positions the iterator at the next entry of the total
  // frame, returns 1 if it could position the iterator
  // correctly, 0 if not, and -1 if an error occurred.

int 
Dispatch_Proxy_Iterator::retreat ()
{
  int result = 1;

  if (iter_.done ())
  {
    result = 0; // cannot retreat if we're out of bounds
  }
  else if (current_call_ > 0)
  {
    // if we're still in the same set of calls, decrement the call counter
    --current_call_;
  }
  else
  {
    // roll over the call counter
    current_call_ = number_of_calls_ - 1;

    // back up the iterator in the current sub-frame
    if (!iter_.retreat ())
    {
      // if we're not already in the 0th sub_frame
      if (current_frame_offset_ > 0)
      {
        // decrement the sub-frame offset
        current_frame_offset_ -= actual_frame_size_;

        // restart the iterator at the tail of the sub-frame
        result = iter_.last ();
      }
      else
      {
        result = 0; // cannot retreat if we're already at the start
      }
    }
  }

  return result;
}
  // positions the iterator at the previous entry of the total
  // frame, returns 1 if it could position the iterator
  // correctly, 0 if not, and -1 if an error occurred.

u_long 
Dispatch_Proxy_Iterator::arrival ()
{
  Dispatch_Entry_Link *link;
  if ((iter_.done ()) || (iter_.next(link) == 0) || (! link))
  {
    return 0;
  }

  return link->dispatch_entry ().arrival () + current_frame_offset_;
}
  // returns the adjusted arrival time of the virtual entry

u_long 
Dispatch_Proxy_Iterator::deadline ()
{
  Dispatch_Entry_Link *link;
  if ((iter_.done ()) || (iter_.next(link) == 0) || (! link))
  {
    return 0;
  }

  return link->dispatch_entry ().deadline () + current_frame_offset_;
}
  // returns the adjusted deadline time of the virtual entry

Preemption_Priority 
Dispatch_Proxy_Iterator::priority ()
{
  Dispatch_Entry_Link *link;
  if ((iter_.done ()) || (iter_.next(link) == 0) || (! link))
  {
    return 0;
  }

  return link->dispatch_entry ().priority ();
}
  // returns the scheduler priority of the virtual entry



//////////////////////////
// Class TimeLine_Entry //
//////////////////////////


    // time slice constructor 
TimeLine_Entry::Timeline_Entry (Dispatch_Entry &dispatch_entry,
                                u_long start, u_long stop,
                                TimeLine_Entry *next,
                                TimeLine_Entry *prev)
  : dispatch_entry_ (dispatch_entry)
  , start_ (start)
  , stop_ (stop)
  , next_ (next)
  , prev_ (prev)
{
}

