
//=============================================================================
/**
 *  @file    SchedEntry.cpp
 *
 *  $Id$
 *
 *  @author Chris Gill
 */
//=============================================================================


#include "orbsvcs/Sched/SchedEntry.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdio.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Sched/SchedEntry.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Task_Entry::Task_Entry (void)
  : rt_info_ (0),
    effective_period_(0),
    dfs_status_ (NOT_VISITED),
    discovered_ (-1),
    finished_ (-1),
    is_thread_delineator_ (0),
    has_unresolved_remote_dependencies_ (0),
    has_unresolved_local_dependencies_ (0),
    calls_ (),
    callers_ ()
{
}

Task_Entry::~Task_Entry (void)
{
  // Zero out the task entry ACT in the corresponding rt_info
  rt_info_->volatile_token = 0;

  ACE_Unbounded_Set_Iterator <Task_Entry_Link *> iter(calls_);
  Task_Entry_Link **link = 0;

  // Iterate through the "calls" set of Task Entry Links and free each one

  for (iter.first ();
       ! iter.done ();
       iter.advance (), link = 0)
    {
      if (iter.next (link) != 0 && link != 0 && *link != 0)
        {
          // remove the link object pointer from the calling entry's
          // "callers" set and destroy the link object
          (*link)->called ().callers_.remove (*link);
          delete (*link);
        }
    }
}

// Merge dispatches according to info type and type of call, update
// relevant scheduling characteristics for this entry.

Task_Entry::Propagation_Status
Task_Entry::merge_dispatches (ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
                              ACE_CString &unresolved_locals,
                              ACE_CString &unresolved_remotes)
{
  Task_Entry::Propagation_Status result = SUCCEEDED;
  switch (info_type ())
    {
    case RtecScheduler::DISJUNCTION:

      // Prohibit two-way dispatches of a disjunction group, and
      // disjunctively merge its one-way dispatches.  NOTE: one
      // interpretation of disjunction for two-way calls is that the
      // caller calls one OR the other, but this is problematic: how
      // do we map the dispatches for this ?
      if (prohibit_dispatches (RtecBase::TWO_WAY_CALL) < 0)
        result = TWO_WAY_DISJUNCTION;

      if (disjunctive_merge (RtecBase::ONE_WAY_CALL,
                             dispatch_entries,
                             unresolved_locals,
                             unresolved_remotes) < 0)
        result = INTERNAL_ERROR;
      break;

    case RtecScheduler::CONJUNCTION:

      // Prohibit two-way dispatches of a conjunction group,
      // and conjunctively merge its one-way dispatches.
      // NOTE: one interpretation of disjunction for two-way calls
      //       is that the caller calls BOTH, so that there is a
      //       disjunctive merge of each two-way, as for the OPERATION
      //       (prohibit for now, as the additional complexity of allowing
      //       conjunctions of two-ways, but not disjunctions does not
      //       buy us anything, anyway).
      if (prohibit_dispatches (RtecBase::TWO_WAY_CALL) < 0)
        result = TWO_WAY_CONJUNCTION;
      if (conjunctive_merge (RtecBase::ONE_WAY_CALL,
                             dispatch_entries,
                             unresolved_locals,
                             unresolved_remotes) < 0)
        result = INTERNAL_ERROR;
      break;

    case RtecScheduler::OPERATION:
    case RtecScheduler::REMOTE_DEPENDANT:

      // Disjunctively merge the operation's two-way dispatches, and
      // conjunctively merge its one-way dispatches.
      if (disjunctive_merge (RtecBase::TWO_WAY_CALL,
                             dispatch_entries,
                             unresolved_locals,
                             unresolved_remotes) < 0)
        result = INTERNAL_ERROR;
      if (conjunctive_merge (RtecBase::ONE_WAY_CALL,
                             dispatch_entries,
                             unresolved_locals,
                             unresolved_remotes) < 0)
        result = INTERNAL_ERROR;
      break;

    default:

      // There should not be any other kind of RT_Info, or if there
      // is, the above switch logic is in need of repair.
      result = UNRECOGNIZED_INFO_TYPE;
      break;
    }

  return result;
}

// Prohibit calls of the given type: currently used to enforce the
// notion that two-way calls to disjunctive or conjunctive RT_Infos do
// not have any defined meaning, and thus should be considered
// dependency specification errors: if these constraints are removed
// in the future, this method should be removed as well Returns 0 if
// all is well, or -1 if an error has occurred.

int
Task_Entry::prohibit_dispatches (Dependency_Type dt)
{
  // Iterate over the set of dependencies, ensuring none of them has
  // the given dependency type.
  for (ACE_Unbounded_Set_Iterator <Task_Entry_Link *> iter (callers_);
       ! iter.done ();
      iter.advance ())
    {
      Task_Entry_Link **link;

      if (iter.next (link) == 0
          || link == 0
          || *link == 0
          || (*link)->dependency_type () == dt)
        return -1;
    }

  return 0;
}

// Perform disjunctive merge of arrival times of oneway calls: all
// arrival times of all dependencies are duplicated by the multiplier
// and repetition over the new frame size.

int
Task_Entry::disjunctive_merge (Dependency_Type dt,
                               ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
                               ACE_CString &unresolved_locals,
                               ACE_CString &unresolved_remotes)
{
  char string_buffer[BUFSIZ];

  // Iterate over the set of dependencies, merging dispatches of the
  // callers over the enclosing frame size.
  for (ACE_Unbounded_Set_Iterator <Task_Entry_Link *> iter (callers_);
       ! iter.done ();
       iter.advance ())
    {
      Task_Entry_Link **link;

      if (iter.next (link) == 0
          || link == 0
          || *link == 0)
        return -1;

      // The link matches the dependency type given
      if ((*link)->dependency_type () == dt)
        {
          // Check for and warn about unresolved remote dependencies
          // in the ONE_WAY call graph.
          if ((*link)->dependency_type () == RtecBase::ONE_WAY_CALL
              && (*link)->caller ().has_unresolved_remote_dependencies ()
              && ! this->has_unresolved_remote_dependencies ())
            {
              // Propagate the unresolved remote dependency flag, and
              // issue a debug scheduler warning.
              this->has_unresolved_remote_dependencies (1);
              ACE_DEBUG ((LM_DEBUG,
                          "Warning: an operation identified by "
                          "\"%s\" has unresolved remote dependencies.\n",
                          (const char*) this->rt_info ()->entry_point));

              // Record entry point in list of unresolved remote
              // dependencies
              ACE_OS::sprintf (string_buffer,
                               "// %s\n",
                               (const char*) this->rt_info ()->entry_point);
              unresolved_remotes +=
                ACE_CString (string_buffer);

            }

          // Check for and warn about unresolved local dependencies in
          // the ONE_WAY call graph.
          if ((*link)->dependency_type () == RtecBase::ONE_WAY_CALL
              && (*link)->caller ().has_unresolved_local_dependencies ()
              && ! this->has_unresolved_local_dependencies ())
            {
              // Propagate the unresolved local dependency flag, and
              // issue a debug scheduler warning.
              this->has_unresolved_local_dependencies (1);
              ACE_DEBUG ((LM_DEBUG,
                          "Warning: an operation identified by "
                          "\"%s\" has unresolved local dependencies.\n",
                          (const char*) this->rt_info ()->entry_point));

              // Record entry point in list of unresolved local
              // dependencies
              ACE_OS::sprintf (string_buffer,
                               "// %s\n",
                               (const char*) this->rt_info ()->entry_point);
              unresolved_locals +=
                ACE_CString (string_buffer);
            }

          // Merge the caller's dispatches into the current set.
          if (merge_frames (dispatch_entries,
                            *this,
                            dispatches_,
                            (*link)->caller ().dispatches_, effective_period_,
                            (*link)->caller ().effective_period_,
                            (*link)->number_of_calls ()) < 0)
            return -1;
        }
    }

  return 0;
}

// Perform conjunctive merge of arrival times of calls: all arrival
// times of all dependencies are duplicated by the multiplier and
// repetition over the new frame size and then iteratively merged by
// choosing the maximal arrival time at the current position in each
// queue (iteration is in lockstep over all queues, and ends when any
// queue ends).

int
Task_Entry::conjunctive_merge (Dependency_Type dt,
                               ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
                               ACE_CString &unresolved_locals,
                               ACE_CString &unresolved_remotes)
{
  int result = 0;
  char string_buffer [BUFSIZ];

  // Iterate over the dependencies, and determine the total frame
  // size.

  u_long frame_size = 1;

  ACE_Unbounded_Set_Iterator <Task_Entry_Link *> dep_iter (callers_);

  for (dep_iter.first ();
       dep_iter.done () == 0;
       dep_iter.advance ())
    {
      Task_Entry_Link **link;

      if (dep_iter.next (link) == 0
          || link == 0
          || *link == 0)
        return -1;

      // The link matches the dependency type given.
      if ((*link)->dependency_type () == dt)
        {
          // Check for and warn about unresolved remote dependencies
          // in the ONE_WAY call graph.
          if ((*link)->dependency_type () == RtecBase::ONE_WAY_CALL
              && (*link)->caller ().has_unresolved_remote_dependencies ()
              && ! this->has_unresolved_remote_dependencies ())
            {
              // Propagate the unresolved remote dependency flag, and
              // issue a debug scheduler warning.
              this->has_unresolved_remote_dependencies (1);
              ACE_DEBUG ((LM_DEBUG,
                          "Warning: an operation identified by "
                          "\"%s\" has unresolved remote dependencies.\n",
                          (const char*) this->rt_info ()->entry_point));

              // Record entry point in list of unresolved remote
              // dependencies
              ACE_OS::sprintf (string_buffer,
                               "// %s\n",
                               (const char*) this->rt_info ()->entry_point);
              unresolved_remotes +=
                ACE_CString (string_buffer);
            }

          // Check for and warn about unresolved local dependencies in
          // the ONE_WAY call graph.
          if ((*link)->dependency_type () == RtecBase::ONE_WAY_CALL
              && (*link)->caller ().has_unresolved_local_dependencies ()
              && ! this->has_unresolved_local_dependencies ())
            {
              // Propagate the unresolved local dependency flag, and
              // issue a debug scheduler warning.
              this->has_unresolved_local_dependencies (1);
              ACE_DEBUG ((LM_DEBUG,
                          "Warning: an operation identified by "
                          "\"%s\" has unresolved local dependencies.\n",
                          (const char*) this->rt_info ()->entry_point));

              // Record entry point in list of unresolved local dependencies
              ACE_OS::sprintf (string_buffer,
                               "// %s\n",
                               (const char*) this->rt_info ()->entry_point);
              unresolved_locals +=
                ACE_CString (string_buffer);
            }

          frame_size = ACE::minimum_frame_size (frame_size,
                                                (*link)->caller ().effective_period_);
        }
    }

  // Reframe dispatches in the set to the new frame size (expands the
  // set's effective period to be the new enclosing frame).
  if (reframe (dispatch_entries,
               *this, dispatches_,
               effective_period_,
               frame_size) < 0)
    return -1;

  // A container and iterator for virtual dispatch sets over which the
  // conjunction will operate
  ACE_Ordered_MultiSet <Dispatch_Proxy_Iterator *> conj_set;
  ACE_Ordered_MultiSet_Iterator <Dispatch_Proxy_Iterator *> conj_set_iter (conj_set);

  // Iterate over the dependencies, and for each of the given call
  // type, create a Dispatch_Proxy_Iterator for the caller's dispatch
  // set, using the caller's period, the total frame size, and the
  // number of calls: if any of the sets is empty, just return 0;
  for (dep_iter.first ();
       dep_iter.done () == 0;
       dep_iter.advance ())
    {
      Task_Entry_Link **link;
      if (dep_iter.next (link) == 0
          || link == 0
          || *link == 0)
        return -1;

      // The link matches the dependency type given.
      if ((*link)->dependency_type () == dt)
        {
          Dispatch_Proxy_Iterator *proxy_ptr;
          ACE_NEW_RETURN (proxy_ptr,
                          Dispatch_Proxy_Iterator ((*link)->caller ().dispatches_,
                                                   (*link)->caller ().effective_period_,
                                                   frame_size,
                                                   (*link)->number_of_calls ()),
                          -1);

          // If there are no entries in the virtual set, we're done.
          if (proxy_ptr->done ())
            return 0;
          else if (conj_set.insert (proxy_ptr, conj_set_iter) < 0)
            return -1;
        }
    }

  // loop, adding conjunctive dispatches, until one of the conjunctive
  // dispatch sources runs out of entries over the total frame
  conj_set_iter.first ();
  int more_dispatches = (conj_set_iter.done ()) ? 0 : 1;
  while (more_dispatches)
    {
      Time arrival = 0;
      Time deadline = 0;
      Preemption_Priority priority = 0;
      OS_Priority OS_priority = 0;

      for (conj_set_iter.first ();
           conj_set_iter.done () == 0;
           conj_set_iter.advance ())
        {
          // initialize to earliest arrival and deadline, and highest priority
          arrival = 0;
          deadline = 0;
          priority = 0;
          OS_priority = 0;

          // Policy: conjunctively dispatched operations get the
          // latest deadline of any of the dispatches in the
          // conjunction at the time they were dispatched - when and
          // if it is useful to change any of the merge policies, this
          // should be one of the decisions factored out into the
          // conjunctive merge strategy class.

          // Policy: conjunctively dispatched operations get the
          // lowest priority of any of the dispatches in the
          // conjunction at the time they were dispatched - when and
          // if it is useful to change any of the merge policies, this
          // should be one of the decisions factored out into the
          // conjunctive merge strategy class.

          // Obtain a pointer to the current dispatch proxy iterator.
          Dispatch_Proxy_Iterator **proxy_iter;
          if (conj_set_iter.next (proxy_iter) == 0
              || proxy_iter == 0
              || *proxy_iter == 0)
            return -1;

          // Use latest arrival, latest deadline, lowest priority (0 is highest).
          if (arrival <= (*proxy_iter)->arrival ())
            arrival = (*proxy_iter)->arrival ();
          if (deadline <= (*proxy_iter)->deadline ())
            deadline = (*proxy_iter)->deadline ();
          if (priority <= (*proxy_iter)->priority ())
            {
              priority = (*proxy_iter)->priority ();
              OS_priority = (*proxy_iter)->OS_priority ();
            }

          (*proxy_iter)->advance ();

          if ((*proxy_iter)->done ())
            more_dispatches = 0;
        }

      Dispatch_Entry *entry_ptr;
      ACE_NEW_RETURN (entry_ptr,
                      Dispatch_Entry (arrival,
                                      deadline,
                                      priority,
                                      OS_priority,
                                      *this),
                      -1);

      // If even one new dispatch was inserted, result is "something
      // happened".
      result = 1;

      // Add the new dispatch entry to the set of all dispatches, and
      // a link to it to the dispatch links for this task entry.
      if (dispatch_entries.insert (entry_ptr) < 0)
        return -1;

      // Use iterator for efficient insertion into the dispatch set.
      ACE_Ordered_MultiSet_Iterator <Dispatch_Entry_Link> insert_iter (dispatches_);
      if (dispatches_.insert (Dispatch_Entry_Link (*entry_ptr),
                              insert_iter) < 0)
        return -1;

      // TBD - Clients are not assigned priority, but rather obtain it
      // from their call dependencies.  We could complain here if
      // there is a priority specified that doesn't match (or is lower
      // QoS?)
    }

  return result;
}

// This static method is used to reframe an existing dispatch set to
// the given new period multiplier, creating new instances of each
// existing dispatch (with adjusted arrival and deadline) in each
// successive sub-frame.  Returns 1 if the set was reframed to a new
// period, 0 if the set was not changed (the new period was not a
// multiple of the old one), or -1 if an error occurred.

int
Task_Entry::reframe (ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
                     Task_Entry &owner,
                     ACE_Ordered_MultiSet <Dispatch_Entry_Link> &set,
                     u_long &set_period, u_long new_period)
{
  int result = 0;

  // if the set period is zero, treat it as uninitialized,
  // and simply value the set period with the new period
  if (set_period)
    {
      // make sure the new period is greater than the current
      // set period, and that they are harmonically related
      if (new_period <= set_period)
        // return an error if they're not harmonically related,
        // do nothing if set's frame is a multiple of the new frame
        return (set_period % new_period) ? -1 : 0;
      else if (new_period % set_period)
        return -1;

      // make a shallow copy of the set in a new ordered multiset
      // using the Dispatch_Entry_Link smart pointers
      ACE_Ordered_MultiSet <Dispatch_Entry_Link> new_set;
      ACE_Ordered_MultiSet_Iterator <Dispatch_Entry_Link> new_iter (new_set);
      ACE_Ordered_MultiSet_Iterator <Dispatch_Entry_Link> set_iter (set);

      for (set_iter.first (); set_iter.done () == 0; set_iter.advance ())
        {
          Dispatch_Entry_Link *link;

          if (set_iter.next (link) == 0)
            return -1;
          else if (new_set.insert (*link, new_iter) < 0)
            return -1;
        }

      // Do a deep copy merge back into the set using the new period
      // and starting after the 0th sub-frame: this puts all
      // dispatches after the 0th sub-frame of the new period into the
      // set, and leaves existing dispatches in the 0th sub-frame of
      // the new period in the set as well.
      result = merge_frames (dispatch_entries,
                             owner,
                             set,
                             new_set,
                             new_period,
                             set_period,
                             1,
                             1);
    }

  // update the set's period to be the new frame
  set_period = new_period;

  return result;
}

// This static method is used to merge an existing dispatch set,
// multiplied by the given multipliers for the period and number of
// instances in each period of each existing dispatch, into the given
// "into" set, without affecting the "from set".

int
Task_Entry::merge_frames (ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
                          Task_Entry &owner,
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
  if (reframe (dispatch_entries,
               owner,
               dest,
               dest_period,
               ACE::minimum_frame_size (dest_period,
                                        src_period)) < 0)
      return -1;

  // use iterator for efficient insertion into the destination set
  ACE_Ordered_MultiSet_Iterator <Dispatch_Entry_Link> dest_iter (dest);

  // do virtual iteration over the source set in the new frame, adding
  // adjusted dispatch entries to the destination
  Dispatch_Proxy_Iterator src_iter (src,
                                    src_period,
                                    dest_period,
                                    number_of_calls,
                                    starting_dest_sub_frame);

  for (src_iter.first (starting_dest_sub_frame);
       src_iter.done () == 0;
       src_iter.advance ())
    {
      // Policy: disjunctively dispatched operations get their
      // deadline and priority from the original dispatch - when and
      // if it is useful to change any of the merge policies, this
      // should be one of the decisions factored out into the
      // disjunctive merge strategy class.

      Dispatch_Entry *entry_ptr;
      ACE_NEW_RETURN (entry_ptr,
                      Dispatch_Entry (src_iter.arrival (),
                                      src_iter.deadline (),
                                      src_iter.priority (),
                                      src_iter.OS_priority (),
                                      owner),
                      -1);

      // if even one new dispatch was inserted, status is "something happened".
      status = 1;

      // add the new dispatch entry to the set of all dispatches, and
      // a link to it to the dispatch links for this task entry
      if (dispatch_entries.insert (entry_ptr) < 0)
        return -1;

      else if (dest.insert (Dispatch_Entry_Link (*entry_ptr), dest_iter) < 0)
        return -1;

      // TBD - Clients are not assigned priority, but rather obtain it
      // from their call dependencies.  We could complain here if
      // there is a priority specified that doesn't match (or is lower
      // QoS?)
    }

  return status;
}

Task_Entry_Link::Task_Entry_Link (Task_Entry &caller,
                                  Task_Entry &called,
                                  CORBA::Long number_of_calls,
                                  RtecScheduler::Dependency_Type_t dependency_type)
  : number_of_calls_ (number_of_calls),
    caller_ (caller),
    called_ (called),
    dependency_type_ (dependency_type)
{
}

Dispatch_Entry::Dispatch_Id Dispatch_Entry::next_id_ = 0;

Dispatch_Entry::Dispatch_Entry (Time arrival,
                                Time deadline,
                                Preemption_Priority priority,
                                OS_Priority os_priority,
                                Task_Entry &task_entry,
                                Dispatch_Entry *original_dispatch)
  : priority_ (priority),
    OS_priority_ (os_priority),
    dynamic_subpriority_ (0),
    static_subpriority_ (0),
    arrival_ (arrival),
    deadline_ (deadline),
    task_entry_ (task_entry),
    original_dispatch_ (original_dispatch)
{
  // obtain, increment the next id
  dispatch_id_ = next_id_++;
}

Dispatch_Entry::Dispatch_Entry (const Dispatch_Entry &d)
  : priority_ (d.priority_),
    OS_priority_ (d.OS_priority_),
    dynamic_subpriority_ (d.dynamic_subpriority_),
    static_subpriority_ (d.static_subpriority_),
    arrival_ (d.arrival_),
    deadline_ (d.deadline_),
    task_entry_ (d.task_entry_),
    original_dispatch_ (d.original_dispatch_)
{
  // obtain, increment the next id
  dispatch_id_ = next_id_++;
}

bool
Dispatch_Entry::operator < (const Dispatch_Entry &d) const
{
  // for positioning in the ordered dispatch multiset

  // lowest arrival time first
  if (this->arrival_ != d.arrival_)
    return this->arrival_ < d.arrival_ ? true : false;

  // highest priority second
  if (this->priority_ != d.priority_)
    return this->priority_ > d.priority_ ? true : false;

  // lowest laxity (highest dynamic sub-priority) third Just use low
  // 32 bits of worst_case_execution_time.  This will have to change
  // when TimeBase.idl is finalized.
  //
  // NOTE: Leave the -= code intact as it's a workaround of a BCB4
  // internal compiler error.
  Time this_laxity = deadline_;
  this_laxity -= task_entry ().rt_info ()->worst_case_execution_time;

  Time that_laxity = d.deadline_;
  that_laxity -= d.task_entry ().rt_info ()->worst_case_execution_time;

  if (this_laxity != that_laxity)
    return (this_laxity < that_laxity) ? true : false;

  // finally, by higher importance
  return (task_entry ().rt_info ()->importance >
          d.task_entry ().rt_info ()->importance) ? true : false;
}

// ctor

Dispatch_Entry_Link::Dispatch_Entry_Link (Dispatch_Entry &d)
  : dispatch_entry_ (d)
{
}

// copy ctor

Dispatch_Entry_Link::Dispatch_Entry_Link (const Dispatch_Entry_Link &d)
  : dispatch_entry_ (d.dispatch_entry_)
{
}

// ctor

Dispatch_Proxy_Iterator::Dispatch_Proxy_Iterator
  (ACE_Ordered_MultiSet <Dispatch_Entry_Link> &set,
   u_long actual_frame_size,
   u_long virtual_frame_size,
   u_long number_of_calls,
   u_long starting_sub_frame)
  : number_of_calls_ (number_of_calls),
    current_call_ (0),
    actual_frame_size_ (actual_frame_size),
    virtual_frame_size_ (virtual_frame_size),
    current_frame_offset_ (actual_frame_size * starting_sub_frame),
    iter_ (set)
{
  first (starting_sub_frame);
}

// positions the iterator at the first entry of the passed sub-frame,
// returns 1 if it could position the iterator correctly, 0 if not,
// and -1 if an error occurred.

int
Dispatch_Proxy_Iterator::first (u_int sub_frame)
{
  if (actual_frame_size_ * (sub_frame) >= virtual_frame_size_)
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

// positions the iterator at the last entry of the total frame,
// returns 1 if it could position the iterator correctly, 0 if not,
// and -1 if an error occurred.

int
Dispatch_Proxy_Iterator::last (void)
{
  // use the last call
  current_call_ = number_of_calls_ - 1;

  // use the last sub-frame
  current_frame_offset_ = virtual_frame_size_ - actual_frame_size_;

  // position the iterator at the last dispatch
  return iter_.first ();
}

// positions the iterator at the next entry of the total frame,
// returns 1 if it could position the iterator correctly, 0 if not,
// and -1 if an error occurred.

int
Dispatch_Proxy_Iterator::advance (void)
{
  int result = 1;

  if (iter_.done ())
    result = 0; // cannot retreat if we're out of bounds
  else if (current_call_ < number_of_calls_ - 1)
    // if we're still in the same set of calls, increment the call counter
    ++current_call_;
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
            result = 0; // cannot advance if we're already at the end
        }
    }

  return result;
}

// positions the iterator at the previous entry of the total frame,
// returns 1 if it could position the iterator correctly, 0 if not,
// and -1 if an error occurred.

int
Dispatch_Proxy_Iterator::retreat (void)
{
  int result = 1;

  if (iter_.done ())
    result = 0; // cannot retreat if we're out of bounds
  else if (current_call_ > 0)
    // if we're still in the same set of calls, decrement the call counter
    --current_call_;
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
            result = 0; // cannot retreat if we're already at the start
        }
    }

  return result;
}

// returns the adjusted arrival time of the virtual entry

RtecScheduler::Time
Dispatch_Proxy_Iterator::arrival (void) const
{
  Dispatch_Entry_Link *link;
  if (iter_.done ()
      || iter_.next(link) == 0
      || link == 0)
    return 0;

  // Just use low 32 bits of arrival.  This will have to change when
  // TimeBase.idl is finalized.
  return link->dispatch_entry ().arrival () +
         RtecScheduler::Time (current_frame_offset_);
}

// returns the adjusted deadline time of the virtual entry

RtecScheduler::Time
Dispatch_Proxy_Iterator::deadline (void) const
{
  Dispatch_Entry_Link *link;
  if (iter_.done ()
      || iter_.next(link) == 0
      || link == 0)
    return 0;

  // Just use low 32 bits of deadline.  This will have to change when
  // TimeBase.idl is finalized.
  return link->dispatch_entry ().deadline () +
         RtecScheduler::Time (current_frame_offset_);
}

// returns the scheduler priority of the virtual entry

Dispatch_Proxy_Iterator::Preemption_Priority
Dispatch_Proxy_Iterator::priority (void) const
{
  Dispatch_Entry_Link *link;

  if (iter_.done ()
      || iter_.next(link) == 0
      || link == 0)
    return 0;

  return link->dispatch_entry ().priority ();
}

// returns the OS priority of the virtual entry

Dispatch_Proxy_Iterator::OS_Priority
Dispatch_Proxy_Iterator::OS_priority (void) const
{
  Dispatch_Entry_Link *link;
  if (iter_.done ()
      || iter_.next(link) == 0
      || link == 0)
    return 0;

  return link->dispatch_entry ().OS_priority ();
}

// time slice constructor

TimeLine_Entry::TimeLine_Entry (Dispatch_Entry &dispatch_entry,
                                Time start, Time stop,
                                Time arrival, Time deadline,
                                TimeLine_Entry *next,
                                TimeLine_Entry *prev)
  : dispatch_entry_ (dispatch_entry),
    start_ (start),
    stop_ (stop),
    arrival_ (arrival),
    deadline_ (deadline),
    next_ (next),
    prev_ (prev)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
