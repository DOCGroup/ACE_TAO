// $Id$

#include "ace/OS.h"
#include "testC.h"
#include "interceptors.h"
#include "marker.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Benchmark, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;
int register_interceptor = 1;
// The different type of interceptors have different functionality
// to perform.
// NOOP: does nothing on all interception points
// CONTEXT: does service context manipulation
// DYNAMIC: call upon dynamic interface methods and does extraction
// from anys.
enum Interceptor_Type
{
  IT_NONE,
  IT_NOOP,
  IT_CONTEXT,
  IT_DYNAMIC
};
static Interceptor_Type interceptor_type = IT_NONE;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ef:n:r:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
        break;
      case 'f':
        ior = get_opts.optarg;
        break;
      case 'n':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'r':
        {
          if (ACE_OS::strcmp (get_opts.optarg, ACE_TEXT ("none")) == 0)
            interceptor_type = IT_NONE;
          if (ACE_OS::strcmp (get_opts.optarg, ACE_TEXT ("noop")) == 0)
            interceptor_type = IT_NOOP;
          if (ACE_OS::strcmp (get_opts.optarg, ACE_TEXT ("context")) == 0)
            interceptor_type = IT_CONTEXT;
          if (ACE_OS::strcmp (get_opts.optarg, ACE_TEXT ("dynamic")) == 0)
            interceptor_type = IT_DYNAMIC;

          break;
        }
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
run_test (Test_Interceptors::Secure_Vault_ptr server,
          CORBA::Environment &ACE_TRY_ENV)
{
  int i=0;
  const char user [BUFSIZ] = "root";
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

      server->ready (ACE_TRY_ENV);

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

              server->authenticate (user, ACE_TRY_ENV);

              // Grab timestamp again.
              ACE_hrtime_t now = ACE_OS::gethrtime ();

              // Record statistics.
              marker.sample (now - throughput_base,
                             now - latency_base,
                             2);

              ACE_TRY_CHECK;
              if (TAO_debug_level > 0 && i % 100 == 0)
                ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));

            }
          marker.dump_stats ("Authenticate method  ", gsf, 2);

          ACE_TRY_CHECK;

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
                              record,
                              ACE_TRY_ENV);

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
  ACE_CHECK;

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

      Test_Interceptors::Secure_Vault_var server =
        Test_Interceptors::Secure_Vault::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

#if (TAO_HAS_INTERCEPTORS == 1)
      switch (interceptor_type)
        {
        case IT_NONE:
          break;
        case IT_NOOP:
          {
            PortableInterceptor::ClientRequestInterceptor_ptr interceptor = 0;
            // Installing the Vault interceptor
            ACE_NEW_RETURN (interceptor,
                            Vault_Client_Request_NOOP_Interceptor (orb.in ()),
                            -1);
            orb->_register_client_interceptor (interceptor);
            break;
          }
        case IT_CONTEXT:
          {
            PortableInterceptor::ClientRequestInterceptor_ptr interceptor = 0;
            // Installing the Vault interceptor
            ACE_NEW_RETURN (interceptor,
                            Vault_Client_Request_Context_Interceptor (orb.in ()),
                            -1);
            orb->_register_client_interceptor (interceptor);
            break;
          }
        case IT_DYNAMIC:
          {
            PortableInterceptor::ClientRequestInterceptor_ptr interceptor = 0;
            // Installing the Vault interceptor
            ACE_NEW_RETURN (interceptor,
                            Vault_Client_Request_Dynamic_Interceptor (orb.in ()),
                            -1);
            orb->_register_client_interceptor (interceptor);
            break;
          }

        }
#endif /* (TAO_HAS_INTERCEPTORS == 1) */

      ACE_DEBUG ((LM_DEBUG, "\nFunctionality test begins now...\n"));

      // This test is useful for  benchmarking the differences when
      // the same method is intercepted by different interceptors
      // wanting to achieve different functionality.
      run_test (server.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
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
