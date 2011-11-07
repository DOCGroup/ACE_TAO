// $Id$

// This program performa a simple scheduler configuration run,
// and dumps the results of one scheduling into a C++ header file.

#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Naming/Naming_Server.h"



const char* service_name = "ScheduleService";

const char* format_string = "{%-12s, %d, %d, %d, %d, %8d, "
                            "static_cast<RtecScheduler::Criticality_t> (%d), "
                            "static_cast<RtecScheduler::Importance_t> (%d), "
                            "%d, %d, %3d, %d, %d, "
                            "static_cast<RtecScheduler::Info_Type_t> (%d)}\n";

int
parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          service_name = get_opt.opt_arg ();
          break;
        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n service_name "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) != 0)
  {
    return 1;
  }

  // create initial data for supplier and consumer operations
  const int operation_count = 8;
  ACE_Scheduler_Factory::POD_RT_Info config_infos[operation_count] = {
                  // good supplier (no unresolved dependencies)
                  { "good_supplier",    // entry point
                    0,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    500000,             // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    1,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::OPERATION           // info type
                  },
                  // good consumer (no unresolved dependencies)
                  { "good_consumer",    // entry point
                    1,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    0,                  // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    0,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::OPERATION           // info type
                  },
                  // supplier with unresolved remote dependencies
                  { "unresolved_remote_supplier",    // entry point
                    2,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    0,                  // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    0,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::REMOTE_DEPENDANT    // info type
                  },
                  // consumer with unresolved remote dependencies
                  { "unresolved_remote_consumer",    // entry point
                    3,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    0,                  // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    0,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::OPERATION           // info type
                  },
                  // supplier with unresolved local dependencies
                  { "unresolved_local_supplier",    // entry point
                    4,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    0,                  // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    0,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::OPERATION           // info type
                  },
                  // consumer with unresolved local dependencies
                  { "unresolved_local_consumer",    // entry point
                    5,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    0,                  // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    0,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::OPERATION           // info type
                  },
                  // consumer with unresolved local and remote dependencies
                  { "both_unresolved_consumer_1",    // entry point
                    6,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    0,                  // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    0,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::OPERATION           // info type
                  },
                  // another consumer with unresolved
                  // local and remote dependencies
                  { "both_unresolved_consumer_2",    // entry point
                    7,                  // handle
                    5000,               // worst case execution time
                    5000,               // typical execution time (unused)
                    5000,               // cached execution time
                    0,                  // period (100 ns)
                    RtecScheduler::HIGH_CRITICALITY,   // criticality
                    RtecScheduler::LOW_IMPORTANCE,     // importance
                    0,                  // quantum (unused)
                    0,                  // threads
                    0,                  // OS priority
                    0,                  // Preemption subpriority
                    0,                  // Preemption priority
                    RtecScheduler::OPERATION           // info type
                  }
  };

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

      // Initialize the naming services
      TAO_Naming_Client my_name_client;
      if (my_name_client.init (orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      CosNaming::NamingContext_var context =
        my_name_client.get_context ();

      if (ACE_Scheduler_Factory::use_config (context.in (),
                                             service_name) < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to bind to the scheduling service.\n"),
                          1);

      // create and initialize RT_Infos in the scheduler,
      // make second half of array depend on first half.
      for (int i = 0; i < operation_count; ++i)
        {
          // create the RT_Info
          config_infos[i].handle =
            ACE_Scheduler_Factory::server ()->create (config_infos[i].entry_point);

          // initialize the RT_Info
          ACE_Scheduler_Factory::server ()->
            set (config_infos[i].handle,
                 static_cast<RtecScheduler::Criticality_t> (config_infos[i].criticality),
                 config_infos[i].worst_case_execution_time,
                 config_infos[i].typical_execution_time,
                 config_infos[i].cached_execution_time,
                 config_infos[i].period,
                 static_cast<RtecScheduler::Importance_t> (config_infos[i].importance),
                 config_infos[i].quantum,
                 config_infos[i].threads,
                 static_cast<RtecScheduler::Info_Type_t> (config_infos[i].info_type));
        }


      // register dependency of good consumer on good supplier
      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[1].handle,
                        config_infos[0].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      // register dependency of consumer that will have unresolved remote
      // dependencies on supplier with unresolved remote dependencies
      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[3].handle,
                        config_infos[2].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );


      // register dependency of consumer that will have unresolved local
      // dependencies on supplier with unresolved local dependencies
      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[5].handle,
                        config_infos[4].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );


      // register dependencies on each supplier of first consumer that will
      // have both unresolved local and unresolved remote dependencies.
      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[6].handle,
                        config_infos[0].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[6].handle,
                        config_infos[2].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[6].handle,
                        config_infos[4].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      // Register dependencies on each of the other consumers by second
      // consumer that will have both unresolved local and unresolved remote
      // dependencies.
      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[7].handle,
                        config_infos[1].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[7].handle,
                        config_infos[3].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[7].handle,
                        config_infos[5].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      ACE_Scheduler_Factory::server ()->
        add_dependency (config_infos[7].handle,
                        config_infos[6].handle,
                        1,                            // number of calls
                        RtecBase::ONE_WAY_CALL  // type of dependency
                        );

      RtecScheduler::RT_Info_Set_var infos;
      RtecScheduler::Dependency_Set_var deps;
      RtecScheduler::Config_Info_Set_var configs;
      RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

      ACE_Scheduler_Factory::server ()->compute_scheduling
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         infos.out (), deps.out (),
         configs.out (), anomalies.out ());

      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            deps.in (),
                                            configs.in (),
                                            anomalies.in (),
                                            "Sched_Conf_Anomalies_Runtime.h",
                                            format_string);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SYS_EX");
    }

  return 0;
}
