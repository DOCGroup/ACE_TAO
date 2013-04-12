/**
 *  @file    Reconfig_Sched_Utils_T.cpp
 *
 *  $Id$
 *
 *  @author  Chris Gill <cdgill@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_RECONFIG_SCHED_UTILS_T_CPP
#define TAO_RECONFIG_SCHED_UTILS_T_CPP

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Sched/Reconfig_Sched_Utils_T.h"
#include "ace/Sched_Params.h"
#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

////////////////////////////////
// TAO_RSE_Dependency_Visitor //
////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_Dependency_Visitor
    (DEPENDENCY_SET_MAP & dependency_map, RT_INFO_MAP & rt_info_map)
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

  /* WSOA merge - commented out
  // Call unconditional action method, which performs any necessary
  // modifications that are applied to each node unconditionally.
  if (this->unconditional_action (rse) < 0)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         "TAO_RSE_Dependency_Visitor::"
                         "visit: error from unconditional action.\n"), -1);
    }
  */

  // Call precondition hook method, and only proceed if the
  // precondition returns 0 for success.

  result = this->precondition (rse);
  if (result < 0)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         "TAO_RSE_Dependency_Visitor::"
                         "visit: error from precondition evaluation.\n"), -1);
    }

  if (result == 0)
    {
      // Call prefix action method, which performs any necessary
      // modifications on the node prior to visiting its successors.
      if (this->prefix_action (rse) < 0)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
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
          TAO_RT_Info_Ex *next_rt_info = 0;
          for (u_int i = 0; i < dependency_set->length (); ++i)
            {
              // Skip over disabled dependencies
              if ((*dependency_set) [i].enabled == RtecBase::DEPENDENCY_DISABLED)
                {
                  continue;
                }

              // Take the handle from the dependency and use it
              // to obtain an RT_Info pointer from the map.
              if (rt_info_map_.find ((*dependency_set) [i].rt_info,
                                     next_rt_info) != 0)
                {
                  ORBSVCS_ERROR_RETURN ((LM_ERROR, "RT_Info (%i) not found.\n",
                                     (*dependency_set) [i].rt_info), -1);
                }

              // Extract a pointer to the scheduling entry from the RT_Info.

              if (next_rt_info == 0)
                {
                  ORBSVCS_ERROR_RETURN ((LM_ERROR, "RT_Info in map was null.\n"),
                                    -1);
                }

              // Reference the associated scheduling entry: the double cast is
              // needed to ensure that the size of the pointer and the size of the
              // stored magic cookie are the same
              next_rse =
                ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                                     next_rt_info->volatile_token);
              if (next_rse == 0)
                {
                  ORBSVCS_ERROR_RETURN ((LM_ERROR,
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
                  ORBSVCS_ERROR_RETURN ((LM_ERROR,
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
          ORBSVCS_ERROR_RETURN ((LM_ERROR, "TAO_RSE_Dependency_Visitor::"
                             "visit: error from postfix action.\n"), -1);
        }
    }

  return 0;
}

// WSOA merge - commented out
// // Performs an unconditional action when the entry is first reached.
// // Returns 0 for success, and -1 if an error occurred.

// template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
// TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
// unconditional_action (TAO_Reconfig_Scheduler_Entry & /* rse */)
// {
//   // Default behavior: just return success.
//   return 0;
// }
//

// Tests whether or not any conditional actions should be taken for
// the entry.  Returns 0 if the actions should be applied, 1 if the
// entry should be left alone, and -1 if an error occurred.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
precondition (TAO_Reconfig_Scheduler_Entry &rse)
{
  // Only signal to proceed (0) if the passed entry is enabled or non-volatile
  return (rse.enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    ? 1
    : 0;
}


// Performs an action on the entry prior to visiting any of
// its successors.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
prefix_action (TAO_Reconfig_Scheduler_Entry & /* rse */)
{
  // Default behavior: just return success.
  return 0;
}


// Performs an action on a successor entry prior to visiting
// it.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry & /* entry */,
                    TAO_Reconfig_Scheduler_Entry & /* successor */,
                    const RtecScheduler::Dependency_Info & /* di */)
{
  // Default behavior: just return success.
  return 0;
}


// Performs an action on the entry after visiting all of
// its successors.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
postfix_action (TAO_Reconfig_Scheduler_Entry & /* rse */)
{
  // Default behavior: just return success.
  return 0;
}



/////////////////////////
// TAO_RSE_DFS_Visitor //
/////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_DFS_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_DFS_Visitor
  (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
   typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map)
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
  int result =
    TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
      precondition (rse);

  return (result == 0)
    ? ((rse.fwd_dfs_status () == TAO_Reconfig_Scheduler_Entry::NOT_VISITED)
          ? 0
          : 1)
    : result;
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
pre_recurse_action (TAO_Reconfig_Scheduler_Entry & /* entry */,
                    TAO_Reconfig_Scheduler_Entry & successor,
                    const RtecScheduler::Dependency_Info & /* di */)
{
  // Enabled operations we reached via a dependency and that do not
  // specify a period are not thread delineators.
  if (successor.enabled_state () != RtecScheduler::RT_INFO_DISABLED
      && successor.actual_rt_info ()->period == 0
      && successor.actual_rt_info ()->threads == 0)
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
  (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
   typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map)
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

/* WSOA merge - commented out
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
*/

// Makes sure the entry has not previously been visited in the
// reverse DFS (call graph transpose) direction.  Returns 0 if
// the actions should be applied, 1 if the entry should be left
// alone, and -1 if an error occurred.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_SCC_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
precondition (TAO_Reconfig_Scheduler_Entry &rse)
{
  int result =
    TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
      precondition (rse);

  return (result == 0)
    ? ((rse.rev_dfs_status () == TAO_Reconfig_Scheduler_Entry::NOT_VISITED)
          ? 0
          : 1)
    : 1;
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
                    const RtecScheduler::Dependency_Info & /* di */)
{
  if  (successor.enabled_state () !=
         RtecScheduler::RT_INFO_DISABLED
       && successor.rev_dfs_status () ==
       TAO_Reconfig_Scheduler_Entry::NOT_VISITED)
    {
      if (this->in_a_cycle () == 0)
        {
          this->in_a_cycle (1);
          ++this->number_of_cycles_;
        }

      ORBSVCS_DEBUG ((LM_ERROR,
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

/////////////////////////////////////////
// TAO_RSE_Reverse_Propagation_Visitor //
/////////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_Reverse_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_Reverse_Propagation_Visitor
  (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
   typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map)
  : TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK> (dependency_map, rt_info_map)
{
}


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Reverse_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                    TAO_Reconfig_Scheduler_Entry &successor,
                    const RtecScheduler::Dependency_Info & /* di */)
{
  // @TODO - check for conjunction nodes here and perform conjunctive
  // function on existing rate tuples.  Idea: treat conjunctive tuples
  // as skolem functions over the possible rates of their incedent
  // edges thread delineators!!!  Then, can tentatively compute
  // utilization for rate combinations.  Question: can I find a case
  // where this makes tuple rate admission non-monotonic???  I.e.,
  // where a higher rate for an input results in a lower utilization?
  // Might require a skew in the exec times and rates.  What are the
  // determining characteristics of this?  What impact if any does
  // phasing have on this?

  // Check for conjunction nodes and don't propagate
  // upward from them: they represent a cut point in the graph.
  // Do not allow conjunction nodes for now.
  if (entry.actual_rt_info ()->info_type == RtecScheduler::CONJUNCTION)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Conjunction Nodes are not supported currently.")),
                        -1);
    }
  else
    {
      // @TODO - replace the explicit WCET attribute propagation with
      // a scheduling strategy functor that propagates arbitrary
      // execution time attributes.  BTW, for conjunctions BCET and WCET
      // are probably needed relative the upper and lower bounds on
      // arrival waveforms.

      // Add the successor's aggregate time to the entry's aggregate time.
      // Since we're visiting in topological order (called nodes before
      // calling nodes), the successor's aggregate time is up to date.
      if (successor.enabled_state () != RtecScheduler::RT_INFO_DISABLED)
        {
          entry.aggregate_exec_time (entry.aggregate_exec_time ()
                                     + successor.aggregate_exec_time ());
        }
    }


  // Do not recurse on the successor node, just continue to the next successor.
  return 1;
}


/////////////////////////////////////////
// TAO_RSE_Forward_Propagation_Visitor //
/////////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_Forward_Propagation_Visitor
  (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
   typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map)
  : TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK> (dependency_map, rt_info_map),
    unresolved_locals_ (0),
    unresolved_remotes_ (0),
    thread_specification_errors_ (0)
{
}


// Accessor for number of nodes with unresolved local dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_locals (void)
{
  return this->unresolved_locals_;
}


// Mutator for number of nodes with unresolved local dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_locals (int i)
{
  this->unresolved_locals_ = i;
}


// Accessor for number of nodes with unresolved remote dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_remotes (void)
{
  return this->unresolved_remotes_;
}


// Mutator for number of nodes with unresolved remote dependencies.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
unresolved_remotes (int i)
{
  this->unresolved_remotes_ = i;
}

// Accessor for number of nodes with thread specification errors.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
thread_specification_errors (void)
{
  return this->thread_specification_errors_;
}


// Mutator for number of nodes with thread specification errors.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> void
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
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
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
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

                ORBSVCS_DEBUG ((LM_ERROR,
                            "RT_Info \"%s\" has unresolved "
                            "remote dependencies.\n",
                            rse.actual_rt_info ()->entry_point.in ()));
              }
            else
              {
                ++this->unresolved_locals_;

                ORBSVCS_DEBUG ((LM_ERROR,
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
            ORBSVCS_DEBUG ((LM_ERROR,
                        "RT_Info \"%s\" specifies %1d "
                        "threads, but no period.\n",
                        rse.actual_rt_info ()->entry_point.in (),
                        rse.actual_rt_info ()->threads));
          }
        }
    }

  return 0;
}


// Propagates effective period from entry to successor prior to
// visiting successor.  Returns 0 on success and -1 on error.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Forward_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                    TAO_Reconfig_Scheduler_Entry &successor,
                    const RtecScheduler::Dependency_Info & /* di */)
{
  if (successor.enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    {
      return 1;
    }

  TAO_RT_Info_Tuple **tuple_ptr_ptr;

  TUPLE_SET_ITERATOR orig_tuple_iter (entry.orig_tuple_subset ());

  while (orig_tuple_iter.done () == 0)
    {
      if (orig_tuple_iter.next (tuple_ptr_ptr) == 0
          || tuple_ptr_ptr == 0 || *tuple_ptr_ptr == 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Failed to access tuple under iterator"));
          return -1;
        }

      // @TODO - check for conjunction nodes here and perform conjunctive
      // function on existing rate tuples.

#ifdef SCHEDULER_LOGGING
      ORBSVCS_DEBUG((LM_DEBUG, "Inserting new propagated tuple for RT_Info: %d, entry_ptr: 0x%x, tuple_ptr: 0x%x\n",
                 successor.actual_rt_info ()->handle,
                 &successor,
                 (*tuple_ptr_ptr)));
#endif
      // Propagate tuples disjunctively.
      successor.insert_tuple (**tuple_ptr_ptr,
                              TAO_Reconfig_Scheduler_Entry::PROPAGATED);

      successor.actual_rt_info ()->period =
        (successor.actual_rt_info ()->period == 0)
        ? (*tuple_ptr_ptr)->period
        : ACE::minimum_frame_size (successor.actual_rt_info ()->period,
                                   (*tuple_ptr_ptr)->period);
      orig_tuple_iter.advance ();
    }

  TUPLE_SET_ITERATOR prop_tuple_iter (entry.prop_tuple_subset ());

  while (prop_tuple_iter.done () == 0)
    {
      if (prop_tuple_iter.next (tuple_ptr_ptr) == 0
          || tuple_ptr_ptr == 0 || *tuple_ptr_ptr == 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Failed to access tuple under iterator"));
          return -1;
        }

      // @TODO - check for conjunction nodes here and perform conjunctive
      // function on existing rate tuples.

#ifdef SCHEDULER_LOGGING
      ORBSVCS_DEBUG((LM_DEBUG, "Inserting new propagated tuple for RT_Info: %d, entry_ptr: 0x%x, tuple_ptr: 0x%x\n",
                 successor.actual_rt_info ()->handle,
                 &successor,
                 (*tuple_ptr_ptr)));
#endif

      // Propagate tuples disjunctively.
      successor.insert_tuple (**tuple_ptr_ptr,
                              TAO_Reconfig_Scheduler_Entry::PROPAGATED);

      successor.actual_rt_info ()->period =
        (successor.actual_rt_info ()->period == 0)
        ? (*tuple_ptr_ptr)->period
        : ACE::minimum_frame_size (successor.actual_rt_info ()->period,
                                   (*tuple_ptr_ptr)->period);

      prop_tuple_iter.advance ();
    }

  // Do not recurse on the successor node, just continue to the next successor.
  return 1;
}

////////////////////////////////////
// class TAO_RSE_Priority_Visitor //
////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
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


// Visit a RT_Info tuple.  This method assigns a priority and
// subpriority value to each tuple.  Priorities are assigned in
// increasing numeric order, with lower numbers corresponding to
// higher priorities.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::visit (TAO_Reconfig_Scheduler_Entry &rse)
{
  int result = 0;
#ifdef SCHEDULER_LOGGING
  ORBSVCS_DEBUG ((LM_DEBUG,
              "Priority_Visitor visiting %s[%d],crit=%d,period=%d\n",
              rse.actual_rt_info ()->entry_point.in(),
              rse.actual_rt_info ()->handle,
              rse.actual_rt_info ()->criticality,
              rse.actual_rt_info ()->period));
#endif

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
#ifdef SCHEDULER_LOGGING
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "Previous entry %s[%d],crit=%d,period=%d\n",
                  previous_entry_->actual_rt_info ()->entry_point.in(),
                  previous_entry_->actual_rt_info ()->handle,
                  previous_entry_->actual_rt_info ()->criticality,
                  previous_entry_->actual_rt_info ()->period));
#endif
      // Don't change priority levels on a disabled node.
      if (rse.enabled_state () == RtecScheduler::RT_INFO_DISABLED
          || RECONFIG_SCHED_STRATEGY::compare_priority (*previous_entry_, rse) == 0)
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
              (*first_subpriority_entry_)->actual_rt_info ()->
                  preemption_subpriority += subpriority_;
            }

          subpriority_ = 0;
          rse.actual_rt_info ()->preemption_subpriority = subpriority_;

          ++priority_;
#ifdef SCHEDULER_LOGGING
          ORBSVCS_DEBUG ((LM_DEBUG, "New priority %d formed\n", priority_));
#endif
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
template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Priority_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::finish ()
{
  // Iterate back through and adjust the subpriority levels.
  for (int i = 0; i <= subpriority_; ++i, ++first_subpriority_entry_)
    {
      (*first_subpriority_entry_)->actual_rt_info ()->
        preemption_subpriority += subpriority_;
    }

  // Indicate no new priority level was identified.
  return 0;
}

///////////////////////////////////////
// class TAO_Tuple_Admission_Visitor //
///////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY>
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::
TAO_Tuple_Admission_Visitor (const CORBA::Double & critical_utilization_threshold,
                               const CORBA::Double & noncritical_utilization_threshold)
  : critical_utilization_ (0.0),
    noncritical_utilization_ (0.0),
    total_critical_utilization_ (0.0),
    total_noncritical_utilization_ (0.0),
    critical_utilization_threshold_ (critical_utilization_threshold),
    noncritical_utilization_threshold_ (noncritical_utilization_threshold)
{
}

// Visit a Reconfig Scheduler Entry.  This method
// determines the utilization by the entry, and
// adds it to the critical or non-critical utilization,
// depending on whether or not the strategy says the
// operation is critical.

template <class RECONFIG_SCHED_STRATEGY> int
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::visit (TAO_RT_Info_Tuple &t)
{
  TAO_Reconfig_Scheduler_Entry *entry =
    ACE_LONGLONG_TO_PTR (TAO_Reconfig_Scheduler_Entry *,
                         t.volatile_token);

  // Ignore disabled tuples and entries
  if (t.enabled_state () == RtecScheduler::RT_INFO_DISABLED
      || entry->enabled_state () == RtecScheduler::RT_INFO_DISABLED)
    {
      return 0;
    }

  // Compute the current tuple's utilization.
  CORBA::Double delta_utilization =
    (static_cast<CORBA::Double> (t.threads)
     * static_cast<CORBA::Double> (ACE_UINT64_DBLCAST_ADAPTER (entry->
                                                      aggregate_exec_time ())))
    / static_cast<CORBA::Double> (t.period);

  // Subtract the previous tuple's utilization (if any) for the entry.
  if (entry->current_admitted_tuple ())
    {
      delta_utilization -=
        (static_cast<CORBA::Double> (entry->current_admitted_tuple ()->threads)
         * static_cast<CORBA::Double> (ACE_UINT64_DBLCAST_ADAPTER (entry->
                                                          aggregate_exec_time ())))
        / static_cast<CORBA::Double> (entry->current_admitted_tuple ()->period);
    }

  if (RECONFIG_SCHED_STRATEGY::is_critical (t))
    {
      this->total_critical_utilization_ += delta_utilization;

      if (this->critical_utilization_ + this->noncritical_utilization_
          +delta_utilization
          < this->critical_utilization_threshold_)
        {
          this->critical_utilization_ += delta_utilization;
          entry->current_admitted_tuple (&t);
          entry->actual_rt_info ()->period = t.period;
        }
    }
  else
    {
      this->total_noncritical_utilization_ += delta_utilization;
      if (this->critical_utilization_ + this->noncritical_utilization_
          +delta_utilization
          < this->noncritical_utilization_threshold_)
        {
          this->noncritical_utilization_ += delta_utilization;
          entry->current_admitted_tuple (&t);
          entry->actual_rt_info ()->period = t.period;
        }
    }
  return 0;
}


// Accessor for utilization by critical operations.

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::critical_utilization ()
{
  return this->critical_utilization_;
}


// Accessor for utilization by noncritical operations.

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::noncritical_utilization ()
{
  return this->noncritical_utilization_;
}

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::total_critical_utilization ()
{
  return this->total_critical_utilization_;
}


// Accessor for utilization by noncritical operations.

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::total_noncritical_utilization ()
{
  return this->total_noncritical_utilization_;
}

// Accessor for utilization threshold for critical operations.

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::critical_utilization_threshold ()
{
  return this->critical_utilization_threshold_;
}


// Accessor for utilization by noncritical operations.

template <class RECONFIG_SCHED_STRATEGY> CORBA::Double
TAO_Tuple_Admission_Visitor<RECONFIG_SCHED_STRATEGY>::noncritical_utilization_threshold ()
{
  return this->noncritical_utilization_threshold_;
}


/////////////////////////////////////////
// TAO_RSE_Criticality_Propagation_Visitor //
/////////////////////////////////////////

// Constructor.

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
TAO_RSE_Criticality_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
TAO_RSE_Criticality_Propagation_Visitor
  (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
   typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map)
  : TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK> (dependency_map, rt_info_map)
{
}


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> int
TAO_RSE_Criticality_Propagation_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                    TAO_Reconfig_Scheduler_Entry &successor,
                    const RtecScheduler::Dependency_Info & /* di */)
{
#ifdef SCHEDULER_LOGGING
  ORBSVCS_DEBUG ((LM_DEBUG,
              "Crit Prop_Visitor visiting %s[%d], successor is %s[%d]\n",
              entry.actual_rt_info ()->entry_point.in(),
              entry.actual_rt_info ()->handle,
              successor.actual_rt_info ()->entry_point.in(),
              successor.actual_rt_info ()->handle));
#endif

  if (successor.enabled_state () != RtecScheduler::RT_INFO_DISABLED)
    {
      RtecScheduler::Criticality_t entry_crit =
        entry.actual_rt_info ()->criticality;
      RtecScheduler::Criticality_t succ_crit =
        successor.actual_rt_info ()->criticality;
      RtecScheduler::Criticality_t max_crit = entry_crit;

      if (max_crit < succ_crit)
        max_crit = succ_crit;

      successor.actual_rt_info ()->criticality = max_crit;

#ifdef SCHEDULER_LOGGING
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "Successor's new criticality is %d\n",
                  successor.actual_rt_info ()->criticality));
#endif
    }

  // Do not recurse on the successor node, just continue to the next successor.
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_RECONFIG_SCHED_UTILS_T_CPP */
