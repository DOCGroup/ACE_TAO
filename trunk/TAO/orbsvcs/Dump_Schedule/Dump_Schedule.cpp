
// $Id$

#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_main.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"

ACE_RCSID (Dump_Schedule,
           Dump_Schedule,
           "$Id$")

// This program dumps the results of one scheduling in a C++ file.

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (command_line.get_argc(), command_line.get_ASCII_argv(), "internet" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil(naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to find the Naming Service\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
         infos.out (), deps.out (), configs.out (), anomalies.out ()
         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            deps.in (),
                                            configs.in (),
                                            anomalies.in (),
                                            "Scheduler_Runtime.cpp");
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Dump_Schedule");
    }
  ACE_ENDTRY;

  return 0;
}
