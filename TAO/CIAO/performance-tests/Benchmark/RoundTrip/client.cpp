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

const char *ior = "file://test.ior";
int niterations = 100;
int do_dump_history = 0;

int 
parse_args (int argc, char *argv[])
{
  //parse arguments
  ACE_Get_Opt get_opts (argc, argv, "hxk:i:");
  char c;
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
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-h (dump history) "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
set_priority () 
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

}
  

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      //set priority on the platform
      set_priority();

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);

      // Resolve HomeFinder interface
      CORBA::Object_var obj
        = orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Benchmark::RoundTripHome_var home
        = Benchmark::RoundTripHome::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (home.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire TestHome objref\n"), -1);

      Benchmark::RoundTrip_var test
        = home->create (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //Get the RoundTrip reference
      Benchmark::LatencyTest_var round_trip = test->provide_latency ();

      //Warmup the System by making some empty calls
      long start = 0L;
      for (int j = 0; j < 100; ++j)
        {
          round_trip->makeCall (start ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ///// Start Test ////////////////////////////////////////////
      ACE_Sample_History history (niterations);

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      for (int i = 0; i < niterations; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          //Test value to be sent to the server
          long test = 0;

          (void) round_trip->makeCall (test ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          history.sample (now - start);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "test finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception Handled:");
      return 1;
    }

  ACE_ENDTRY;
  
  return 0;
}
