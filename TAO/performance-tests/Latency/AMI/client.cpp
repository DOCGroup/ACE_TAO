#include "Roundtrip_Handler.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/OS_NS_errno.h"

struct Client_Options
{
  ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");
  int iterations = 1000;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage: %s -k <ior> -i <niterations>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  // Enable FIFO scheduling
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

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (options.ior);
      Test::Roundtrip_var roundtrip = Test::Roundtrip::_narrow (object.in ());

      if (CORBA::is_nil (roundtrip.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("Nil Test::Roundtrip reference <%s>\n"),
                           options.ior),
                          1);

      for (int i = 0; i < 100; ++i)
        {
          ACE_hrtime_t const start {};
          (void) roundtrip->test_method (start);
        }

      Roundtrip_Handler *roundtrip_handler_impl {};
      ACE_NEW_RETURN (roundtrip_handler_impl,
                      Roundtrip_Handler (options.iterations),
                      1);
      PortableServer::ServantBase_var owner_transfer (roundtrip_handler_impl);

      Test::AMI_RoundtripHandler_var roundtrip_handler =
        roundtrip_handler_impl->_this ();

      poa_manager->activate ();

      ACE_hrtime_t const test_start = ACE_OS::gethrtime ();

      for (int i = 0; i != options.iterations; ++i)
        {
          roundtrip->sendc_test_method (roundtrip_handler.in (),
                                        ACE_OS::gethrtime ());
          if (orb->work_pending ())
            orb->perform_work ();
        }

      ACE_Time_Value tv (0, 2000);

      while (roundtrip_handler_impl->pending_callbacks ())
        orb->perform_work (tv);

      ACE_hrtime_t const test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_High_Res_Timer::global_scale_factor_type const gsf =
        ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      roundtrip_handler_impl->dump_results (gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                             test_end - test_start,
                                             options.iterations);

      roundtrip->shutdown ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught: ");
      return 1;
    }

  return 0;
}
