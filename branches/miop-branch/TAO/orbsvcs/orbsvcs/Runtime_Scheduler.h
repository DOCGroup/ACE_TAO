/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Runtime_Scheduler.h
//
// = AUTHOR
//     Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_RUNTIME_SCHEDULER_H
#define ACE_RUNTIME_SCHEDULER_H
#include "ace/pre.h"

#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "orbsvcs/Sched/sched_export.h"

class TAO_RTSched_Export ACE_Runtime_Scheduler : public POA_RtecScheduler::Scheduler
{
  // = TITLE
  //   A fast and simple servant for RtecScheduler::Scheduler based on
  //   precomputed schedules.
  //
  // = DESCRIPTION
  //   This class offers the services of the scheduler, but based on
  //   precomputed and precompiled information. This results in a
  //   highly optimized execution after the configuration runs.
public:
  ACE_Runtime_Scheduler (int config_count,
                         ACE_Scheduler_Factory::POD_Config_Info config_info[],
                         int entry_count,
                         ACE_Scheduler_Factory::POD_RT_Info rt_info[]);
  // Constructor. Initialize the data from the POD_RT_Info array.

  virtual RtecScheduler::handle_t create (const char * entry_point
                                          TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::DUPLICATE_NAME));
  // Create an RT_Info.  In the config run scheduler this actually constructs
  // a new RT_Info.  Here, we just return its handle, or an error value if
  // it's not present.

  virtual RtecScheduler::handle_t lookup (const char * entry_point
                                          TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  // Lookup a handle for an RT_Info, and return its handle, or an error
  // value if it's not present.

  virtual RtecScheduler::RT_Info* get (RtecScheduler::handle_t handle
                                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
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
                    RtecScheduler::Info_Type_t info_type
                    TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK));
  // Set characteristics of the RT_Info corresponding to the passed handle.

  virtual void priority (RtecScheduler::handle_t handle,
                         RtecScheduler::OS_Priority& o_priority,
                         RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                         RtecScheduler::Preemption_Priority_t& p_priority
                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED));
  // Returns the priority and subpriority values assigned to an RT_Info,
  // based on its handle.

  virtual void entry_point_priority (const char * entry_point,
                                     RtecScheduler::OS_Priority& o_priority,
                                     RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                                     RtecScheduler::Preemption_Priority_t& p_priority
                                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED));
  // Returns the priority and subpriority values assigned to an RT_Info,
  // based on its entry point name.

  virtual void add_dependency (RtecScheduler::handle_t handle,
                               RtecScheduler::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type
                               TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK));
  // In the config run scheduler, this method registers a dependency between
  // two RT_Infos.  In the run time scheduler, this is a no-op.

  virtual void compute_scheduling (CORBA::Long minimum_priority,
                                   CORBA::Long maximum_priority,
                                   RtecScheduler::RT_Info_Set_out infos,
                                   RtecScheduler::Config_Info_Set_out configs,
                                   RtecScheduler::Scheduling_Anomaly_Set_out anomalies
                                   TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                     RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                     RtecScheduler::TASK_COUNT_MISMATCH));
  // In the config run scheduler, this method causes scheduling information
  // to be computed for all registered RT_Infos.  In the run time scheduler,
  // this is a no-op.

  virtual void dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                       RtecScheduler::OS_Priority& o_priority,
                                       RtecScheduler::Dispatching_Type_t & d_type
                                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED,
                    RtecScheduler::UNKNOWN_PRIORITY_LEVEL));
  // Provides the thread priority and queue type for the given priority level.

  virtual RtecScheduler::Preemption_Priority_t last_scheduled_priority (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED));
  // Returns the last priority number assigned to an operation in the
  // schedule.  The number returned is one less than the total number
  // of scheduled priorities.  All scheduled priorities range from 0
  // to the number returned, inclusive.

private:
  int config_count_;
  // The number of elements in the config array.

  ACE_Scheduler_Factory::POD_Config_Info* config_info_;
  // The array of precomputed queue configuration structures.

  int entry_count_;
  // The number of elements in the RT_Info array.

  ACE_Scheduler_Factory::POD_RT_Info* rt_info_;
  // The array of precomputed RT_Info structures.
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Runtime_Scheduler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RUNTIME_SCHEDULER_H */
