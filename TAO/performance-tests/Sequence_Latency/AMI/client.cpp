#include "Roundtrip_Handler.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/Time_Value.h"

struct Client_Options
{
  ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");
  ACE_TCHAR const *data_type = ACE_TEXT("octet");
  int iterations = 1000;
  int size = 512;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:s:k:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        options.data_type = get_opts.opt_arg ();
        if (ACE_OS::strcmp (options.data_type, ACE_TEXT("octet")) != 0 &&
            ACE_OS::strcmp (options.data_type, ACE_TEXT("char")) != 0 &&
            ACE_OS::strcmp (options.data_type, ACE_TEXT("long")) != 0 &&
            ACE_OS::strcmp (options.data_type, ACE_TEXT("short")) != 0 &&
            ACE_OS::strcmp (options.data_type, ACE_TEXT("double")) != 0 &&
            ACE_OS::strcmp (options.data_type, ACE_TEXT("longlong")) != 0)
          return -1;
        break;
      case 's':
        options.size = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage: %s -t <datatype> -s <size> "
                                    "-k <ior> -i <niterations>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

template <typename Load>
void
test_sequence (Test::Roundtrip_ptr roundtrip,
               CORBA::ORB_ptr orb,
               Test::AMI_RoundtripHandler_ptr roundtrip_handler,
               Roundtrip_Handler *roundtrip_handler_impl,
               Client_Options const &options,
               void (Test::Roundtrip::*operation) (Test::AMI_RoundtripHandler_ptr,
                                                   Load const &,
                                                   Test::Timestamp))
{
  Load load (options.size);
  load.length (options.size);
  Test::AMI_RoundtripHandler_var handler =
    Test::AMI_RoundtripHandler::_duplicate (roundtrip_handler);

  ACE_hrtime_t const test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < options.iterations; ++i)
    {
      (roundtrip->*operation) (handler.in (), load, ACE_OS::gethrtime ());
      if (orb->work_pending ())
        orb->perform_work ();
    }

  ACE_Time_Value timeout (0, 2000);
  while (roundtrip_handler_impl->pending_callbacks ())
    orb->perform_work (timeout);

  ACE_hrtime_t const test_end = ACE_OS::gethrtime ();
  ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
  ACE_High_Res_Timer::global_scale_factor_type const gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));
  roundtrip_handler_impl->dump_results (gsf);
  ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                         test_end - test_start,
                                         options.iterations);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int const priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_DEBUG ((LM_DEBUG,
                    "client (%P|%t): user is not superuser, "
                    "test runs in time-shared class\n"));
      else
        ACE_ERROR ((LM_ERROR, "client (%P|%t): sched_params failed\n"));
    }

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, " (%P|%t) Unable to initialize the POA.\n"), 1);

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
      CORBA::Object_var object = orb->string_to_object (options.ior);
      Test::Roundtrip_var roundtrip = Test::Roundtrip::_narrow (object.in ());
      if (CORBA::is_nil (roundtrip.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("Nil Test::Roundtrip reference <%s>\n"),
                           options.ior),
                          1);

      Test::octet_load initial_load;
      for (int i = 0; i < 100; ++i)
        {
          ACE_hrtime_t start {};
          (void) roundtrip->test_octet_method (initial_load, start);
        }

      Roundtrip_Handler *roundtrip_handler_impl {};
      ACE_NEW_RETURN (roundtrip_handler_impl,
                      Roundtrip_Handler (options.iterations),
                      1);
      PortableServer::ServantBase_var owner_transfer (roundtrip_handler_impl);
      Test::AMI_RoundtripHandler_var roundtrip_handler = roundtrip_handler_impl->_this ();
      poa_manager->activate ();

      if (ACE_OS::strcmp (options.data_type, ACE_TEXT("octet")) == 0)
        test_sequence<Test::octet_load> (roundtrip.in (), orb.in (),
                                         roundtrip_handler.in (),
                                         roundtrip_handler_impl, options,
                                         &Test::Roundtrip::sendc_test_octet_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("char")) == 0)
        test_sequence<Test::char_load> (roundtrip.in (), orb.in (),
                                        roundtrip_handler.in (),
                                        roundtrip_handler_impl, options,
                                        &Test::Roundtrip::sendc_test_char_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("long")) == 0)
        test_sequence<Test::long_load> (roundtrip.in (), orb.in (),
                                        roundtrip_handler.in (),
                                        roundtrip_handler_impl, options,
                                        &Test::Roundtrip::sendc_test_long_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("short")) == 0)
        test_sequence<Test::short_load> (roundtrip.in (), orb.in (),
                                         roundtrip_handler.in (),
                                         roundtrip_handler_impl, options,
                                         &Test::Roundtrip::sendc_test_short_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("double")) == 0)
        test_sequence<Test::double_load> (roundtrip.in (), orb.in (),
                                          roundtrip_handler.in (),
                                          roundtrip_handler_impl, options,
                                          &Test::Roundtrip::sendc_test_double_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("longlong")) == 0)
        test_sequence<Test::longlong_load> (roundtrip.in (), orb.in (),
                                            roundtrip_handler.in (),
                                            roundtrip_handler_impl, options,
                                            &Test::Roundtrip::sendc_test_longlong_method);

      roundtrip->shutdown ();
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
