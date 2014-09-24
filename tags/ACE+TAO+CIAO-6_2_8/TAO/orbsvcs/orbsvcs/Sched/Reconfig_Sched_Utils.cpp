//=============================================================================
/**
 *  @file    Reconfig_Sched_Utils.cpp
 *
 *  $Id$
 *
 *  @author  Chris Gill <cdgill@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_RECONFIG_SCHED_UTILS_C
#define TAO_RECONFIG_SCHED_UTILS_C

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Sched/Reconfig_Sched_Utils_T.h"
#include "orbsvcs/Sched/Reconfig_Sched_Utils.h"


#include "orbsvcs/Time_Utilities.h"




TAO_BEGIN_VERSIONED_NAMESPACE_DECL

///////////////////////////
// struct TAO_RT_Info_Ex //
///////////////////////////

// Default Constructor.

TAO_RT_Info_Ex::TAO_RT_Info_Ex ()
{
  // Note: the entry_point string takes care of itself.
  handle = 0;
  criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  worst_case_execution_time = 0;
  typical_execution_time = 0;
  cached_execution_time = 0;
  period = 0;
  importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  quantum = 0;
  threads = 0;
  info_type = RtecScheduler::OPERATION;
  priority = 0;
  preemption_subpriority = 0;
  preemption_priority = 0;
  enabled = RtecScheduler::RT_INFO_ENABLED;
  volatile_token = 0;
}


// Constructor from an RT_Info
// (Also serves as a copy constructor)

TAO_RT_Info_Ex::TAO_RT_Info_Ex (const RtecScheduler::RT_Info &info)
{
  this->entry_point = info.entry_point;
  this->handle = info.handle;
  this->criticality = info.criticality;
  this->worst_case_execution_time = info.worst_case_execution_time;
  this->typical_execution_time = info.typical_execution_time;
  this->cached_execution_time = info.cached_execution_time;
  this->period = info.period;
  this->importance = info.importance;
  this->quantum = info.quantum;
  this->threads = info.threads;
  this->info_type = info.info_type;
  this->priority = info.priority;
  this->preemption_subpriority = info.preemption_subpriority;
  this->preemption_priority = info.preemption_priority;
  this->enabled = info.enabled;  // TODO- rethink?
  this->volatile_token = info.volatile_token;
}


// Destructor.
TAO_RT_Info_Ex::~TAO_RT_Info_Ex ()
{
}


// Assignment operator with an RT_Info on the RHS.

void
TAO_RT_Info_Ex::operator = (const RtecScheduler::RT_Info &info)
{
  // IMPORTANT: we don't copy the name or the handle or the output
  // attributes or the volatile token (entry pointer) or the valid
  // flag.  These can only be copied in the copy ctor at
  // initialization.

  criticality = info.criticality;
  worst_case_execution_time = info.worst_case_execution_time;
  typical_execution_time = info.typical_execution_time;
  cached_execution_time = info.cached_execution_time;
  period = info.period;
  importance = info.importance;
  quantum = info.quantum;
  threads = info.threads;
  info_type = info.info_type;
  enabled = info.enabled;
}


// Resets all data members to initial (invalid) values, and removes
// tuples corresponding to the reset flags.

void
TAO_RT_Info_Ex::reset (u_long reset_flags)
{
  // IMPORTANT: among the input arguments, we only reset the period ...
  // TBD - if execution times etc. can be selected as well, then reset those, e.g.,
  //
  // criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  // worst_case_execution_time = 0;
  // typical_execution_time = 0;
  // cached_execution_time = 0;
  // importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  // quantum = 0;
  // threads = 0;
  // info_type = RtecScheduler::OPERATION;

  period = 0;

  // ... However, we do reset the output attributes ...

  priority = 0;
  preemption_subpriority = 0;
  preemption_priority = 0;

  // ... and the appropriate tuples associated with the entry.
  TAO_Reconfig_Scheduler_Entry * entry_ptr =
      ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                           volatile_token);

  //ORBSVCS_DEBUG((LM_DEBUG, "Removing Entries for RT_Info: %d, entry_ptr: %x\n", handle, entry_ptr));
  if (entry_ptr)
    {
      entry_ptr->remove_tuples (reset_flags);
    }
  else
    {
      ORBSVCS_ERROR ((LM_ERROR, "Pointer to associated entry is zero."));
    }
}

void
TAO_RT_Info_Ex::enabled_state (RtecScheduler::RT_Info_Enabled_Type_t enabled_in)
{
  TAO_Reconfig_Scheduler_Entry * entry_ptr =
      ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                           volatile_token);
  if (entry_ptr)
    {
      this->enabled = enabled_in;
      entry_ptr->enabled_state (enabled_in);
    }
  else
    {
      ORBSVCS_ERROR ((LM_ERROR, "Pointer to associated entry is zero."));
    }
}

RtecScheduler::RT_Info_Enabled_Type_t
TAO_RT_Info_Ex::enabled_state ()
{
  return this->enabled;
}



//////////////////////////////
// struct TAO_RT_Info_Tuple //
//////////////////////////////

// Default Constructor.
TAO_RT_Info_Tuple::TAO_RT_Info_Tuple ()
  : rate_index (0)
{
}


// Constructor from an RT_Info.
// (Also serves as a copy constructor)

TAO_RT_Info_Tuple::TAO_RT_Info_Tuple (const RtecScheduler::RT_Info &info)
  : TAO_RT_Info_Ex (info),
    rate_index (0)
{
}

// Destructor.

TAO_RT_Info_Tuple::~TAO_RT_Info_Tuple ()
{
}


// Assignment operator with an RT_Info on the RHS.

void
TAO_RT_Info_Tuple::operator = (const RtecScheduler::RT_Info &info)
{
  static_cast<TAO_RT_Info_Ex> (*this) = info;
}


// Less-than comparison operator: orders tuples by ascending rate (descending period).

bool
TAO_RT_Info_Tuple::operator < (const TAO_RT_Info_Tuple &t)
{
  return (this->period > t.period) ? true : false;
}


////////////////////////////////////////
// class TAO_Reconfig_Scheduler_Entry //
////////////////////////////////////////

// Constructor.

TAO_Reconfig_Scheduler_Entry::
TAO_Reconfig_Scheduler_Entry (TAO_RT_Info_Ex &rt_info)
  : actual_rt_info_ (&rt_info),
    fwd_dfs_status_ (NOT_VISITED),
    rev_dfs_status_ (NOT_VISITED),
    fwd_discovered_ (-1),
    rev_discovered_ (-1),
    fwd_finished_ (-1),
    rev_finished_ (-1),
    is_thread_delineator_ (0),
    has_unresolved_remote_dependencies_ (0),
    has_unresolved_local_dependencies_ (0),
    aggregate_exec_time_ (0),
    orig_tuple_period_sum_ (0),
    prop_tuple_period_sum_ (0),
    orig_tuple_count_ (0),
    prop_tuple_count_ (0),
    current_admitted_tuple_ (0),
    enabled_ (rt_info.enabled)
  //    effective_exec_multiplier_ (0),  //WSOA merge commented out
  //  effective_period_ (0)              //WSOA merge commented out
{
  // Store the RT_Info fields.
  this->orig_rt_info_data (*actual_rt_info_);
}

// Constructor.

TAO_Reconfig_Scheduler_Entry::
~TAO_Reconfig_Scheduler_Entry ()
{
  this->remove_tuples (ORIGINAL | PROPAGATED);
}


// Removes all tuples from the entry.

void
TAO_Reconfig_Scheduler_Entry::
remove_tuples (u_long tuple_flags)
{
  TAO_RT_Info_Tuple **tuple_ptr_ptr;

  if (tuple_flags & ORIGINAL)
    {
      TUPLE_SET_ITERATOR orig_tuple_iter (this->orig_tuple_subset_);

      while (orig_tuple_iter.done () == 0)
        {
          if (orig_tuple_iter.next (tuple_ptr_ptr) == 0
              || tuple_ptr_ptr == 0 || *tuple_ptr_ptr == 0)
            {
              ORBSVCS_ERROR ((LM_ERROR,
                          "Failed to access tuple under iterator"));
              return;
            }

          delete (*tuple_ptr_ptr);

          orig_tuple_iter.advance ();
        }

      this->orig_tuple_subset_.reset ();
    }

  // If either the originals or the propagated tuple pointers are to
  // be removed, we have to get rid of the propagated pointers lest
  // they become handles to access violations after the original
  // tuples are destroyed.
  if (tuple_flags & PROPAGATED
      || tuple_flags & ORIGINAL)
    {
      this->prop_tuple_subset_.reset ();
    }
}


// Adds a new tuple to the entry and updates the
// rate indices and mean rate for the tuples.

int
TAO_Reconfig_Scheduler_Entry::
insert_tuple (TAO_RT_Info_Tuple &tuple,
              Tuple_Type tuple_type,
              int replace)
{
  // Choose the appropriate tuple subset.
  TUPLE_SET *set_ptr = (tuple_type == ORIGINAL) ? & orig_tuple_subset_ : & prop_tuple_subset_;

  // Recompute rate indices.

  tuple.rate_index = 0;
  TAO_RT_Info_Tuple **tuple_ptr_ptr;

  TUPLE_SET_ITERATOR tuple_iter (*set_ptr);

  while (tuple_iter.done () == 0)
    {
      // Get a pointer to the tuple under the iterator.
      if (tuple_iter.next (tuple_ptr_ptr) == 0
          || tuple_ptr_ptr == 0 || *tuple_ptr_ptr == 0)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR, "Failed to access tuple under iterator"), -1);
        }

      // Update existing tuples
      if ((*tuple_ptr_ptr)->period > tuple.period)
        {
          // Move the tuple's rate index higher than any in the set
          // with lower rates.
          ++tuple.rate_index;
        }
      else if (replace && (*tuple_ptr_ptr)->period == tuple.period)
        {
          // If the replace flag is set, and there is already a tuple
          // with the same rate in the set, just update that tuple and
          // return.
          **tuple_ptr_ptr = tuple;
          return 1;
        }
      else
        {
          // Otherwise, just update the rate index of the subsequent
          // tuples, which have the same or higher rates.
          ++(*tuple_ptr_ptr)->rate_index;
        }

      tuple_iter.advance ();
    }

  // Update aggregate rate data, insert the tuple
  if (tuple_type == ORIGINAL)
    {
      this->orig_tuple_period_sum_ += tuple.period;
      ++this->orig_tuple_count_;
      return (this->orig_tuple_subset_.insert (&tuple) < 0) ? -1 : 0;
    }
  else
    {
      this->prop_tuple_period_sum_ += tuple.period;
      ++this->prop_tuple_count_;
      return (this->prop_tuple_subset_.insert (&tuple) < 0) ? -1 : 0;
    }
}


// Updates a matching tuple.

int
TAO_Reconfig_Scheduler_Entry::
update_tuple (TAO_RT_Info_Ex &info,
              Tuple_Type tuple_type)
{
  // Choose the appropriate tuple subset.
  TUPLE_SET *set_ptr = (tuple_type == ORIGINAL) ? & orig_tuple_subset_ : & prop_tuple_subset_;

  // Find and update the first matching tuple, if any.

  TAO_RT_Info_Tuple **tuple_ptr_ptr;
  TUPLE_SET_ITERATOR tuple_iter (*set_ptr);

  while (tuple_iter.done () == 0)
    {
      // Get a pointer to the tuple under the iterator.
      if (tuple_iter.next (tuple_ptr_ptr) == 0
          || tuple_ptr_ptr == 0 || *tuple_ptr_ptr == 0)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR, "Failed to access tuple under iterator"), -1);
        }
      else if ((*tuple_ptr_ptr)->period < info.period)
        {
          // If we've hit a tuple with a shorter period (higher rate),
          // then we're done.
          break;
        }
      else if ((*tuple_ptr_ptr)->period == info.period)
        {
          // If the replace flag is set, and there is already a tuple
          // with the same rate in the set, just update that tuple and
          // return.
          **tuple_ptr_ptr = info;
          return 1;
        }

      tuple_iter.advance ();
    }

  return 0;
}


// Registers tuples into the passed tuple pointer array.
int
TAO_Reconfig_Scheduler_Entry::
register_tuples (TAO_RT_Info_Tuple ** tuple_ptr_array,
                 long &tuple_count)
{
  // Iterate over the tuples, adding them to the pointer array.

  TAO_RT_Info_Tuple **tuple_ptr_ptr;
  TUPLE_SET_ITERATOR tuple_iter (orig_tuple_subset_);

  while (tuple_iter.done () == 0)
    {
      // Get a pointer to the tuple under the iterator.
      if (tuple_iter.next (tuple_ptr_ptr) == 0
          || tuple_ptr_ptr == 0 || *tuple_ptr_ptr == 0)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR, "Failed to access tuple under iterator"), -1);
        }
      else
        {
          tuple_ptr_array [tuple_count] = *tuple_ptr_ptr;
          ++tuple_count;
        }

      tuple_iter.advance ();
    }

  return 0;
}




// Accessor for original RT_Info data.

TAO_RT_Info_Ex &
TAO_Reconfig_Scheduler_Entry::orig_rt_info_data ()
{
  return orig_rt_info_data_;
}


// Mutator for stored original RT_Info data.

void
TAO_Reconfig_Scheduler_Entry::orig_rt_info_data (TAO_RT_Info_Ex &data)
{
  // Only store the information that can be updated by the public interface at run-time.
  this->orig_rt_info_data_.worst_case_execution_time = data.worst_case_execution_time;
  this->orig_rt_info_data_.typical_execution_time = data.typical_execution_time;
  this->orig_rt_info_data_.cached_execution_time = data.cached_execution_time;
  this->orig_rt_info_data_.period = data.period;
  this->orig_rt_info_data_.criticality = data.criticality;
  this->orig_rt_info_data_.importance = data.importance;
  this->orig_rt_info_data_.quantum = data.quantum;
  this->orig_rt_info_data_.threads = data.threads;
  this->orig_rt_info_data_.info_type = data.info_type;
  this->orig_rt_info_data_.enabled = data.enabled;
}

// Accessor for actual RT_Info pointer.

TAO_RT_Info_Ex *
TAO_Reconfig_Scheduler_Entry::
actual_rt_info ()
{
  return this->actual_rt_info_;
}


// Mutator for actual RT_Info pointer.

void
TAO_Reconfig_Scheduler_Entry::actual_rt_info (TAO_RT_Info_Ex *rt_info)
{
  this->actual_rt_info_ = rt_info;
}


// Accessor for when the node was discovered in forward DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::fwd_discovered () const
{
  return this->fwd_discovered_;
}


// Mutator for when the node was discovered in forward DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::fwd_discovered (long l)
{
  this->fwd_discovered_ = l;
}


// Accessor for when the node was discovered in reverse DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::rev_discovered () const
{
  return this->rev_discovered_;
}


// Mutator for when the node was discovered in reverse DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::rev_discovered (long l)
{
  this->rev_discovered_ = l;
}


// Accessor for when the node was finished in forward DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::fwd_finished () const
{
  return this->fwd_finished_;
}


// Mutator for when the node was finished in forward DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::fwd_finished (long l)
{
  this->fwd_finished_ = l;
}


// Accessor for when the node was finished in reverse DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::rev_finished () const
{
  return this->rev_finished_;
}


// Mutator for when the node was finished in reverse DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::rev_finished (long l)
{
  this->rev_finished_ = l;
}


// Accessor for forward DFS traversal status of the node.

TAO_Reconfig_Scheduler_Entry::DFS_Status
TAO_Reconfig_Scheduler_Entry::fwd_dfs_status () const
{
  return this->fwd_dfs_status_;
}


// Mutator for forward DFS traversal status of the node.

void
TAO_Reconfig_Scheduler_Entry::
fwd_dfs_status (TAO_Reconfig_Scheduler_Entry::DFS_Status ds)
{
  this->fwd_dfs_status_ = ds;
}



// Accessor for DFS traversal status of the node.

TAO_Reconfig_Scheduler_Entry::DFS_Status
TAO_Reconfig_Scheduler_Entry::rev_dfs_status () const
{
  return this->rev_dfs_status_;
}


// Mutator for DFS traversal status of the node.

void
TAO_Reconfig_Scheduler_Entry::
rev_dfs_status (TAO_Reconfig_Scheduler_Entry::DFS_Status ds)
{
  this->rev_dfs_status_ = ds;
}


// Accessor for flag indicating whether node is a thread
// delineator.

int
TAO_Reconfig_Scheduler_Entry::is_thread_delineator () const
{
  return this->is_thread_delineator_;
}


// Mutator for flag indicating whether node is a thread
// delineator.

void
TAO_Reconfig_Scheduler_Entry::is_thread_delineator (int i)
{
  this->is_thread_delineator_ = i;
}


// Accessor for flag indicating whether node has unresolved remote
// dependencies.

int
TAO_Reconfig_Scheduler_Entry::
has_unresolved_remote_dependencies () const
{
  return this->has_unresolved_remote_dependencies_;
}


// Mutator for flag indicating whether node has unresolved remote
// dependencies.

void
TAO_Reconfig_Scheduler_Entry::has_unresolved_remote_dependencies (int i)
{
  this->has_unresolved_remote_dependencies_ = i;
}


// Accessor for flag indicating whether node has unresolved local
// dependencies.

int
TAO_Reconfig_Scheduler_Entry::has_unresolved_local_dependencies () const
{
  return this->has_unresolved_local_dependencies_;
}


// Mutator for flag indicating whether node has unresolved local
// dependencies.

void
TAO_Reconfig_Scheduler_Entry::has_unresolved_local_dependencies (int i)
{
  this->has_unresolved_local_dependencies_ = i;
}

/* WSOA merge - commented out
// Accessor for effective period of corresponding RT_Info.

RtecScheduler::Period_t
TAO_Reconfig_Scheduler_Entry::
effective_period ()
{
  return this->effective_period_;
}


// Mutator for effective period of corresponding RT_Info.

void
TAO_Reconfig_Scheduler_Entry::
effective_period (RtecScheduler::Period_t p)
{
  this->effective_period_ = p;
}


// Accessor for effective execution time of corresponding RT_Info.

CORBA::Long
TAO_Reconfig_Scheduler_Entry::
effective_exec_multiplier ()
{
  return this->effective_exec_multiplier_;
}


// Mutator for effective execution time of corresponding RT_Info.

void
TAO_Reconfig_Scheduler_Entry::
effective_exec_multiplier (CORBA::Long l)
{
  this->effective_exec_multiplier_ = l;
}
*/

///////////////////////////

TAO_Reconfig_Sched_Entry_Visitor::~TAO_Reconfig_Sched_Entry_Visitor (void)
{
}

///////////////////////////
// TAO_RSE_Reset_Visitor //
///////////////////////////

// Constructor.

TAO_RSE_Reset_Visitor::TAO_RSE_Reset_Visitor ()
{
}


// Resets the fields in the entry to pre-DFS traversal states.
// Returns 0 on success and -1 on error.

int
TAO_RSE_Reset_Visitor::visit (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Note that this value differs from what is set in the
  // constructor for the entry.  This is because the reset
  // visitor is applied prior to a DFS traversal, in which callers
  // *unset* the thread delineator status of any of their called
  // operations that do not specify a period or threads.

  if (rse.actual_rt_info ()->enabled != RtecScheduler::RT_INFO_NON_VOLATILE)
  {
     rse.is_thread_delineator (1);

     // Only reset the period for entries that are not root nodes.  Added by BRM.
     if (rse.actual_rt_info ()->threads == 0)
     {
     rse.actual_rt_info ()->period = 0;
     }
  }

     // Remove the propagated tuples in the entry.
     rse.remove_tuples (TAO_Reconfig_Scheduler_Entry::PROPAGATED);

  rse.fwd_dfs_status (TAO_Reconfig_Scheduler_Entry::NOT_VISITED);
  rse.rev_dfs_status (TAO_Reconfig_Scheduler_Entry::NOT_VISITED);
  rse.fwd_discovered (-1);
  rse.rev_discovered (-1);
  rse.fwd_finished (-1);
  rse.rev_finished (-1);
  rse.has_unresolved_remote_dependencies (0);
  rse.has_unresolved_local_dependencies (0);
  rse.aggregate_exec_time (rse.actual_rt_info ()->worst_case_execution_time);
  rse.current_admitted_tuple (0);

  //WSOA merge - commented out
  // These settings are used for a conservative but
  // efficient approach to estimating utilization:
  // for an exact algorithm using frame merging,
  // other initial settings might be needed.
  //rse.effective_exec_multiplier (0);
  //rse.effective_period (0);

  return 0;
}


// Accessor for effective execution time of corresponding RT_Info.

RtecScheduler::Time
TAO_Reconfig_Scheduler_Entry::aggregate_exec_time ()
{
  return this->aggregate_exec_time_;
}


// Mutator for effective execution time of corresponding RT_Info.

void
TAO_Reconfig_Scheduler_Entry::aggregate_exec_time (RtecScheduler::Time t)
{
  this->aggregate_exec_time_ = t;
}

// Accessor for the sum of periods for tuples directly associated
// with the entry.
RtecScheduler::Period_t
TAO_Reconfig_Scheduler_Entry::orig_tuple_period_sum ()
{
  return orig_tuple_period_sum_;
}


// Mutator for the sum of periods for tuples directly associated
// with the entry.
void
TAO_Reconfig_Scheduler_Entry::orig_tuple_period_sum (RtecScheduler::Period_t p)
{
  orig_tuple_period_sum_ = p;
}


// Accessor for the sum of periods for tuples propagated via
// dependencies on other entries.
RtecScheduler::Period_t
TAO_Reconfig_Scheduler_Entry::prop_tuple_period_sum ()
{
  return prop_tuple_period_sum_;
}


// Mutator for the sum of periods for tuples propagated via
// dependencies on other entries.
void
TAO_Reconfig_Scheduler_Entry::prop_tuple_period_sum (RtecScheduler::Period_t p)
{
  prop_tuple_period_sum_ = p;
}


//  Accessor for the number of tuples directly associated with the
//  entry.
u_int
TAO_Reconfig_Scheduler_Entry::orig_tuple_count ()
{
  return orig_tuple_count_;
}


//  Mutator for the number of tuples directly associated with the
//  entry.
void
TAO_Reconfig_Scheduler_Entry::orig_tuple_count (u_int c)
{
  orig_tuple_count_ = c;
}


// Accessor for the number of tuples propagated via dependencies on
// other entries.
u_int
TAO_Reconfig_Scheduler_Entry::prop_tuple_count ()
{
  return prop_tuple_count_;
}


// Mutator for the number of tuples propagated via dependencies on
// other entries.
void
TAO_Reconfig_Scheduler_Entry::prop_tuple_count (u_int c)
{
  prop_tuple_count_ = c;
}


// Accessor for the set of tuples directly associated with the
// entry.
TUPLE_SET &
TAO_Reconfig_Scheduler_Entry::orig_tuple_subset ()
{
  return orig_tuple_subset_;
}


// Accessor for the set of tuples propagated via dependencies on
// other entries.
TUPLE_SET &
TAO_Reconfig_Scheduler_Entry::prop_tuple_subset ()
{
  return prop_tuple_subset_;
}


TAO_RT_Info_Tuple *
TAO_Reconfig_Scheduler_Entry::current_admitted_tuple ()
{
  return current_admitted_tuple_;
}


void
TAO_Reconfig_Scheduler_Entry::current_admitted_tuple (TAO_RT_Info_Tuple * t)
{
  current_admitted_tuple_ = t;
}

// Accessor for flag indicating whether or not node is enabled.

RtecScheduler::RT_Info_Enabled_Type_t
TAO_Reconfig_Scheduler_Entry::enabled_state () const
{
  return this->enabled_;
}


// Mutator for flag indicating whether or not node is enabled.

void
TAO_Reconfig_Scheduler_Entry::
enabled_state (RtecScheduler::RT_Info_Enabled_Type_t et)
{
  this->enabled_ = et;
}


////////////////////////////////////////////

TAO_RT_Info_Tuple_Visitor::~TAO_RT_Info_Tuple_Visitor (void)
{
}

////////////////////////////////////////////
// class TAO_Reconfig_Sched_Strategy_Base //
////////////////////////////////////////////


// Ordering function to compare the DFS finish times of
// two RT_Info_Tuples
int
TAO_Reconfig_Sched_Strategy_Base::comp_tuple_finish_times (const void *first, const void *second)
{
  // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_RT_Info_Tuple **first_tuple =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (first));

  //volatile_token is a TAO_Reconfig_Scheduler_Entry*, but we need to treat it as a void*
  void * first_entry = ACE_LONGLONG_TO_PTR (void *,
                         (*first_tuple)->volatile_token);

  TAO_RT_Info_Tuple **second_tuple =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (second));

  //volatile_token is a TAO_Reconfig_Scheduler_Entry*, but we need to treat it as a void*
  void * second_entry = ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                          (*second_tuple)->volatile_token);

  return TAO_Reconfig_Sched_Strategy_Base::comp_entry_finish_times(first_entry,second_entry);
}

// Ordering function to compare the DFS finish times of
// two task entries, so qsort orders these in topological
// order, with the higher times *first*
int
TAO_Reconfig_Sched_Strategy_Base::comp_entry_finish_times (const void *first, const void *second)
{
  const TAO_Reconfig_Scheduler_Entry *first_entry =
    * reinterpret_cast<const TAO_Reconfig_Scheduler_Entry *const *> (first);

  const TAO_Reconfig_Scheduler_Entry *second_entry =
    * reinterpret_cast<const TAO_Reconfig_Scheduler_Entry *const *> (second);

  // sort blank entries to the end
  if (! first_entry)
  {
    return (second_entry) ? 1 : 0;
  }
  else if (! second_entry)
  {
    return -1;
  }

  // sort disabled entries to the end
  if (first_entry->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return (second_entry->enabled_state () == RtecScheduler::RT_INFO_DISABLED) ? 0 : 1;
  }
  else if (second_entry->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return -1;
  }

  // Sort entries with higher forward DFS finishing times before those
  // with lower forward DFS finishing times.
  if (first_entry->fwd_finished () >
      second_entry->fwd_finished ())
  {
    return -1;
  }
  else if (first_entry->fwd_finished () <
           second_entry->fwd_finished ())
  {
    return 1;
  }

  return 0;
}

// Determines whether or not an entry is critical, based on operation characteristics.
// returns 1 if critical, 0 if not

int
TAO_Reconfig_Sched_Strategy_Base::is_critical (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Look at the underlying RT_Info's criticality field.
  return (rse.actual_rt_info ()->criticality == RtecScheduler::HIGH_CRITICALITY ||
          rse.actual_rt_info ()->criticality == RtecScheduler::VERY_HIGH_CRITICALITY)
         ? 1 : 0;
}

// Determines whether or not a tuple is critical, based on operation
// characteristics.  returns 1 if critical, 0 if not

int
TAO_Reconfig_Sched_Strategy_Base::is_critical (TAO_RT_Info_Tuple &t)
{
  // Look at the underlying RT_Info's criticality field.
  return (t.criticality == RtecScheduler::HIGH_CRITICALITY ||
          t.criticality == RtecScheduler::VERY_HIGH_CRITICALITY)
         ? 1 : 0;
}


// Compares two entries by subpriority alone.  Returns -1 if the first
// one is higher, 0 if they're the same, and 1 if the second one is
// higher.

int
TAO_Reconfig_Sched_Strategy_Base::compare_subpriority (TAO_Reconfig_Scheduler_Entry &lhs,
                                                       TAO_Reconfig_Scheduler_Entry &rhs)
{
  // First, compare importance.

  if (lhs.actual_rt_info ()->importance > rhs.actual_rt_info ()->importance)
    {
      return -1;
    }
  else if (lhs.actual_rt_info ()->importance < rhs.actual_rt_info ()->importance)
    {
      return 1;
    }

  // Same importance, so look at dfs finish time as a tiebreaker.

  else if (lhs.fwd_finished () > rhs.fwd_finished ())
    {
      return -1;
    }
  else if (lhs.fwd_finished () < rhs.fwd_finished ())
    {
      return 1;
    }

  // Same dfs finish time, so look at handle as a tiebreaker.

  else if (lhs.actual_rt_info ()->handle > rhs.actual_rt_info ()->handle)
    {
      return -1;
    }
  else if (lhs.actual_rt_info ()->handle < rhs.actual_rt_info ()->handle)
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


////////////////////////////////////////////////
// class TAO_MUF_FAIR_Reconfig_Sched_Strategy //
////////////////////////////////////////////////

// Ordering function used to qsort an array of TAO_RT_Info_Tuple
// pointers into a total <priority, subpriority> ordering.  Returns -1
// if the first one is higher, 0 if they're the same, and 1 if the
// second one is higher.

int
TAO_MUF_FAIR_Reconfig_Sched_Strategy::total_priority_comp (const void *s, const void *t)
{
  // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_Reconfig_Scheduler_Entry **first =
    reinterpret_cast<TAO_Reconfig_Scheduler_Entry **> (const_cast<void *> (s));
  TAO_Reconfig_Scheduler_Entry **second =
    reinterpret_cast<TAO_Reconfig_Scheduler_Entry **> (const_cast<void *> (t));

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  // sort disabled entries to the end
  if ((*first)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED) ? 0 : 1;
  }
  else if ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return -1;
  }

  int result =
    TAO_MUF_FAIR_Reconfig_Sched_Strategy::compare_priority (**first,
                                                            **second);

  // Check whether they were distinguished by priority.
  if (result == 0)
    {
      return TAO_Reconfig_Sched_Strategy_Base::compare_subpriority (**first,
                                                                    **second);
    }
  else
    {
      return result;
    }
}


// Ordering function used to qsort an array of RT_Info_Tuple
// pointers into a total ordering for admission control.  Returns
// -1 if the first one is higher, 0 if they're the same, and 1 if
// the second one is higher.

int
TAO_MUF_FAIR_Reconfig_Sched_Strategy::total_admission_comp (const void *s,
                                                            const void *t)
{
    // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_RT_Info_Tuple **first =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (s));

  TAO_Reconfig_Scheduler_Entry * first_entry =
      ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                           (*first)->volatile_token);

  TAO_RT_Info_Tuple **second =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (t));

  TAO_Reconfig_Scheduler_Entry * second_entry =
      ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                           (*second)->volatile_token);

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  // sort disabled tuples to the end
  if ((*first)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED) ? 0 : 1;
  }
  else if ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return -1;
  }

  // First, compare according to rate index.

  if ((*first)->rate_index < (*second)->rate_index)
    {
      return -1;
    }
  else if ((*second)->rate_index < (*first)->rate_index)
    {
      return 1;
    }

  // Then compare by priority.

  int result =
    TAO_MUF_FAIR_Reconfig_Sched_Strategy::compare_priority (**first, **second);
  if (result != 0)
    {
      return result;
    }

  // Then compare by subpriority.

  result = TAO_Reconfig_Sched_Strategy_Base::compare_subpriority (*first_entry,
                                                                  *second_entry);
  if (result != 0)
    {
      return result;
    }

  return 0;
}



// Compares two RT_Info entries by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_MUF_FAIR_Reconfig_Sched_Strategy::compare_priority (TAO_Reconfig_Scheduler_Entry &lhs,
                                                        TAO_Reconfig_Scheduler_Entry &rhs)
{
  // In MUF, priority is per criticality level: compare criticalities.
  if (lhs.actual_rt_info ()->criticality >
      rhs.actual_rt_info ()->criticality)
    {
      return -1;
    }
  else if (lhs.actual_rt_info ()->criticality <
           rhs.actual_rt_info ()->criticality)
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Compares two RT_Info tuples by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_MUF_FAIR_Reconfig_Sched_Strategy::compare_priority (TAO_RT_Info_Tuple &lhs,
                                                        TAO_RT_Info_Tuple &rhs)
{
  // In MUF, priority is per criticality level: compare criticalities.
  if (lhs.criticality > rhs.criticality)
    {
      return -1;
    }
  else if (lhs.criticality < rhs.criticality)
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Fills in a static dispatch configuration for a priority level, based
// on the operation characteristics of a representative scheduling entry.

int
TAO_MUF_FAIR_Reconfig_Sched_Strategy::assign_config (RtecScheduler::Config_Info &info,
                                                     TAO_Reconfig_Scheduler_Entry &rse)
{
    // Global and thread priority of dispatching queue are simply
    // those assigned the representative operation it will dispatch.
    info.preemption_priority = rse.actual_rt_info ()->preemption_priority;
    info.thread_priority = rse.actual_rt_info ()->priority;

    // Dispatching queues are all laxity-based in this strategy.
    info.dispatching_type = RtecScheduler::LAXITY_DISPATCHING;

  return 0;
}

///////////////////////////////////////////////////
// class TAO_RMS_FAIR_Reconfig_Sched_Strategy //
///////////////////////////////////////////////////


// Ordering function used to qsort an array of TAO_RT_Info_Tuple
// pointers into a total <priority, subpriority> ordering.  Returns -1
// if the first one is higher, 0 if they're the same, and 1 if the
// second one is higher.

int
TAO_RMS_FAIR_Reconfig_Sched_Strategy::total_priority_comp (const void *s, const void *t)
{
  // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_Reconfig_Scheduler_Entry **first =
    reinterpret_cast<TAO_Reconfig_Scheduler_Entry **> (const_cast<void *> (s));
  TAO_Reconfig_Scheduler_Entry **second =
    reinterpret_cast<TAO_Reconfig_Scheduler_Entry **> (const_cast<void *> (t));

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  // sort disabled entries to the end
  if ((*first)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED) ? 0 : 1;
  }
  else if ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return -1;
  }


  // Check whether they are distinguished by priority, and if not,
  // then by subpriority.

  int result =
    TAO_RMS_FAIR_Reconfig_Sched_Strategy::compare_priority (**first,
                                                               **second);

  if (result == 0)
    {
      return TAO_Reconfig_Sched_Strategy_Base::compare_subpriority (**first,
                                                                    **second);
    }
  else
    {
      return result;
    }
}


// Ordering function used to qsort an array of RT_Info_Tuple
// pointers into a total ordering for admission control.  Returns
// -1 if the first one is higher, 0 if they're the same, and 1 if
// the second one is higher.

int
TAO_RMS_FAIR_Reconfig_Sched_Strategy::total_admission_comp (const void *s,
                                                               const void *t)
{
    // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_RT_Info_Tuple **first =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (s));

  TAO_Reconfig_Scheduler_Entry * first_entry =
      ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                           (*first)->volatile_token);

  TAO_RT_Info_Tuple **second =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (t));

  TAO_Reconfig_Scheduler_Entry * second_entry =
      ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                           (*second)->volatile_token);

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  // sort disabled tuples to the end
  if ((*first)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED) ? 0 : 1;
  }
  else if ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
  {
    return -1;
  }

  // First, compare by rate index.

  if ((*first)->rate_index < (*second)->rate_index)
    {
      return -1;
    }
  else if ((*second)->rate_index < (*first)->rate_index)
    {
      return 1;
    }

  // Then compare by priority.

  int result =
    TAO_RMS_FAIR_Reconfig_Sched_Strategy::compare_criticality (**first,
                                                               **second);
  if (result != 0)
    {
      return result;
    }

  // Then compare by subpriority.

  result = TAO_Reconfig_Sched_Strategy_Base::compare_subpriority (*first_entry,
                                                                  *second_entry);
  if (result != 0)
    {
      return result;
    }

  return 0;
}


// Compares two RT_Info entries by criticality alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_FAIR_Reconfig_Sched_Strategy::compare_criticality(TAO_Reconfig_Scheduler_Entry &lhs,
                                                           TAO_Reconfig_Scheduler_Entry &rhs)
{
  ACE_UNUSED_ARG (lhs);
  ACE_UNUSED_ARG (rhs);
  // In RMS_FAIR, no consideration of criticalities
  return 0;
}

// Compares two RT_Info entries by criticality alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.
int
TAO_RMS_FAIR_Reconfig_Sched_Strategy::compare_criticality(TAO_RT_Info_Tuple &,
                                                     TAO_RT_Info_Tuple &)
{
  // In plain RMS, no consideration of criticalities
  return 0;
}

// Compares two RT_Info entries by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_FAIR_Reconfig_Sched_Strategy::compare_priority (TAO_Reconfig_Scheduler_Entry &lhs,
                                                           TAO_Reconfig_Scheduler_Entry &rhs)
{
  //differentiate by rate.
  if (lhs.actual_rt_info ()->period < rhs.actual_rt_info ()->period)
    {
      return -1;
    }
  else if (lhs.actual_rt_info ()->period > rhs.actual_rt_info ()->period)
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Compares two RT_Info tuples by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_FAIR_Reconfig_Sched_Strategy::compare_priority (TAO_RT_Info_Tuple &lhs,
                                                           TAO_RT_Info_Tuple &rhs)
{
  // In RMS_FAIR, priority is partitioned based on rate:
  if (lhs.period < rhs.period)
    {
      return -1;
    }
  else if (lhs.period > rhs.period)
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Fills in a static dispatch configuration for a priority level, based
// on the operation characteristics of a representative scheduling entry.

int
TAO_RMS_FAIR_Reconfig_Sched_Strategy::assign_config (RtecScheduler::Config_Info &info,
                                                        TAO_Reconfig_Scheduler_Entry &rse)
{
    // Global and thread priority of dispatching queue are simply
    // those assigned the representative operation it will dispatch.
    info.preemption_priority = rse.actual_rt_info ()->preemption_priority;
    info.thread_priority = rse.actual_rt_info ()->priority;

    // In RMS_FAIR, all queues are static
    info.dispatching_type = RtecScheduler::STATIC_DISPATCHING;

  return 0;
}

///////////////////////////////////////////////////
// class TAO_RMS_MLF_Reconfig_Sched_Strategy //
///////////////////////////////////////////////////


// Ordering function used to qsort an array of TAO_RT_Info_Tuple
// pointers into a total <priority, subpriority> ordering.  Returns -1
// if the first one is higher, 0 if they're the same, and 1 if the
// second one is higher.

int
TAO_RMS_MLF_Reconfig_Sched_Strategy::total_priority_comp (const void *s, const void *t)
{
  // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_Reconfig_Scheduler_Entry **first =
    reinterpret_cast<TAO_Reconfig_Scheduler_Entry **> (const_cast<void *> (s));
  TAO_Reconfig_Scheduler_Entry **second =
    reinterpret_cast<TAO_Reconfig_Scheduler_Entry **> (const_cast<void *> (t));

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  // sort disabled entries to the end
  if ((*first)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    {
      return ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED) ? 0 : 1;
    }
  else if ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    {
      return -1;
    }


  // Check whether they are distinguished by priority, and if not,
  // then by subpriority.

  int result =
    TAO_RMS_MLF_Reconfig_Sched_Strategy::compare_priority (**first,
                                                               **second);

  if (result == 0)
    {
      return TAO_Reconfig_Sched_Strategy_Base::compare_subpriority (**first,
                                                                    **second);
    }
  else
    {
      return result;
    }
}


// Ordering function used to qsort an array of RT_Info_Tuple
// pointers into a total ordering for admission control.  Returns
// -1 if the first one is higher, 0 if they're the same, and 1 if
// the second one is higher.

int
TAO_RMS_MLF_Reconfig_Sched_Strategy::total_admission_comp (const void *s,
                                                               const void *t)
{
  // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_RT_Info_Tuple **first =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (s));

  TAO_Reconfig_Scheduler_Entry * first_entry =
    ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                         (*first)->volatile_token);

  TAO_RT_Info_Tuple **second =
    reinterpret_cast<TAO_RT_Info_Tuple **> (const_cast<void *> (t));

  TAO_Reconfig_Scheduler_Entry * second_entry =
    ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                         (*second)->volatile_token);

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  // sort disabled tuples to the end
  if ((*first)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    {
      return ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED) ? 0 : 1;
    }
  else if ((*second)->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    {
      return -1;
    }

  // First, compare by rate index.

  if ((*first)->rate_index < (*second)->rate_index)
    {
      return -1;
    }
  else if ((*second)->rate_index < (*first)->rate_index)
    {
      return 1;
    }

  // Then compare by priority.

  int result =
    TAO_RMS_MLF_Reconfig_Sched_Strategy::compare_criticality (**first,
                                                                  **second);
  if (result != 0)
    {
      return result;
    }

  // Then compare by subpriority.

  result = TAO_Reconfig_Sched_Strategy_Base::compare_subpriority (*first_entry,
                                                                  *second_entry);
  if (result != 0)
    {
      return result;
    }

  return 0;
}


// Compares two RT_Info entries by criticality alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_MLF_Reconfig_Sched_Strategy::compare_criticality(TAO_Reconfig_Scheduler_Entry &lhs,
                                                             TAO_Reconfig_Scheduler_Entry &rhs)
{
  // In RMS+MLF, priority is per criticality level: compare criticalities.

  if (lhs.actual_rt_info ()->criticality > rhs.actual_rt_info ()->criticality)
    {
      return -1;
    }
  else if (lhs.actual_rt_info ()->criticality < rhs.actual_rt_info ()->criticality)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

// Compares two RT_Info entries by criticality alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.
int
TAO_RMS_MLF_Reconfig_Sched_Strategy::compare_criticality(TAO_RT_Info_Tuple &lhs,
                                                             TAO_RT_Info_Tuple &rhs)
{
  if (lhs.criticality > rhs.criticality)
    {
      return -1;
    }
  else if (lhs.criticality < rhs.criticality)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

// Compares two RT_Info entries by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_MLF_Reconfig_Sched_Strategy::compare_priority (TAO_Reconfig_Scheduler_Entry &lhs,
                                                           TAO_Reconfig_Scheduler_Entry &rhs)
{
  // In RMS+MLF, priority is per criticality level: compare criticalities.
  int result = TAO_RMS_MLF_Reconfig_Sched_Strategy::compare_criticality(lhs, rhs);

  if (result != 0)
    {
      return result;
    }

  // Same criticality: if high criticality, differentiate by rate.
  if (TAO_Reconfig_Sched_Strategy_Base::is_critical (rhs))
    {
      if (lhs.actual_rt_info ()->period < rhs.actual_rt_info ()->period)
        {
          return -1;
        }
      else if (lhs.actual_rt_info ()->period > rhs.actual_rt_info ()->period)
        {
          return 1;
        }
    }

  // They're the same if we got here.
  return 0;
}


// Compares two RT_Info tuples by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_MLF_Reconfig_Sched_Strategy::compare_priority (TAO_RT_Info_Tuple &lhs,
                                                           TAO_RT_Info_Tuple &rhs)
{
  // In RMS_Dyn, priority is first partitioned per criticality level:
  // compare criticalities.

  if (lhs.criticality > rhs.criticality)
    {
      return -1;
    }
  else if (lhs.criticality < rhs.criticality)
    {
      return 1;
    }

  // Same criticality: if high criticality, differentiate by rate.
  else if (TAO_Reconfig_Sched_Strategy_Base::is_critical (rhs))
    {
      if (lhs.period < rhs.period)
        {
          return -1;
        }
      else if (lhs.period > rhs.period)
        {
          return 1;
        }
    }

  // They're the same if we got here.
  return 0;
}

// Fills in a static dispatch configuration for a priority level, based
// on the operation characteristics of a representative scheduling entry.

int
TAO_RMS_MLF_Reconfig_Sched_Strategy::assign_config (RtecScheduler::Config_Info &info,
                                                        TAO_Reconfig_Scheduler_Entry &rse)
{
  // Global and thread priority of dispatching queue are simply
  // those assigned the representative operation it will dispatch.
  info.preemption_priority = rse.actual_rt_info ()->preemption_priority;
  info.thread_priority = rse.actual_rt_info ()->priority;

  // Critical queues are static, and non-critical ones are
  // laxity-based in this strategy.
  if (TAO_Reconfig_Sched_Strategy_Base::is_critical (rse))
    {
      info.dispatching_type = RtecScheduler::STATIC_DISPATCHING;
    }
  else
    {
      info.dispatching_type = RtecScheduler::LAXITY_DISPATCHING;
    }

  return 0;
}


#endif /* TAO_RECONFIG_SCHED_UTILS_C */

TAO_END_VERSIONED_NAMESPACE_DECL
