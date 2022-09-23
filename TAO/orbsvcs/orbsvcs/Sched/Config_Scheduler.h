// -*- C++ -*-

#ifndef ACE_CONFIG_SCHEDULER_H
#define ACE_CONFIG_SCHEDULER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/RtecSchedulerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Sched/sched_export.h"

#if defined (TAO_USES_STRATEGY_SCHEDULER)
#include "orbsvcs/Sched/Strategy_Scheduler.h"
#else
#include "orbsvcs/Sched/Scheduler_Generic.h"
#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTSched_Export ACE_Config_Scheduler
: public POA_RtecScheduler::Scheduler
  // = TITLE
  //   A (local) implementation for the RtecScheduler::Scheduler service.
  //
  // = DESCRIPTION
  //   This class implements a servant for the
  //   RtecScheduler::Scheduler service, using the Scheduler classes
  //   distributed with the EC.
{
public:
  ACE_Config_Scheduler ();
  ~ACE_Config_Scheduler () override;

  RtecScheduler::handle_t create (const char * entry_point) override;

  RtecScheduler::handle_t lookup (const char * entry_point) override;

  RtecScheduler::RT_Info* get (RtecScheduler::handle_t handle) override;

  void set (RtecScheduler::handle_t handle,
                    RtecScheduler::Criticality_t criticality,
                    RtecScheduler::Time time,
                    RtecScheduler::Time typical_time,
                    RtecScheduler::Time cached_time,
                    RtecScheduler::Period_t period,
                    RtecScheduler::Importance_t importance,
                    RtecScheduler::Quantum_t quantum,
                    CORBA::Long threads,
                    RtecScheduler::Info_Type_t info_type) override;

  void priority (RtecScheduler::handle_t handle,
                         RtecScheduler::OS_Priority& priority,
                         RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                         RtecScheduler::Preemption_Priority_t& p_priority) override;

  void entry_point_priority (const char * entry_point,
                                     RtecScheduler::OS_Priority& priority,
                                     RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                                     RtecScheduler::Preemption_Priority_t& p_priority) override;

  void add_dependency (RtecScheduler::handle_t handle,
                               RtecScheduler::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type) override;

  void compute_scheduling (CORBA::Long minimum_priority,
                                   CORBA::Long maximum_priority,
                                   RtecScheduler::RT_Info_Set_out infos,
                                   RtecScheduler::Dependency_Set_out dependencies,
                                   RtecScheduler::Config_Info_Set_out configs,
                                   RtecScheduler::Scheduling_Anomaly_Set_out anomalies) override;

  void dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                       RtecScheduler::OS_Priority& priority,
                                       RtecScheduler::Dispatching_Type_t & d_type) override;
  // provide the thread priority and queue type for the given priority level

  RtecScheduler::Preemption_Priority_t
      last_scheduled_priority () override;
  // Returns the last priority number assigned to an operation in the
  // schedule.
  // The number returned is one less than the total number of
  // scheduled priorities.
  // All scheduled priorities range from 0 to the number returned,
  // inclusive.

  void get_config_infos (RtecScheduler::Config_Info_Set_out configs) override;
  // Provides the set of Config_Infos associated with the current schedule.

  void reset (RtecScheduler::handle_t handle,
                      RtecScheduler::Criticality_t criticality,
                      RtecScheduler::Time time,
                      RtecScheduler::Time typical_time,
                      RtecScheduler::Time cached_time,
                      RtecScheduler::Period_t period,
                      RtecScheduler::Importance_t importance,
                      RtecScheduler::Quantum_t quantum,
                      CORBA::Long threads,
                      RtecScheduler::Info_Type_t info_type) override;
  // Reset characteristics of the RT_Info corresponding to the passed handle.

  void set_seq (const RtecScheduler::RT_Info_Set& infos) override;
  // Set characteristics of the RT_Infos corresponding to the passed handles.
  // Tuples are added in the case of existing and/or multiple definitions.

  void reset_seq (const RtecScheduler::RT_Info_Set& infos) override;
  // Reset characteristics of the RT_Infos corresponding to the passed handles.
  // Tuples are replaced in the case of existing and/or multiple definitions.

  void replace_seq (const RtecScheduler::RT_Info_Set& infos) override;
  // Replace all RT_Infos, resetting characteristics of the RT_Infos
  // corresponding to the passed handles.  All other RT_Infos are
  // reset to their uninitialized values, i.e., the same they have
  // just after the create call.

  void remove_dependency (RtecScheduler::handle_t handle,
                                  RtecScheduler::handle_t dependency,
                                  CORBA::Long number_of_calls,
                                  RtecScheduler::Dependency_Type_t dependency_type) override;
  // This method removes a dependency between two RT_Infos.

  void set_dependency_enable_state (RtecScheduler::handle_t handle,
                                            RtecScheduler::handle_t dependency,
                                            CORBA::Long number_of_calls,
                                            RtecScheduler::Dependency_Type_t dependency_type,
                                            RtecScheduler::Dependency_Enabled_Type_t enabled) override;
  // This method sets the enable state of a dependency between two RT_Infos.

  void set_dependency_enable_state_seq (const RtecScheduler::Dependency_Set & dependencies) override;
  // This method sets the enable state of a sequence of dependencies.

  void set_rt_info_enable_state (RtecScheduler::handle_t handle,
                                         RtecScheduler::RT_Info_Enabled_Type_t enabled) override;
  // This method enables or disables an RT_Info.

  void set_rt_info_enable_state_seq (const RtecScheduler::RT_Info_Enable_State_Pair_Set & pair_set) override;
  // This method enables or disables a sequence of RT_Infos.

  void recompute_scheduling (CORBA::Long minimum_priority,
                                     CORBA::Long maximum_priority,
                                     RtecScheduler::Scheduling_Anomaly_Set_out anomalies) override;
  // Recomputes the scheduling priorities, etc.

  void get_rt_info_set (RtecScheduler::RT_Info_Set_out infos) override;
  // Returns the set of rt_infos, with their assigned priorities (as
  // of the last schedule re-computation).

  void get_dependency_set (RtecScheduler::Dependency_Set_out dependencies) override;
  // Returns the set of rt_infos, with their assigned priorities (as
  // of the last schedule re-computation).

  void get_config_info_set (RtecScheduler::Config_Info_Set_out configs) override;
  // Returns the set of config_infos, describing the appropriate
  // number, types, and priority levels for the dispatching lanes.


private:
#if defined (TAO_USES_STRATEGY_SCHEDULER)

  // trait for the scheduler implementation base class
  typedef ACE_DynScheduler BaseSchedImplType;

  // traits for the scheduler strategy
#if defined (TAO_USES_MUF_SCHEDULING)

  typedef ACE_MUF_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_MLF_SCHEDULING)
#  if ! defined (TAO_MIN_CRITICAL_PRIORITY)
#    define TAO_MIN_CRITICAL_PRIORITY 0
#  endif /* ! defined (TAO_MIN_CRITICAL_PRIORITY) */
  typedef ACE_MLF_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_EDF_SCHEDULING)

#  if ! defined (TAO_MIN_CRITICAL_PRIORITY)
#    define TAO_MIN_CRITICAL_PRIORITY 0
#  endif /* ! defined (TAO_MIN_CRITICAL_PRIORITY) */
  typedef ACE_EDF_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_RMS_SCHEDULING)
#  if ! defined (TAO_MIN_CRITICAL_PRIORITY)
#    define TAO_MIN_CRITICAL_PRIORITY 0
#  endif /* ! defined (TAO_MIN_CRITICAL_PRIORITY) */
  typedef ACE_RMS_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_CRITICALITY_SCHEDULING)
  typedef ACE_Criticality_Scheduler_Strategy Scheduler_Strategy;

#else
  #error scheduling strategy must be defined

#endif /* defined (TAO_USES_MUF_SCHEDULING) */

  Scheduler_Strategy scheduler_strategy_;

#else /* ! defined (TAO_USES_STRATEGY_SCHEDULER) */

  // trait for the scheduler implementation base class
  typedef ACE_Scheduler BaseSchedImplType;

#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */

  // implementation base class pointer
  BaseSchedImplType* impl;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_SCHEDULER_H */
