// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/Smart_Proxies/Benchmark
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is the client program that tests TAO's Smart Proxy extension.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=========================================================================

#include "testC.h"
#include "Smart_Proxy_Impl.h"
#include "tao/debug.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Benchmark, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;
int register_smart_proxy = 1;

class Marker
{
 public:

  void accumulate_into (ACE_Throughput_Stats &throughput) const
    {
      // Accumulate the throughput statistics into <throughput>
      throughput.accumulate (this->throughput_);
    }
  void dump_stats (const char* msg,
                   ACE_UINT32 gsf)
    {
      // Print stats
      this->throughput_.dump_results (msg, gsf);
    }
  void sample (ACE_hrtime_t throughput_diff,
               ACE_hrtime_t latency_diff)
    {
      // get the sample.
      this->throughput_.sample (throughput_diff,
                                latency_diff);

    }
 private:
  ACE_Throughput_Stats throughput_;
  // Keep throughput statistics on a per-thread basis
};


int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:n:r:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ior = ACE_OS::strdup (get_opts.opt_arg ());
      break;
      case 'n':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'r':
        register_smart_proxy = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i -n"
                           "\n",
                           argv [0]),
                          -1);
      }
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
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

       CORBA::Object_var object =
        orb->string_to_object (ior
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (register_smart_proxy == 1)
        {
          // To use the smart proxy it is necessary to allocate the
          // user-defined smart factory on the heap as the smart proxy
          // generated classes take care of destroying the object. This
          // way it a win situation for the application developer who
          // doesnt have to make sure to destoy it and also for the smart
          // proxy designer who now can manage the lifetime of the object
          // much surely.

          Smart_Test_Factory *test_factory = 0;
          ACE_NEW_RETURN (test_factory,
                          Smart_Test_Factory,
                          -1);

          // To make KAI Compiler happy as it considers <test_factory> to be
          // an unused variable.
          ACE_UNUSED_ARG (test_factory);
        }

      Test_var server =
        Test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil\n",
                           ior),
                          1);
       Marker marker;
       ACE_Throughput_Stats throughput;
       int i=0;
       ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
       ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
       ACE_DEBUG ((LM_DEBUG, "done\n"));

       marker.accumulate_into (throughput);
       CORBA::Short price =0;
       CORBA::Long cost =0;
       ACE_hrtime_t throughput_base = ACE_OS::gethrtime ();
       for (i = 0; i < niterations ; ++i)
         {
           // Record current time.
           ACE_hrtime_t latency_base = ACE_OS::gethrtime ();

           price = server->box_prices (ACE_ENV_SINGLE_ARG_PARAMETER);
           ACE_TRY_CHECK;

           if (price < 300)
             cost = server->tickets (5);

           // Grab timestamp again.
           ACE_hrtime_t now = ACE_OS::gethrtime ();

           // Record statistics.
           marker.sample (now - throughput_base,
                          now - latency_base);

           ACE_TRY_CHECK;
           if (TAO_debug_level > 0 && i % 100 == 0)
             ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
         }

       marker.dump_stats ("buying tickets ", gsf);

       server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      /*

      Test_var server1 =
        Test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server1.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil\n",
                           ior),
                          1);
       Marker marker1;
       ACE_Throughput_Stats throughput1;

       ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
       ACE_UINT32 gsf1 = ACE_High_Res_Timer::global_scale_factor ();
       ACE_DEBUG ((LM_DEBUG, "done\n"));

       marker1.accumulate_into (throughput1);
       CORBA::Short price1 =0;
       CORBA::Long cost1 =0;
       ACE_hrtime_t throughput_base1 = ACE_OS::gethrtime ();
       for (i = 0; i < niterations ; ++i)
         {
           // Record current time.
           ACE_hrtime_t latency_base = ACE_OS::gethrtime ();

           price1 = server1->box_prices (ACE_ENV_SINGLE_ARG_PARAMETER);
           ACE_TRY_CHECK;

           if (price1 < 300)
             cost = server1->tickets (5);

           // Grab timestamp again.
           ACE_hrtime_t now = ACE_OS::gethrtime ();

           // Record statistics.
           marker.sample (now - throughput_base1,
                          now - latency_base);

           ACE_TRY_CHECK;
           if (TAO_debug_level > 0 && i % 100 == 0)
             ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
         }

       marker1.dump_stats ("buying tickets using a default proxy ", gsf1);

       server1->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      */
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client-side exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
