// $Id$

#include "tao/DynamicInterface/Request.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/Any.h"

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_errno.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 100;
int do_dump_history = 0;
int do_shutdown = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("hxk:i:"));
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

      if (CORBA::is_nil (object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil CORBA::Object reference <%s>\n",
                             ior),
                            1);
        }

      for (int j = 0; j < 100; ++j)
        {
          CORBA::Request_var request =
            object->_request ("test_method");

          CORBA::ULongLong dummy = 0;
          request->add_in_arg("send_time") <<= dummy;

          request->set_return_type (CORBA::_tc_ulonglong);
          request->invoke ();
        }

      ACE_Sample_History history (niterations);

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      for (int i = 0; i < niterations; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          CORBA::Request_var request =
            object->_request ("test_method");

          CORBA::ULongLong start_time = static_cast <CORBA::ULongLong> (start);
          request->add_in_arg("send_time") <<= start_time;

          request->set_return_type (CORBA::_tc_ulonglong);
          request->invoke ();

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
          CORBA::Request_var request =
            object->_request ("shutdown");

          request->invoke ();

        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
