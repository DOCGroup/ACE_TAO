/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Reconfig_Scheduler_Utils_T.h
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_RECONFIG_SCHED_UTILS_T_H
#define TAO_RECONFIG_SCHED_UTILS_T_H
#include "ace/pre.h"

#include "ace/config-all.h"

#include "Reconfig_Sched_Utils.h"

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> DEPENDENCY_SET_MAP;
  // Type of map used for O(1) lookup of RT_Info
  // dependency sets by caller or called handle.

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::RT_Info*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> RT_INFO_MAP;
  // Type of map used for O(1) lookup of RT_Infos by their handles.

  TAO_RSE_Dependency_Visitor
    (DEPENDENCY_SET_MAP & dependency_map,
     RT_INFO_MAP & rt_info_map);
  // Constructor.

  virtual int visit (TAO_Reconfig_Scheduler_Entry &);
  // Visit a Reconfig Scheduler Entry.  This method calls
  // protected hook methods that can be overridden by
  // derived classes, according to the Template Method
  // design pattern.

protected:

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

  DEPENDENCY_SET_MAP & dependency_map_;
  // Map of dependencies between RT_Info handles.

  RT_INFO_MAP & rt_info_map_;
  // Map of handles into RT_Infos.

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
                                  ACE_LOCK>& rt_info_map);
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
                                  ACE_LOCK> & rt_info_map);
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
                                  ACE_LOCK> & rt_info_map);
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

template <class RECONFIG_SCHED_STRATEGY>
class TAO_RSE_Priority_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor
  // = TITLE
  //   A scheduler entry visitor that assigns static priority and
  //   subpriority values to entries in an array already sorted in
  //   static <priority, subpriority> order.
  //
  // = DESCRIPTION
  //   The visitor uses the parameterized strategy type to determine
  //   priority and subpriority boundaries.
{
public:

  TAO_RSE_Priority_Visitor (RtecScheduler::handle_t handles,
                            TAO_Reconfig_Scheduler_Entry ** entry_ptr_array);
  // Constructor.

    virtual int visit (TAO_Reconfig_Scheduler_Entry &);
  // Visit a Reconfig Scheduler Entry.  This method
  // assigns a priority and subpriority value to each
  // entry.  Priorities are assigned in increasing value
  // order, with lower numbers corresponding to higher
  // priorities.  Returns -1 on error, 1 if a new priority
  // was assigned, or 0 otherwise.

  int finish ();
  // Finishes scheduler entry priority assignment by iterating over the
  // remaining entries in the last subpriority level, and adjusting
  // their subpriorities.

private:

  TAO_Reconfig_Scheduler_Entry *previous_entry_;
  // Pointer to previous entry most recently seen in the iteration.

  TAO_Reconfig_Scheduler_Entry **first_subpriority_entry_;
  // Pointer to first subpriority entry in the priority level.

  RtecScheduler::Preemption_Priority_t priority_;
  // Current priority value.

  RtecScheduler::Preemption_Subpriority_t subpriority_;
  // Current subpriority value.

  RtecScheduler::OS_Priority os_priority_;
  // Current OS (thread) priority value.

  RtecScheduler::handle_t handles_;
  // Number of handles in the entry pointer array.

  TAO_Reconfig_Scheduler_Entry ** entry_ptr_array_;
  // An array of pointers to entries we will be visiting.
};

template <class RECONFIG_SCHED_STRATEGY>
class TAO_RSE_Utilization_Visitor :
  public TAO_Reconfig_Sched_Entry_Visitor
  // = TITLE
  //   A scheduler entry visitor that accumulates utilization separately
  //   for the critical operations and the non-critical operations.
  //
  // = DESCRIPTION
  //   The visitor uses the parameterized strategy type to determine
  //   whether or not a given operation is critical.
{
public:

  TAO_RSE_Utilization_Visitor ();
  // Constructor.

    virtual int visit (TAO_Reconfig_Scheduler_Entry &);
  // Visit a Reconfig Scheduler Entry.  This method
  // determines the utilization by the entry, and
  // adds it to the critical or non-critical utilization,
  // depending on whether or not the strategy says the
  // operation is critical.

  CORBA::Double critical_utilization ();
  // Accessor for utilization by critical operations.

  CORBA::Double noncritical_utilization ();
  // Accessor for utilization by noncritical operations.

private:

  CORBA::Double critical_utilization_;
  // Utilization by critical operations.

  CORBA::Double noncritical_utilization_;
  // Utilization by noncritical operations.
};



#if defined (__ACE_INLINE__)
#include "Reconfig_Sched_Utils_T.i"
#endif /* __ACE_INLINE__ */


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Reconfig_Sched_Utils_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Reconfig_Sched_Utils_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_RECONFIG_SCHED_UTILS_T_H */
