// $Id$

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Reconfig_Scheduler.h"

#if defined (__ACE_INLINE__)
#include "orbsvcs/Reconfig_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Reconfig_Scheduler, "$Id$")


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
    effective_period_ (0);
{
  // Store the RT_Info fields
  this->orig_rt_info_data_.entry_point =
    this->actual_rt_info_->entry_point;
  this->orig_rt_info_data_.handle =
    this->actual_rt_info_->handle;
  this->orig_rt_info_data_.worst_case_execution_time =
    this->actual_rt_info_->worst_case_execution_time;
  this->orig_rt_info_data_.typical_execution_time =
    this->actual_rt_info_->typical_execution_time;
  this->orig_rt_info_data_.cached_execution_time =
    this->actual_rt_info_->cached_execution_time;
  this->orig_rt_info_data_.period =
    this->actual_rt_info_->period;
  this->orig_rt_info_data_.criticality =
    this->actual_rt_info_->criticality;
  this->orig_rt_info_data_.importance =
    this->actual_rt_info_->importance;
  this->orig_rt_info_data_.quantum =
    this->actual_rt_info_->quantum;
  this->orig_rt_info_data_.threads =
    this->actual_rt_info_->threads;
  this->orig_rt_info_data_.priority =
    this->actual_rt_info_->priority;
  this->orig_rt_info_data_.static_subpriority =
    this->actual_rt_info_->preemption_subpriority;
  this->orig_rt_info_data_.preemption_priority =
    this->actual_rt_info_->preemption_priority;
  this->orig_rt_info_data_.info_type =
    this->actual_rt_info_->info_type;
}


// Static helper method to give an RT_Info some reasonable default values.

void
TAO_Reconfig_Scheduler_Entry::init_rt_info (RtecScheduler::RT_Info &rt_info)
{
  // Set some reasonable default values.
  rt_info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  rt_info.worst_case_execution_time = 0;
  rt_info.typical_execution_time = 0;
  rt_info.cached_execution_time = 0;
  rt_info.period = 0;
  rt_info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  rt_info.quantum = 0;
  rt_info.threads = 0;
  rt_info.info_type = RtecScheduler::OPERATION;
  rt_info.priority = 0;
  rt_info.preemption_subpriority = 0;
  rt_info.preemption_priority = 0;
  rt_info.volatile_token = 0;
}


// Accessor for original RT_Info data.

ACE_Scheduler_Factory::POD_RT_Info &
TAO_Reconfig_Scheduler_Entry::orig_rt_info_data ()
{
  return orig_rt_info_data_;
}


// Mutator for original RT_Info data.

void 
TAO_Reconfig_Scheduler_Entry::
orig_rt_info_data (const RtecScheduler::RT_Info &rt_info)
{
  this->orig_rt_info_data_ = rt_info;
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

RtecScheduler::Period
TAO_Reconfig_Scheduler_Entry::
effective_period ()
{
  return this->effective_period_;
}


// Mutator for effective period of corresponding RT_Info.

void
TAO_Reconfig_Scheduler_Entry::
effective_period (RtecScheduler::Period p)
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
effective_exec_multiplier_ (CORBA::Long l)
{
  this->effective_exec_multiplier_ = l;
}



//////////////////////////////////////
// TAO_Reconfig_Sched_Entry_Visitor //
//////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_Reconfig_Sched_Entry_Visitor
    (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       DEPENDENCY_SET_MAP & dependency_map,
     TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       RT_INFO_MAP & rt_info_map)
  : dependency_map_ (dependency_map),
    rt_info_map_ (rt_info_map)
{
}


// Visit a Reconfig Scheduler Entry.  This method calls protected hook
// methods that can be overridden by derived classes, according to the
// Template Method design pattern.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
visit (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Call unconditional action method, which performs any necessary
  // modifications that are applied to each node unconditionally.
  if (this->unconditional_action (rse) < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_Reconfig_Sched_Entry_Visitor::"
                         "visit: error from unconditional action.\n"), -1)
    }

  // Call precondition hook method, and only proceed if the
  // precondition returns 0 for success.
  if (this->precondition (rse) == 0)
    {
      // Call prefix action method, which performs any necessary
      // modifications on the node prior to visiting its successors.
      if (this->prefix_action (rse) < 0)
	{
          ACE_ERROR_RETURN ((LM_ERROR,
                             "TAO_Reconfig_Sched_Entry_Visitor::"
                             "visit: error from prefix action.\n"), -1)
	}

      // Get the dependency set for the current entry.
      RtecScheduler::Dependency_Set *dependency_set = 0;
      if (dependency_map_.find (rse.actual_rt_info ()->handle,
                                dependency_set) != 0)
	{
          ACE_ERROR_RETURN ((LM_ERROR, "Dependency set not found"), -1);
	}

      // Iterate over the set of dependencies for the current entry.
      TAO_Reconfig_Scheduler_Entry * next_rse = 0;
      RtecScheduler::RT_Info *next_rt_info;
      for (int i = 0; i < dependency_set->length (); ++i)
        {
          // Take the handle from the dependency and use it
          // to obtain an RT_Info pointer from the map.
          if (rt_info_map_.find ((*dependency_set) [i].rt_info,
                                 next_rt_info) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR, "RT_Info not found"), -1);
            }

          // Extract a pointer to the scheduling entry from the RT_Info.

          if (next_rt_info == 0)
	    {
              ACE_ERROR_RETURN ((LM_ERROR, "RT_Info in map was null"), -1);
            }

          next_rse = ACE_static_cast (TAO_Reconfig_Scheduler_Entry *,
                                      next_rt_info->volatile_token);
          if (next_rse == 0)
	    {
              ACE_ERROR_RETURN ((LM_ERROR, "entry pointer in RT_Info was null"), -1);
            }

          // Call pre-recursion action method, which performs any necessary
          // modifications to a successor (or the entry) prior to recursing
          // on the successor.
          if (this->pre_recurse_action (rse, *next_rse,
                                        (*dependency_set) [i]) < 0)
            {
              ACE_ERROR_RETURN (("TAO_Reconfig_Sched_Entry_Visitor::"
                                 "visit: error from pre-recursion action.\n"), -1)
            }

           // Visit the successor.
          this->visit (*next_rse);
        }

      // Call postfix action method, which performs any necessary
      // modifications on the node after visiting all its successors.
      if (this->postfix_action (rse) < 0)
	{
          ACE_ERROR_RETURN (("TAO_Reconfig_Sched_Entry_Visitor::"
                             "visit: error from postfix action.\n"), -1)
	}
    }

  return 0;
}


// Performs an unconditional action when the entry is first reached.
// Returns 0 for success, and -1 if an error occurred.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unconditional_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (rse);
  return 0;
}


// Tests whether or not any conditional actions should be taken for
// the entry.  Returns 0 if the actions should be applied, 1 if the
// entry should be left alone, and -1 if an error occurred.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
precondition (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (rse);
  return 0;
}


// Performs an action on the entry prior to visiting any of
// its successors.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
prefix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (rse);
  return 0;
}


// Performs an action on a successor entry prior to visiting
// it.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                    TAO_Reconfig_Scheduler_Entry &successor,
                    const RtecScheduler::Dependency_Info &di)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (entry);
  ACE_UNUSED_ARG (successor);
  ACE_UNUSED_ARG (di);
  return 0;
}


// Performs an action on the entry after visiting all of
// its successors.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
postfix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (rse);
  return 0;
}


///////////////////////////
// TAO_RSE_Reset_Visitor //
///////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
TAO_RSE_Reset_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_Reset_Visitor
  (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     DEPENDENCY_SET_MAP & dependency_map,
   TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     RT_INFO_MAP & rt_info_map)
  : TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
      (dependency_map, rt_info_map)
{
}


// Resets the fields in the entry to pre-DFS traversal states.
// Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Reset_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unconditional_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Note that this value differs from what is set in the
  // constructor for the entry.  This is because the reset
  // visitor is applied prior to a DFS traversal, in which callers
  // *unset* the thread delineator status of any of their called
  // operations that do not specify a period or threads.
  rse.is_thread_delineator (1);

  rse.fwd_dfs_status (NOT_VISITED);
  rse.rev_dfs_status (NOT_VISITED);
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


// Tests whether or not any conditional actions should be taken for
// the entry.  Always returns 1 so that after the unconditional action
// is applied, nothing further is done with the entry.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Reset_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
precondition (TAO_Reconfig_Scheduler_Entry &rse)
{
  ACE_UNUSED_ARG (rse);
  return 1;
}


/////////////////////////
// TAO_RSE_DFS_Visitor //
/////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_DFS_Visitor
  (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     DEPENDENCY_SET_MAP & dependency_map,
   TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     RT_INFO_MAP & rt_info_map)
  : TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
      (dependency_map, rt_info_map),
    DFS_time_ (0)
{
}


// Makes sure the entry has not previously been visited in forward DFS.
// Returns 0 if the actions should be applied, 1 if the entry
// should be left alone, and -1 if an error occurred.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
precondition (TAO_Reconfig_Scheduler_Entry &rse)
{
  return (rse.fwd_dfs_status () ==
          TAO_Reconfig_Scheduler_Entry::NOT_VISITED)
          ? 0 : 1;
}


// Marks entry as forward visited and sets its forward DFS start
// time, prior to visiting any of its successors.  Returns 0 on
// success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
prefix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  rse.fwd_dfs_status (TAO_Reconfig_Scheduler_Entry::VISITED);
  rse.fwd_discovered (this->DFS_time_++);
  return 0;
}


// Marks whether or not successor is a thread delineator prior to
// visiting it.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                    TAO_Reconfig_Scheduler_Entry &successor,
                    const RtecScheduler::Dependency_Info &di);
{
  ACE_UNUSED_ARG (rse);
  ACE_UNUSED_ARG (di);

  // Operations we reached via a dependency and that do not
  // specify a period are not thread delineators.
  if (successor.actual_rt_info ()->period == 0 &&
      successor.actual_rt_info ()->threads == 0)
    {
      successor.is_thread_delineator (0);
    }

  return 0;
}


// Marks entry as forward finished and sets its forward DFS finish
// time, after all of its successors have been visited.  Returns 0
// on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
postfix_action (TAO_Reconfig_Scheduler_Entry &rse);
{
  rse.fwd_dfs_status (TAO_Reconfig_Scheduler_Entry::FINISHED);
  rse.fwd_finished (this->DFS_time_++);
  return 0;
}


/////////////////////////
// TAO_RSE_SCC_Visitor //
/////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_SCC_Visitor
  (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     DEPENDENCY_SET_MAP & dependency_map,
   TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     RT_INFO_MAP & rt_info_map)
  : TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
      (dependency_map, rt_info_map),
    DFS_time_ (0),
    number_of_cycles_ (0),
    in_a_cycle_ (0)
{
}


// Accessor for number of cycles detected in traversal.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
number_of_cycles (void)
{
  return this->number_of_cycles_;
}


// Accessor for whether or not the recursion is within a previously
// detected cycle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
in_a_cycle (void)
{
  return this->in_a_cycle_;
}


// Mutator for whether or not the recursion is within a previously
// detected cycle.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
in_a_cycle (int i)
{
  this->in_a_cycle_ = i;
}


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unconditional_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  if (rse.is_thread_delineator () &&
      rse.effective_period () == 0)
    {
      rse.effective_period (rse.actual_rt_info ()->period);
      long threads = rse.actual_rt_info ()->threads;
      rse.effective_exec_multiplier (threads > 0 ? threads : 1);
    }

  return 0;
}

// Makes sure the entry has not previously been visited in the
// reverse DFS (call graph transpose) direction.  Returns 0 if
// the actions should be applied, 1 if the entry should be left
// alone, and -1 if an error occurred.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
precondition (TAO_Reconfig_Scheduler_Entry &rse)
{
  return (rse.rev_dfs_status () ==
          TAO_Reconfig_Scheduler_Entry::NOT_VISITED)
         ? 0 : 1;
}


// Marks reverse status as visited and sets reverse start time for
// entry, prior to visiting any of its successors.  Returns 0 on
// success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
prefix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  rse.rev_dfs_status (TAO_Reconfig_Scheduler_Entry::VISITED);
  rse.rev_discovered (this->DFS_time_++);
  return 0;
}


// Checks reverse status of each successor.  For any that have not
// been previously visited, it complains about the entry and
// successor being part of a cycle, stores the fact that a cycle was
// detected, and maintains a count of the total number of cycles
// (strongly connected components).  Returns 0 on success and -1 on
// an error (finding a cycle is not considered an error, at least as
// far as this method is concerned).

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                    TAO_Reconfig_Scheduler_Entry &successor,
                    const RtecScheduler::Dependency_Info &di)
{
  ACE_UNUSED_ARG (di);

  if  (successor.rev_dfs_status () ==
       TAO_Reconfig_Scheduler_Entry::NOT_VISITED)
    {
      if (this->in_a_cycle () == 0)
	{
          this->in_a_cycle (1);
          ++this->number_of_cycles_;
	}

      ACE_DEBUG ((LM_ERROR,
                  "RT_Infos \"%s\" and \"%s\" are part of dependency cycle %d.\n",
                  entry.actual_rt_info ()->entry_point,
                  successor.actual_rt_info ()->entry_point,
                  this->number_of_cycles_));
    }

  return 0;
}


// Sets the entry's reverse finish time and marks it as finished in
// the reverse DFS traversal, after visiting all of its successors.
// Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
postfix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  rse.rev_dfs_status (TAO_Reconfig_Scheduler_Entry::FINISHED);
  rse.rev_finished (this->DFS_time_++);
  return 0;
}


/////////////////////////////////
// TAO_RSE_Propagation_Visitor //
/////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_Propagation_Visitor
  (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     DEPENDENCY_SET_MAP dependency_map,
   TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
     RT_INFO_MAP rt_info_map)
  : TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
      (dependency_map, rt_info_map),
    unresolved_locals_ (0),
    unresolved_remotes_ (0),
    thread_specification_errors_ (0)
{
}


// Accessor for number of nodes with unresolved local dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_locals (void)
{
  return this->unresolved_locals_;
}


// Mutator for number of nodes with unresolved local dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_locals (int i)
{
  this->unresolved_locals_ = i;
}


// Accessor for number of nodes with unresolved remote dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_remotes (void)
{
  return this->unresolved_remotes_;
}


// Mutator for number of nodes with unresolved remote dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_remotes (int i)
{
  this->unresolved_remotes_ = i;
}

// Accessor for number of nodes with thread specification errors.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
thread_specification_errors (void)
{
  return this->thread_specification_errors_;
}


// Mutator for number of nodes with thread specification errors.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
thread_specification_errors (int i)
{
  this->thread_specification_errors_ = i;
}


// Tests the entry for possibly having unresolved remote or local
// dependencies prior to visiting any of its successors, and also
// checks for thread specification errors.  Returns 0 on success and
// -1 on error (having unresolved dependencies or thread specification
// problems is not considered an error, at least for this method).

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
prefix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Complain about anything that is still marked as a thread
  // delineator but does not have a period: if it has threads, it is a
  // specification error.  Otherwise, if it's not a remote dependant
  // (per RT_Info's info_type field) it has unresolved *local*
  // dependencies.

  if (rse.is_thread_delineator ())
    {
      if (rse.actual_rt_info ()->period == 0)
        {
          if (rse.actual_rt_info ()->threads == 0)
          {
            if (rse.actual_rt_info ()->info_type == 
                RtecScheduler::REMOTE_DEPENDANT)
              {
                ++this->unresolved_remotes_;

                ACE_DEBUG ((LM_ERROR, 
                            "RT_Info \"%s\" has unresolved "
                            "remote dependencies.\n"),
                            rse.actual_rt_info ()->entry_point);
              }
            else
              {
                ++this->unresolved_locals_;

                ACE_DEBUG ((LM_ERROR, 
                            "RT_Info \"%s\" has unresolved "
                            "local dependencies.\n"),
                            rse.actual_rt_info ()->entry_point);
              }
          }
        else
          {
            // Specification error: any RT_Info that specifies threads
            // must also specify a period.
            ++this->thread_specification_errors_;
            ACE_DEBUG ((LM_ERROR, 
                        "RT_Info \"%s\" specifies %ld "
                        "threads, but no period.\n",
                        rse.actual_rt_info ()->entry_point,
                        rse.actual_rt_info ()->threads));
          }
        }
    }

  return 0;
}


// Propagates effective period and execution time multiplier from
// entry to successor prior to visiting successor.  Returns 0 on
// success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                    TAO_Reconfig_Scheduler_Entry &successor,
                    const RtecScheduler::Dependency_Info &di)
{
  // This method makes a conservative estimate in cases where periods
  // differ, taking the minimum frame size and dividing down the
  // execution multiplier of the longer frame (and rounding the result
  // upward as needed).  A more exact computation could be achieved
  // by merging sets of arrivals and frame offsets, but that
  // would in turn cost more in run-time computation time.
  // NOTE: this approach *only* works for harmonic periods.  For
  // non-harmonic periods, the set merge approach is necessary.

  if (successor.effective_period () == 0)
    {
      // If this is the first dependency by which the successor has
      // been reached, and the successor is not itself a thread
      // delineator, then simply adopt the effective period and
      // execution time multiplier of the shorter period.
      successor.effective_period (entry.effective_period ());
      successor.effective_exec_multiplier (entry.effective_exec_multiplier ());
    }
  else
    {
      // Otherwise, take the smaller of the two periods, and divide down
      // the execution multipliers accordingly.

      long new_exec_multiplier = 0;
      long old_exec_multiplier = 0;
     
      if (successor.effective_period () < entry.effective_period ())
        {
          // Store the previous execution multiplier.
          old_exec_multiplier = successor.effective_exec_multiplier ();

          // Divide down the new execution multiplier.
          long new_exec_multiplier =
            ACE_static_cast (long, 
                             old_exec_multiplier * 
                             successor.effective_period () /
                             entry.effective_period ());

          // Adjust for round-off error.
          if (old_exec_multiplier > 
              ACE_static_cast (long,
                               new_exec_multiplier * 
                               entry.effective_period () /
                               successor.effective_period ()))
            {
              ++new_exec_multiplier;
            }

          // Set the successor's effective period and execution multiplier.
          successor.effective_period (entry.effective_period ());
          successor.effective_exec_multiplier (entry.effective_exec_multiplier () + 
                                               new_exec_multiplier);
        }
      else
        {
          // Store the previous execution multiplier.
          old_exec_multiplier = entry.effective_exec_multiplier ();

          // Divide down the new execution multiplier.
          long new_exec_multiplier =
            ACE_static_cast (long, 
                             old_exec_multiplier * 
                             entry.effective_period () /
                             successor.effective_period ());

          // Adjust for round-off error.
          if (old_exec_multiplier > 
              ACE_static_cast (long,
                               new_exec_multiplier * 
                               successor.effective_period () /
                               entry.effective_period ()))
            {
              ++new_exec_multiplier;
            }

          // Just set the successor's execution multiplier (the period is unchanged).
          successor.effective_exec_multiplier (successor.effective_exec_multiplier () + 
                                               new_exec_multiplier);
        }
    }
}

