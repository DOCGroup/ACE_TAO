// $Id$

#ifndef ACE_CONFIG_SCHEDULER_H
#define ACE_CONFIG_SCHEDULER_H

#include "ace/OS.h"

#include "orbsvcs/RtecSchedulerS.h"
#include "orbsvcs/Event_Service_Constants.h"

#if defined (TAO_USES_STRATEGY_SCHEDULER)
#include "orbsvcs/Sched/Strategy_Scheduler.h"
#else
#include "orbsvcs/Sched/Scheduler_Generic.h"
#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */


class TAO_ORBSVCS_Export ACE_Config_Scheduler
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

  ACE_Config_Scheduler (void);
  virtual ~ACE_Config_Scheduler (void);

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
				   RtecScheduler::Config_Info_Set_out configs,
                                   CORBA::Environment &_env)
    TAO_THROW_SPEC((CORBA::SystemException,
                    RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
                    RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
                    RtecScheduler::TASK_COUNT_MISMATCH));

  virtual void dispatch_configuration (RtecScheduler::Preemption_Priority p_priority,
                                       RtecScheduler::OS_Priority& priority,
				       RtecScheduler::Dispatching_Type & d_type,
				       CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED,
                    RtecScheduler::UNKNOWN_PRIORITY_LEVEL));
  // provide the thread priority and queue type for the given priority level

  virtual RtecScheduler::Preemption_Priority last_scheduled_priority (CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                    RtecScheduler::NOT_SCHEDULED));
  // Returns the last priority number assigned to an operation in the schedule.
  // The number returned is one less than the total number of scheduled priorities.
  // All scheduled priorities range from 0 to the number returned, inclusive.


private:

#if defined (TAO_USES_STRATEGY_SCHEDULER)

  // trait for the scheduler implementation base class
  typedef ACE_DynScheduler BaseSchedImplType;

  // traits for the scheduler strategy 
#if defined (TAO_USES_MUF_SCHEDULING)

  typedef ACE_MUF_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_MLF_SCHEDULING)

  #if ! defined (TAO_MIN_CRITICAL_PRIORITY)
    #define TAO_MIN_CRITICAL_PRIORITY 0
  #endif /* ! defined (TAO_MIN_CRITICAL_PRIORITY) */

  typedef ACE_MLF_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_EDF_SCHEDULING)

  #if ! defined (TAO_MIN_CRITICAL_PRIORITY)
    #define TAO_MIN_CRITICAL_PRIORITY 0
  #endif /* ! defined (TAO_MIN_CRITICAL_PRIORITY) */

  typedef ACE_EDF_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_RMS_SCHEDULING)

  typedef ACE_RMS_Scheduler_Strategy Scheduler_Strategy;

#elif defined (TAO_USES_RMS_DYN_SCHEDULING)

  typedef ACE_RMS_Dyn_Scheduler_Strategy Scheduler_Strategy;

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

#if defined (__ACE_INLINE__)
#include "Config_Scheduler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_CONFIG_SCHEDULER_H */









