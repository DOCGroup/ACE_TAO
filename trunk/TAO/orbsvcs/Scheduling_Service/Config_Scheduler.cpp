// ============================================================================
//
// $Id$
//
// ============================================================================

#include "orbsvcs/Scheduler_Factory.h"

#include "Scheduler_Generic.h"
#include "Config_Scheduler.h"

#if defined (__ACE_INLINE__)
#include "Config_Scheduler.i"
#endif /* __ACE_INLINE__ */

ACE_Config_Scheduler::ACE_Config_Scheduler (void)
  : impl(new Scheduler_Generic)
{
  impl->output_level (10);
}

ACE_Config_Scheduler::~ACE_Config_Scheduler (void)
{
  delete impl;
}

RtecScheduler::handle_t
ACE_Config_Scheduler::create (const char * entry_point,
			      CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::DUPLICATE_NAME))
{
  typedef RtecScheduler::RT_Info* RT_Info_ptr;

  RtecScheduler::RT_Info** rt_info;
  ACE_NEW_RETURN (rt_info, RT_Info_ptr[1], -1);

  ACE_NEW_RETURN (rt_info[0], RtecScheduler::RT_Info, -1);

  rt_info[0]->entry_point = CORBA::string_dup(entry_point);
  rt_info[0]->handle = -1;
  rt_info[0]->worst_case_execution_time = 0;
  rt_info[0]->typical_execution_time = 0;
  rt_info[0]->cached_execution_time = 0;
  rt_info[0]->period = 0;
  rt_info[0]->importance = RtecScheduler::VERY_LOW;
  rt_info[0]->quantum = RtecScheduler::NO_QUANTUM;
  rt_info[0]->threads = 0;
  rt_info[0]->priority = 0;
  rt_info[0]->subpriority = 0;
  rt_info[0]->preemption_priority = 0;
  
  RtecScheduler::handle_t handle = -1;
  switch (impl->register_task (rt_info, 1, handle))
    {
    case ACE_Scheduler::SUCCEEDED:
      break;
    case ACE_Scheduler::ST_VIRTUAL_MEMORY_EXHAUSTED:
    case ACE_Scheduler::ST_TASK_ALREADY_REGISTERED:
    default:
      delete rt_info[0];
      delete[] rt_info;
      ACE_ERROR ((LM_ERROR,
		  "Config_Scheduler::create - register_task failed\n"));
      // TODO: throw something.
      break;
    }
  return handle;
}

RtecScheduler::handle_t
ACE_Config_Scheduler::lookup (const char * entry_point,
			      CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException))
{
  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->get_rt_info (entry_point, rt_info))
    {
    case ACE_Scheduler::SUCCEEDED:
      return rt_info->handle;
      break;
    case ACE_Scheduler::FAILED:
    case ACE_Scheduler::ST_UNKNOWN_TASK:
    default:
      ACE_ERROR ((LM_ERROR,
		  "Config_Scheduler::lookup - get_rt_info failed\n"));
      // TODO: throw something.
      break;
    }
  return -1;
}

RtecScheduler::RT_Info*
ACE_Config_Scheduler::get (RtecScheduler::handle_t handle,
			   CORBA::Environment &_env)
     TAO_THROW_SPEC((CORBA::SystemException,
		     RtecScheduler::UNKNOWN_TASK))
{
  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->lookup_rt_info (handle, rt_info))
    {
    case ACE_Scheduler::SUCCEEDED:
      {
	// IDL memory managment semantics require the we return a copy
	RtecScheduler::RT_Info* copy;
	ACE_NEW_RETURN (copy, RtecScheduler::RT_Info (*rt_info), 0);
	return copy;
      }
      break;
    case ACE_Scheduler::FAILED:
    case ACE_Scheduler::ST_UNKNOWN_TASK:
    default:
      ACE_ERROR ((LM_ERROR,
		  "Config_Scheduler::get - lookup_rt_info failed\n"));
      // TODO: throw something.
      break;
    }
  return 0;
}

void ACE_Config_Scheduler::set (RtecScheduler::handle_t handle,
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
  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->lookup_rt_info (handle, rt_info))
    {
    case ACE_Scheduler::SUCCEEDED:
      rt_info->worst_case_execution_time = time;
      rt_info->typical_execution_time = typical_time;
      rt_info->cached_execution_time = cached_time;
      rt_info->period = period;
      rt_info->importance = importance;
      rt_info->quantum = quantum;
      rt_info->threads = threads;
      break;
    case ACE_Scheduler::FAILED:
    case ACE_Scheduler::ST_UNKNOWN_TASK:
    default:
      ACE_ERROR ((LM_ERROR,
		  "Config_Scheduler::set - lookup_rt_info failed\n"));
      // TODO: throw something.
      break;
    }
}

void ACE_Config_Scheduler::priority (RtecScheduler::handle_t handle,
				     RtecScheduler::OS_Priority& priority,
				     RtecScheduler::Sub_Priority& subpriority,
				     RtecScheduler::Preemption_Priority& p_priority,
				     CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::UNKNOWN_TASK,
		      RtecScheduler::NOT_SCHEDULED))
{
  if (impl->priority (handle, priority, subpriority, p_priority) == -1)
    {
      ACE_ERROR ((LM_ERROR,
		  "Config_Scheduler::priority - priority failed\n"));
      // TODO: throw something.
    }
}
           
void ACE_Config_Scheduler::entry_point_priority (const char * entry_point,
						 RtecScheduler::OS_Priority& priority,
						 RtecScheduler::Sub_Priority& subpriority,
						 RtecScheduler::Preemption_Priority& p_priority,
						 CORBA::Environment &_env)
     TAO_THROW_SPEC((CORBA::SystemException,
		     RtecScheduler::UNKNOWN_TASK,
		     RtecScheduler::NOT_SCHEDULED))
{
  this->priority (lookup (entry_point, _env),
		  priority, subpriority, p_priority,
		  _env);
}

void ACE_Config_Scheduler::add_dependency (RtecScheduler::handle_t handle,
					   RtecScheduler::handle_t dependency,
					   CORBA::Long number_of_calls,
					   CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::UNKNOWN_TASK))
{
  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->lookup_rt_info (handle, rt_info))
    {
    case ACE_Scheduler::SUCCEEDED:
      {
	RtecScheduler::Dependency_Info dep;
	dep.rt_info = dependency;
	dep.number_of_calls = number_of_calls;
	ACE_Scheduler::add_dependency(rt_info, dep);
      }
      break;
    case ACE_Scheduler::FAILED:
    case ACE_Scheduler::ST_UNKNOWN_TASK:
    default:
      ACE_ERROR ((LM_ERROR,
		  "cannot find %d to add dependency", handle));
      // TODO: throw something.
      break;
    }
}

void ACE_Config_Scheduler::compute_scheduling (CORBA::Long minimum_priority,
					       CORBA::Long maximum_priority,
					       RtecScheduler::RT_Info_Set_out infos,
					       CORBA::Environment &_env)
     TAO_THROW_SPEC ((CORBA::SystemException,
		      RtecScheduler::UTILIZATION_BOUND_EXCEEDED,
		      RtecScheduler::INSUFFICIENT_THREAD_PRIORITY_LEVELS,
		      RtecScheduler::TASK_COUNT_MISMATCH))
{
  impl->init (minimum_priority, maximum_priority);
  if (impl->schedule () != ACE_Scheduler::SUCCEEDED)
    {
      // TODO: throw something.
      ACE_ERROR ((LM_ERROR, "schedule failed\n"));
      return;
    }
  if (infos == 0)
    {
      infos = new RtecScheduler::RT_Info_Set(impl->tasks ());
    }
  infos->length (impl->tasks ());
  for (RtecScheduler::handle_t handle = 1;
       handle <= impl->tasks ();
       ++handle)
    {
      RtecScheduler::RT_Info* rt_info = 0;
      switch (impl->lookup_rt_info (handle, rt_info))
	{
	case ACE_Scheduler::SUCCEEDED:
	  // We know that handles start at 1.
	  infos[CORBA::ULong(handle - 1)] = *rt_info;
	  break;
	case ACE_Scheduler::FAILED:
	case ACE_Scheduler::ST_UNKNOWN_TASK:
	default:
	  ACE_ERROR ((LM_ERROR,
		      "Config_Scheduler::schedule - lookup_rt_info failed\n"));
	  // TODO: throw something.
	  break;
	}
    }
  ACE_DEBUG ((LM_DEBUG, "schedule prepared\n"));

  ACE_DEBUG ((LM_DEBUG, "dumping to stdout\n"));
  ACE_Scheduler_Factory::dump_schedule (*(infos.ptr()), 0);
  ACE_DEBUG ((LM_DEBUG, "dump done\n"));
}
