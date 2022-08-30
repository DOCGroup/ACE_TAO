// -*- C++ -*-

//=============================================================================
/**
 *  @file    Runtime_Scheduler.h
 *
 *  @author  Chris Gill <cdgill@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_RUNTIME_SCHEDULER_H
#define ACE_RUNTIME_SCHEDULER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "orbsvcs/Sched/sched_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Runtime_Scheduler
 *
 * @brief A fast and simple servant for RtecScheduler::Scheduler based on
 * precomputed schedules.
 *
 * This class offers the services of the scheduler, but based on
 * precomputed and precompiled information. This results in a
 * highly optimized execution after the configuration runs.
 */
class TAO_RTSched_Export ACE_Runtime_Scheduler : public POA_RtecScheduler::Scheduler
{
public:
  /// Constructor. Initialize the data from the POD_RT_Info array.
  ACE_Runtime_Scheduler (int config_count,
                         ACE_Scheduler_Factory::POD_Config_Info config_info[],
                         int entry_count,
                         ACE_Scheduler_Factory::POD_RT_Info rt_info[]);

  /**
   * Create an RT_Info.  In the config run scheduler this actually constructs
   * a new RT_Info.  Here, we just return its handle, or an error value if
   * it's not present.
   */
  RtecScheduler::handle_t create (const char * entry_point) override;

  /// Lookup a handle for an RT_Info, and return its handle, or an error
  /// value if it's not present.
  RtecScheduler::handle_t lookup (const char * entry_point) override;

  /// Return a pointer to the RT_Info corresponding to the passed handle.
  RtecScheduler::RT_Info* get (RtecScheduler::handle_t handle) override;

  /// Set characteristics of the RT_Info corresponding to the passed handle.
  void set (::RtecScheduler::handle_t handle,
                    ::RtecScheduler::Criticality_t criticality,
                    ::RtecScheduler::Time time,
                    ::RtecScheduler::Time typical_time,
                    ::RtecScheduler::Time cached_time,
                    ::RtecScheduler::Period_t period,
                    ::RtecScheduler::Importance_t importance,
                    ::RtecScheduler::Quantum_t quantum,
                    ::RtecScheduler::Threads_t threads,
                    ::RtecScheduler::Info_Type_t info_type) override;

  /// Reset characteristics of the RT_Info corresponding to the passed handle.
  void reset (RtecScheduler::handle_t handle,
                      RtecScheduler::Criticality_t criticality,
                      RtecScheduler::Time time,
                      RtecScheduler::Time typical_time,
                      RtecScheduler::Time cached_time,
                      RtecScheduler::Period_t period,
                      RtecScheduler::Importance_t importance,
                      RtecScheduler::Quantum_t quantum,
                      RtecScheduler::Threads_t threads,
                      RtecScheduler::Info_Type_t info_type) override;

  /// Set characteristics of the RT_Infos corresponding to the passed handles.
  /// Tuples are added in the case of existing and/or multiple definitions.
  void set_seq (const RtecScheduler::RT_Info_Set& infos) override;

  /// Replace characteristics of the RT_Infos corresponding to the
  /// passed handles.
  void replace_seq (const RtecScheduler::RT_Info_Set& infos) override;

  /// Reset characteristics of the RT_Infos corresponding to the passed handles.
  /// Tuples are replaced in the case of existing and/or multiple definitions.
  void reset_seq (const RtecScheduler::RT_Info_Set& infos) override;

  /// Returns the priority and subpriority values assigned to an RT_Info,
  /// based on its handle.
  void priority (RtecScheduler::handle_t handle,
                         RtecScheduler::OS_Priority& o_priority,
                         RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                         RtecScheduler::Preemption_Priority_t& p_priority) override;

  /// Returns the priority and subpriority values assigned to an RT_Info,
  /// based on its entry point name.
  void entry_point_priority (const char * entry_point,
                                     RtecScheduler::OS_Priority& o_priority,
                                     RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                                     RtecScheduler::Preemption_Priority_t& p_priority) override;

  /// In the config run scheduler, this method registers a dependency between
  /// two RT_Infos.  In the run time scheduler, this is a no-op.
  void add_dependency (RtecScheduler::handle_t handle,
                               RtecScheduler::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type) override;

  /// In the reconfig scheduler, this method removes a dependency between
  /// two RT_Infos.  In the run time scheduler, this is a no-op.
  void remove_dependency (RtecScheduler::handle_t handle,
                                  RtecScheduler::handle_t dependency,
                                  CORBA::Long number_of_calls,
                                  RtecScheduler::Dependency_Type_t dependency_type) override;

  /// In the reconfig scheduler, this method (re)enabes a dependency between
  /// two RT_Infos.  In the run time scheduler, this is a no-op.
  void set_dependency_enable_state (RtecScheduler::handle_t handle,
                                            RtecScheduler::handle_t dependency,
                                            CORBA::Long number_of_calls,
                                            RtecScheduler::Dependency_Type_t dependency_type,
                                            RtecScheduler::Dependency_Enabled_Type_t enabled) override;

  /// In the reconfig scheduler, enables or disables an RT_Info.
  /// In the run time scheduler, this is a no-op.
  void set_rt_info_enable_state (RtecScheduler::handle_t handle,
                                         RtecScheduler::RT_Info_Enabled_Type_t enabled) override;

  /// This method sets the enable state of a sequence of dependencies.
  void set_dependency_enable_state_seq (const RtecScheduler::Dependency_Set & dependencies) override;

  /// This method enables or disables a sequence of RT_Infos.
  void set_rt_info_enable_state_seq (const RtecScheduler::RT_Info_Enable_State_Pair_Set & pair_set) override;

  /**
   * In the config run scheduler, this method causes scheduling information
   * to be computed for all registered RT_Infos.  In the run time scheduler,
   * this is a no-op.
   */
  void compute_scheduling (CORBA::Long minimum_priority,
                                   CORBA::Long maximum_priority,
                                   RtecScheduler::RT_Info_Set_out infos,
                                   RtecScheduler::Dependency_Set_out deps,
                                   RtecScheduler::Config_Info_Set_out configs,
                                   RtecScheduler::Scheduling_Anomaly_Set_out anomalies) override;

  /// Recomputes the scheduling priorities, etc.
  void recompute_scheduling (CORBA::Long minimum_priority,
                                     CORBA::Long maximum_priority,
                                     RtecScheduler::Scheduling_Anomaly_Set_out anomalies) override;

  /// Returns the set of rt_infos, with their assigned priorities (as
  /// of the last schedule re-computation).
  void get_rt_info_set (RtecScheduler::RT_Info_Set_out infos) override;

  /// Returns the set of rt_infos, with their assigned priorities (as
  /// of the last schedule re-computation).
  void get_dependency_set (RtecScheduler::Dependency_Set_out dependencies) override;

  /// Returns the set of config_infos, describing the appropriate
  /// number, types, and priority levels for the dispatching lanes.
  void get_config_info_set (RtecScheduler::Config_Info_Set_out configs) override;

  /// Provides the thread priority and queue type for the given priority level.
  void dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                       RtecScheduler::OS_Priority& o_priority,
                                       RtecScheduler::Dispatching_Type_t & d_type) override;

  /**
   * Returns the last priority number assigned to an operation in the
   * schedule.  The number returned is one less than the total number
   * of scheduled priorities.  All scheduled priorities range from 0
   * to the number returned, inclusive.
   */
  RtecScheduler::Preemption_Priority_t last_scheduled_priority (void) override;

  void get_config_infos (RtecScheduler::Config_Info_Set_out configs) override;

private:
  /// The number of elements in the config array.
  int config_count_;

  /// The array of precomputed queue configuration structures.
  ACE_Scheduler_Factory::POD_Config_Info* config_info_;

  /// The number of elements in the RT_Info array.
  int entry_count_;

  /// The array of precomputed RT_Info structures.
  ACE_Scheduler_Factory::POD_RT_Info* rt_info_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_RUNTIME_SCHEDULER_H */
