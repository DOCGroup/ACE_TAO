#include "tao/DynamicInterface/Request.h"  /* This must come first for
                                              G++ 3.4 or better */

#include "TestC.h"

#include "tao/AnyTypeCode/TypeCode_Constants.h"

#include "tao/Strategies/advanced_resource.h"

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_errno.h"
#include <memory>
#include <new>

struct Client_Options
{
  ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");
  int iterations = 1000;
  int burst = 10;
  bool shutdown = true;
  bool dump_history = false;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("hxk:i:b:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        options.dump_history = true;
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

      case 'b':
        options.burst = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage: %s -k <ior> -i <niterations> -b <burst> -x -h\n"),
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

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (options.ior);
      Test::Roundtrip_var roundtrip = Test::Roundtrip::_narrow (object.in ());

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("Nil Test::Roundtrip reference <%s>\n"),
                             options.ior),
                            1);
        }

      for (int i = 0; i < 100; ++i)
        {
          ACE_hrtime_t const start {};
          (void) roundtrip->test_method (start);
        }

      ACE_Sample_History history (options.iterations);

      ACE_hrtime_t const test_start = ACE_OS::gethrtime ();

      std::unique_ptr<CORBA::Request_var[]> requests {
        new (std::nothrow) CORBA::Request_var[options.burst]
      };
      if (!requests)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot allocate request array\n"), 1);

      for (int i = 0; i < options.iterations; ++i)
        {
          for (int j = 0; j != options.burst; ++j)
            {
              CORBA::ULongLong const start = ACE_OS::gethrtime ();
              requests [j] = roundtrip->_request ("test_method");
              requests [j]->add_in_arg () <<= start;
              requests [j]->set_return_type (CORBA::_tc_ulonglong);
              requests [j]->send_deferred ();
            }

          for (int j = 0; j != options.burst; ++j)
            {
              requests [j]->get_response ();
              CORBA::ULongLong retval {};
              if ((requests [j]->return_value () >>= retval) == 1)
                {
                  ACE_hrtime_t const now = ACE_OS::gethrtime ();
                  history.sample (ACE_HRTIME_TO_U64(now) - retval);
                }
            }
        }

      ACE_hrtime_t const test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "test finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_High_Res_Timer::global_scale_factor_type const gsf =
        ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      if (options.dump_history)
        {
          history.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results (ACE_TEXT("Total"), gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

      if (options.shutdown)
        {
          roundtrip->shutdown ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
