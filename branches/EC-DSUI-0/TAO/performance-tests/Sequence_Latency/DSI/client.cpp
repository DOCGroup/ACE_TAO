//
// $Id$
//

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

#include "tao/Strategies/advanced_resource.h"

ACE_RCSID(DSI_Latency, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 100;
int do_dump_history = 0;
int do_shutdown = 1;
int sz = 512;

const char *data_type = "octet";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "t:s:hxk:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        data_type = get_opts.opt_arg ();

        if (ACE_OS::strcmp (data_type, "octet") != 0 &&
            ACE_OS::strcmp (data_type, "char") != 0 &&
            ACE_OS::strcmp (data_type, "long") != 0 &&
            ACE_OS::strcmp (data_type, "short") != 0 &&
            ACE_OS::strcmp (data_type, "double") != 0 &&
            ACE_OS::strcmp (data_type, "longlong") != 0)
          return -1;
        break;

                        case 's':
        sz = ACE_OS::atoi (get_opts.opt_arg ());
        break;

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

void
test_octet_seq (Test::Roundtrip_ptr roundtrip ACE_ENV_ARG_DECL)
{
  ACE_Sample_History history (niterations);

  Test::octet_load ol (sz);
  ol.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) roundtrip->test_octet_method (ol,
                                           start
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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

void
test_long_seq (Test::Roundtrip_ptr roundtrip ACE_ENV_ARG_DECL)
{
  ACE_Sample_History history (niterations);

  Test::long_load ll (sz);
  ll.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) roundtrip->test_long_method (ll,
                                          start
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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


void
test_short_seq (Test::Roundtrip_ptr roundtrip ACE_ENV_ARG_DECL)
{
  ACE_Sample_History history (niterations);

  Test::short_load sl (sz);
  sl.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) roundtrip->test_short_method (sl,
                                           start
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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


void
test_char_seq (Test::Roundtrip_ptr roundtrip ACE_ENV_ARG_DECL)
{
  ACE_Sample_History history (niterations);

  Test::char_load cl (sz);
  cl.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) roundtrip->test_char_method (cl,
                                          start
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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


void
test_longlong_seq (Test::Roundtrip_ptr roundtrip ACE_ENV_ARG_DECL)
{
  ACE_Sample_History history (niterations);

  Test::longlong_load ll (sz);
  ll.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) roundtrip->test_longlong_method (ll,
                                              start
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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


void
test_double_seq (Test::Roundtrip_ptr roundtrip ACE_ENV_ARG_DECL)
{
  ACE_Sample_History history (niterations);

  Test::double_load dl (sz);
  dl.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) roundtrip->test_double_method (dl,
                                            start
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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

      Test::Roundtrip_var roundtrip =
        Test::Roundtrip::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (roundtrip.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             ior),
                            1);
        }


                        Test::octet_load oc;

      for (int j = 0; j < 100; ++j)
        {
          ACE_hrtime_t start = 0;
          (void) roundtrip->test_octet_method (oc,
                                               start
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (ACE_OS::strcmp (data_type, "octet") == 0 )
        {
          test_octet_seq (roundtrip.in ()
                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (ACE_OS::strcmp (data_type, "char") == 0)
        {
          test_char_seq (roundtrip.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (ACE_OS::strcmp (data_type, "long") == 0)
        {
          test_long_seq (roundtrip.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (ACE_OS::strcmp (data_type, "short") == 0)
        {
          test_short_seq (roundtrip.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (ACE_OS::strcmp (data_type, "double") == 0)
        {
          test_double_seq (roundtrip.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (ACE_OS::strcmp (data_type, "longlong") == 0)
        {
          test_longlong_seq (roundtrip.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (do_shutdown)
        {
          roundtrip->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
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
