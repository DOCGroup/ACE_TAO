// $Id$

#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/OS_NS_errno.h"

#include "tao/Strategies/advanced_resource.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 1000;
int do_shutdown = 1;
int sz = 512;

const ACE_TCHAR *data_type = ACE_TEXT("octet");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:s:xk:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        data_type = get_opts.opt_arg ();

        if (ACE_OS::strcmp (data_type, ACE_TEXT("octet")) != 0 &&
            ACE_OS::strcmp (data_type, ACE_TEXT("char")) != 0 &&
            ACE_OS::strcmp (data_type, ACE_TEXT("long")) != 0 &&
            ACE_OS::strcmp (data_type, ACE_TEXT("short")) != 0 &&
            ACE_OS::strcmp (data_type, ACE_TEXT("double")) != 0 &&
            ACE_OS::strcmp (data_type, ACE_TEXT("longlong")) != 0)
          return -1;
        break;

          case 's':
        sz = ACE_OS::atoi (get_opts.opt_arg ());
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
                           "-t <datatype> "
                                                   "-s <size> "
                           "-k <ior> "
                           "-i <niterations> "
                           "-x (disable shutdown) "
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

          /// Begin the test

      ACE_DEBUG ((LM_DEBUG, "Starting threads\n"));

      Client_Task task0 (data_type, sz, roundtrip.in (), niterations);
      Client_Task task1 (data_type, sz, roundtrip.in (), niterations);
      Client_Task task2 (data_type, sz, roundtrip.in (), niterations);
      Client_Task task3 (data_type, sz, roundtrip.in (), niterations);

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      task0.activate (THR_NEW_LWP | THR_JOINABLE);
      task1.activate (THR_NEW_LWP | THR_JOINABLE);
      task2.activate (THR_NEW_LWP | THR_JOINABLE);
      task3.activate (THR_NEW_LWP | THR_JOINABLE);

      task0.thr_mgr()->wait ();
      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "Threads finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      ACE_Basic_Stats totals;
      task0.accumulate_and_dump (totals, ACE_TEXT("Task[0]"), gsf);
      task1.accumulate_and_dump (totals, ACE_TEXT("Task[1]"), gsf);
      task2.accumulate_and_dump (totals, ACE_TEXT("Task[2]"), gsf);
      task3.accumulate_and_dump (totals, ACE_TEXT("Task[3]"), gsf);

      totals.dump_results (ACE_TEXT("Total"), gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                             test_end - test_start,
                                             totals.samples_count ());

      if (do_shutdown)
        {
          roundtrip->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
