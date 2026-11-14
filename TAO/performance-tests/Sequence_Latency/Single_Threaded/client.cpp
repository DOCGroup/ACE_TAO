#include "TestC.h"

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_errno.h"

#include "tao/Strategies/advanced_resource.h"

struct Client_Options
{
  ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");
  ACE_TCHAR const *data_type = ACE_TEXT("octet");
  int iterations = 100;
  int size = 512;
  bool dump_history = false;
  bool shutdown = true;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("ht:xs:k:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        options.dump_history = true;
        break;
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
      case 'x':
        options.shutdown = false;
        break;
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 's':
        options.size = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage: %s -k <ior> -i <niterations> -x "
                                    "(disable shutdown) -h (dump history)\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

template <typename Load>
void
test_sequence (Test::Roundtrip_ptr roundtrip,
               Client_Options const &options,
               Test::Timestamp (Test::Roundtrip::*operation) (Load const &,
                                                              Test::Timestamp))
{
  ACE_Sample_History history (options.iterations);
  Load load (options.size);
  load.length (options.size);

  ACE_hrtime_t const test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < options.iterations; ++i)
    {
      ACE_hrtime_t const start = ACE_OS::gethrtime ();
      (void) (roundtrip->*operation) (load, start);
      ACE_hrtime_t const now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }

  ACE_hrtime_t const test_end = ACE_OS::gethrtime ();
  ACE_DEBUG ((LM_DEBUG, "test finished\n"));
  ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
  ACE_High_Res_Timer::global_scale_factor_type const gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));

  if (options.dump_history)
    history.dump_samples (ACE_TEXT("HISTORY"), gsf);

  ACE_Basic_Stats stats;
  history.collect_basic_stats (stats);
  stats.dump_results (ACE_TEXT("Total"), gsf);
  ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                         test_end - test_start,
                                         stats.samples_count ());
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

      if (ACE_OS::strcmp (options.data_type, ACE_TEXT("octet")) == 0)
        test_sequence<Test::octet_load> (roundtrip.in (), options,
                                         &Test::Roundtrip::test_octet_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("char")) == 0)
        test_sequence<Test::char_load> (roundtrip.in (), options,
                                        &Test::Roundtrip::test_char_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("long")) == 0)
        test_sequence<Test::long_load> (roundtrip.in (), options,
                                        &Test::Roundtrip::test_long_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("short")) == 0)
        test_sequence<Test::short_load> (roundtrip.in (), options,
                                         &Test::Roundtrip::test_short_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("double")) == 0)
        test_sequence<Test::double_load> (roundtrip.in (), options,
                                          &Test::Roundtrip::test_double_method);
      else if (ACE_OS::strcmp (options.data_type, ACE_TEXT("longlong")) == 0)
        test_sequence<Test::longlong_load> (roundtrip.in (), options,
                                            &Test::Roundtrip::test_longlong_method);

      if (options.shutdown)
        roundtrip->shutdown ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
