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
#include /**/ "ace/pre.h"

// Uncomment this to turn on some extra trace level debugging info,
// comment it out to turn off that extra debugging info.
#define SCHEDULER_LOGGING

#include "ace/config-all.h"

#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "sched_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

struct TAO_RTSched_Export TAO_RT_Info_Ex
  : public RtecScheduler::RT_Info
  // = TITLE
  //   A wrapper class for the IDL-generated RT_Info operation descriptors.
  //
  // = DESCRIPTION
  //   This class provides (re)initialization methods and a validity flag
  //   for the underlying IDL-generated RT_Info descriptor.
{
  TAO_RT_Info_Ex ();
  // Default Constructor.

  TAO_RT_Info_Ex (const RtecScheduler::RT_Info &);
  // Constructor from an RT_Info
  // (also serves as a copy constructor).

  virtual ~TAO_RT_Info_Ex ();
  // Destructor.

  void operator = (const RtecScheduler::RT_Info &);
  // Assignment operator with an RT_Info on the RHS.

  virtual void reset (u_long reset_flags);
  // Resets output data members to initial values, and removes tuples
  // corresponding to the reset flags.

  virtual void enabled_state (RtecScheduler::RT_Info_Enabled_Type_t);
  // Sets the info and underlying entry's enable states.

  virtual RtecScheduler::RT_Info_Enabled_Type_t enabled_state ();
  // Returns the info's enable state.
};


struct TAO_RTSched_Export TAO_RT_Info_Tuple
  : public TAO_RT_Info_Ex
  // = TITLE
  //   An implementation class used to wrap available operation descriptors.
  //   Each operation may have one or more tuples describing different rates,
  //   etc., for the same operation.
  //
  // = DESCRIPTION
  //   This class enables admission control within the Reconfig Scheduler,
  //   which offers improved performance compared to admission control
  //   outside the scheduler.
{
  TAO_RT_Info_Tuple ();
  // Default Constructor.

  TAO_RT_Info_Tuple (const RtecScheduler::RT_Info &);
  // Constructor from an RT_Info.
  // (also serves as a copy constructor).

  virtual ~TAO_RT_Info_Tuple ();
  // Destructor.

  void operator = (const RtecScheduler::RT_Info &);
  // Assignment operator with an RT_Info on the RHS.

  int operator < (const TAO_RT_Info_Tuple &t);
  // Less-than comparison operator: orders tuples by ascending rate (descending period).

  u_long rate_index;
  // Index of the tuple in the operation's ordered available rates
};


class TAO_RTSched_Export TAO_Reconfig_Scheduler_Entry
  // = TITLE
  //   An implementation class used to decouple the available descriptors
  //   from the admitted descriptors, and to store interim scheduling results
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

  enum Tuple_Type {ORIGINAL = 0x01UL, PROPAGATED = 0x02UL};

  TAO_Reconfig_Scheduler_Entry (TAO_RT_Info_Ex &rt_info);
  // Constructor.

  ~TAO_Reconfig_Scheduler_Entry ();
  // Destructor.

  void remove_tuples (u_long tuple_flags = ORIGINAL | PROPAGATED);
  // Removes all tuples from the entry.

  int insert_tuple (TAO_RT_Info_Tuple &tuple,
                    Tuple_Type tuple_type = ORIGINAL,
                    int replace = 0);
  // Inserts a tuple into the appropriate tuple multiset.

  int update_tuple (TAO_RT_Info_Ex &info,
                    Tuple_Type tuple_type = ORIGINAL);
  // Updates a matching tuple.

  int register_tuples (TAO_RT_Info_Tuple ** tuple_ptr_array,
                       long &tuple_count);
  // Registers tuples into the passed tuple pointer array.

  TAO_RT_Info_Ex & orig_rt_info_data (void);
  // Accessor for stored original RT_Info data.

  void orig_rt_info_data (TAO_RT_Info_Ex &data);
  // Mutator for stored original RT_Info data.

  TAO_RT_Info_Ex * actual_rt_info ();
  // Accessor for actual RT_Info.

  void actual_rt_info (TAO_RT_Info_Ex *);
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

  RtecScheduler::Time aggregate_exec_time ();
  // Accessor for effective execution time of the corresponding
  // RT_Info and all of its disjunctively (i.e., dispatching waveforms
  // are additive) executed dependants.

  void aggregate_exec_time (RtecScheduler::Time t);
  // Mutator for effective execution time of the corresponding RT_Info
  // and its disjunctively executed dependants.

  RtecScheduler::Period_t orig_tuple_period_sum ();
  // Accessor for the sum of periods for tuples directly associated
  // with the entry.  It can be used to compute the mean rate for the
  // entry.

  void orig_tuple_period_sum (RtecScheduler::Period_t p);
  // Mutator for the sum of periods for tuples directly associated
  // with the entry.  It can be used to compute the mean rate for the
  // entry.

  RtecScheduler::Period_t prop_tuple_period_sum ();
  // Accessor for the sum of periods for tuples propagated via
  // dependencies on other entries.  It can be used to compute the
  // mean rate for the entry.

  void prop_tuple_period_sum (RtecScheduler::Period_t p);
  // Mutator for the sum of periods for tuples propagated via
  // dependencies on other entries.  It can be used to compute the
  // mean rate for the entry.

  u_int orig_tuple_count ();
  //  Accessor for the number of tuples directly associated with the
  //  entry.

  void orig_tuple_count (u_int c);
  //  Mutator for the number of tuples directly associated with the
  //  entry.

  u_int prop_tuple_count ();
  // Accessor for the number of tuples propagated via dependencies on
  // other entries.

  void prop_tuple_count (u_int c);
  // Mutator for the number of tuples propagated via dependencies on
  // other entries.

  TUPLE_SET& orig_tuple_subset ();
  // Accessor for the set of tuples directly associated with the
  // entry.

  TUPLE_SET& prop_tuple_subset ();
  // Accessor for the set of tuples propagated via dependencies on
  // other entries.

  TAO_RT_Info_Tuple * current_admitted_tuple ();
  // Returns a pointer to the entry's most recently admitted tuple.
  // The pointer is zero if no tuples have been admitted so far.

  void current_admitted_tuple (TAO_RT_Info_Tuple *);
  // Sets a pointer to the entry's most recently admitted tuple.
  // The pointer is zero if no tuples have been admitted so far.

  RtecScheduler::RT_Info_Enabled_Type_t enabled_state () const;
  // Accessor for flag indicating whether or not node is enabled.

  void enabled_state (RtecScheduler::RT_Info_Enabled_Type_t);
  // Mutator for flag indicating whether or not node is enabled.

/* WSOA merge - commented out
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
*/

private:

  TAO_RT_Info_Ex orig_rt_info_data_;
  // Stores the values of operation characteristics as they were specified
  // in the most recent call to the Reconfig_Scheduler's set () method.
  // That way, the scheduler propagation pass can overwrite RT_Info fields
  // without losing the original values.  This is useful when

  TAO_RT_Info_Ex *actual_rt_info_;
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

  RtecScheduler::Time aggregate_exec_time_;
  // Effective execution time for corresponding RT_Info and its
  // disjunctively executed dependants.

  RtecScheduler::Period_t orig_tuple_period_sum_;
  // Sum of periods for tuples directly associated with the entry.  It
  // can be used to compute the mean rate for the entry.

  RtecScheduler::Period_t prop_tuple_period_sum_;
  // The sum of periods for tuples propagated via dependencies on
  // other entries.  It can be used to compute the mean rate for the
  // entry.

  u_int orig_tuple_count_;
  // The number of tuples directly associated with the entry.

  u_int prop_tuple_count_;
  // The number of tuples propagated via dependencies on other
  // entries.

  TUPLE_SET orig_tuple_subset_;
  // The set of tuples directly associated with the entry.

  TUPLE_SET prop_tuple_subset_;
  // The set of tuples propagated via dependencies on other entries.

  TAO_RT_Info_Tuple * current_admitted_tuple_;
  // A pointer to the entry's most recently admitted tuple.

  RtecScheduler::RT_Info_Enabled_Type_t enabled_;
  // Flag indicating whether or not node is enabled.

  /* - WSOA merge - commented out
  CORBA::Long effective_exec_multiplier_;
  // Effective execution time multiplier for corresponding RT_Info.

  RtecScheduler::Period_t effective_period_;
  // Effective period of corresponding RT_Info.
  */
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

  virtual ~TAO_RSE_Reset_Visitor () {}
  // Destructor.

  virtual int visit (TAO_Reconfig_Scheduler_Entry &rse);
  // Resets the fields in the entry to pre-DFS traversal states.
  // Returns 0 on success and -1 on error.
};

class TAO_RTSched_Export TAO_RT_Info_Tuple_Visitor
  // = TITLE
  //   An abstract base class for RT_Info tuple visitors.
  //
  // = DESCRIPTION
  //   This class simplifies the reconfig scheduler implementation
  //   by giving a common interface for distinct visitors over the
  //   RT_Info tuples.
{
public:

  virtual int visit (TAO_RT_Info_Tuple &) = 0;
  // Visit a RT_Info tuple.

};


class TAO_RTSched_Export TAO_Reconfig_Sched_Strategy_Base
  // = TITLE 
  //   A base class for scheduling strategies
  //
  // = DESCRIPTION This class provides a DFS finish time comparison
  // function, a static subpriority comparison function, and a
  // criticality evaluation function for all scheduling strategies.
{
public:

  static int comp_entry_finish_times (const void *first, const void *second);
  // Ordering function to compare the DFS finish times of
  // two task entries, so qsort orders these in topological
  // order, with the higher times *first*.

  static int is_critical (TAO_Reconfig_Scheduler_Entry &rse);
  // Determines whether or not an entry is critical, based on
  // operation characteristics.  returns 1 if critical, 0 if not

  static int is_critical (TAO_RT_Info_Tuple &t);
  // Determines whether or not a tuple is critical, based on operation
  // characteristics.  returns 1 if critical, 0 if not

  static int compare_subpriority (TAO_Reconfig_Scheduler_Entry &,
                                  TAO_Reconfig_Scheduler_Entry &);
  // Compares two entries by subpriority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.
};

class TAO_RTSched_Export TAO_MUF_FAIR_Reconfig_Sched_Strategy
  : public TAO_Reconfig_Sched_Strategy_Base
  // = TITLE 
  //   A scheduling strategy that implements the Maximum Urgency First
  //   scheduling algorithm with Fair Admission of Indexed Rates
  //   (FAIR).
  //
  // = DESCRIPTION
  //   The strategy assigns static thread and global priority according
  //   to operation criticality, assigns static subpriority according to
  //   importance and then topological order, and assigns a dispatching
  //   configuration with a minimum laxity dispatching queue for each
  //   distinct criticality level.  It admits operation tuples in order
  //   of ascending rate index, where the lowest rate for an operation
  //   has index 0, the next higher rate has index 1, etc.
{
public:

  static int total_priority_comp (const void *, const void *);
  // Ordering function used to qsort an array of RT_Info_Tuple
  // pointers into a total <priority, subpriority> ordering.  Returns
  // -1 if the first one is higher, 0 if they're the same, and 1 if
  // the second one is higher.

  static int total_admission_comp (const void *, const void *);
  // Ordering function used to qsort an array of RT_Info_Tuple
  // pointers into a total ordering for admission control.  Returns
  // -1 if the first one is higher, 0 if they're the same, and 1 if
  // the second one is higher.

  static int compare_priority (TAO_Reconfig_Scheduler_Entry &,
                               TAO_Reconfig_Scheduler_Entry &);
  // Compares two entries by priority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int compare_priority (TAO_RT_Info_Tuple &,
                               TAO_RT_Info_Tuple &);
  // Compares two tuples by priority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int compare_admission_order (TAO_RT_Info_Tuple &,
                                      TAO_RT_Info_Tuple &);
  // Compares two tuples by the given admission ordering.  Returns -1 if the
  // first one is earlier, 0 if they're the same, and 1 if the second one is earlier.

  static int assign_config (RtecScheduler::Config_Info &,
                            TAO_Reconfig_Scheduler_Entry &);
  // Fills in a static dispatch configuration for a priority level, based
  // on the operation characteristics of a representative scheduling entry.
};

typedef TAO_MUF_FAIR_Reconfig_Sched_Strategy TAO_MUF_Reconfig_Sched_Strategy;

class TAO_RTSched_Export TAO_RMS_Dyn_MNO_Reconfig_Sched_Strategy
  : public TAO_Reconfig_Sched_Strategy_Base
  // = TITLE
  //   A scheduling strategy that implements the Boeing RMS-Dynamic
  //   scheduling algorithm, and the Honeywell MNO admission control
  //   algorithm.
  //
  // = DESCRIPTION
  //   The strategy assigns static thread and global priority
  //   according to criticality and rate, assigns static subpriority
  //   according to importance and then topological order, and assigns
  //   a dispatching configuration with a static dispatching queue for
  //   each high criticality rate, and a single minimum laxity
  //   dispatching queue for all low criticality operations.  It
  //   admits the lowest rate-index tuple for each operation, then
  //   admits operations at their highest admissible rates in priority
  //   order.
{
public:

  static int total_priority_comp (const void *, const void *);
  // Ordering function used to qsort an array of RT_Info_Tuple
  // pointers into a total <priority, subpriority> ordering.  Returns
  // -1 if the first one is higher, 0 if they're the same, and 1 if
  // the second one is higher.

  static int total_admission_comp (const void *, const void *);
  // Ordering function used to qsort an array of RT_Info_Tuple
  // pointers into a total ordering for admission control.  Returns
  // -1 if the first one is higher, 0 if they're the same, and 1 if
  // the second one is higher.

  static int compare_criticality(TAO_Reconfig_Scheduler_Entry &lhs,
                                 TAO_Reconfig_Scheduler_Entry &rhs);
  // Compares two entries by criticality alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int compare_criticality(TAO_RT_Info_Tuple &lhs,
                                 TAO_RT_Info_Tuple &rhs);
  // Compares two entries by criticality alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int compare_priority (TAO_Reconfig_Scheduler_Entry &,
                               TAO_Reconfig_Scheduler_Entry &);
  // Compares two entries by priority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int compare_priority (TAO_RT_Info_Tuple &,
                               TAO_RT_Info_Tuple &);
  // Compares two tuples by priority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

  static int compare_admission_order (TAO_RT_Info_Tuple &,
                                      TAO_RT_Info_Tuple &);
  // Compares two entries by admission ordering policy.  Returns -1 if the
  // first one is earlier, 0 if they're the same, and 1 if the second one is earlier.

  static int compare_subpriority (TAO_Reconfig_Scheduler_Entry &,
                                  TAO_Reconfig_Scheduler_Entry &);
  // Compares two entries by subpriority alone.  Returns -1 if the
  // first one is higher, 0 if they're the same, and 1 if the second one is higher.

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

#include /**/ "ace/post.h"
#endif /* TAO_RECONFIG_SCHED_UTILS_H */
