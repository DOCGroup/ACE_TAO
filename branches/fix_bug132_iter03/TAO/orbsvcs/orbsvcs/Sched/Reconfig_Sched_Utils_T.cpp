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
//    Reconfig_Sched_Utils_T.cpp
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHED_UTILS_T_C
#define TAO_RECONFIG_SCHED_UTILS_T_C

#include "Reconfig_Sched_Utils_T.h"
#include "ace/Sched_Params.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "Reconfig_Sched_Utils_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Sched, Reconfig_Sched_Utils_T, "$Id$")

////////////////////////////////
// TAO_RSE_Dependency_Visitor //
////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_Dependency_Visitor
    (ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & dependency_map, ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::RT_Info*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & rt_info_map)
  : dependency_map_ (dependency_map),
    rt_info_map_ (rt_info_map)
{
}


// Visit a Reconfig Scheduler Entry.  This method calls protected hook
// methods that can be overridden by derived classes, according to the
// Template Method design pattern.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
visit (TAO_Reconfig_Scheduler_Entry &rse)
{
  int result = 0;

  // Call unconditional action method, which performs any necessary
  // modifications that are applied to each node unconditionally.
  if (this->unconditional_action (rse) < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_RSE_Dependency_Visitor::"
                         "visit: error from unconditional action.\n"), -1);
    }

  // Call precondition hook method, and only proceed if the
  // precondition returns 0 for success.

  result = this->precondition (rse);
  if (result < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO_RSE_Dependency_Visitor::"
                         "visit: error from precondition evaluation.\n"), -1);
    }

  if (result == 0)
    {
      // Call prefix action method, which performs any necessary
      // modifications on the node prior to visiting its successors.
      if (this->prefix_action (rse) < 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "TAO_RSE_Dependency_Visitor::"
                             "visit: error from prefix action.\n"), -1);
        }

      // Get the dependency set for the current entry.
      RtecScheduler::Dependency_Set *dependency_set = 0;
      if (dependency_map_.find (rse.actual_rt_info ()->handle,
                                dependency_set) == 0)
        {
          // Iterate over the set of dependencies for the current entry.
          TAO_Reconfig_Scheduler_Entry * next_rse = 0;
          RtecScheduler::RT_Info *next_rt_info;
          for (u_int i = 0; i < dependency_set->length (); ++i)
            {
              // Take the handle from the dependency and use it
              // to obtain an RT_Info pointer from the map.
              if (rt_info_map_.find ((*dependency_set) [i].rt_info,
                                     next_rt_info) != 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR, "RT_Info not found.\n"), -1);
                }

              // Extract a pointer to the scheduling entry from the RT_Info.

              if (next_rt_info == 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR, "RT_Info in map was null.\n"),
                                    -1);
                }

              // Reference the associated scheduling entry: the double cast is
              // needed to ensure that the size of the pointer and the size of the
              // stored magic cookie are the same (see the definition of
              // ptr_arith_t in ACE to grok how this works portably).
              next_rse =
                ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                                     next_rt_info->volatile_token);
              if (next_rse == 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "Entry pointer in RT_Info was null.\n"),
                                    -1);
                }

              // Call pre-recursion action method, which performs any necessary
              // modifications to a successor (or the entry) prior to recursing
              // on the successor.
              result = this->pre_recurse_action (rse, *next_rse,
                                                 (*dependency_set) [i]);
              if (result < 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "TAO_RSE_Dependency_Visitor::visit: "
                                     "error from pre-recursion action.\n"),
                                    -1);
                }

              // If the pre-recursion action returned 0, visit the successor.
              if (result == 0)
                {
                  this->visit (*next_rse);
                }
            }

        }

      // Call postfix action method, which performs any necessary
      // modifications on the node after visiting all its successors.
      if (this->postfix_action (rse) < 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "TAO_RSE_Dependency_Visitor::"
                             "visit: error from postfix action.\n"), -1);
        }
    }

  return 0;
}


// Performs an unconditional action when the entry is first reached.
// Returns 0 for success, and -1 if an error occurred.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
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
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
precondition (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (rse);
  return 0;
}


// Performs an action on the entry prior to visiting any of
// its successors.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
prefix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (rse);
  return 0;
}


// Performs an action on a successor entry prior to visiting
// it.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
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
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
postfix_action (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Default behavior: just return success.
  ACE_UNUSED_ARG (rse);
  return 0;
}



/////////////////////////
// TAO_RSE_DFS_Visitor //
/////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_DFS_Visitor
  (ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & dependency_map,
   ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::RT_Info*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & rt_info_map)
  : TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
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
                    const RtecScheduler::Dependency_Info &di)
{
  ACE_UNUSED_ARG (entry);
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
postfix_action (TAO_Reconfig_Scheduler_Entry &rse)
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
  (ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & dependency_map,
   ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::RT_Info*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & rt_info_map)
  : TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
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
                  entry.actual_rt_info ()->entry_point.in (),
                  successor.actual_rt_info ()->entry_point.in (),
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
  (ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & dependency_map,
   ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::RT_Info*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> & rt_info_map)
  : TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK> (dependency_map, rt_info_map),
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
                            "remote dependencies.\n",
                            rse.actual_rt_info ()->entry_point.in ()));
              }
            else
              {
                ++this->unresolved_locals_;

                ACE_DEBUG ((LM_ERROR,
                            "RT_Info \"%s\" has unresolved "
                            "local dependencies.\n",
                            rse.actual_rt_info ()->entry_point.in ()));
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
                        rse.actual_rt_info ()->entry_point.in (),
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
  ACE_UNUSED_ARG (di);

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
          new_exec_multiplier =
            ACE_static_cast (long,
                             (old_exec_multiplier *
                              successor.effective_period ()) /
                             entry.effective_period ());

          // Adjust for round-off error.
          if (old_exec_multiplier >
              ACE_static_cast (long,
                               (new_exec_multiplier *
                                entry.effective_period ()) /
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
          new_exec_multiplier =
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

  // Do not recurse on the successor node, just continue to the next successor.
  return 1;
}


////////////////////////////////////
// class TAO_RSE_Priority_Visitor //
////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY>
TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY>::
TAO_RSE_Priority_Visitor (RtecScheduler::handle_t handles,
                          TAO_Reconfig_Scheduler_Entry ** entry_ptr_array)
  : previous_entry_ (0),
    first_subpriority_entry_ (0),
    priority_ (0),
    subpriority_ (0),
    os_priority_ (ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                                  ACE_SCOPE_PROCESS)),
    handles_ (handles),
    entry_ptr_array_ (entry_ptr_array)
{
}


// Visit a Reconfig Scheduler Entry.  This method
// assigns a priority and subpriority value to each
// entry.  Priorities are assigned in increasing value
// order, with lower numbers corresponding to higher
// priorities.

template <class RECONFIG_SCHED_STRATEGY> int
TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY>::visit (TAO_Reconfig_Scheduler_Entry &rse)
{
  int result = 0;

  if (previous_entry_ == 0)
    {
      // Indicate a new priority level was assigned.
      result = 1;

      // If we're on the first node, store the start of the array
      // as the start of the priority level.
      first_subpriority_entry_ = this->entry_ptr_array_;
      rse.actual_rt_info ()->preemption_subpriority = subpriority_;
    }
  else
    {
      if (RECONFIG_SCHED_STRATEGY::compare_priority (*previous_entry_, rse) == 0)
        {
          // Subpriority is increased at each new node.
          ++subpriority_;

          // Store negative value of subpriority level: will be
          // adjusted by adding back in the total number of
          // subpriorities in the priority level, so the
          // subpriorities are assigned in decreasing order.
          rse.actual_rt_info ()->preemption_subpriority = - subpriority_;
        }
      else
        {
          // Indicate a new priority level was assigned.
          result = 1;

          // Iterate back through and adjust the subpriority levels.
          for (int i = 0; i <= subpriority_; ++i, ++first_subpriority_entry_)
            {
              (*first_subpriority_entry_)->
                actual_rt_info ()->
                  preemption_subpriority += subpriority_;
            }

          subpriority_ = 0;
          rse.actual_rt_info ()->preemption_subpriority = subpriority_;

          ++priority_;
          os_priority_ = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                    os_priority_,
                                                    ACE_SCOPE_PROCESS);
        }
    }

  // Assign the entry's priority and subpriority values
  rse.actual_rt_info ()->priority = os_priority_;
  rse.actual_rt_info ()->preemption_priority = priority_;

  // Remember the current entry for the next visit.
  previous_entry_ = &rse;

  return result;
}


// Finishes scheduler entry priority assignment by iterating over the
// remaining entries in the last subpriority level, and adjusting
// their subpriorities.
template <class RECONFIG_SCHED_STRATEGY> int
TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY>::finish ()
{
  // Iterate back through and adjust the subpriority levels.
  for (int i = 0; i <= subpriority_; ++i, ++first_subpriority_entry_)
    {
      (*first_subpriority_entry_)->
        actual_rt_info ()->
          preemption_subpriority += subpriority_;
    }

  // Indicate no new proirity level was identified.
  return 0;
}

///////////////////////////////////////
// class TAO_RSE_Utilization_Visitor //
///////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY>
TAO_RSE_Utilization_Visitor<RECONFIG_SCHED_STRATEGY>::TAO_RSE_Utilization_Visitor ()
  : critical_utilization_ (0.0),
    noncritical_utilization_ (0.0)
{
}

// Visit a Reconfig Scheduler Entry.  This method
// determines the utilization by the entry, and
// adds it to the critical or non-critical utilization,
// depending on whether or not the strategy says the
// operation is critical.

template <class RECONFIG_SCHED_STRATEGY> int
TAO_RSE_Utilization_Visitor<RECONFIG_SCHED_STRATEGY>::visit (TAO_Reconfig_Scheduler_Entry &rse)
{
  CORBA::Double entry_period = rse.effective_period ();
  CORBA::Double entry_time = ACE_static_cast (
    CORBA::Double,
    ACE_UINT64_DBLCAST_ADAPTER (rse.actual_rt_info ()->
                                worst_case_execution_time));
  CORBA::Double entry_mult = rse.effective_exec_multiplier ();

  if (RECONFIG_SCHED_STRATEGY::is_critical (rse))
    {
      this->critical_utilization_ =
        (entry_mult * entry_time) / entry_period;
    }
  else
    {
      this->noncritical_utilization_ =
        (entry_mult * entry_time) / entry_period;
    }

  return 0;
}


// Accessor for utilization by critical operations.

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_RSE_Utilization_Visitor<RECONFIG_SCHED_STRATEGY>::critical_utilization ()
{
  return this->critical_utilization_;
}


// Accessor for utilization by noncritical operations.

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_RSE_Utilization_Visitor<RECONFIG_SCHED_STRATEGY>::noncritical_utilization ()
{
  return this->noncritical_utilization_;
}


#endif /* TAO_RECONFIG_SCHED_UTILS_T_C */
