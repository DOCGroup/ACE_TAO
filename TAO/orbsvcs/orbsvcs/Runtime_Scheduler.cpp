// ============================================================================
//
// $Id$
//
// ============================================================================

#include "orbsvcs/Runtime_Scheduler.h"

#if defined (__ACE_INLINE__)
#include "orbsvcs/Runtime_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_Runtime_Scheduler::
ACE_Runtime_Scheduler (int entry_count,
		       RtecScheduler::RT_Info* rt_info[])
:  POA_RtecScheduler::Scheduler ("Static_Scheduler"),
   entry_count_ (entry_count),
   rt_info_ (rt_info)
{
}

RtecScheduler::handle_t
ACE_Runtime_Scheduler::create (const char * entry_point,
			       CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::DUPLICATE_NAME))
{
  // Just make sure its there and returns its handle (position).
  int i;
  for (i = 0; i < entry_count_; ++i)
    {
      if (strcmp (entry_point, rt_info_[i]->entry_point) == 0)
	{
	  return i;
	}
    }
  // TODO: throw an exception or print an error.
  return -1;
}

RtecScheduler::handle_t
ACE_Runtime_Scheduler::lookup (const char * entry_point,
			       CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException))
{
  return create (entry_point, _env);
}

RtecScheduler::RT_Info*
ACE_Runtime_Scheduler::get (RtecScheduler::handle_t handle,
			    CORBA::Environment &_env)
     TAO_THROW_SPEC((CORBA::SystemException,
		     RtecScheduler::UNKNOWN_TASK))
{
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW_RETURN (RtecScheduler::UNKNOWN_TASK(), 0);
    }
  return rt_info_[handle];
}

void ACE_Runtime_Scheduler::set (RtecScheduler::handle_t handle,
				 RtecScheduler::Time time,
				 RtecScheduler::Time typical_time,
				 RtecScheduler::Time cached_time,
				 RtecScheduler::Period period,
				 RtecScheduler::Importance importance,
				 RtecScheduler::Quantum quantum,
				 CORBA::Long threads,
				 CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::UNKNOWN_TASK))
{
  // We compare the values with the ones stored and print a message on
  // any differences.
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW (RtecScheduler::UNKNOWN_TASK);
      return;
    }
  if (rt_info_[handle]->worst_case_execution_time != time
      || rt_info_[handle]->typical_execution_time != typical_time
      || rt_info_[handle]->cached_execution_time != cached_time
      || rt_info_[handle]->period != period
      || rt_info_[handle]->importance != importance
      || rt_info_[handle]->quantum != quantum
      || rt_info_[handle]->threads != threads)
    {
      ACE_ERROR ((LM_ERROR, "invalid data for RT_Info: %s\n",
		  (const char*)rt_info_[handle]->entry_point));
      // TODO: throw something here.
    }
}


void ACE_Runtime_Scheduler::priority (RtecScheduler::handle_t handle,
				      RtecScheduler::OS_Priority& priority,
				      RtecScheduler::Sub_Priority& subpriority,
				      RtecScheduler::Preemption_Priority& p_priority,
				      CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::UNKNOWN_TASK,
		      RtecScheduler::NOT_SCHEDULED))
{
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW (RtecScheduler::UNKNOWN_TASK());
      return;
    }
  priority = rt_info_[handle]->priority;
  subpriority = rt_info_[handle]->subpriority;
  p_priority = rt_info_[handle]->preemption_priority;
}
           
void ACE_Runtime_Scheduler::entry_point_priority (const char * entry_point,
						  RtecScheduler::OS_Priority& priority,
						  RtecScheduler::Sub_Priority& subpriority,
						  RtecScheduler::Preemption_Priority& p_priority,
						  CORBA::Environment &_env)
     TAO_THROW_SPEC((CORBA::SystemException,
		     RtecScheduler::UNKNOWN_TASK,
		     RtecScheduler::NOT_SCHEDULED))
{
  RtecScheduler::handle_t handle = lookup (entry_point, _env);
  if (handle < -1)
    {
      // The exception was thrown or is in _env already.
      return;
    }
  this->priority (handle, priority, subpriority, p_priority, _env);
}

void ACE_Runtime_Scheduler::add_dependency (RtecScheduler::handle_t handle,
					    RtecScheduler::handle_t dependency,
					    CORBA::Long number_of_calls,
					    CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::UNKNOWN_TASK))
{
  if (handle < 0 || handle > entry_count_)
    {
      TAO_THROW (RtecScheduler::UNKNOWN_TASK);
      return;
    }
  // Just check that the information is consistent.
  RtecScheduler::Dependency_Set& deps = rt_info_[handle]->dependencies;
  for (CORBA::ULong i = 0; i < deps.length (); ++i)
    {
      if (deps[i].rt_info == dependency
	  && deps[i].number_of_calls == number_of_calls)
	{
	  return;
	}
    }
  ACE_ERROR ((LM_ERROR, "unmatched dependency on %s\n",
	      (const char*)rt_info_[handle]->entry_point));
}

void ACE_Runtime_Scheduler::compute_scheduling (CORBA::Long minimum_priority,
						CORBA::Long maximum_priority,
						RtecScheduler::RT_Info_Set_out infos,
						CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
		      RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
		      RtecScheduler::TASK_COUNT_MISMATCH))
{
  // TODO: Right now just do nothing, later we could validate the
  // priorities (without recomputing).
  // TODO: fill up the infos.
  return;
}

