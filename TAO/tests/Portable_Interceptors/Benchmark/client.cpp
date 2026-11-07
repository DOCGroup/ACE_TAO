#include "testC.h"
#include "marker.h"
#include "Client_ORBInitializer.h"
#include "Interceptor_Type.h"

#include "tao/debug.h"
#include "tao/ORBInitializer_Registry.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_errno.h"
#include "ace/SString.h"

struct Options
{
  ACE_TString ior { ACE_TEXT ("file://test.ior") };
  int niterations { 5 };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("f:n:r:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          options.ior = get_opts.opt_arg ();
          break;
        case 'n':
          options.niterations = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'r':
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s -f <IOR> -n <iterations>\n",
                             argv [0]),
                            -1);
        }
    }
  return 0;
}


void
run_test (Test_Interceptors::Secure_Vault_ptr server, int niterations)
{
  char const user[] = "root";
  Marker marker {};
  ACE_Throughput_Stats throughput {};

  ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
  ACE_High_Res_Timer::global_scale_factor_type const gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));

  marker.accumulate_into (throughput, Benchmark_Method::ready);
  ACE_hrtime_t throughput_base = ACE_OS::gethrtime ();
  for (int i {}; i < niterations; ++i)
    {
      // Record current time.
      ACE_hrtime_t const latency_base = ACE_OS::gethrtime ();

      server->ready ();

      // Grab timestamp again.
      ACE_hrtime_t const now = ACE_OS::gethrtime ();

      // Record statistics.
      marker.sample (now - throughput_base,
                     now - latency_base,
                     Benchmark_Method::ready);

      if (TAO_debug_level > 0 && i % 100 == 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
    }

  marker.dump_stats (ACE_TEXT ("Ready method  "), gsf, Benchmark_Method::ready);

  try
    {
      marker.accumulate_into (throughput, Benchmark_Method::authenticate);
      throughput_base = ACE_OS::gethrtime ();

      for (int i {}; i < niterations; ++i)
        {
          // Record current time.
          ACE_hrtime_t const latency_base = ACE_OS::gethrtime ();

          server->authenticate (user);

          // Grab timestamp again.
          ACE_hrtime_t const now = ACE_OS::gethrtime ();

          // Record statistics.
          marker.sample (now - throughput_base,
                         now - latency_base,
                         Benchmark_Method::authenticate);

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
        }
      marker.dump_stats (ACE_TEXT ("Authenticate method  "), gsf,
                         Benchmark_Method::authenticate);
    }
  catch (Test_Interceptors::Invalid const &)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid user\n"));
    }

  Test_Interceptors::Secure_Vault::Record record {};
  record.check_num = 1;
  record.amount = 1000;
  CORBA::Long const id = 1;

  marker.accumulate_into (throughput, Benchmark_Method::update_records);
  throughput_base = ACE_OS::gethrtime ();

  for (int i {}; i < niterations; ++i)
    {
      // Record current time.
      ACE_hrtime_t const latency_base = ACE_OS::gethrtime ();

      server->update_records (id,
                              record);

      // Grab timestamp again.
      ACE_hrtime_t const now = ACE_OS::gethrtime ();

      // Record statistics.
      marker.sample (now - throughput_base,
                     now - latency_base,
                     Benchmark_Method::update_records);

      if (TAO_debug_level > 0 && i % 100 == 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
    }

  marker.dump_stats (ACE_TEXT ("update records  method  "), gsf,
                     Benchmark_Method::update_records);
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

  Interceptor_Type const interceptor_type = get_interceptor_type (argc, argv);

  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer (interceptor_type),
                      -1);  // No exceptions yet!
      // The var takes ownership of the allocated initializer.
      PortableInterceptor::ORBInitializer_var const initializer (temp_initializer);

      PortableInterceptor::register_orb_initializer (initializer.in ());

      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (options.ior.c_str ());

      Test_Interceptors::Secure_Vault_var const server =
        Test_Interceptors::Secure_Vault::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior.c_str ()),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "\nFunctionality test begins now...\n"));

      // This test is useful for  benchmarking the differences when
      // the same method is intercepted by different interceptors
      // wanting to achieve different functionality.
      run_test (server.in (), options.niterations);

      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
