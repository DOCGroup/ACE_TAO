// $Id$

//==============================================================
/**
 * @file client.cpp
 *
 * This is a simple client test program that interact with the RoundTrip
 * component implementation. The single threaded client issues
 * two-way operations, the total latency (response time) is measured
 * NOTE: this client implementation has been adapted from the
 * $TAO_ROOT/performance-results/Latency/Single_Threaded/
 *
 * @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//==============================================================
#include "RoundTripC.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_errno.h"
#include "ace/Throughput_Stats.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 100;
int do_dump_history = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  // Parse arguments.
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("hxk:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        /* Could be any other parameter like -ORBSvcConf file so ignore */
       break;
      }

  // Indicates successful parsing of the command line
  return 0;
}

void
set_priority ()
{
  int const priority =
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

}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Set priority on the platform
      set_priority();

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args(argc, argv) == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "Error parsing Arguments\n"));
          return -1;
        }

      // Resolve HomeFinder interface
      CORBA::Object_var obj
        = orb->string_to_object (ior);

      Benchmark::RoundTripHome_var home
        = Benchmark::RoundTripHome::_narrow (obj.in ());

      if (CORBA::is_nil (home.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire TestHome objref\n"), -1);

      Benchmark::RoundTrip_var test = home->create ();

      // Get the RoundTrip reference
      obj = test->provide_facet ("latency");
      Benchmark::LatencyTest_var round_trip =
        Benchmark::LatencyTest::_narrow (obj.in ());

      if (CORBA::is_nil (round_trip.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire LatencyTest objref\n"), -1);

      // Warmup the System by making some empty calls
      for (int j = 0; j < 100; ++j)
        {
          round_trip->makeCall (0L);
        }

      ///// Start Test ////////////////////////////////////////////
      ACE_Sample_History history (niterations);

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      for (int i = 0; i < niterations; ++i)
        {
          ACE_hrtime_t const start = ACE_OS::gethrtime ();

          // Test value to be sent to the server
          long test = 0;

          (void) round_trip->makeCall (test);

          ACE_hrtime_t const now = ACE_OS::gethrtime ();
          history.sample (now - start);
        }

      ACE_hrtime_t const test_end = ACE_OS::gethrtime ();

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

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception Handled:");
      return 1;
    }


  return 0;
}
