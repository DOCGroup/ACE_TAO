// ============================================================================
//
// $Id$
//
// ============================================================================

#ifndef ACE_RUNTIME_SCHEDULER_H
#define ACE_RUNTIME_SCHEDULER_H

#include "ace/OS.h"

#include "orbsvcs/RtecSchedulerS.h"

class ACE_Runtime_Scheduler
  : public POA_RtecScheduler::Scheduler
  // = TITLE
  //   A proxy server for RtecScheduler::Scheduler.
  //
  // = DESCRIPTION
  //   This class offers the services of the scheduler, but based on
  //   precomputed and precompiled information. This results in a
  //   highly optimized execution after the configuration runs.
{
public:
  ACE_Runtime_Scheduler (int entry_count,
			 RtecScheduler::RT_Info* rt_info[]);
  // Initialize the data from the RT_Info array.
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
		    RtecScheduler::Time time,
		    RtecScheduler::Time typical_time,
		    RtecScheduler::Time cached_time,
		    RtecScheduler::Period period,
		    RtecScheduler::Importance importance,
		    RtecScheduler::Quantum quantum,
		    CORBA::Long threads,
		    CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException, RtecScheduler::UNKNOWN_TASK));

  virtual void priority (RtecScheduler::handle_t handle,
			 RtecScheduler::OS_Priority& priority,
			 RtecScheduler::Sub_Priority& subpriority,
			 RtecScheduler::Preemption_Priority& p_priority,
			 CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     RtecScheduler::UNKNOWN_TASK,
		     RtecScheduler::NOT_SCHEDULED));
           
  virtual void entry_point_priority (const char * entry_point,
				     RtecScheduler::OS_Priority& priority,
				     RtecScheduler::Sub_Priority& subpriority,
				     RtecScheduler::Preemption_Priority& p_priority,
				     CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     RtecScheduler::UNKNOWN_TASK,
		     RtecScheduler::NOT_SCHEDULED));

  virtual void add_dependency (RtecScheduler::handle_t handle,
			       RtecScheduler::handle_t dependency,
			       CORBA::Long number_of_calls,
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
  RtecScheduler::RT_Info** rt_info_;
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Runtime_Scheduler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_RUNTIME_SCHEDULER_H */
