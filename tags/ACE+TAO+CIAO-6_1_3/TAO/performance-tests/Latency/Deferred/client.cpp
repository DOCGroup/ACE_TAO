// $Id$

#include "tao/DynamicInterface/Request.h"  /* This must come first for
                                              G++ 3.4 or better */

#include "TestC.h"

#include "tao/AnyTypeCode/TypeCode_Constants.h"

#include "tao/Strategies/advanced_resource.h"

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_errno.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 1000;
int burst = 10;
int do_shutdown = 1;
int do_dump_history = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("hxk:i:b:"));
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

      case 'b':
        burst = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-b <burst> "
                           "-x (disable shutdown) "
                           "-h (dump history) "
                           "\n",
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

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in ());

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             ior),
                            1);
        }

      for (int j = 0; j < 100; ++j)
        {
          ACE_hrtime_t start = 0;
          (void) roundtrip->test_method (start);
        }

      ACE_Sample_History history (niterations);

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();

      CORBA::Request_var *request;
      ACE_NEW_RETURN (request, CORBA::Request_var[burst], 1);

      for (int i = 0; i < niterations; ++i)
        {
          int j;

          for (j = 0; j != burst; ++j)
            {
              CORBA::ULongLong start = ACE_OS::gethrtime ();

              request[j] =
                roundtrip->_request ("test_method");

              request[j]->add_in_arg () <<= start;
              request[j]->set_return_type (CORBA::_tc_ulonglong);

              request[j]->send_deferred ();
            }

          for (j = 0; j != burst; ++j)
            {
              request[j]->get_response ();

              CORBA::ULongLong retval;
              if ((request[j]->return_value () >>= retval) == 1)
                {
                  ACE_hrtime_t now = ACE_OS::gethrtime ();
                  history.sample (ACE_HRTIME_TO_U64(now) - retval);
                }
            }
        }
      delete[] request;

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "test finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_High_Res_Timer::global_scale_factor_type gsf =
        ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      if (do_dump_history)
        {
          history.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results (ACE_TEXT("Total"), gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

      if (do_shutdown)
        {
          roundtrip->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
