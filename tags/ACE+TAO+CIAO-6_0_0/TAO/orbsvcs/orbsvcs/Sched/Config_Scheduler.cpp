// ============================================================================
//
// $Id$
//
// ============================================================================

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"

#include "orbsvcs/Sched/Config_Scheduler.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Config_Scheduler::ACE_Config_Scheduler (void)
#if defined (TAO_USES_STRATEGY_SCHEDULER)
  : scheduler_strategy_ (static_cast<RtecScheduler::Preemption_Priority_t> (TAO_MIN_CRITICAL_PRIORITY))
  , impl (new ACE_Strategy_Scheduler (scheduler_strategy_))
#else
  : impl (new Scheduler_Generic)
#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */
{
  // impl->output_level (10);
}

ACE_Config_Scheduler::~ACE_Config_Scheduler (void)
{
  delete impl;
}

RtecScheduler::handle_t
ACE_Config_Scheduler::create (const char * entry_point)
{
  typedef RtecScheduler::RT_Info* RT_Info_ptr;

  RtecScheduler::RT_Info** rt_info = 0;
  ACE_NEW_RETURN (rt_info, RT_Info_ptr[1], -1);

  ACE_NEW_RETURN (rt_info[0], RtecScheduler::RT_Info, -1);

  rt_info[0]->entry_point = CORBA::string_dup(entry_point);
  rt_info[0]->handle = -1;
  rt_info[0]->worst_case_execution_time = ORBSVCS_Time::zero ();
  rt_info[0]->typical_execution_time = ORBSVCS_Time::zero ();
  rt_info[0]->cached_execution_time = ORBSVCS_Time::zero ();
  rt_info[0]->period = 0;
  rt_info[0]->criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  rt_info[0]->importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  rt_info[0]->quantum = ORBSVCS_Time::zero ();
  rt_info[0]->threads = 0;
  rt_info[0]->priority = 0;
  rt_info[0]->preemption_subpriority = 0;
  rt_info[0]->preemption_priority = 0;
  rt_info[0]->info_type = RtecScheduler::OPERATION;
  rt_info[0]->volatile_token = 0;

  RtecScheduler::handle_t handle = -1;
#if defined (TAO_USES_STRATEGY_SCHEDULER)
  switch (impl->register_task (rt_info[0], handle))
#else
  switch (impl->register_task (rt_info, 1, handle))
#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */

    {
    case BaseSchedImplType::SUCCEEDED:
      break;
    case BaseSchedImplType::ST_VIRTUAL_MEMORY_EXHAUSTED:
    case BaseSchedImplType::ST_TASK_ALREADY_REGISTERED:
    default:
      delete rt_info[0];
      delete[] rt_info;
      ACE_ERROR ((LM_ERROR,
                  "Config_Scheduler::create - register_task failed\n"));
      // @@ TODO: throw something.
      break;
    }
  return handle;
}

RtecScheduler::handle_t
ACE_Config_Scheduler::lookup (const char * entry_point)
{
  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->get_rt_info (entry_point, rt_info))
    {
    case BaseSchedImplType::SUCCEEDED:
      return rt_info->handle;
      ACE_NOTREACHED (break);
    case BaseSchedImplType::FAILED:
    case BaseSchedImplType::ST_UNKNOWN_TASK:
    default:
      ACE_ERROR ((LM_ERROR,
                  "Config_Scheduler::lookup - get_rt_info failed\n"));
      // @@ TODO: throw something.
      break;
    }
  return -1;
}

RtecScheduler::RT_Info*
ACE_Config_Scheduler::get (RtecScheduler::handle_t handle)
{
  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->lookup_rt_info (handle, rt_info))
    {
    case BaseSchedImplType::SUCCEEDED:
      {
        // IDL memory managment semantics require that we return a copy.
        RtecScheduler::RT_Info* copy;
        ACE_NEW_RETURN (copy, RtecScheduler::RT_Info (*rt_info), 0);
        return copy;
      }
      ACE_NOTREACHED (break);
    case BaseSchedImplType::FAILED:
    case BaseSchedImplType::ST_UNKNOWN_TASK:
    default:
      ACE_ERROR ((LM_ERROR,
                  "Config_Scheduler::get - lookup_rt_info failed\n"));
      // @@ TODO: throw something.
      break;
    }
  return 0;
}

void ACE_Config_Scheduler::set (RtecScheduler::handle_t handle,
                                RtecScheduler::Criticality_t criticality,
                                RtecScheduler::Time time,
                                RtecScheduler::Time typical_time,
                                RtecScheduler::Time cached_time,
                                RtecScheduler::Period_t period,
                                RtecScheduler::Importance_t importance,
                                RtecScheduler::Quantum_t quantum,
                                CORBA::Long threads,
                                RtecScheduler::Info_Type_t info_type)
{
  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->lookup_rt_info (handle, rt_info))
    {
    case BaseSchedImplType::SUCCEEDED:
      rt_info->criticality = criticality,
      rt_info->worst_case_execution_time = time;
      rt_info->typical_execution_time = typical_time;
      rt_info->cached_execution_time = cached_time;
      rt_info->period = period;
      rt_info->importance = importance;
      rt_info->quantum = quantum;
      rt_info->threads = threads;
      rt_info->info_type = info_type;
      break;
    case BaseSchedImplType::FAILED:
    case BaseSchedImplType::ST_UNKNOWN_TASK:
    default:
      ACE_ERROR ((LM_ERROR,
                  "Config_Scheduler::set - lookup_rt_info failed\n"));
      // @@ TODO: throw something.
      break;
    }
}

void ACE_Config_Scheduler::priority (RtecScheduler::handle_t handle,
                                     RtecScheduler::OS_Priority& priority,
                                     RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                                     RtecScheduler::Preemption_Priority_t& p_priority)
{

  if (impl->priority (handle, priority, p_subpriority, p_priority) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Config_Scheduler::priority - priority failed\n"));
      // TODO: throw something.
    }
}

void ACE_Config_Scheduler::entry_point_priority (const char * entry_point,
                                                 RtecScheduler::OS_Priority& priority,
                                                 RtecScheduler::Preemption_Subpriority_t& p_subpriority,
                                                 RtecScheduler::Preemption_Priority_t& p_priority)
{
  this->priority (lookup (entry_point),
                  priority, p_subpriority, p_priority);
}

void ACE_Config_Scheduler::add_dependency (RtecScheduler::handle_t handle,
                                           RtecScheduler::handle_t dependency,
                                           CORBA::Long number_of_calls,
                                           RtecScheduler::Dependency_Type_t
                                             dependency_type)
{

  RtecScheduler::RT_Info* rt_info = 0;
  switch (impl->lookup_rt_info (handle, rt_info))
    {
    case BaseSchedImplType::SUCCEEDED:
      {
        RtecScheduler::Dependency_Info dep;
        dep.rt_info = dependency;
        dep.number_of_calls = number_of_calls;
        dep.dependency_type = dependency_type;
#if defined (TAO_USES_STRATEGY_SCHEDULER)
        impl->add_dependency (rt_info, dep);
#else
        BaseSchedImplType::add_dependency (rt_info, dep);
#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */

      }
      break;
    case BaseSchedImplType::FAILED:
    case BaseSchedImplType::ST_UNKNOWN_TASK:
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
                                               RtecScheduler::Dependency_Set_out dependencies,
                                               RtecScheduler::Config_Info_Set_out configs,
                                               RtecScheduler::Scheduling_Anomaly_Set_out anomalies)
{

  // Initialize the scheduler implementation.
  impl->init (minimum_priority, maximum_priority);

  // Construct an unbounded set to hold any scheduling anomalies.
  ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> anomaly_set;

  // Invoke the imlementation's scheduling method.
  BaseSchedImplType::status_t schedule_status;
  schedule_status = impl->schedule (anomaly_set);

  if (dependencies.ptr () == 0)
    {
      dependencies = new RtecScheduler::Dependency_Set ();
    }

  // Iterate over the set of anomalies, reporting each one, storing
  // it in the set of anomalies to return, and determining the worst
  // anomaly severity.
  RtecScheduler::Anomaly_Severity severity = RtecScheduler::ANOMALY_NONE;
  RtecScheduler::Scheduling_Anomaly **anomaly = 0;
  const char *anomaly_severity_msg = "NONE";
  CORBA::ULong anomaly_index = 0;
  CORBA::ULong anomaly_set_size =
                    static_cast<CORBA::ULong> (anomaly_set.size ());
  if (anomalies.ptr () == 0)
    {
      anomalies =
        new RtecScheduler::Scheduling_Anomaly_Set (anomaly_set_size);
    }
  anomalies->length (anomaly_set_size);
  ACE_Unbounded_Set_Iterator<RtecScheduler::Scheduling_Anomaly *>
    anomaly_iter (anomaly_set);
  for (anomaly_iter.first (), anomaly_index = 0;
       anomaly_iter.next (anomaly);
       anomaly_iter.advance (), ++anomaly_index)
    {
      if (0 == *anomaly)
        {
          // if for some reason we stored a null anomaly pointer,
          // just give default values to that entry in the sequence.
          anomalies[anomaly_index].severity = RtecScheduler::ANOMALY_NONE;
          anomalies[anomaly_index].description = "";
          continue;
        }

      // Keep track of the *worst* anomaly severity
      switch ((*anomaly)->severity)
        {
          case RtecScheduler::ANOMALY_FATAL:
            anomaly_severity_msg = "FATAL";
            severity = RtecScheduler::ANOMALY_FATAL;
            break;

          case RtecScheduler::ANOMALY_ERROR:
            anomaly_severity_msg = "ERROR";
            if (severity != RtecScheduler::ANOMALY_FATAL)
              {
                severity = RtecScheduler::ANOMALY_ERROR;
              }
            break;

          case RtecScheduler::ANOMALY_WARNING:
            anomaly_severity_msg = "WARNING";
            if ((severity != RtecScheduler::ANOMALY_FATAL) &&
                (severity != RtecScheduler::ANOMALY_ERROR))
              {
                severity = RtecScheduler::ANOMALY_WARNING;
              }
            break;

          // case RtecScheduler::ANOMALY_NONE:
          // case Anomaly_Severity_TAO_ENUM_32BIT_ENFORCER:
          default:
            anomaly_severity_msg = "UNKNOWN";
            break;
        }

        // Output the anomaly message
        ACE_DEBUG ((LM_DEBUG,
                    "%s: %s\n",
                    anomaly_severity_msg,
                    (*anomaly)->description.in ()));

        // Store the anomaly in the anomaly sequence out parameter
        anomalies[anomaly_index] = **anomaly;

        // Release the anomaly node.
        delete *anomaly;
    }

  switch (severity)
    {
      // On a fatal anomaly abort without generating a schedule.
      case RtecScheduler::ANOMALY_FATAL:
        // TODO: throw something.
        ACE_ERROR ((LM_ERROR, "Schedule failed due to FATAL anomaly.\n"));
        return;

      // Otherwise, make sure we didn't get a fatal return type.
      default:
        switch (schedule_status)
        {
          case BaseSchedImplType::ST_BAD_INTERNAL_POINTER :
            // TODO: throw something.
            ACE_ERROR ((LM_ERROR,
                        "Schedule failed due to bad internal pointer.\n"));
            return;

          case BaseSchedImplType::ST_VIRTUAL_MEMORY_EXHAUSTED :
            // TODO: throw something.
            ACE_ERROR ((LM_ERROR,
                        "Schedule failed due to insufficient memory.\n"));
            return;

          case BaseSchedImplType::THREAD_COUNT_MISMATCH :
            // TODO: throw something.
            ACE_ERROR ((LM_ERROR,
                        "Schedule failed due to thread count mismatch.\n"));
            return;

          case BaseSchedImplType::TASK_COUNT_MISMATCH :
            // TODO: throw something.
            ACE_ERROR ((LM_ERROR,
                        "Schedule failed due to task count mismatch.\n"));
            return;

          // Otherwise, go ahead and generate a schedule.
          default:
            break;
        }
        break;
    }

  // return the set of scheduled RT_Infos
  if (infos.ptr () == 0)
    {
      infos = new RtecScheduler::RT_Info_Set (impl->tasks ());
    }
  infos->length (impl->tasks ());
  for (RtecScheduler::handle_t handle = 1;
       handle <= static_cast<RtecScheduler::handle_t> (impl->tasks ());
       ++handle)
    {
      RtecScheduler::RT_Info* rt_info = 0;
      switch (impl->lookup_rt_info (handle, rt_info))
        {
        case BaseSchedImplType::SUCCEEDED:
          // We know that handles start at 1.
          infos[static_cast<CORBA::ULong> (handle - 1)] = *rt_info;
          break;
        case BaseSchedImplType::FAILED:
        case BaseSchedImplType::ST_UNKNOWN_TASK:
        default:
          ACE_ERROR ((LM_ERROR,
                      "Config_Scheduler::schedule - lookup_rt_info failed\n"));
          // TODO: throw something.
          break;
        }
    }

  // return the set of scheduled Config_Infos
  if (configs.ptr () == 0)
    {
      configs =
        new RtecScheduler::Config_Info_Set(impl->minimum_priority_queue () + 1);
    }
  configs->length (impl->minimum_priority_queue () + 1);
  for (RtecScheduler::Preemption_Priority_t priority = 0;
       priority <=
         static_cast<RtecScheduler::Preemption_Priority_t> (impl->minimum_priority_queue ());
       ++priority)
    {
      RtecScheduler::Config_Info* config_info = 0;
      switch (impl->lookup_config_info (priority, config_info))
        {
        case BaseSchedImplType::SUCCEEDED:
          // We know that handles start at 1.
          configs[CORBA::ULong(priority)] = *config_info;
          break;
        case BaseSchedImplType::FAILED:
        case BaseSchedImplType::ST_UNKNOWN_TASK:
        default:
          ACE_ERROR ((LM_ERROR,
                      "Config_Scheduler::schedule - "
                      "lookup_config_info failed\n"));
          // TODO: throw something.
          break;
        }
    }

  ACE_DEBUG ((LM_DEBUG, "Schedule prepared.\n"));
  ACE_DEBUG ((LM_DEBUG, "Dumping to stdout.\n"));
  ACE_Scheduler_Factory::dump_schedule (*(infos.ptr()),
                                        *(dependencies.ptr()),
                                        *(configs.ptr()),
                                        *(anomalies.ptr()), 0);
  ACE_DEBUG ((LM_DEBUG, "Dump done.\n"));
}


void ACE_Config_Scheduler::dispatch_configuration (RtecScheduler::Preemption_Priority_t p_priority,
                                                   RtecScheduler::OS_Priority& priority,
                                                   RtecScheduler::Dispatching_Type_t & d_type)
{

  if (impl->dispatch_configuration (p_priority, priority, d_type) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Config_Scheduler::dispatch_configuration -"
                  " dispatch_configuration failed\n"));
      // TODO: throw something.
    }
}
  // provide the thread priority and queue type for the given priority level


RtecScheduler::Preemption_Priority_t
ACE_Config_Scheduler::last_scheduled_priority (void)
{

  RtecScheduler::Preemption_Priority_t priority = impl->minimum_priority_queue ();

  if (priority < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Config_Scheduler::last_scheduled_priority - priorities failed\n"));
      // TODO: throw something.
    }

  return priority;
}
  // Returns the last priority number assigned to an operation in the schedule.
  // The number returned is one less than the total number of scheduled priorities.
  // All scheduled priorities range from 0 to the number returned, inclusive.

void
ACE_Config_Scheduler::get_config_infos (RtecScheduler::Config_Info_Set_out configs)
{
  ACE_UNUSED_ARG ((configs));

  //TODO: fill the Config_Info_Set with the runtime Config_Infos
  //for now, this function is unimplemented
  return;
}

void ACE_Config_Scheduler::reset (RtecScheduler::handle_t,
            RtecScheduler::Criticality_t,
            RtecScheduler::Time,
            RtecScheduler::Time,
            RtecScheduler::Time,
            RtecScheduler::Period_t,
            RtecScheduler::Importance_t,
            RtecScheduler::Quantum_t,
            CORBA::Long,
            RtecScheduler::Info_Type_t)
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::set_seq (const RtecScheduler::RT_Info_Set&)
// Set characteristics of the RT_Infos corresponding to the passed handles.
// Tuples are added in the case of existing and/or multiple definitions.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::reset_seq (const RtecScheduler::RT_Info_Set&)
// Reset characteristics of the RT_Infos corresponding to the passed handles.
// Tuples are replaced in the case of existing and/or multiple definitions.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::replace_seq (const RtecScheduler::RT_Info_Set&)
// Replace all RT_Infos, resetting characteristics of the RT_Infos
// corresponding to the passed handles.  All other RT_Infos are
// reset to their uninitialized values, i.e., the same they have
// just after the create call.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::remove_dependency (RtecScheduler::handle_t,
                                RtecScheduler::handle_t,
                                CORBA::Long,
                                RtecScheduler::Dependency_Type_t)
// This method removes a dependency between two RT_Infos.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::set_dependency_enable_state (RtecScheduler::handle_t,
                                  RtecScheduler::handle_t,
                                  CORBA::Long,
                                  RtecScheduler::Dependency_Type_t,
                                  RtecScheduler::Dependency_Enabled_Type_t)
// This method sets the enable state of a dependency between two RT_Infos.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::set_dependency_enable_state_seq (const RtecScheduler::Dependency_Set &)
// This method sets the enable state of a sequence of dependencies.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::set_rt_info_enable_state (RtecScheduler::handle_t,
                               RtecScheduler::RT_Info_Enabled_Type_t)
// This method enables or disables an RT_Info.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::set_rt_info_enable_state_seq (
                  const RtecScheduler::RT_Info_Enable_State_Pair_Set &)
// This method enables or disables a sequence of RT_Infos.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::recompute_scheduling (CORBA::Long,
                           CORBA::Long,
                           RtecScheduler::Scheduling_Anomaly_Set_out)
// Recomputes the scheduling priorities, etc.
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::get_rt_info_set (RtecScheduler::RT_Info_Set_out)
// Returns the set of rt_infos, with their assigned priorities (as
// of the last schedule re-computation).
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::get_dependency_set (RtecScheduler::Dependency_Set_out)
// Returns the set of rt_infos, with their assigned priorities (as
// of the last schedule re-computation).
{
  throw CORBA::NO_IMPLEMENT ();
}

void ACE_Config_Scheduler::get_config_info_set (RtecScheduler::Config_Info_Set_out)
// Returns the set of config_infos, describing the appropriate
// number, types, and priority levels for the dispatching lanes.
{
  throw CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
