// $Id$

// This program exercises a run-time scheduler,
// using the config run generated C++ header file.

#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"
#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Naming/Naming_Server.h"

#include "Sched_Conf_Runtime.h"





int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil(poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Store a pointer to the local run-time scheduler.
      RtecScheduler::Scheduler_ptr runtime_scheduler =
      ACE_Scheduler_Factory::server ();

      // Some useful variables for retrieving values from the scheduler.
      RtecScheduler::RT_Info *rt_info = 0;
      RtecScheduler::OS_Priority priority;
      RtecScheduler::Preemption_Subpriority_t subpriority;
      RtecScheduler::Preemption_Priority_t p_priority;
      RtecScheduler::Dispatching_Type_t dispatching_type;

      for (int i = 0; i < infos_size; ++i)
        {
          // Make sure the correct handle is returned by the
          // run-time scheduler's create and lookup methods.
          ACE_ASSERT (infos [i].handle ==
                      runtime_scheduler->create (infos [i].entry_point));

          ACE_ASSERT (infos [i].handle ==
                      runtime_scheduler->lookup (infos [i].entry_point));

          // Make sure the values in the RT_Info returned by get are OK.
          delete rt_info;
          rt_info = runtime_scheduler->get (infos [i].handle);

          ACE_ASSERT (rt_info != 0);
          ACE_ASSERT (ACE_OS::strcmp (rt_info->entry_point,
                                      infos [i].entry_point) == 0);
          ACE_ASSERT (rt_info->handle ==
            infos [i].handle);
          ACE_ASSERT (rt_info->worst_case_execution_time ==
            infos [i].worst_case_execution_time);
          ACE_ASSERT (rt_info->typical_execution_time ==
            infos [i].typical_execution_time);
          ACE_ASSERT (rt_info->cached_execution_time ==
            infos [i].cached_execution_time);
          ACE_ASSERT (rt_info->period ==
            infos [i].period);
          ACE_ASSERT (rt_info->criticality ==
            static_cast<RtecScheduler::Criticality_t> (infos [i].criticality));
          ACE_ASSERT (rt_info->importance ==
            static_cast<RtecScheduler::Importance_t> (infos [i].importance));
          ACE_ASSERT (rt_info->quantum ==
            infos [i].quantum);
          ACE_ASSERT (rt_info->threads ==
            infos [i].threads);
          ACE_ASSERT (rt_info->priority ==
            infos [i].priority);
          ACE_ASSERT (rt_info->preemption_subpriority ==
            infos [i].static_subpriority);
          ACE_ASSERT (rt_info->preemption_priority ==
            infos [i].preemption_priority);
          ACE_ASSERT (rt_info->info_type ==
            static_cast<RtecScheduler::Info_Type_t> (infos [i].info_type));

          // Make sure the values in the scheduler's internal RT_Info are OK.
          // Scheduler will generate messages if any values don't match.
          runtime_scheduler->set (infos [i].handle,
                                  static_cast<RtecScheduler::Criticality_t> (infos [i].criticality),
                                  infos [i].worst_case_execution_time,
                                  infos [i].typical_execution_time,
                                  infos [i].cached_execution_time,
                                  infos [i].period,
                                  static_cast<RtecScheduler::Importance_t> (infos [i].importance),
                                  infos [i].quantum,
                                  infos [i].threads,
                                  static_cast<RtecScheduler::Info_Type_t> (infos [i].info_type));

          // Make sure the correct priority values are returned.
          runtime_scheduler->priority (infos [i].handle,
                                       priority,
                                       subpriority,
                                       p_priority);

          ACE_ASSERT (priority == infos [i].priority);
          ACE_ASSERT (subpriority == infos [i].static_subpriority);
          ACE_ASSERT (p_priority == infos [i].preemption_priority);
          runtime_scheduler->entry_point_priority (infos [i].entry_point,
                                                   priority,
                                                   subpriority,
                                                   p_priority);

          ACE_ASSERT (priority == infos [i].priority);
          ACE_ASSERT (subpriority == infos [i].static_subpriority);
          ACE_ASSERT (p_priority == infos [i].preemption_priority);
        }

      for (int j = 0; j < configs_size; ++j)
        {
          // Make sure the correct preemption priority is stored.
          ACE_ASSERT (j == configs [j].preemption_priority);

          // Make sure the correct queue configuration values are returned.
          runtime_scheduler->
            dispatch_configuration (configs [j].preemption_priority,
                                    priority,
                                    dispatching_type);

          ACE_ASSERT (priority == configs [j].thread_priority);
          ACE_ASSERT (dispatching_type == configs [j].dispatching_type);

        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SYS_EX");
    }

  return 0;
}
