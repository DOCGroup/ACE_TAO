// $Id$

#include "Roundtrip_Handler.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"

ACE_RCSID(AMI_Latency, client, "$Id$")

const char *ior = "file://test.ior";

ACE_hrtime_t throughput_base;

int niterations = 1000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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
                           "-p <period (msecs)> "
                           "-b <burst size> "
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
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (roundtrip.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Nil Test::Roundtrip reference <%s>\n",
                           ior),
                          1);

      for (int j = 0; j < 100; ++j)
        {
          ACE_hrtime_t start = 0;
          (void) roundtrip->test_method (start TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      Roundtrip_Handler *roundtrip_handler_impl;
      ACE_NEW_RETURN (roundtrip_handler_impl,
                      Roundtrip_Handler (niterations),
                      1);
      PortableServer::ServantBase_var owner_transfer(roundtrip_handler_impl);

      Test::AMI_RoundtripHandler_var roundtrip_handler =
        roundtrip_handler_impl->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();

      for (int i = 0; i != niterations; ++i)
         {
           // Invoke asynchronous operation....
           roundtrip->sendc_test_method (roundtrip_handler.in (),
                                         ACE_OS::gethrtime ()
                                         TAO_ENV_ARG_PARAMETER);
           if (orb->work_pending (TAO_ENV_SINGLE_ARG_PARAMETER))
              orb->perform_work (TAO_ENV_SINGLE_ARG_PARAMETER);

           ACE_TRY_CHECK;
         }

       ACE_Time_Value tv (0, 2000);

       while (roundtrip_handler_impl->pending_callbacks ())
         {
           orb->perform_work (tv TAO_ENV_ARG_PARAMETER);
           ACE_TRY_CHECK;
         }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      roundtrip_handler_impl->dump_results (gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             test_end - test_start,
                                             niterations);

      roundtrip->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
