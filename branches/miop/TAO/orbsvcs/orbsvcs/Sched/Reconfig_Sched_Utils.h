/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Scheduler_Utils.h
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHED_UTILS_H
#define TAO_RECONFIG_SCHED_UTILS_H
#include "ace/pre.h"

#include "ace/config-all.h"

#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "sched_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_RTSched_Export TAO_Reconfig_Scheduler_Entry
  // = TITLE
  //   An implementation class used to store interim scheduling results
  //   such as DFS finishing order, etc.
  //
  // = DESCRIPTION
  //   This class improves the performance of the Reconfig Scheduler
  //   by saving information from one computation for use in other
  //   computations, and simplifies the implementation logic.
{
public:

  // Info for DFS traversal, topological sort of call graph.
  enum DFS_Status {NOT_VISITED, VISITED, FINISHED};

  TAO_Reconfig_Scheduler_Entry (RtecScheduler::RT_Info &rt_info);
  // Constructor.

  RtecScheduler::RT_Info & orig_rt_info_data (void);
  // Accessor for stored original RT_Info data.

  void orig_rt_info_data (RtecScheduler::RT_Info &data);
  // Mutator for stored original RT_Info data.

  RtecScheduler::RT_Info * actual_rt_info ();
  // Accessor for actual RT_Info.

  void actual_rt_info (RtecScheduler::RT_Info *);
  // Mutator for actual RT_Info.

  long fwd_discovered () const;
  // Accessor for when the node was discovered in forward DFS traversal.

  void fwd_discovered (long l);
  // Mutator for when the node was discovered in forward DFS traversal.

  long rev_discovered () const;
  // Accessor for when the node was discovered in reverse DFS traversal.

  void rev_discovered (long l);
  // Mutator for when the node was discovered in reverse DFS traversal.

  long fwd_finished () const;
  // Accessor for when the node was finished in forward DFS traversal.

  void fwd_finished (long l);
  // Mutator for when the node was finished in forward DFS traversal.

  long rev_finished () const;
  // Accessor for when the node was finished in reverse DFS traversal.

  void rev_finished (long l);
  // Mutator for when the node was finished in reverse DFS traversal.

  DFS_Status fwd_dfs_status () const;
  // Accessor for forward DFS traversal status of the node.

  DFS_Status rev_dfs_status () const;
  // Accessor for reverse DFS traversal status of the node.

  void fwd_dfs_status (DFS_Status ds);
  // Mutator for forward DFS traversal status of the node.

  void rev_dfs_status (DFS_Status ds);
  // Mutator for reverse DFS traversal status of the node.

  int is_thread_delineator () const;
  // Accessor for flag indicating whether node is a thread
  // delineator.

  void is_thread_delineator (int i);
  // Mutator for flag indicating whether node is a thread
  // delineator.

  int has_unresolved_remote_dependencies () const;
  // Accessor for flag indicating whether node has unresolved remote
  // dependencies.

  void has_unresolved_remote_dependencies (int i);
  // Mutator for flag indicating whether node has unresolved remote
  // dependencies.

  int has_unresolved_local_dependencies () const;
  // Accessor for flag indicating whether node has unresolved local
  // dependencies.

  void has_unresolved_local_dependencies (int i);
  // Mutator for flag indicating whether node has unresolved local
  // dependencies.

  RtecScheduler::Period_t effective_period ();
  // Accessor for effective period of corresponding RT_Info.

  void effective_period (RtecScheduler::Period_t p);
  // Mutator for effective period of corresponding RT_Info.

  CORBA::Long effective_exec_multiplier ();
  // Accessor for effective execution time multiplier of corresponding
  // RT_Info.

  void effective_exec_multiplier (CORBA::Long l);
  // Mutator for effective execution time multiplier of corresponding
  // RT_Info.

private:

  RtecScheduler::RT_Info orig_rt_info_data_;
  // Stores the values of operation characteristics as they were specified
  // in the most recent call to the Reconfig_Scheduler's set () method.
  // That way, the scheduler propagation pass can overwrite RT_Info fields
  // without losing the original values.  This is useful when

  RtecScheduler::RT_Info *actual_rt_info_;
  // Points to the actual RT_Info to which the schedling entry corresponds.

  DFS_Status fwd_dfs_status_;
  // Forward depth-first-search status of the entry.

  DFS_Status rev_dfs_status_;
  // Reverse depth-first-search status of the entry.

  long fwd_discovered_;
  // Forward depth-first-search discovery order of the entry.

  long rev_discovered_;
  // Reverse depth-first-search discovery order of the entry.

  long fwd_finished_;
  // Forward depth-first-search completion order of the entry.

  long rev_finished_;
  // Reverse depth-first-search completion order of the entry.

  int is_thread_delineator_;
  // Flag identifying threads in the call graph.

  int has_unresolved_remote_dependencies_;
  // Flag indicating whether or not there are unresolved remote
  // dependencies in the entry's dependency call chain.

  int has_unresolved_local_dependencies_;
  // Flag indicating whether or not there are unresolved local
  // dependencies in the entry's dependency call chain.

  CORBA::Long effective_exec_multiplier_;
  // Effective execution time multiplier for corresponding RT_Info.

  RtecScheduler::Period_t effective_period_;
  // Effective period of corresponding RT_Info.

};


class TAO_RTSched_Export TAO_Reconfig_Sched_Entry_Visitor
  // = TITLE
  //   An abstract base class for scheduler entry visitors.
  //
  // = DESCRIPTION
  //   This class simplifies the reconfig scheduler implementation
  //   by giving a common interface for distinct visitors over the
  //   scheduling entries.
{
public:

  virtual int visit (TAO_Reconfig_Scheduler_Entry &) = 0;
  // Visit a Reconfig Scheduler Entry.

};


class TAO_RTSched_Export TAO_RSE_Reset_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor
  // = TITLE
  //   A scheduler entry visitor that resets nodes.
  //
  // = DESCRIPTION
  //   This class resets each node it visits to an initial state
  //   prior to the first DFS traversal for topological sorting.
{
public:

  TAO_RSE_Reset_Visitor ();
  // Constructor.

  virtual int visit (TAO_Reconfig_Scheduler_Entry &rse);
  // Resets the fields in the entry to pre-DFS traversal states.
  // Returns 0 on success and -1 on error.
};




class TAO_RTSched_Export TAO_MUF_Reconfig_Sched_Strategy
  // = TITLE
  //   A scheduling strategy that implements the Maximum
  //   Urgency First scheduling algorithm.
  //
  // = DESCRIPTION
  //   The strategy assigns static thread and global priority according
  //   to operation criticality, assigns static subpriority according to
  //   importance and then topological order, and assigns a dispatching
  //   configuration with a minimum laxity dispatching queue for each
  //   distinct criticality level.
{
public:

  static int comp_entry_finish_times (const void *first, const void *second);
  // Ordering function to compare the DFS finish times of
  // two task entries, so qsort orders these in topological
  // order, with the higher times *first*.

  static int total_priority_comp (const void *, const void *);
  // Ordering function used to qsort an array of
  // TAO_Reconfig_Scheduler_Entry pointers into a total <priority,
  // subpriority> ordering.  Returns -1 if the first one is higher, 0
  // if they're the same, and 1 if the second one is higher.

  static int compare_priority (TAO_Reconfig_Scheduler_Entry &,
                               TAO_Reconfig_Scheduler_Entry &);
  // Compares two entries by priority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.


  static int compare_subpriority (TAO_Reconfig_Scheduler_Entry &,
                                  TAO_Reconfig_Scheduler_Entry &);
  // Compares two entries by subpriority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int priority_diff (RtecScheduler::RT_Info &s,
                            RtecScheduler::RT_Info &t);
  // Compares two RT_Infos by priority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int is_critical (TAO_Reconfig_Scheduler_Entry &rse);
  // Determines whether or not an entry is critical, based on operation characteristics.
  // returns 1 if critical, 0 if not

  static int assign_config (RtecScheduler::Config_Info &,
                            TAO_Reconfig_Scheduler_Entry &);
  // Fills in a static dispatch configuration for a priority level, based
  // on the operation characteristics of a representative scheduling entry.
};


#if defined (__ACE_INLINE__)
#include "Reconfig_Sched_Utils.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "Reconfig_Sched_Utils_T.h"

#include "ace/post.h"
#endif /* TAO_RECONFIG_SCHED_UTILS_H */
