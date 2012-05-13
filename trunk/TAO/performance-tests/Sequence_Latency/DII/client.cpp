// $Id$

#include "tao/DynamicInterface/Request.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/Any.h"

#include "TestC.h"

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
int sz = 512;

const ACE_TCHAR *data_type = ACE_TEXT("octet");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:s:hxk:i:"));
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


void
test_octet_seq (const CORBA::Object_var object)
{
  ACE_Sample_History history (niterations);

  Test::octet_load ol (sz);
  ol.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();
      Test::Timestamp start_time = static_cast <Test::Timestamp> (start);

      CORBA::Request_var request =
        object->_request ("test_octet_method");

      request->add_in_arg("octet_load") <<= ol;
      request->add_in_arg("send_time") <<= start_time;

      request->set_return_type (CORBA::_tc_ulonglong);
      request->invoke ();

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }

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
}

void
test_long_seq (const CORBA::Object_var object)
{
  ACE_Sample_History history (niterations);

  Test::long_load ll (sz);
  ll.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      CORBA::Request_var request =
        object->_request ("test_long_method");

      request->add_in_arg("long_load") <<= ll;
      request->add_in_arg("send_time") <<= static_cast <Test::Timestamp> (start);

      request->set_return_type (CORBA::_tc_ulonglong);
      request->invoke ();

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }

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
}

void
test_short_seq (const CORBA::Object_var object)
{
  ACE_Sample_History history (niterations);

  Test::short_load sl (sz);
  sl.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      CORBA::Request_var request =
        object->_request ("test_short_method");

      request->add_in_arg("short_load") <<= sl;
      request->add_in_arg("send_time") <<= static_cast <Test::Timestamp> (start);

      request->set_return_type (CORBA::_tc_ulonglong);
      request->invoke ();

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }

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
}

void
test_char_seq (const CORBA::Object_var object)
{
  ACE_Sample_History history (niterations);

  Test::char_load cl (sz);
  cl.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      CORBA::Request_var request =
        object->_request ("test_char_method");

      request->add_in_arg("char_load") <<= cl;
      request->add_in_arg("send_time") <<= static_cast <Test::Timestamp> (start);

      request->set_return_type (CORBA::_tc_ulonglong);
      request->invoke ();

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }

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
}

void
test_double_seq (const CORBA::Object_var object)
{
  ACE_Sample_History history (niterations);

  Test::double_load dl (sz);
  dl.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      CORBA::Request_var request =
        object->_request ("test_double_method");

      request->add_in_arg("double_load") <<= dl;
      request->add_in_arg("send_time") <<= static_cast <Test::Timestamp> (start);

      request->set_return_type (CORBA::_tc_ulonglong);
      request->invoke ();

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }

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
}

void
test_longlong_seq (const CORBA::Object_var object)
{
  ACE_Sample_History history (niterations);

  Test::longlong_load ll (sz);
  ll.length (sz);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      CORBA::Request_var request =
        object->_request ("test_longlong_method");

      request->add_in_arg("longlong_load") <<= ll;
      request->add_in_arg("send_time") <<= static_cast <Test::Timestamp> (start);

      request->set_return_type (CORBA::_tc_ulonglong);
      request->invoke ();

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }

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
}


// The main() function starts here

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

      Test::octet_load oc;

      for (int j = 0; j < 100; ++j)
        {
          CORBA::Request_var request =
            object->_request ("test_octet_method");

          Test::Timestamp dummy = 0;
          request->add_in_arg("octet_load") <<= oc;
          request->add_in_arg("send_time") <<= dummy;

          request->set_return_type (CORBA::_tc_ulonglong);
          request->invoke ();
        }

      // Test various sequence types

      if (ACE_OS::strcmp (data_type, ACE_TEXT("octet")) == 0 )
        {
          test_octet_seq (object);
        }
      else if (ACE_OS::strcmp (data_type, ACE_TEXT("char")) == 0)
        {
          test_char_seq (object);
        }
      else if (ACE_OS::strcmp (data_type, ACE_TEXT("long")) == 0)
        {
          test_long_seq (object);
        }
      else if (ACE_OS::strcmp (data_type, ACE_TEXT("short")) == 0)
        {
          test_short_seq (object);
        }
      else if (ACE_OS::strcmp (data_type, ACE_TEXT("double")) == 0)
        {
          test_double_seq (object);
        }
      else if (ACE_OS::strcmp (data_type, ACE_TEXT("longlong")) == 0)
        {
          test_longlong_seq (object);
        }

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
