// $Id$

#ifndef ACE_RUNTIME_SCHEDULER_H
#define ACE_RUNTIME_SCHEDULER_H

#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"

class TAO_ORBSVCS_Export ACE_Runtime_Scheduler : public POA_RtecScheduler::Scheduler
{
  // = TITLE
  //   A fast and simple servant for RtecScheduler::Scheduler based on
  //   precomputed schedules.
  //
  // = DESCRIPTION
  //   This class offers the services of the scheduler, but based on
  //   precomputed and precompiled information. This results in a
  //   highly optimized execution after the configuration runs.
  //
public:
  ACE_Runtime_Scheduler (int entry_count,
                         ACE_Scheduler_Factory::POD_RT_Info rt_info[]);
  // Initialize the data from the POD_RT_Info array.
  // Only basic validation is done.

  virtual RtecScheduler::handle_t create (const char * entry_point,
                                          CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::DUPLICATE_NAME));

  virtual RtecScheduler::handle_t lookup (const char * entry_point,
                                          CORBA::Environment &_env)
    TAO_THROW_SPEC((CORBA::SystemException));

  virtual RtecScheduler::RT_Info* get (RtecScheduler::handle_t handle,
                                       CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::UNKNOWN_TASK));

  virtual void set (RtecScheduler::handle_t handle,
                    RtecScheduler::Criticality criticality,
                    const RtecScheduler::Time &time,
                    const RtecScheduler::Time &typical_time,
                    const RtecScheduler::Time &cached_time,
                    RtecScheduler::Period period,
                    RtecScheduler::Importance importance,
                    const RtecScheduler::Quantum &quantum,
                    CORBA::Long threads,
                    RtecScheduler::Info_Type info_type,
                    CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::UNKNOWN_TASK));

  virtual void priority (RtecScheduler::handle_t handle,
                         RtecScheduler::OS_Priority& priority,
                         RtecScheduler::Preemption_Subpriority& p_subpriority,
                         RtecScheduler::Preemption_Priority& p_priority,
                         CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED));

  virtual void entry_point_priority (const char * entry_point,
                                     RtecScheduler::OS_Priority& priority,
                                     RtecScheduler::Preemption_Subpriority& p_subpriority,
                                     RtecScheduler::Preemption_Priority& p_priority,
                                     CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK,
                     RtecScheduler::NOT_SCHEDULED));

  virtual void add_dependency (RtecScheduler::handle_t handle,
                               RtecScheduler::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type dependency_type,
                               CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecScheduler::UNKNOWN_TASK));

  virtual void compute_scheduling (CORBA::Long minimum_priority,
                                   CORBA::Long maximum_priority,
                                   RtecScheduler::RT_Info_Set_out infos,
                                   CORBA::Environment &_env)
    TAO_THROW_SPEC((CORBA::SystemException,
                    RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                    RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                    RtecScheduler::TASK_COUNT_MISMATCH));

private:
  int entry_count_;
  // The number of elements in the array.

  ACE_Scheduler_Factory::POD_RT_Info* rt_info_;
  // The array of precomputed RT_Info structures.
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Runtime_Scheduler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_RUNTIME_SCHEDULER_H */
