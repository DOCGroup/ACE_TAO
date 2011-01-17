// -*- C++ -*-

//=============================================================================
/**
 *  @file    Reconfig_Scheduler_T.h
 *
 *  $Id$
 *
 *  @author  Chris Gill <cdgill@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_RECONFIG_SCHEDULER_T_H
#define TAO_RECONFIG_SCHEDULER_T_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#include "orbsvcs/Sched/Reconfig_Scheduler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_Reconfig_Scheduler :
  public POA_RtecScheduler::Scheduler
  // = TITLE
  //   A servant for RtecScheduler::Scheduler that can be initialized
  //   and run solely with precomputed scheduling information, but is also
  //   designed to perform efficient recomputation of scheduling info
  //   in the face of run-time changes to RT_Infos in the schedule.
  //
  // = DESCRIPTION
  //   This class implements the TAO scheduling service.  It has
  //   the ability to accept and use precomputed scheduling information
  //   with deterministic performance once the schedule is fully configured.
  //   It also provides efficient and flexible lookup and computation
  //   during schedule configuration, so that it can be used during
  //   both schedule configuration and run-time phases of operation.
  //
{
public:

  /// Type of map used for O(1) lookup of RT_Infos by their handles.
  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  TAO_RT_Info_Ex *,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> RT_INFO_MAP;

  /// Type of tree used for O(log n) lookup of RT_Infos by their names.
  typedef ACE_RB_Tree<const char *,
                      TAO_RT_Info_Ex*,
                      ACE_Less_Than<const char *>,
                      ACE_LOCK> RT_INFO_TREE;

  /// Type of map used for O(1) lookup of Config_Infos by their priorities.
  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::Preemption_Priority_t,
                                  RtecScheduler::Config_Info*,
                                  ACE_Hash<RtecScheduler::Preemption_Priority_t>,
                                  ACE_Equal_To<RtecScheduler::Preemption_Priority_t>,
                                  ACE_LOCK> CONFIG_INFO_MAP;

  /// Type of map used for O(1) lookup of RT_Info
  /// dependency sets by caller or called handle.
  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t,
                                  RtecScheduler::Dependency_Set*,
                                  ACE_Hash<RtecScheduler::handle_t>,
                                  ACE_Equal_To<RtecScheduler::handle_t>,
                                  ACE_LOCK> DEPENDENCY_SET_MAP;

  /// Type of iterator for traversal of RT_Info dependency sets by
  /// caller or called handle.
  typedef typename DEPENDENCY_SET_MAP::ITERATOR DEPENDENCY_SET_MAP_ITERATOR;

  /// Default constructor.
  TAO_Reconfig_Scheduler (int enforce_schedule_stability = 0,
                          const CORBA::Double & critical_utilization_threshold = 1.0,
                          const CORBA::Double & noncritical_utilization_threshold = 1.1);

  /// Constructor. Initialize the scheduler from POD_Config_Info, POD_RT_Info,
  /// and POD_Dependency arrays, plus schedule stability flags.
  TAO_Reconfig_Scheduler (int config_count,
                          ACE_Scheduler_Factory::POD_Config_Info config_info[],
                          int entry_count,
                          ACE_Scheduler_Factory::POD_RT_Info rt_info[],
                          int dependency_count,
                          ACE_Scheduler_Factory::POD_Dependency_Info dependency_info[],
                          u_long stability_flags,
                          int enforce_schedule_stability = 0,
                          const CORBA::Double & critical_utilization_threshold = 1.0,
                          const CORBA::Double & noncritical_utilization_threshold = 1.1);

  /// Destructor.
  ~TAO_Reconfig_Scheduler ();

  /// Initializes the scheduler with the passed information.
  int init (int config_count,
            ACE_Scheduler_Factory::POD_Config_Info config_info[],
            int rt_info_count,
            ACE_Scheduler_Factory::POD_RT_Info rt_info[],
            int dependency_count,
            ACE_Scheduler_Factory::POD_Dependency_Info dependency_info[],
            u_long stability_flags);

  /// Closes the scheduler, releasing all current resources.
  void close (void);

  /**
   * Create an RT_Info.  If it does not exist, a new RT_Info is
   * created and inserted into the schedule, and the handle of the new
   * RT_Info is returned.  If the RT_Info already exists, an exception
   * is thrown.
   */
  virtual RtecScheduler::handle_t create (const char * entry_point);

  /// Lookup a handle for an RT_Info, and return its handle, or an error
  /// value if it's not present.
  virtual RtecScheduler::handle_t lookup (const char * entry_point);

  /// Return a pointer to the RT_Info corresponding to the passed handle.
  virtual RtecScheduler::RT_Info* get (RtecScheduler::handle_t handle);

  /// Set characteristics of the RT_Info corresponding to the passed handle.
  virtual void set (::RtecScheduler::handle_t handle,
                    ::RtecScheduler::Criticality_t criticality,
                    ::RtecScheduler::Time time,
                    ::RtecScheduler::Time typical_time,
                    ::RtecScheduler::Time cached_time,
                    ::RtecScheduler::Period_t period,
                    ::RtecScheduler::Importance_t importance,
                    ::RtecScheduler::Quantum_t quantum,
                    ::RtecScheduler::Threads_t threads,
                    ::RtecScheduler::Info_Type_t info_type);

  /// Reset characteristics of the RT_Info corresponding to the passed handle.
  virtual void reset (RtecScheduler::handle_t handle,
                      RtecScheduler::Criticality_t criticality,
                      RtecScheduler::Time time,
                      RtecScheduler::Time typical_time,
                      RtecScheduler::Time cached_time,
                      RtecScheduler::Period_t period,
                      RtecScheduler::Importance_t importance,
                      RtecScheduler::Quantum_t quantum,
                      CORBA::Long threads,
                      RtecScheduler::Info_Type_t info_type);

  /// Set characteristics of the RT_Infos corresponding to the passed handles.
  /// Tuples are added in the case of existing and/or multiple definitions.
  virtual void set_seq (const RtecScheduler::RT_Info_Set& infos);

  /// Reset characteristics of the RT_Infos corresponding to the passed handles.
  /// Tuples are replaced in the case of existing and/or multiple definitions.
  virtual void reset_seq (const RtecScheduler::RT_Info_Set& infos);

  /**
   * Replace all RT_Infos, resetting characteristics of the RT_Infos
   * corresponding to the passed handles.  All other RT_Infos are
   * reset to their uninitialized values, i.e., the same they have
   * just after the create call.
   */
  virtual void replace_seq (const RtecScheduler::RT_Info_Set& infos);

  /// Returns the priority and subpriority values assigned to an RT_Info,
  /// based on its handle.
  virtual void priority (RtecScheduler::handle_t handle,
                         RtecScheduler::OS_Priority& o_priority,
                         RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                         RtecScheduler::Preemption_Priority_t& p_priority);

  /// Returns the priority and subpriority values assigned to an RT_Info,
  /// based on its entry point name.
  virtual void entry_point_priority (const char * entry_point,
                                     RtecScheduler::OS_Priority& o_priority,
                                     RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                                     RtecScheduler::Preemption_Priority_t& p_priority);

  /// This method registers a dependency between two RT_Infos.
  virtual void add_dependency (RtecScheduler::handle_t handle,
                               RtecScheduler::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type);

  /// This method removes a dependency between two RT_Infos.
  virtual void remove_dependency (RtecScheduler::handle_t handle,
                                  RtecScheduler::handle_t dependency,
                                  CORBA::Long number_of_calls,
                                  RtecScheduler::Dependency_Type_t dependency_type);

  /// This method sets the enable state of a dependency between two RT_Infos.
  virtual void set_dependency_enable_state (RtecScheduler::handle_t handle,
                                            RtecScheduler::handle_t dependency,
                                            CORBA::Long number_of_calls,
                                            RtecScheduler::Dependency_Type_t dependency_type,
                                            RtecScheduler::Dependency_Enabled_Type_t enabled);

  /// This method sets the enable state of a sequence of dependencies.
  virtual void set_dependency_enable_state_seq (const RtecScheduler::Dependency_Set & dependencies);

  /// This method enables or disables an RT_Info.
  virtual void set_rt_info_enable_state (RtecScheduler::handle_t handle,
                                         RtecScheduler::RT_Info_Enabled_Type_t enabled);

  /// This method enables or disables a sequence of RT_Infos.
  virtual void set_rt_info_enable_state_seq (const RtecScheduler::RT_Info_Enable_State_Pair_Set & pair_set);

  /**
   * If information has been added or changed since the last stable
   * schedule was computed, this method causes scheduling information
   * to be computed for all registered RT_Infos.  If the schedule is
   * already stable, this is a no-op.
   */
  virtual void compute_scheduling (CORBA::Long minimum_priority,
                                   CORBA::Long maximum_priority,
                                   RtecScheduler::RT_Info_Set_out infos,
                                   RtecScheduler::Dependency_Set_out dependencies,
                                   RtecScheduler::Config_Info_Set_out configs,
                                   RtecScheduler::Scheduling_Anomaly_Set_out anomalies);

  /// Recomputes the scheduling priorities, etc.
  virtual void recompute_scheduling (CORBA::Long minimum_priority,
                                     CORBA::Long maximum_priority,
                                     RtecScheduler::Scheduling_Anomaly_Set_out anomalies);

  /// Returns the set of rt_infos, with their assigned priorities (as
  /// of the last schedule re-computation).
  virtual void get_rt_info_set (RtecScheduler::RT_Info_Set_out infos);

  /// Returns the set of rt_infos, with their assigned priorities (as
  /// of the last schedule re-computation).
  virtual void get_dependency_set (RtecScheduler::Dependency_Set_out dependencies);

  /// Returns the set of config_infos, describing the appropriate
  /// number, types, and priority levels for the dispatching lanes.
  virtual void get_config_info_set (RtecScheduler::Config_Info_Set_out configs);


  /// Provides the thread priority and queue type for the given priority level.
  virtual void dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                       RtecScheduler::OS_Priority& o_priority,
                                       RtecScheduler::Dispatching_Type_t & d_type);

  /**
   * Returns the last priority number assigned to an operation in the
   * schedule.  The number returned is one less than the total number
   * of scheduled priorities.  All scheduled priorities range from 0
   * to the number returned, inclusive.
   */
  virtual RtecScheduler::Preemption_Priority_t last_scheduled_priority (void);

  /// Provides the set of Config_Infos associated with the current schedule.
  virtual void get_config_infos (RtecScheduler::Config_Info_Set_out configs);

  // = Accessors that allow controlled relaxations of encapsulation.

  /// Accesses scheduling strategy for the reconfig scheduler.
  RECONFIG_SCHED_STRATEGY & sched_strategy ();

  /// Accesses map for O(1) lookup of Config_Infos by priority level.
  CONFIG_INFO_MAP & config_info_map ();

  /// Returns the number of config infos, which is also the number of
  /// assigned priority levels.
  long config_info_count ();

  /// Accesses map for O(1) lookup of RT_Infos by handle.
  RT_INFO_MAP & rt_info_map ();

  /// Returns the number of registered RT_Infos.
  long rt_info_count ();

  /// Returns the number of registered RT_Info tuples.
  long rt_info_tuple_count ();

  /// Accesses tree for O(log n) lookup of RT_Infos by name.
  RT_INFO_TREE & rt_info_tree ();

  /// Accesses map for O(1) lookup of RT_Info dependency
  /// set by the caller operation's handle.
  DEPENDENCY_SET_MAP & calling_dependency_set_map ();

  /// Accesses map for O(1) lookup of RT_Info dependency
  /// set by the called operation's handle.
  DEPENDENCY_SET_MAP & called_dependency_set_map ();

  /// Returns the number of dependencies in the dependency lists of all RT_Infos.
  /// This is used when traversing the dependency graph.
  int dependency_count ();

  /// Accessor for utilization by noncritical tasks.
  CORBA::Double noncritical_utilization ();

  /// Accessor for utilization by critical tasks.
  CORBA::Double critical_utilization ();

  /// Accessor for noncritical task utilization threshold.
  CORBA::Double noncritical_utilization_threshold ();

  /// Mutator for noncritical task utilization threshold.
  void noncritical_utilization_threshold (const CORBA::Double &);

  /// Accessor for critical task utilization threshold.
  CORBA::Double critical_utilization_threshold ();

  /// Mutator for critical task utilization threshold.
  void critical_utilization_threshold (const CORBA::Double &);

protected:

  // @@ TO DO: use a memento to save and restore scheduler state without
  //           breaking encapsulation, particularly of these flags.

  // @@ TO DO: Recheck the applicability and fine-grain management of
  //           these flags.  Do these still correctly reflect the phases
  //           of the computation?

  enum Stability_Flags
  {
    // This should always be zero.
    SCHED_ALL_STABLE = 0x00UL,

    // Individual stability flags, each of
    // which should have a distinct bit value.

    // Utilization may need to be recomputed.
    SCHED_UTILIZATION_NOT_STABLE = 0x01UL,

    // Priorities may need to be recomputed.
    SCHED_PRIORITY_NOT_STABLE = 0x02UL,

    // Characteristics may need to be repropagated.
    SCHED_PROPAGATION_NOT_STABLE = 0x04UL,

    // This should be the disjunction of
    // all the individual stability flags.
    SCHED_NONE_STABLE =
      SCHED_UTILIZATION_NOT_STABLE |
      SCHED_PRIORITY_NOT_STABLE |
      SCHED_PROPAGATION_NOT_STABLE
  };
  // Flags indicating stability conditions of schedule.

  /**
   * Internal method to create an RT_Info.  If it does not exist, a
   * new RT_Info is created and inserted into the schedule, and the
   * handle of the new RT_Info is returned.  If the RT_Info already
   * exists, then if the ignore_duplicates flag is set, the handle
   * is simply returned; otherwise, an exception is thrown.
   */
  TAO_RT_Info_Ex * create_i (const char * entry_point,
                             RtecScheduler::handle_t handle,
                             int ignore_duplicates);

  /// Internal method to set characteristics of the passed RT_Info.
  void set_i (TAO_RT_Info_Ex *rt_info,
                      RtecScheduler::Criticality_t criticality,
                      RtecScheduler::Time time,
                      RtecScheduler::Time typical_time,
                      RtecScheduler::Time cached_time,
                      RtecScheduler::Period_t period,
                      RtecScheduler::Importance_t importance,
                      RtecScheduler::Quantum_t quantum,
                      CORBA::Long threads,
              RtecScheduler::Info_Type_t info_type);

  /// Internal method to look up a handle for an RT_Info, and return
  /// its handle, or an exception if it's not present.
  virtual RtecScheduler::handle_t lookup_i (const char * entry_point);

  /// Internal method to return the priority and subpriority
  /// values assigned to an RT_Info, based on its handle.
  virtual void priority_i (RtecScheduler::handle_t handle,
                           RtecScheduler::OS_Priority& o_priority,
                           RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                           RtecScheduler::Preemption_Priority_t& p_priority);

  /**
   * Internal method that registers a dependency between two RT_Infos.
   * Assumes it is being called with all locks held, and does *not*
   * set any schedule stability flags.
   */
  virtual void add_dependency_i (RtecScheduler::handle_t handle,
                                 RtecScheduler::handle_t dependency,
                                 CORBA::Long number_of_calls,
                         RtecScheduler::Dependency_Type_t dependency_type,
                         RtecScheduler::Dependency_Enabled_Type_t enabled);

  /**
   * Internal method that removes a dependency between two RT_Infos.
   * Assumes it is being called with all locks held, and does *not*
   * set any schedule stability flags.
   */
  virtual void remove_dependency_i (RtecScheduler::handle_t handle,
                                    RtecScheduler::handle_t dependency,
                                    CORBA::Long number_of_calls,
                                    RtecScheduler::Dependency_Type_t dependency_type);

  /**
   * Internal method that enables or disables a dependency between two RT_Infos.
   * Assumes it is being called with all locks held, and does *not*
   * set any schedule stability flags.
   */
  virtual void set_dependency_enable_state_i (RtecScheduler::handle_t handle,
                                              RtecScheduler::handle_t dependency,
                                              CORBA::Long number_of_calls,
                                              RtecScheduler::Dependency_Type_t dependency_type,
                                              RtecScheduler::Dependency_Enabled_Type_t enabled);


  /// This method installs a dependency in a dependency set map.
  virtual void map_dependency_i
                 (RtecScheduler::handle_t key,
                  RtecScheduler::handle_t handle,
                         typename TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &dependency_map,
                  CORBA::Long number_of_calls,
                         RtecScheduler::Dependency_Type_t dependency_type,
                         RtecScheduler::Dependency_Enabled_Type_t enabled);

  /// This method removes a dependency from a dependency set map.
  void unmap_dependency_i (RtecScheduler::handle_t key,
                           RtecScheduler::handle_t handle,
                           typename TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &dependency_map,
                           CORBA::Long number_of_calls,
                           RtecScheduler::Dependency_Type_t dependency_type);

  /// This method updates the enable state of a dependency in a dependency set map.
  void map_dependency_enable_state_i (RtecScheduler::handle_t key,
                                      RtecScheduler::handle_t handle,
                                      typename
                                        TAO_Reconfig_Scheduler<RECONFIG_SCHED_STRATEGY, ACE_LOCK>::DEPENDENCY_SET_MAP &dependency_map,
                                      CORBA::Long number_of_calls,
                                      RtecScheduler::Dependency_Type_t dependency_type,
                                      RtecScheduler::Dependency_Enabled_Type_t enabled);

  /// Traverses dependency graph, assigning a topological ordering.
  /// Resets DFS map entries, do DFS traversal, constructs DFS map.
  virtual void dfs_traverse_i (void);

  /// Sorts an array of RT_info handles in topological order, then
  /// checks for loops, marks unresolved remote dependencies.
  virtual void detect_cycles_i (void);

  /// Compute aggregate execution times, then performs admission over
  /// rate tuples.
  void perform_admission_i (void);


  /**
   * Traverses criticality dependency graph, assigning a topological
   * ordering.  Resets DFS map entries, do DFS traversal, constructs
   * DFS map.
   */
  void crit_dfs_traverse_i (void);


  /// Propagates criticalities.
  void propagate_criticalities_i (void);


  /// Propagates effective execution time and period, sets total frame size.
  void propagate_characteristics_i (void);

  /**
   * Sort operations by urgency (done by strategy), then assign
   * priorities and subpriorities in one pass.  (Re)computes utilization
   * and sets last scheduled priority and last feasible priority.
   */
  virtual void assign_priorities_i (void);

  /// Refreshes the array of tuple pointers, corrects the count.
  void refresh_tuple_ptr_array_i (void);

  /// Compute utilization, set last feasible priority.
/* WSOA merge - commented out
  virtual void compute_utilization_i (void);


  /// Helper method to give an RT_Info some reasonable default values
  /// = Protected class members.
  static void init_rt_info (RtecScheduler::RT_Info &rt_info);
*/

  /// Scheduling strategy for the reconfig scheduler.
  RECONFIG_SCHED_STRATEGY sched_strategy_;

  /// Map for O(1) lookup of Config_Infos by priority level.
  CONFIG_INFO_MAP config_info_map_;

  /// The number of config infos, which is also the number of priority
  /// levels.
  long config_info_count_;

  /// Map for O(1) lookup of RT_Infos by handle.
  RT_INFO_MAP rt_info_map_;

  /// The number of registered RT_Infos.
  long rt_info_count_;

  /// The number of registered RT_Info tuples.
  long rt_info_tuple_count_;

  /// Tree for O(log n) lookup of RT_Infos by name.
  RT_INFO_TREE rt_info_tree_;

  /// Map for O(1) lookup of RT_Info dependency
  /// set by the caller operation's handle.
  DEPENDENCY_SET_MAP calling_dependency_set_map_;

  /// Map for O(1) lookup of RT_Info dependency
  /// set by the called operation's handle.
  DEPENDENCY_SET_MAP called_dependency_set_map_;

  /**
   * Map for O(1) lookup of RT_Info dependency set. The above two maps
   * store the calling and called dependencies. This map stores the
   * true dependencies based on criticality propagation direction. For
   * two-ways, this is the same as the direction of invocation,
   * whereas for one-ways, it is in the opposite direction.
   */
  DEPENDENCY_SET_MAP crit_dependency_set_map_;

  /// Next RT_Info descriptor handle to allocate.  The first handle is
  /// always 1.
  RtecScheduler::handle_t next_handle_;

  /**
   * Array of pointers to scheduling entries.  This
   * array is maintained by the methods that create
   * scheduling entries, and sorted in topological
   * order and then priority order at various points
   * during schedule computation.
   */
  TAO_Reconfig_Scheduler_Entry ** entry_ptr_array_;

  /// Size of the array of scheduling entry pointers.
  long entry_ptr_array_size_;

  /**
   * Array of pointers to scheduling entries.  This
   * array is maintained by the methods that create
   * scheduling entries, and sorted in topological
   * order and then priority order at various points
   * during schedule computation.
   */
  TAO_RT_Info_Tuple ** tuple_ptr_array_;

  /// Size of the array of scheduling entry pointers.
  long tuple_ptr_array_size_;

  /**
   * Flags indicating whether a stable schedule has been computed
   * since the last addition or modification of information, and which
   * parts of the schedule are unstable.
   */
  u_long stability_flags_;

  /**
   * Indicates whether the scheduler should enforce stability by
   * throwing an exception: otherwise for operations where this
   * matters, it will simply return a default value if nothing
   * has been generated.
   */
  int enforce_schedule_stability_;

  /// The number of dependencies in the dependency lists of all RT_Infos.
  /// This is used when traversing the dependency graph.
  int dependency_count_;

  /// Stores the last priority for which an operation can be scheduled
  RtecScheduler::Preemption_Priority_t last_scheduled_priority_;

  /// Utilization by noncritical tasks.
  CORBA::Double noncritical_utilization_;

  /// Utilization by critical tasks.
  CORBA::Double critical_utilization_;

  /// Utilization by noncritical tasks.
  CORBA::Double noncritical_utilization_threshold_;

  /// Utilization by critical tasks.
  CORBA::Double critical_utilization_threshold_;

  /// Mutual exclusion lock for the scheduler itself.  This is needed to
  /// synchronize updates and accesses to scheduling information.
  ACE_LOCK mutex_;
};


template <class ARRAY_ELEMENT_TYPE> void
maintain_scheduling_array (ARRAY_ELEMENT_TYPE ** & current_ptr_array,
                           long & current_ptr_array_size,
                           RtecScheduler::handle_t handle);
// Helper function: makes sure there is room in the scheduling pointer
// arrays.  This function expands the array eagerly, to minimize time
// overhead for memory allocation (at a cost of some unused space).

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Sched/Reconfig_Scheduler_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Reconfig_Scheduler_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"
#endif /* TAO_RECONFIG_SCHEDULER_T_H */
