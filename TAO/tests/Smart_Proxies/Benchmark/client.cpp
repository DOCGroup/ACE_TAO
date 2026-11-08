
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is the client program that tests TAO's Smart Proxy extension.
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "testC.h"
#include "Smart_Proxy_Impl.h"
#include "tao/debug.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Throughput_Stats.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_errno.h"

struct Options
{
  ACE_TCHAR const *ior {ACE_TEXT("file://test.ior")};
  int iterations {5};
  bool register_smart_proxy {true};
};

class Marker
{
 public:
  Marker () = default;
  Marker (Marker const &) = delete;
  Marker (Marker &&) = delete;
  Marker &operator= (Marker const &) = delete;
  Marker &operator= (Marker &&) = delete;

  void dump_stats (ACE_TCHAR const *msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf)
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
  /// Keep throughput statistics on a per-thread basis
  ACE_Throughput_Stats throughput_;
};


static int
parse_args (Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:n:r:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        options.ior = get_opts.opt_arg ();
      break;
      case 'n':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'r':
        options.register_smart_proxy = ACE_OS::atoi (get_opts.opt_arg ()) == 1;
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int const priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  // Enable FIFO scheduling

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
      Options options;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      if (parse_args (options, argc, argv) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (options.ior);
      if (options.register_smart_proxy)
        {
          // To use the smart proxy it is necessary to allocate the
          // user-defined smart factory on the heap as the smart proxy
          // generated classes take care of destroying the object. This
          // way it a win situation for the application developer who
          // doesnt have to make sure to destroy it and also for the smart
          // proxy designer who now can manage the lifetime of the object
          // much surely.

          Smart_Test_Factory *test_factory {};
          ACE_NEW_RETURN (test_factory,
                          Smart_Test_Factory,
                          -1);

          // The factory registers itself and is owned by the proxy adapter.
          ACE_UNUSED_ARG (test_factory);
        }

      Test_var server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil.\n",
                           options.ior),
                          1);
      Marker marker;
      ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
      ACE_High_Res_Timer::global_scale_factor_type const gsf =
        ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));
      CORBA::Short price {};
      CORBA::Long cost {};
      ACE_hrtime_t const throughput_base = ACE_OS::gethrtime ();
      for (int i = 0; i < options.iterations; ++i)
        {
          ACE_hrtime_t const latency_base = ACE_OS::gethrtime ();
          price = server->box_prices ();
          if (price < 300)
            cost = server->tickets (5);
          ACE_hrtime_t const now = ACE_OS::gethrtime ();
          marker.sample (now - throughput_base, now - latency_base);
          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration <%d> - price <%d> - cost <%d>\n",
                        i, price, cost));
        }
      marker.dump_stats (ACE_TEXT("buying tickets "), gsf);
      server->shutdown ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client-side exception:");
      return 1;
    }

  return 0;
}
