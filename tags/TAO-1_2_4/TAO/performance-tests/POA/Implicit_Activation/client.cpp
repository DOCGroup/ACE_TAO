// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

ACE_RCSID(Activation, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 100;
int do_dump_history = 0;
int do_shutdown = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "hxk:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'x':
        do_shutdown = 0;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-x (disable shutdown) "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Factory_var factory =
        Test::Factory::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Factory reference <%s>\n",
                             ior),
                            1);
        }

      Test::Simple_Sequence references (niterations);
      references.length (niterations);

      ACE_Sample_History activation (niterations);

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      ACE_DEBUG ((LM_DEBUG, "Activating %d objects\n", niterations));
      for (int i = 0; i != niterations; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          references[i] =
            factory->create_simple_object (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          activation.sample (now - start);
        }
      ACE_DEBUG ((LM_DEBUG, "Activations completed\n"));

      if (do_dump_history)
        {
          activation.dump_samples ("ACTIVATION_HISTORY", gsf);
        }

      ACE_Basic_Stats activation_stats;
      activation.collect_basic_stats (activation_stats);
      activation_stats.dump_results ("Activation", gsf);

      ACE_Sample_History destruction (niterations);

      ACE_DEBUG ((LM_DEBUG, "Destroying %d objects\n", niterations));
      for (int j = 0; j != niterations; ++j)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          references[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          destruction.sample (now - start);
        }
      ACE_DEBUG ((LM_DEBUG, "Destructions completed\n"));

      if (do_dump_history)
        {
          destruction.dump_samples ("DESTRUCTION_HISTORY", gsf);
        }

      ACE_Basic_Stats destruction_stats;
      destruction.collect_basic_stats (destruction_stats);
      destruction_stats.dump_results ("Destruction", gsf);

      if (do_shutdown)
        {
          factory->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
