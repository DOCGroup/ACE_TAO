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

#ifndef ACE_RECONFIG_SCHEDULER_H
#define ACE_RECONFIG_SCHEDULER_H

#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"

template <class RECONFIG_SCHED_STRATEGY, class ACE_LOCK>
class TAO_ORBSVCS_Export ACE_Reconfig_Scheduler : 
  public POA_RtecScheduler::Scheduler
{
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
public:

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

    // Dispatching configuration may need to be recomputed.
    SCHED_CONFIG_NOT_STABLE = 0x04UL,

    // This should be the disjunction of
    // all the individual stability flags.
    SCHED_NONE_STABLE = 
      SCHED_UTILIZATION_NOT_STABLE |
      SCHED_PRIORITY_NOT_STABLE |
      SCHED_CONFIG_NOT_STABLE
  };
  // Flags indicating stability conditions of schedule.

  ACE_Reconfig_Scheduler ();
  // Default constructor.

  ACE_Reconfig_Scheduler (int config_count,
                          ACE_Scheduler_Factory::POD_Config_Info config_info[],
                          int entry_count,
                          ACE_Scheduler_Factory::POD_RT_Info rt_info[],
                          int dependency_count,
                          ACE_Scheduler_Factory::POD_Dependency_Info dependency_info[],
                          u_long stability_flags = SCHED_NONE_STABLE);
  // Constructor. Initialize the scheduler from POD_Config_Info, POD_RT_Info, 
  // and POD_Dependency arrays, plus schedule stability flags.

  int init (int config_count,
            ACE_Scheduler_Factory::POD_Config_Info config_info[],
            int entry_count,
            ACE_Scheduler_Factory::POD_RT_Info rt_info[],
            int dependency_count,
            ACE_Scheduler_Factory::POD_Dependency_Info dependency_info[],
            u_long stability_flags,
            CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::DUPLICATE_NAME,
                     RtecScheduler::UNKNOWN_TASK));
  // Initializes the scheduler with the passed information.

  void close (CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::UNKNOWN_TASK));
  // Closes the scheduler, releasing all current resources.

  virtual RtecScheduler::handle_t create (const char * entry_point,
                                          CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::DUPLICATE_NAME));
  // Create an RT_Info.  If it does not exist, a new RT_Info is
  // created and inserted into the schedule, and the handle of the new
  // RT_Info is returned.  If the RT_Info already exists, an exception
  // is thrown.

  virtual RtecScheduler::handle_t lookup (const char * entry_point,
                                          CORBA::Environment &_env)
    TAO_THROW_SPEC((CORBA::SystemException));
  // Lookup a handle for an RT_Info, and return its handle, or an error
  // value if it's not present.

  virtual RtecScheduler::RT_Info* get (RtecScheduler::handle_t handle,
                                       CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK));
  // Return a pointer to the RT_Info corresponding to the passed handle.

  virtual void set (RtecScheduler::handle_t handle,
                    RtecScheduler::Criticality_t criticality,
                    RtecScheduler::Time time,
                    RtecScheduler::Time typical_time,
                    RtecScheduler::Time cached_time,
                    RtecScheduler::Period_t period,
                    RtecScheduler::Importance_t importance,
                    RtecScheduler::Quantum_t quantum,
                    CORBA::Long threads,
                    RtecScheduler::Info_Type_t info_type,
                    CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK));
  // Set characteristics of the RT_Info corresponding to the passed handle.

  virtual void priority (RtecScheduler::handle_t handle,
                         RtecScheduler::OS_Priority& o_priority,
                         RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                         RtecScheduler::Preemption_Priority_t& p_priority,
                         CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED));
  // Returns the priority and subpriority values assigned to an RT_Info,
  // based on its handle.

  virtual void entry_point_priority (const char * entry_point,
                                     RtecScheduler::OS_Priority& o_priority,
                                     RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                                     RtecScheduler::Preemption_Priority_t& p_priority,
                                     CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED));
  // Returns the priority and subpriority values assigned to an RT_Info,
  // based on its entry point name.

  virtual void add_dependency (RtecScheduler::handle_t handle,
                               RtecScheduler::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type,
                               CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK));
  // This method registers a dependency between two RT_Infos.

  virtual void compute_scheduling (CORBA::Long minimum_priority,
                                   CORBA::Long maximum_priority,
                                   RtecScheduler::RT_Info_Set_out infos,
                                   RtecScheduler::Config_Info_Set_out configs,
                                   RtecScheduler::Scheduling_Anomaly_Set_out anomalies,
                                   CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                     RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                     RtecScheduler::TASK_COUNT_MISMATCH));
  // If information has been added or changed since the last stable
  // schedule was computed, this method causes scheduling information
  // to be computed for all registered RT_Infos.  If the schedule is
  // already stable, this is a no-op

  virtual void dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                       RtecScheduler::OS_Priority& o_priority,
                                       RtecScheduler::Dispatching_Type_t & d_type,
                                       CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED,
                    RtecScheduler::UNKNOWN_PRIORITY_LEVEL));
  // Provides the thread priority and queue type for the given priority level.

  virtual RtecScheduler::Preemption_Priority_t last_scheduled_priority (CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED));
  // Returns the last priority number assigned to an operation in the
  // schedule.  The number returned is one less than the total number
  // of scheduled priorities.  All scheduled priorities range from 0
  // to the number returned, inclusive.

protected:

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::handle_t, 
                                  RtecScheduler::RT_Info*, 
                                  ACE_Hash<RtecScheduler::handle_t>, 
                                  ACE_Equal_To<RtecScheduler::handle_t>, 
                                  ACE_LOCK> RT_INFO_MAP;
  // Type of map used for O(1) lookup of RT_Infos by their handles.

  typedef ACE_RB_Tree<char *,
                      RtecScheduler::RT_Info*,
                      ACE_Less_Than<char *>, 
                      ACE_LOCK> RT_INFO_TREE;
  // Type of tree used for O(log n) lookup of RT_Infos by their names.

  typedef ACE_Hash_Map_Manager_Ex<RtecScheduler::Preemption_Priority_t,
                                  RtecScheduler::Config_Info*,
                                  ACE_Hash<RtecScheduler::Preemption_Priority_t>,
                                  ACE_Equal_To<RtecScheduler::Preemption_Priority_t>,
                                  ACE_LOCK> CONFIG_INFO_MAP;
  // Type of map used for O(1) lookup of Config_Infos by their priorities.

  virtual RtecScheduler::RT_Info * create_i (const char * entry_point,
                                             RtecScheduler::handle_t handle,
                                             CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::DUPLICATE_NAME));
  // Internal method to create an RT_Info.  If it does not exist, 
  // a new RT_Info is created and inserted into the schedule, 
  // and the handle of the new RT_Info is returned.
  // If the RT_Info already exists, an exception is thrown.

  virtual void set_i (RtecScheduler::RT_Info *rt_info,
                      RtecScheduler::Criticality_t criticality,
                      RtecScheduler::Time time,
                      RtecScheduler::Time typical_time,
                      RtecScheduler::Time cached_time,
                      RtecScheduler::Period_t period,
                      RtecScheduler::Importance_t importance,
                      RtecScheduler::Quantum_t quantum,
                      CORBA::Long threads,
                      RtecScheduler::Info_Type_t info_type)
  // Internal method to set characteristics of the passed RT_Info.

  virtual RtecScheduler::handle_t lookup_i (const char * entry_point,
                                            CORBA::Environment &_env)
    TAO_THROW_SPEC((CORBA::SystemException));
  // Internal method to look up a handle for an RT_Info, and return 
  // its handle, or an exception if it's not present.

  virtual void priority_i (RtecScheduler::handle_t handle,
                           RtecScheduler::OS_Priority& o_priority,
                           RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                           RtecScheduler::Preemption_Priority_t& p_priority,
                           CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED));
  // Internal method to return the priority and subpriority
  // values assigned to an RT_Info, based on its handle.

  virtual void add_dependency_i (RtecScheduler::handle_t handle,
                                 RtecScheduler::handle_t dependency,
                                 CORBA::Long number_of_calls,
                                 RtecScheduler::Dependency_Type_t dependency_type,
                                 CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK));
  // Internal method that registers a dependency between two RT_Infos.
  // Assumes it is being called with all locks held, and does *not*
  // set any schedule stability flags.

  RECONFIG_SCHED_STRATEGY sched_strategy_;
  // Scheduling strategy for the reconfig scheduler.

  CONFIG_INFO_MAP config_info_map_;
  // Map for O(1) lookup of Config_Infos by priority level.

  RT_INFO_MAP rt_info_map_;
  // Map for O(1) lookup of RT_Infos by handle.

  RT_INFO_TREE rt_info_tree_;
  // Map for O(1) lookup of RT_Infos by handle.

  RtecScheduler::handle_t next_handle_;
  // Next RT_Info descriptor handle to allocate.

  u_long stability_flags_;
  // Flags indicating whether a stable schedule has been computed
  // since the last addition or modification of information, and which
  // parts of the schedule are unstable.

  int dependency_count_;
  // The number of dependencies in the dependency lists of all RT_Infos.
  // This is used when traversing the dependency graph.

  RtecScheduler::Preemption_Priority_t last_scheduled_priority_;
  // Stores the last priority for which an operation can be scheduled

  ACE_LOCK mutex_;
  // Mutual exclusion lock for the scheduler itself.  This is needed to
  // synchronize updates and accesses to scheduling information.
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Reconfig_Scheduler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_RECONFIG_SCHEDULER_H */
