// $Id$

#include "testC.h"
#include "interceptors.h"
#include "marker.h"
#include "Client_ORBInitializer.h"
#include "Interceptor_Type.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"

ACE_RCSID (Benchmark,
           client,
           "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;
int register_interceptor = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ef:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
        break;
      case 'f':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-v -r <register_interceptor> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}



void
run_test (Test_Interceptors::Secure_Vault_ptr server
          ACE_ENV_ARG_DECL)
{
  int i = 0;
  const char user[] = "root";
  Marker marker;
  ACE_Throughput_Stats throughput;

  ACE_DEBUG ((LM_DEBUG, "High res. timer calibration...."));
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));

  marker.accumulate_into (throughput, 1);
  ACE_hrtime_t throughput_base = ACE_OS::gethrtime ();
  for (i = 0; i < niterations ; ++i)
    {
      // Record current time.
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();

      server->ready (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Grab timestamp again.
      ACE_hrtime_t now = ACE_OS::gethrtime ();

      // Record statistics.
      marker.sample (now - throughput_base,
                     now - latency_base,
                     1);

      ACE_CHECK;
      if (TAO_debug_level > 0 && i % 100 == 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
    }

  marker.dump_stats ("Ready method  ", gsf, 1);

  ACE_TRY
    {
      marker.accumulate_into (throughput, 2);
      throughput_base = ACE_OS::gethrtime ();

      for (i = 0; i < niterations ; ++i)
        {
          // Record current time.
          ACE_hrtime_t latency_base = ACE_OS::gethrtime ();

          server->authenticate (user ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Grab timestamp again.
          ACE_hrtime_t now = ACE_OS::gethrtime ();

          // Record statistics.
          marker.sample (now - throughput_base,
                         now - latency_base,
                         2);

          if (TAO_debug_level > 0 && i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
        }
      marker.dump_stats ("Authenticate method  ", gsf, 2);
    }
  ACE_CATCH (Test_Interceptors::Invalid, userex)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid user\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK;

  Test_Interceptors::Secure_Vault::Record record;
  record.check_num = 1;
  record.amount = 1000;
  CORBA::Long id = 1;

  marker.accumulate_into (throughput, 3);
  throughput_base = ACE_OS::gethrtime ();

  for (i = 0; i < niterations ; ++i)
    {
      // Record current time.
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();

      server->update_records (id,
                              record
                              ACE_ENV_ARG_PARAMETER);

      // Grab timestamp again.
      ACE_hrtime_t now = ACE_OS::gethrtime ();

      // Record statistics.
      marker.sample (now - throughput_base,
                     now - latency_base,
                     3);

      ACE_CHECK;
      if (TAO_debug_level > 0 && i % 100 == 0)
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));

    }

  marker.dump_stats ("update records  method  ", gsf, 3);
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

  int interceptor_type;
  get_interceptor_type (argc, argv, interceptor_type);

  ACE_TRY_NEW_ENV
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer;

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer (interceptor_type),
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_Interceptors::Secure_Vault_var server =
        Test_Interceptors::Secure_Vault::_narrow (object.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "\nFunctionality test begins now...\n"));

      // This test is useful for  benchmarking the differences when
      // the same method is intercepted by different interceptors
      // wanting to achieve different functionality.
      run_test (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
     return 1;
    }
  ACE_ENDTRY;

  return 0;
}
