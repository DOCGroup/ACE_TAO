//
//$Id$
//
//FUZZ: disable check_for_missing_rir_env

#define USE_POA
//#define NESTED

#include <CORBA.h>
#include <coss/CosNaming.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#include "RoundTrip.h"

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

using namespace std;


int do_dump_history = 0;
int niterations = 1000;

int
parse_args (int argc, char *argv[])
{
  //parse arguments
  ACE_Get_Opt get_opts (argc, argv, "hxi:");
  char c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        /* Could be any other parameter like -ORBSvcConf file so ignore */
       break;
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

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
  CosNaming::NamingContextExt_var nc =
    CosNaming::NamingContextExt::_narrow (obj);
  assert (!CORBA::is_nil (nc));

  //Obtain reference from the naming context
  obj = nc->resolve_str ("RoundTrip");

  Roundtrip_var bench = Roundtrip::_narrow (obj);

  parse_args(argc, argv);
  set_priority();

  //Warmup the System by making some empty calls
  CORBA::Long start = 0L;
  for (int j = 0; j < 100; ++j)
    {
      bench->test_method (start);
    }


  CORBA::ULongLong time_stamp = 1234;
  ACE_Sample_History history (niterations);
  ACE_hrtime_t test_start = ACE_OS::gethrtime ();

  for (int i = 0; i < niterations; ++i)
    {

      ACE_hrtime_t start = ACE_OS::gethrtime ();

      bench->test_method (time_stamp);

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

  //Remove the component
  cout << "client finished" << endl;

  return 0;
}
