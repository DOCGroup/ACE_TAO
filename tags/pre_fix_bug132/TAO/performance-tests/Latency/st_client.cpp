// $Id$

#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

#include "tao/Strategies/advanced_resource.h"

ACE_RCSID(Latency, st_client, "$Id$")

//#define USING_QUANTIFY 1

#if defined (USING_QUANTIFY)

#if defined (ACE_WIN32)

#include "pure.h"

#else /* !ACE_WIN32 */

#include "quantify.h"

inline int QuantifyClearData ()
{
  return quantify_clear_data ();
}

inline int QuantifyStartRecordingData ()
{
  return quantify_start_recording_data ();
}

inline int QuantifyStopRecordingData ()
{
  return quantify_stop_recording_data ();
}

#endif /* ACE_WIN32 */

#endif /* USING_QUANTIFY */


const char *ior = "file://test.ior";
int niterations = 100;
int period = -1;
int do_dump_history = 0;
int do_shutdown = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "hk:i:p:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'k':
        ior = get_opts.optarg;
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'p':
        period = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'x':
        do_shutdown = 0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-p <period> "
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
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var server =
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // @@ We should use "validate_connection" for this
      for (int j = 0; j < 100; ++j)
        {
          server->_non_existent (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      ACE_Sample_History history (niterations);

#if defined (USING_QUANTIFY)
  // Reset Quantify data recording; whatever happened in the past is
  // not relevant to this test.
  QuantifyClearData ();
  QuantifyStartRecordingData ();
#endif /* USING_QUANTIFY */

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      for (int i = 0; i < niterations; ++i)
        {
          ACE_hrtime_t latency_base =
            server->test_method (ACE_OS::gethrtime (), ACE_TRY_ENV);
          ACE_TRY_CHECK;
          ACE_hrtime_t now = ACE_OS::gethrtime ();
          history.sample (now - latency_base);

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
	  if (period != -1)
	    {
	      ACE_Time_Value tv (0, period * 1000);
	      ACE_OS::sleep (tv);
	    }
        }

#if defined (USING_QUANTIFY)
  // Stop recording data here; whatever happens after this in the test
  // is not relevant to this test.
  QuantifyStopRecordingData ();
#endif /* USING_QUANTIFY */

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "test finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Single thread", gsf);

      ACE_Throughput_Stats::dump_throughput ("Single thread", gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

      if (do_shutdown)
        {
          server->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
