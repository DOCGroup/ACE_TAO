// $Id$

#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"

ACE_RCSID(Dump_Schedule, Dump_Schedule, "$Id$")

// This program dumps the results of one scheduling in a C++ file.

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet" ACE_ENV_ARG_PARAMETER);
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

      const char *name = 0;
      if (argc > 1)
        {
          name = argv[1];
        }
      ACE_Scheduler_Factory::use_config (naming_context.in (), name);

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
         infos_out, configs_out, anomalies_out
         ACE_ENV_ARG_PARAMETER); // FUZZ: ignore check_for_ace_check
#else  /* ! __SUNPRO_CC */
      ACE_Scheduler_Factory::server ()->compute_scheduling
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                         ACE_SCOPE_THREAD),
         infos.out (), configs.out (), anomalies.out ()
         ACE_ENV_ARG_PARAMETER); // FUZZ: ignore check_for_ace_check
#endif /* ! __SUNPRO_CC */

      ACE_TRY_CHECK;

      ACE_Scheduler_Factory::dump_schedule (infos.in (),
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
