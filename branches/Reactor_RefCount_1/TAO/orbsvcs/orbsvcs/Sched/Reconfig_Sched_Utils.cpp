// ============================================================================
//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Sched_Utils.cpp
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHED_UTILS_C
#define TAO_RECONFIG_SCHED_UTILS_C

#include "Reconfig_Sched_Utils_T.h"
#include "Reconfig_Sched_Utils.h"


#include "orbsvcs/Time_Utilities.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (__ACE_INLINE__)
#include "Reconfig_Sched_Utils.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Sched, Reconfig_Sched_Utils, "$Id$")


////////////////////////////////////////
// class TAO_Reconfig_Scheduler_Entry //
////////////////////////////////////////

// Constructor.

TAO_Reconfig_Scheduler_Entry::
TAO_Reconfig_Scheduler_Entry (RtecScheduler::RT_Info &rt_info)
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
    effective_exec_multiplier_ (0),
    effective_period_ (0)
{
  // Store the RT_Info fields.
  this->orig_rt_info_data (*actual_rt_info_);
}


// Accessor for original RT_Info data.

RtecScheduler::RT_Info &
TAO_Reconfig_Scheduler_Entry::orig_rt_info_data ()
{
  return orig_rt_info_data_;
}


// Mutator for stored original RT_Info data.

void
TAO_Reconfig_Scheduler_Entry::orig_rt_info_data (RtecScheduler::RT_Info &data)
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
}

// Accessor for actual RT_Info pointer.

RtecScheduler::RT_Info *
TAO_Reconfig_Scheduler_Entry::
actual_rt_info ()
{
  return this->actual_rt_info_;
}


// Mutator for actual RT_Info pointer.

void
TAO_Reconfig_Scheduler_Entry::
actual_rt_info (RtecScheduler::RT_Info *rt_info)
{
  this->actual_rt_info_ = rt_info;
}


// Accessor for when the node was discovered in forward DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::
fwd_discovered () const
{
  return this->fwd_discovered_;
}


// Mutator for when the node was discovered in forward DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::
fwd_discovered (long l)
{
  this->fwd_discovered_ = l;
}


// Accessor for when the node was discovered in reverse DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::
rev_discovered () const
{
  return this->rev_discovered_;
}


// Mutator for when the node was discovered in reverse DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::
rev_discovered (long l)
{
  this->rev_discovered_ = l;
}


// Accessor for when the node was finished in forward DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::
fwd_finished () const
{
  return this->fwd_finished_;
}


// Mutator for when the node was finished in forward DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::
fwd_finished (long l)
{
  this->fwd_finished_ = l;
}


// Accessor for when the node was finished in reverse DFS traversal.

long
TAO_Reconfig_Scheduler_Entry::
rev_finished () const
{
  return this->rev_finished_;
}


// Mutator for when the node was finished in reverse DFS traversal.

void
TAO_Reconfig_Scheduler_Entry::
rev_finished (long l)
{
  this->rev_finished_ = l;
}


// Accessor for forward DFS traversal status of the node.

TAO_Reconfig_Scheduler_Entry::DFS_Status
TAO_Reconfig_Scheduler_Entry::
fwd_dfs_status () const
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
TAO_Reconfig_Scheduler_Entry::
rev_dfs_status () const
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
TAO_Reconfig_Scheduler_Entry::
is_thread_delineator () const
{
  return this->is_thread_delineator_;
}


// Mutator for flag indicating whether node is a thread
// delineator.

void
TAO_Reconfig_Scheduler_Entry::
is_thread_delineator (int i)
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
TAO_Reconfig_Scheduler_Entry::
has_unresolved_remote_dependencies (int i)
{
  this->has_unresolved_remote_dependencies_ = i;
}


// Accessor for flag indicating whether node has unresolved local
// dependencies.

int
TAO_Reconfig_Scheduler_Entry::
has_unresolved_local_dependencies () const
{
  return this->has_unresolved_local_dependencies_;
}


// Mutator for flag indicating whether node has unresolved local
// dependencies.

void
TAO_Reconfig_Scheduler_Entry::
has_unresolved_local_dependencies (int i)
{
  this->has_unresolved_local_dependencies_ = i;
}


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
  rse.is_thread_delineator (1);

  rse.fwd_dfs_status (TAO_Reconfig_Scheduler_Entry::NOT_VISITED);
  rse.rev_dfs_status (TAO_Reconfig_Scheduler_Entry::NOT_VISITED);
  rse.fwd_discovered (-1);
  rse.rev_discovered (-1);
  rse.fwd_finished (-1);
  rse.rev_finished (-1);
  rse.has_unresolved_remote_dependencies (0);
  rse.has_unresolved_local_dependencies (0);

  // These settings are used for a conservative but
  // efficient approach to estimating utilization:
  // for an exact algorithm using frame merging,
  // other initial settings might be needed.
  rse.effective_exec_multiplier (0);
  rse.effective_period (0);

  return 0;
}





///////////////////////////////////////////
// class TAO_MUF_Reconfig_Sched_Strategy //
///////////////////////////////////////////

// Ordering function to compare the DFS finish times of
// two task entries, so qsort orders these in topological
// order, with the higher times *first*
int
TAO_MUF_Reconfig_Sched_Strategy::comp_entry_finish_times (const void *first, const void *second)
{
  const TAO_Reconfig_Scheduler_Entry *first_entry =
    * ACE_reinterpret_cast (const TAO_Reconfig_Scheduler_Entry *const *,
                            first);

  const TAO_Reconfig_Scheduler_Entry *second_entry =
    * ACE_reinterpret_cast (const TAO_Reconfig_Scheduler_Entry *const *,
                            second);

  // sort blank entries to the end
  if (! first_entry)
  {
    return (second_entry) ? 1 : 0;
  }
  else if (! second_entry)
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

// Ordering function used to qsort an array of TAO_Reconfig_Scheduler_Entry
// pointers into a total <priority, subpriority> ordering.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_MUF_Reconfig_Sched_Strategy::total_priority_comp (const void *s, const void *t)
{
  // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_Reconfig_Scheduler_Entry **first =
    ACE_reinterpret_cast (TAO_Reconfig_Scheduler_Entry **,
                          ACE_const_cast (void *, s));
  TAO_Reconfig_Scheduler_Entry **second =
    ACE_reinterpret_cast (TAO_Reconfig_Scheduler_Entry **,
                          ACE_const_cast (void *, t));

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  int result =
    TAO_MUF_Reconfig_Sched_Strategy::priority_diff (*((*first)->actual_rt_info ()),
                                                    *((*second)->actual_rt_info ()));

  // Check whether they were distinguished by priority.
  if (result == 0)
    {
      return TAO_MUF_Reconfig_Sched_Strategy::compare_subpriority (**first,
                                                                   **second);
    }
  else
    {
      return result;
    }
}


// Compares two entries by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_MUF_Reconfig_Sched_Strategy::compare_priority (TAO_Reconfig_Scheduler_Entry &s,
                                                   TAO_Reconfig_Scheduler_Entry &t)
{
  // Simply call the corresponding comparison based on the underlying rt_infos.
  return TAO_MUF_Reconfig_Sched_Strategy::priority_diff (*s.actual_rt_info (),
                                                         *t.actual_rt_info ());
}


// Compares two entries by subpriority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_MUF_Reconfig_Sched_Strategy::compare_subpriority (TAO_Reconfig_Scheduler_Entry &s,
                                                      TAO_Reconfig_Scheduler_Entry &t)
{
  // @@ TO DO: add dependency hash tables to strategy, use them to look for
  //           *direct* dependencies between two nodes.

  // Compare importance.
  if (s.actual_rt_info ()->importance > t.actual_rt_info ()->importance)
    {
      return -1;
    }
  else if (s.actual_rt_info ()->importance < t.actual_rt_info ()->importance)
    {
      return 1;
    }
  // Same importance, so look at dfs finish time as a tiebreaker.
  else if (s.fwd_finished () > t.fwd_finished ())
    {
      return -1;
    }
  else if (s.fwd_finished () < t.fwd_finished ())
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Compares two RT_Infos by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_MUF_Reconfig_Sched_Strategy::priority_diff (RtecScheduler::RT_Info &s,
                                                RtecScheduler::RT_Info &t)
{
  // In MUF, priority is per criticality level: compare criticalities.
  if (s.criticality > t.criticality)
    {
      return -1;
    }
  else if (s.criticality < t.criticality)
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Determines whether or not an entry is critical, based on operation characteristics.
// returns 1 if critical, 0 if not

int
TAO_MUF_Reconfig_Sched_Strategy::is_critical (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Look at the underlying RT_Info's criticality field.
  return (rse.actual_rt_info ()->criticality == RtecScheduler::HIGH_CRITICALITY ||
          rse.actual_rt_info ()->criticality == RtecScheduler::VERY_HIGH_CRITICALITY)
         ? 1 : 0;
}

// Fills in a static dispatch configuration for a priority level, based
// on the operation characteristics of a representative scheduling entry.

int
TAO_MUF_Reconfig_Sched_Strategy::assign_config (RtecScheduler::Config_Info &info,
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

///////////////////////////////////////////
// class TAO_RMS_Reconfig_Sched_Strategy //
///////////////////////////////////////////

// Ordering function to compare the DFS finish times of
// two task entries, so qsort orders these in topological
// order, with the higher times *first*
int
TAO_RMS_Reconfig_Sched_Strategy::comp_entry_finish_times (const void *first, const void *second)
{
  const TAO_Reconfig_Scheduler_Entry *first_entry =
    * ACE_reinterpret_cast (const TAO_Reconfig_Scheduler_Entry *const *,
                            first);

  const TAO_Reconfig_Scheduler_Entry *second_entry =
    * ACE_reinterpret_cast (const TAO_Reconfig_Scheduler_Entry *const *,
                            second);

  // sort blank entries to the end
  if (! first_entry)
  {
    return (second_entry) ? 1 : 0;
  }
  else if (! second_entry)
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

// Ordering function used to qsort an array of TAO_Reconfig_Scheduler_Entry
// pointers into a total <priority, subpriority> ordering.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_Reconfig_Sched_Strategy::total_priority_comp (const void *s, const void *t)
{
  // Convert the passed pointers: the double cast is needed to
  // make Sun C++ 4.2 happy.
  TAO_Reconfig_Scheduler_Entry **first =
    ACE_reinterpret_cast (TAO_Reconfig_Scheduler_Entry **,
                          ACE_const_cast (void *, s));
  TAO_Reconfig_Scheduler_Entry **second =
    ACE_reinterpret_cast (TAO_Reconfig_Scheduler_Entry **,
                          ACE_const_cast (void *, t));

  // Check the converted pointers.
  if (first == 0 || *first == 0)
    {
      return (second == 0 || *second == 0) ? 0 : 1;
    }
  else if (second == 0 || *second == 0)
    {
      return -1;
    }

  int result =
    TAO_RMS_Reconfig_Sched_Strategy::priority_diff (*((*first)->actual_rt_info ()),
                                                    *((*second)->actual_rt_info ()));

  // Check whether they were distinguished by priority.
  if (result == 0)
    {
      return TAO_RMS_Reconfig_Sched_Strategy::compare_subpriority (**first,
                                                                   **second);
    }
  else
    {
      return result;
    }
}


// Compares two entries by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_Reconfig_Sched_Strategy::compare_priority (TAO_Reconfig_Scheduler_Entry &s,
                                                   TAO_Reconfig_Scheduler_Entry &t)
{
  // Simply call the corresponding comparison based on the underlying rt_infos.
  return TAO_RMS_Reconfig_Sched_Strategy::priority_diff (*s.actual_rt_info (),
                                                         *t.actual_rt_info ());
}


// Compares two entries by subpriority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_Reconfig_Sched_Strategy::compare_subpriority (TAO_Reconfig_Scheduler_Entry &s,
                                                      TAO_Reconfig_Scheduler_Entry &t)
{
  // @@ TO DO: add dependency hash tables to strategy, use them to look for
  //           *direct* dependencies between two nodes.

  // Compare importance.
  if (s.actual_rt_info ()->importance > t.actual_rt_info ()->importance)
    {
      return -1;
    }
  else if (s.actual_rt_info ()->importance < t.actual_rt_info ()->importance)
    {
      return 1;
    }
  // Same importance, so look at dfs finish time as a tiebreaker.
  else if (s.fwd_finished () > t.fwd_finished ())
    {
      return -1;
    }
  else if (s.fwd_finished () < t.fwd_finished ())
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Compares two RT_Infos by priority alone.  Returns -1 if the
// first one is higher, 0 if they're the same, and 1 if the second one is higher.

int
TAO_RMS_Reconfig_Sched_Strategy::priority_diff (RtecScheduler::RT_Info &s,
                                                RtecScheduler::RT_Info &t)
{
  // In RMS, priority is per criticality level: compare criticalities.
  if (s.period > t.period)
    {
      return -1;
    }
  else if (s.period < t.period)
    {
      return 1;
    }

  // They're the same if we got here.
  return 0;
}


// Determines whether or not an entry is critical, based on operation characteristics.
// returns 1 if critical, 0 if not

int
TAO_RMS_Reconfig_Sched_Strategy::is_critical (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Look at the underlying RT_Info's criticality field.
  return (rse.actual_rt_info ()->criticality == RtecScheduler::HIGH_CRITICALITY ||
          rse.actual_rt_info ()->criticality == RtecScheduler::VERY_HIGH_CRITICALITY)
         ? 1 : 0;
}

// Fills in a static dispatch configuration for a priority level, based
// on the operation characteristics of a representative scheduling entry.

int
TAO_RMS_Reconfig_Sched_Strategy::assign_config (RtecScheduler::Config_Info &info,
                                                TAO_Reconfig_Scheduler_Entry &rse)
{
    // Global and thread priority of dispatching queue are simply
    // those assigned the representative operation it will dispatch.
    info.preemption_priority = rse.actual_rt_info ()->preemption_priority;
    info.thread_priority = rse.actual_rt_info ()->priority;

    // Dispatching queues are all laxity-based in this strategy.
    info.dispatching_type = RtecScheduler::STATIC_DISPATCHING;

  return 0;
}


#endif /* TAO_RECONFIG_SCHED_UTILS_C */
