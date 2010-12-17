// $Id$

#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_main.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"

// This program dumps the results of one scheduling in a C++ file.

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet");

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil(naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to find the Naming Service\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      const char *name = "ScheduleService";
      if (command_line.get_argc() > 1)
        {
          name = command_line.get_ASCII_argv()[1];
        }
      ACE_Scheduler_Factory::use_config (naming_context.in (), name);

      RtecScheduler::RT_Info_Set_var infos;
      RtecScheduler::Dependency_Set_var deps;
      RtecScheduler::Config_Info_Set_var configs;
      RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

      ACE_Scheduler_Factory::server ()->compute_scheduling
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         infos.out (), deps.out (), configs.out (), anomalies.out ());

      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            deps.in (),
                                            configs.in (),
                                            anomalies.in (),
                                            ACE_TEXT("Scheduler_Runtime.cpp"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Dump_Schedule");
    }

  return 0;
}
