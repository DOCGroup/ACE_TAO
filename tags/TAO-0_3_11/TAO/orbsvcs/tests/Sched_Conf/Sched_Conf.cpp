// $Id$

// This program performa a simple scheduler configuration run,
// and dumps the results of one scheduling into a C++ header file.

#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Naming/Naming_Utils.h"

ACE_RCSID(Sched_Conf, Sched_Conf, "$Id$")

const char* service_name = "ScheduleService";

const char* format_string = "{%-12s, %d, %d, %d, %d, %8d, "
                            "ACE_static_cast (RtecScheduler::Criticality_t, %d), "
                            "ACE_static_cast (RtecScheduler::Importance_t, %d), "
                            "%d, %d, %3d, %d, %d, "
                            "ACE_static_cast (RtecScheduler::Info_Type_t, %d)}\n";

int
parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "n:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          service_name = get_opt.optarg;
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
main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
  {
    return 1;
  }

  // create initial data for supplier and consumer operations
  const int operation_count = 16;
  ACE_Scheduler_Factory::POD_RT_Info config_infos[operation_count] = {
                  // 20 Hz high criticality supplier
                  { "high_20_S",            // entry point
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
                  // 20 Hz low criticality supplier
                  { "low_20_S",             // entry point
                        0,                  // handle
                        5000,               // worst case execution time
                        5000,               // typical execution time (unused)
                        5000,               // cached execution time
                        500000,             // period (100 ns)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        1,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 10 Hz high criticality supplier
                  { "high_10_S",            // entry point
                        0,                  // handle
                        10000,              // worst case execution time
                        10000,              // typical execution time (unused)
                        10000,              // cached execution time
                        1000000,            // period (100 ns)
                        RtecScheduler::HIGH_CRITICALITY,   // criticality
                        RtecScheduler::LOW_IMPORTANCE,     // importance
                        0,                  // quantum (unused)
                        1,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 10 Hz low criticality supplier
                  { "low_10_S",         // entry point
                        0,                  // handle
                        10000,              // worst case execution time
                        10000,              // typical execution time (unused)
                        10000,              // cached execution time
                        1000000,            // period (100 ns)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        1,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 5 Hz high criticality supplier
                  { "high_05_S",         // entry point
                        0,                  // handle
                        20000,              // worst case execution time
                        20000,              // typical execution time (unused)
                        20000,              // cached execution time
                        2000000,            // period (100 ns)
                        RtecScheduler::HIGH_CRITICALITY,   // criticality
                        RtecScheduler::LOW_IMPORTANCE,     // importance
                        0,                  // quantum (unused)
                        1,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 5 Hz low criticality supplier
                  { "low_05_S",         // entry point
                        0,                  // handle
                        20000,              // worst case execution time
                        20000,              // typical execution time (unused)
                        20000,              // cached execution time
                        2000000,            // period (100 ns)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        1,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 1 Hz high criticality supplier (declares a rate but no threads)
                  { "high_01_S",            // entry point
                        0,                  // handle
                        100000,             // worst case execution time
                        100000,             // typical execution time (unused)
                        100000,             // cached execution time
                        10000000,           // period (100 ns)
                        RtecScheduler::HIGH_CRITICALITY,   // criticality
                        RtecScheduler::LOW_IMPORTANCE,     // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 1 Hz low criticality supplier (remote dependant: scheduler should warn)
                  { "low_01_S",             // entry point
                        0,                  // handle
                        100000,             // worst case execution time
                        100000,             // typical execution time (unused)
                        100000,             // cached execution time
                        10000000,                  // period (100 ns)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 20 Hz high criticality consumer
                  { "high_20_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::HIGH_CRITICALITY,   // criticality
                        RtecScheduler::LOW_IMPORTANCE,     // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 20 Hz low criticality consumer
                  { "low_20_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 10 Hz high criticality consumer
                  { "high_10_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::HIGH_CRITICALITY,   // criticality
                        RtecScheduler::LOW_IMPORTANCE,     // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 10 Hz low criticality consumer
                  { "low_10_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 5 Hz high criticality consumer
                  { "high_05_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::HIGH_CRITICALITY,   // criticality
                        RtecScheduler::LOW_IMPORTANCE,     // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 5 Hz low criticality consumer
                  { "low_05_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 1 Hz high criticality consumer
                  { "high_01_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::HIGH_CRITICALITY,   // criticality
                        RtecScheduler::LOW_IMPORTANCE,     // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  },
                  // 1 Hz low criticality consumer
                  { "low_01_C",        // entry point
                        0,                  // handle
                        0,                  // worst case execution time
                        0,                  // typical execution time (unused)
                        0,                  // cached execution time
                        0,                  // period (zero)
                        RtecScheduler::LOW_CRITICALITY,    // criticality
                        RtecScheduler::HIGH_IMPORTANCE,    // importance
                        0,                  // quantum (unused)
                        0,                  // threads
                        0,                  // OS priority
                        0,                  // Preemption subpriority
                        0,                  // Preemption priority
                        RtecScheduler::OPERATION           // info type
                  }
  };


  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil(poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the naming services
      TAO_Naming_Client my_name_client;
      if (my_name_client.init (orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      if (ACE_Scheduler_Factory::use_config (my_name_client.get_context (),
                                             service_name) < 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          " (%P|%t) Unable to bind to the scheduling service.\n"),
                          1);
          }
          // create and initialize RT_Infos in the scheduler,
          // make second half of array depend on first half.
          for (int i = 0; i < operation_count; ++i)
          {
                // create the RT_Info
            config_infos[i].handle =
                  ACE_Scheduler_Factory::server ()->create (config_infos[i].entry_point,
                                                            TAO_TRY_ENV);

                // initialize the RT_Info
                ACE_Scheduler_Factory::server ()->
              set (config_infos[i].handle,
                  ACE_static_cast (RtecScheduler::Criticality_t, config_infos[i].criticality),
               config_infos[i].worst_case_execution_time,
               config_infos[i].typical_execution_time,
               config_infos[i].cached_execution_time,
               config_infos[i].period,
               ACE_static_cast (RtecScheduler::Importance_t, config_infos[i].importance),
               config_infos[i].quantum,
               config_infos[i].threads,
               ACE_static_cast (RtecScheduler::Info_Type_t, config_infos[i].info_type),
                           TAO_TRY_ENV);

        // make operations in second half dependant on
                // operations in the first half of the array,
                // and have each called twice as a oneway call
                if (i >= (operation_count / 2))
                {
                  ACE_Scheduler_Factory::server ()->
                add_dependency (config_infos[i].handle,
                                        config_infos[i - (operation_count / 2)].handle,
                                                        2,                             // number of calls
                                                        RtecScheduler::ONE_WAY_CALL,   // type of dependency
                                        TAO_TRY_ENV);
                }
      }

      RtecScheduler::RT_Info_Set_var infos;
      RtecScheduler::Config_Info_Set_var configs;
      RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

#if defined (__SUNPRO_CC)
      // Sun C++ 4.2 warns with the code below:
      //   Warning (Anachronism): Temporary used for non-const
      //   reference, now obsolete.
      //   Note: Type "CC -migration" for more on anachronisms.
      //   Warning (Anachronism): The copy constructor for argument
      //   infos of type RtecScheduler::RT_Info_Set_out should take
      //   const RtecScheduler::RT_Info_Set_out&.
      // But, this code is not CORBA conformant, because users should
      // not define instances of _out types.

      RtecScheduler::RT_Info_Set_out infos_out (infos);
      RtecScheduler::Config_Info_Set_out configs_out (configs);
      RtecScheduler::Scheduling_Anomaly_Set_out anomalies_out (anomalies);
      ACE_Scheduler_Factory::server ()->compute_scheduling
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         infos_out, configs_out, anomalies_out, TAO_TRY_ENV);
#else  /* ! __SUNPRO_CC */
      ACE_Scheduler_Factory::server ()->compute_scheduling
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         infos.out (), configs.out (), anomalies.out (), TAO_TRY_ENV);
#endif /* ! __SUNPRO_CC */

      TAO_CHECK_ENV;

      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            configs.in (),
                                            anomalies.in (),
                                            "Sched_Conf_Runtime.h",
                                            format_string);
    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

  return 0;
}
