/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Scheduler.h
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHED_UTILS_H
#define TAO_RECONFIG_SCHED_UTILS_H

#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"

class TAO_Reconfig_Scheduler_Entry
{
  // = TITLE
  //   An implementation class used to store interim scheduling results
  //   such as DFS finishing order, etc.
  //
  // = DESCRIPTION
  //   This class improves the performance of the Reconfig Scheduler
  //   by saving information from one computation for use in other
  //   computations, and simplifies the implementation logic.

public:

  // @@ TO DO - add support for conjunctions and disjunctions.

  // Info for DFS traversal, topological sort of call graph.
  enum DFS_Status {NOT_VISITED, VISITED, FINISHED};

  TAO_Reconfig_Scheduler_Entry (RtecScheduler::RT_Info &rt_info);
  // Constructor.

  ACE_Scheduler_Factory::POD_RT_Info & orig_rt_info_data ();
  // Accessor for stored orginal RT_Info data.

  void orig_rt_info_data (const RtecScheduler::RT_Info &);
  // Store original RT_Info data.

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

  RtecScheduler::Period effective_period ();
  // Accessor for effective period of corresponding RT_Info.

  void effective_period (RtecScheduler::Period p);
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

  RtecScheduler::Period effective_period_;
  // Effective period of corresponding RT_Info.

  RtecScheduler::Time effective_exec_multiplier_;
  // Effective execution time multiplier for corresponding RT_Info.
};


//////////////////////////////////////////
// Reconfig Sched Entry Visitor classes //
//////////////////////////////////////////


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
class TAO_Reconfig_Sched_Entry_Visitor
{
  // = TITLE
  //   An abstract base class for scheduler entry visitors.
  //
  // = DESCRIPTION
  //   This class improves the performance of the Reconfig Scheduler
  //   by saving information from one computation for use in other
  //   computations, and simplifies the implementation logic.

public:

  TAO_Reconfig_Sched_Entry_Visitor 
    (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       DEPENDENCY_SET_MAP & dependency_map,
     TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       RT_INFO_MAP & rt_info_map);
  // Constructor.

  virtual int visit (TAO_Reconfig_Scheduler_Entry &);
  // Visit a Reconfig Scheduler Entry.  This method calls
  // protected hook methods that can be overridden by
  // derived classes, according to the Template Method
  // design pattern.


protected:
  TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
    DEPENDENCY_SET_MAP & dependency_map_;
  // Map of dependencies between RT_Info handles.

  TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
    RT_INFO_MAP & rt_info_map_;
  // Map of handles into RT_Infos.

  virtual int unconditional_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Performs an unconditional action when the entry is first reached.
  // Returns 0 for success, and -1 if an error occurred.

  virtual int precondition (TAO_Reconfig_Scheduler_Entry &rse);
  // Tests whether or not any conditional actions should be taken for
  // the entry.  Returns 0 if the actions should be applied, 1 if the
  // entry should be left alone, and -1 if an error occurred.

  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Performs an action on the entry prior to visiting any of
  // its successors.  Returns 0 on success and -1 on error.

  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);
  // Performs an action on a successor entry prior to visiting
  // it.  Returns 0 if the successor should be visited recursively,
  // 1 if the successor should not be visited, and -1 on error.

  virtual int postfix_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Performs an action on the entry after visiting all of
  // its successors.  Returns 0 on success and -1 on error.
};

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
class TAO_RSE_Reset_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
{
  // = TITLE
  //   A scheduler entry visitor that resets nodes.
  //
  // = DESCRIPTION
  //   This class resets each node it visits to an initial state
  //   prior to the first DFS traversal for topological sorting.

public:
  TAO_RSE_Reset_Visitor
    (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       DEPENDENCY_SET_MAP & dependency_map,
     TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       RT_INFO_MAP & rt_info_map);
  // Constructor.

protected:
  virtual int unconditional_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Resets the fields in the entry to pre-DFS traversal states.
  // Returns 0 on success and -1 on error.

  virtual int precondition (TAO_Reconfig_Scheduler_Entry &rse);
  // Tests whether or not any conditional actions should be taken for
  // the entry.  Always returns 1 so that after the unconditional
  // action is applied, nothing further is done with the entry.
};


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
class TAO_RSE_DFS_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
{
  // = TITLE
  //   A scheduler entry visitor that marks nodes with DFS start
  //   and finish times.
  //
  // = DESCRIPTION
  //   This class does DFS traversal marking of each node it visits,
  //   as appropriate according to DFS state markers.

public:

  TAO_RSE_DFS_Visitor
    (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       DEPENDENCY_SET_MAP & dependency_map,
     TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       RT_INFO_MAP & rt_info_map);
  // Constructor.

protected:
  virtual int precondition (TAO_Reconfig_Scheduler_Entry &rse);
  // Makes sure the entry has not previously been visited in forward DFS.
  // Returns 0 if the actions should be applied, 1 if the entry
  // should be left alone, and -1 if an error occurred.

  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Marks entry as forward visited and sets its forward DFS start
  // time, prior to visiting any of its successors.  Returns 0 on
  // success and -1 on error.

  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);
  // Marks whether or not successor is a thread delineator prior to
  // visiting it.  Returns 0 on success and -1 on error.

  virtual int postfix_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Marks entry as forward finished and sets its forward DFS finish
  // time, after all of its successors have been visited.  Returns 0
  // on success and -1 on error.

private:
  int DFS_time_;
  // Keeps track of DFS start and finish times.

};


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
class TAO_RSE_SCC_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
{
  // = TITLE
  //   A scheduler entry visitor that checks for cycles in the graph.
  //
  // = DESCRIPTION
  //   This class, when applied to nodes in the transpose of the DFS graph as
  //   ordered for the strongly connected components algorithm, will
  //   flag cycles in the graph.

public:

  TAO_RSE_SCC_Visitor
    (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       DEPENDENCY_SET_MAP & dependency_map,
     TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       RT_INFO_MAP & rt_info_map);
  // Constructor.

  int number_of_cycles (void);
  // Accessor for number of cycles detected in traversal.

  int in_a_cycle (void);
  // Accessor for whether or not the recursion is within a previously
  // detected cycle.

  void in_a_cycle (int);
  // Mutator for whether or not the recursion is within a previously
  // detected cycle.

protected:
  virtual int unconditional_action (TAO_Reconfig_Scheduler_Entry &rse);
  // If the entry is a thread delineator, sets its effective period and
  // execution multiplier from the values in its corresponding RT_Info.
  // Returns 0 for success, and -1 if an error occurred.

  virtual int precondition (TAO_Reconfig_Scheduler_Entry &rse);
  // Makes sure the entry has not previously been visited in the
  // reverse DFS (call graph transpose) direction.  Returns 0 if
  // the actions should be applied, 1 if the entry should be left
  // alone, and -1 if an error occurred.

  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Marks reverse status as visited and sets reverse start time for
  // entry, prior to visiting any of its successors.  Returns 0 on
  // success and -1 on error.

  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);
  // Checks reverse status of each successor.  For any that have not
  // been previously visited, it complains about the entry and
  // successor being part of a cycle, stores the fact that a cycle was
  // detected, and maintains a count of the total number of cycles
  // (strongly connected components).  Returns 0 on success and -1 on
  // an error (finding a cycle is not considered an error, at least as
  // far as this method is concerned).

  virtual int postfix_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Sets the entry's reverse finish time and marks it as finished in
  // the reverse DFS traversal, after visiting all of its successors.
  // Returns 0 on success and -1 on error.

private:
  int DFS_time_;
  // Keeps track of DFS start and finish times.

  int number_of_cycles_;
  // Keeps track of DFS start and finish times.

  int in_a_cycle_;
  // Indicates whether or not the recursion is
  // currently within a previously discovered cycle.
};


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK> 
class TAO_RSE_Propagation_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
{
  // = TITLE 
  //   A scheduler entry visitor that propagates effective periods and
  //   execution time multipliers between nodes in a topologically
  //   ordered graph.
  //
  // = DESCRIPTION
  //   This class computes the effective period of each node, according to
  //   its dependencies, and the period and threads it specifies.

public:

  TAO_RSE_Propagation_Visitor
    (TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       DEPENDENCY_SET_MAP & dependency_map,
     TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::
       RT_INFO_MAP & rt_info_map);
  // Constructor.

  int unresolved_locals (void);
  // Accessor for number of nodes with unresolved local dependencies.

  void unresolved_locals (int);
  // Mutator for numberof nodes with unresolved local dependencies.

  int unresolved_remotes (void);
  // Accessor for number of nodes with unresolved remote dependencies.

  void unresolved_remotes (int);
  // Mutator for number of nodes with unresolved remote dependencies.

  int thread_specification_errors (void);
  // Accessor for number of nodes with thread specification errors.

  void thread_specification_errors (int);
  // Mutator for number of nodes with thread specification errors.

protected:
  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);
  // Tests the entry for possibly having unresolved remote or local
  // dependencies prior to visiting any of its successors.  Returns 0
  // on success and -1 on error (having unresolved remote or local
  // dependencies is not considered an error, at least for this
  // method).

  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);
  // Propagates effective period and execution time multiplier from
  // entry to successor prior to visiting successor.  Returns 1 on
  // success (to prevent recursion on the successor), and -1 on error.

private:

  int unresolved_locals_;
  // Number of nodes with unresolved local dependencies.

  int unresolved_remotes_;
  // Number of nodes with unresolved remote dependencies.

  int thread_specification_errors_;
  // Number of nodes with thread specification errors.
};

CDG - TBD - create classes to sort an array of entry pointers, add arrays
            of entry pointers to the scheduler, can fill in these arrays in the
            create method of the scheduler, expanding arrays as needed (see note in scheduler)

CDG - TBD - Scheduling Strategy classes (Base and MUF for now: RMS, EDF, MLF later)

GENERAL (adapter)

    a. static int operation_sort_method (void *, void *)
       This should be able to just cast the void * args to RtecScheduer::RT_Info * args
       and return the result of a call to the priority_comp method

    b. sched entry pointer array[] sort method

PRIO

    c. static priority comparison method

    d. static subpriority comparison method

    f. whether or not an RT_Info change will affect static priority or subpriority
       int RECONFIG_SCHED_STRATEGY::priority_comp (const RtecScheduler::RT_Info &s,
                                                   const RtecScheduler::RT_Info &t)
			// returns -1 if s < t in <priority, subpriority>,
                        // 0 if same, and 1 if t > s.

CHARACTERISTICS

    g. whether or not an operation is critical, based on its characteristics

    h. static dispatch configuration from an RT_Info, based on its characteristics

    i. whether or not an RT_Info change will affect propagation of characteristics
      int RECONFIG_SCHED_STRATEGY::needs_propagation (const RtecScheduler::RT_Info &s,
                                                      const RtecScheduler::RT_Info &t)
			// returns 1 if s differs from t in propagation values, 0 if same.


#if defined (__ACE_INLINE__)
#include "orbsvcs/Reconfig_Sched_Utils.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_RECONFIG_SCHED_UTILS_H */

