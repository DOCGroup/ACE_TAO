// -*- C++ -*-


//=============================================================================
/**
 *  @file    Reconfig_Sched_Utils_T.h
 *
 *  $Id$
 *
 *  @author  Chris Gill <cdgill@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_RECONFIG_SCHED_UTILS_T_H
#define TAO_RECONFIG_SCHED_UTILS_T_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"
#include "orbsvcs/Sched/Reconfig_Sched_Utils.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//////////////////////////////////////////
// Reconfig Sched Entry Visitor classes //
//////////////////////////////////////////

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_RSE_Dependency_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor
  // = TITLE
  //   A base class for scheduler entry visitors that use a
  //   dependency map to traverse a dependency graph over entries.
  //
  // = DESCRIPTION
  //   This class hold the maps and a constructor that
  //   derived classes may call to pass them in.
{
public:

  /// Type of map used for O(1) lookup of RT_Info
  /// dependency sets by caller or called handle.
  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> DEPENDENCY_SET_MAP;

  /// Type of map used for O(1) lookup of RT_Infos by their handles.
  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  TAO_RT_Info_Ex*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> RT_INFO_MAP;

  /// Constructor.
  TAO_RSE_Dependency_Visitor
    (DEPENDENCY_SET_MAP & dependency_map,
     RT_INFO_MAP & rt_info_map);

  /// Visit a Reconfig Scheduler Entry.  This method calls
  virtual int visit (TAO_Reconfig_Scheduler_Entry &);
  // protected hook methods that can be overridden by
  // derived classes, according to the Template Method
  // design pattern.

protected:
  /**
   * Performs an unconditional action when the entry is first reached.
   * Returns 0 for success, and -1 if an error occurred.
   * Tests whether or not any conditional actions should be taken for
   * the entry.  Returns 0 if the actions should be applied, 1 if the
   * entry should be left alone, and -1 if an error occurred.
   */
  /* WSOA merge - commented out
  virtual int unconditional_action (TAO_Reconfig_Scheduler_Entry &rse);
  */
  virtual int precondition (TAO_Reconfig_Scheduler_Entry &rse);

  /// Performs an action on the entry prior to visiting any of
  /// its successors.  Returns 0 on success and -1 on error.
  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);

  /**
   * Performs an action on a successor entry prior to visiting
   * it.  Returns 0 if the successor should be visited recursively,
   * 1 if the successor should not be visited, and -1 on error.
   */
  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);

  /// Performs an action on the entry after visiting all of
  /// its successors.  Returns 0 on success and -1 on error.
  virtual int postfix_action (TAO_Reconfig_Scheduler_Entry &rse);

  /// Map of dependencies between RT_Info handles.
  DEPENDENCY_SET_MAP & dependency_map_;

  /// Map of handles into RT_Infos.
  RT_INFO_MAP & rt_info_map_;

};



template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_RSE_DFS_Visitor :
  public TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
  // = TITLE
  //   A scheduler entry visitor that marks nodes with DFS start
  //   and finish times.
  //
  // = DESCRIPTION
  //   This class does DFS traversal marking of each node it visits,
  //   as appropriate according to DFS state markers.
{
public:

  /// Constructor.
  TAO_RSE_DFS_Visitor
    (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
     typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map);

protected:
  /**
   * Makes sure the entry has not previously been visited in forward DFS.
   * Returns 0 if the actions should be applied, 1 if the entry
   * should be left alone, and -1 if an error occurred.
   */
  virtual int precondition (TAO_Reconfig_Scheduler_Entry &rse);

  /**
   * Marks entry as forward visited and sets its forward DFS start
   * time, prior to visiting any of its successors.  Returns 0 on
   * success and -1 on error.
   */
  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);

  /// Marks whether or not successor is a thread delineator prior to
  /// visiting it.  Returns 0 on success and -1 on error.
  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);

  /**
   * Marks entry as forward finished and sets its forward DFS finish
   * time, after all of its successors have been visited.  Returns 0
   * on success and -1 on error.
   */
  virtual int postfix_action (TAO_Reconfig_Scheduler_Entry &rse);

private:
  /// Keeps track of DFS start and finish times.
  int DFS_time_;

};


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_RSE_SCC_Visitor :
  public TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
  // = TITLE
  //   A scheduler entry visitor that checks for cycles in the graph.
  //
  // = DESCRIPTION
  //   This class, when applied to nodes in the transpose of the DFS graph as
  //   ordered for the strongly connected components algorithm, will
  //   flag cycles in the graph.
{
public:

  /// Constructor.
  TAO_RSE_SCC_Visitor
    (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
     typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map);

  /// Accessor for number of cycles detected in traversal.
  int number_of_cycles (void);

  /// Accessor for whether or not the recursion is within a previously
  /// detected cycle.
  int in_a_cycle (void);

  /// Mutator for whether or not the recursion is within a previously
  /// detected cycle.
  void in_a_cycle (int);

protected:
  /**
   * If the entry is a thread delineator, sets its effective period and
   * execution multiplier from the values in its corresponding RT_Info.
   * Returns 0 for success, and -1 if an error occurred.
   */
  /* WSOA merge - commented out
  virtual int unconditional_action (TAO_Reconfig_Scheduler_Entry &rse);
  */

  /**
   * Makes sure the entry has not previously been visited in the
   * reverse DFS (call graph transpose) direction.  Returns 0 if
   * the actions should be applied, 1 if the entry should be left
   * alone, and -1 if an error occurred.
   */
  virtual int precondition (TAO_Reconfig_Scheduler_Entry &rse);

  /**
   * Marks reverse status as visited and sets reverse start time for
   * entry, prior to visiting any of its successors.  Returns 0 on
   * success and -1 on error.
   */
  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);

  /**
   * Checks reverse status of each successor.  For any that have not
   * been previously visited, it complains about the entry and
   * successor being part of a cycle, stores the fact that a cycle was
   * detected, and maintains a count of the total number of cycles
   * (strongly connected components).  Returns 0 on success and -1 on
   * an error (finding a cycle is not considered an error, at least as
   * far as this method is concerned).
   */
  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);

  /**
   * Sets the entry's reverse finish time and marks it as finished in
   * the reverse DFS traversal, after visiting all of its successors.
   * Returns 0 on success and -1 on error.
   */
  virtual int postfix_action (TAO_Reconfig_Scheduler_Entry &rse);

private:
  /// Keeps track of DFS start and finish times.
  int DFS_time_;

  /// Keeps track of DFS start and finish times.
  int number_of_cycles_;

  /// Indicates whether or not the recursion is
  /// currently within a previously discovered cycle.
  int in_a_cycle_;
};

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_RSE_Reverse_Propagation_Visitor :
  public TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
  // = TITLE
  //   A scheduler entry visitor that propagates aggregate execution
  //   times from called to calling nodes in a topologically ordered
  //   graph.
  //
  // = DESCRIPTION
  //   This class computes the aggregate execution time of each node
  //   and its dependants, according to its dependencies.
{
public:

  /// Constructor.
  TAO_RSE_Reverse_Propagation_Visitor
    (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
     typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map);

protected:

  /**
   * Propagates aggregate execution time from successor to calling
   * entry.  Returns 1 on success (to prevent recursion on the
   * successor), and -1 on error.
   */
  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);

};


template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_RSE_Forward_Propagation_Visitor :
  public TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
  // = TITLE
  //   A scheduler entry visitor that propagates effective periods and
  //   execution time multipliers between nodes in a topologically
  //   ordered graph.
  //
  // = DESCRIPTION
  //   This class computes the effective period of each node, according to
  //   its dependencies, and the period and threads it specifies.
{
public:

  /// Constructor.
  TAO_RSE_Forward_Propagation_Visitor
    (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
     typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map);

  /// Accessor for number of nodes with unresolved local dependencies.
  int unresolved_locals (void);

  /// Mutator for numberof nodes with unresolved local dependencies.
  void unresolved_locals (int);

  /// Accessor for number of nodes with unresolved remote dependencies.
  int unresolved_remotes (void);

  /// Mutator for number of nodes with unresolved remote dependencies.
  void unresolved_remotes (int);

  /// Accessor for number of nodes with thread specification errors.
  int thread_specification_errors (void);

  /// Mutator for number of nodes with thread specification errors.
  void thread_specification_errors (int);

protected:
  /**
   * Tests the entry for possibly having unresolved remote or local
   * dependencies prior to visiting any of its successors.  Returns 0
   * on success and -1 on error (having unresolved remote or local
   * dependencies is not considered an error, at least for this
   * method).
   */
  virtual int prefix_action (TAO_Reconfig_Scheduler_Entry &rse);

  /**
   * Propagates effective period and execution time multiplier from
   * entry to successor prior to visiting successor.  Returns 1 on
   * success (to prevent recursion on the successor), and -1 on error.
   */
  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);

private:

  /// Number of nodes with unresolved local dependencies.
  int unresolved_locals_;

  /// Number of nodes with unresolved remote dependencies.
  int unresolved_remotes_;

  /// Number of nodes with thread specification errors.
  int thread_specification_errors_;
};

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_RSE_Priority_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor
  // = TITLE
  //   An entry visitor that assigns static priority and subpriority
  //   values to entries in an array already sorted in static
  //   <priority, subpriority> order.
  //
  // = DESCRIPTION
  //   The visitor uses the parameterized strategy type to determine
  //   priority and subpriority boundaries.
{
public:

  /// Constructor.
  TAO_RSE_Priority_Visitor (RtecScheduler::handle_t handles,
                            TAO_Reconfig_Scheduler_Entry ** entry_ptr_array);

  /**
   * Visit a RT_Info tuple.  This method assigns a priority and
   * subpriority value to each tuple.  Priorities are assigned in
   * increasing value order, with lower numbers corresponding to
   * higher priorities.  Returns -1 on error, 1 if a new priority was
   * assigned, or 0 otherwise.
   */
    virtual int visit (TAO_Reconfig_Scheduler_Entry &);

  /**
   * Finishes tuple priority assignment by iterating over the
   * remaining tuples in the last subpriority level, and adjusting
   * their subpriorities.
   */
  int finish ();

private:

  /// Pointer to previous tuple in the iteration.
  TAO_Reconfig_Scheduler_Entry *previous_entry_;

  /// Pointer to first subpriority tuple in the priority level.
  TAO_Reconfig_Scheduler_Entry **first_subpriority_entry_;

  /// Current priority value.
  RtecScheduler::Preemption_Priority_t priority_;

  /// Current subpriority value.
  RtecScheduler::Preemption_Subpriority_t subpriority_;

  /// Current OS (thread) priority value.
  RtecScheduler::OS_Priority os_priority_;

  /// Number of handles in the entry pointer array.
  RtecScheduler::handle_t handles_;

  /// An array of pointers to entries we will be visiting.
  TAO_Reconfig_Scheduler_Entry ** entry_ptr_array_;
};

template <class RECONFIG_SCHED_STRATEGY>
class TAO_Tuple_Admission_Visitor :
  public TAO_RT_Info_Tuple_Visitor
  // = TITLE

  //   A tuple visitor that accumulates utilization separately for
  //   critical and non-critical operations.  Operation tuples that
  //   fit within the threashold defined for their criticality level
  //   are admitted to the schedule, by updating the corresponding
  //   RT_Info with the tuple data.
  //
  // = DESCRIPTION
  //   The visitor uses the parameterized strategy type to determine
  //   whether or not a given operation is critical.
{
public:

  /// Constructor.
  TAO_Tuple_Admission_Visitor (const CORBA::Double & critical_utilization_threshold,
                               const CORBA::Double & noncritical_utilization_threshold);

  /**
   * Visit an RT_Info tuple.  This method determines the utilization by
   * the tuple, and if it's admissible, updates its RT_Info and either
   * the critical or non-critical utilization, depending on whether or
   * not the strategy says the operation is critical.
   */
    virtual int visit (TAO_RT_Info_Tuple &);

  /// Accessor for utilization by critical operations.
  CORBA::Double critical_utilization ();

  /// Accessor for utilization by noncritical operations.
  CORBA::Double noncritical_utilization ();

  /// Accessor for utilization by critical operations.
  CORBA::Double critical_utilization_threshold ();

  /// Accessor for utilization by noncritical operations.
  CORBA::Double noncritical_utilization_threshold ();

  /// Accessor for utilization by critical operations.
  CORBA::Double total_critical_utilization ();

  /// Accessor for utilization by noncritical operations.
  CORBA::Double total_noncritical_utilization ();

private:

  /// Utilization by critical operations.
  CORBA::Double critical_utilization_;

  /// Utilization by noncritical operations.
  CORBA::Double noncritical_utilization_;

  /// Utilization by critical operations.
  CORBA::Double total_critical_utilization_;

  /// Utilization by noncritical operations.
  CORBA::Double total_noncritical_utilization_;

  /// Utilization by critical operations.
  CORBA::Double critical_utilization_threshold_;

  /// Utilization by noncritical operations.
  CORBA::Double noncritical_utilization_threshold_;
};

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_RSE_Criticality_Propagation_Visitor :
  public TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>
  // = TITLE
  //   A scheduler entry visitor that propagates criticality
  //   from called to calling nodes in a topologically ordered
  //   graph.
  //
  // = DESCRIPTION
  //   This class computes the criticality of each node
  //   and its dependants, according to its dependencies.
{
public:

  /// Constructor.
  TAO_RSE_Criticality_Propagation_Visitor
    (typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP & dependency_map,
     typename TAO_RSE_Dependency_Visitor<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::RT_INFO_MAP & rt_info_map);

protected:

  /**
   * Propagates criticality from successor to calling
   * entry.  Returns 1 on success (to prevent recursion on the
   * successor), and -1 on error.
   */
  virtual int pre_recurse_action (TAO_Reconfig_Scheduler_Entry &entry,
                                  TAO_Reconfig_Scheduler_Entry &successor,
                                  const RtecScheduler::Dependency_Info &di);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Sched/Reconfig_Sched_Utils_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Reconfig_Sched_Utils_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_RECONFIG_SCHED_UTILS_T_H */
