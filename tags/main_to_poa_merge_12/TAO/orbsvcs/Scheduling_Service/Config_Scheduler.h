// ============================================================================
//
// $Id$
//
// ============================================================================

#ifndef ACE_CONFIG_SCHEDULER_H
#define ACE_CONFIG_SCHEDULER_H

#include "ace/OS.h"

#include "orbsvcs/RtecSchedulerS.h"
#include "orbsvcs/Event_Service_Constants.h"

class ACE_Config_Scheduler
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
  class ACE_Scheduler* impl;
};

#if defined (__ACE_INLINE__)
#include "Config_Scheduler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_CONFIG_SCHEDULER_H */
