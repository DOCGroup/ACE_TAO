// $Id$

#include "ace/Get_Opt.h"
#include "testC.h"
#include "interceptors.h"
#include "marker.h"

ACE_RCSID (Benchmark, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ef:i:");
  int c;
  
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
        break;
      case 'f':
        ior = get_opts.optarg;
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-v "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

// TODO:check the sched stuff form latency test, for runningtest inrealtime mode!

void
run_test (Test_Interceptors::Secure_Vault_ptr server,
          CORBA::Environment &ACE_TRY_ENV)
{
  int i=0;
  const char passwd [BUFSIZ] = "root";  
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
      
  //throughput.dump_results ("Aggregated", gsf);

  ACE_TRY
        {
          marker.accumulate_into (throughput, 2);
          throughput_base = ACE_OS::gethrtime ();
          
          for (i = 0; i < niterations ; ++i)
            {
              // Record current time.
              ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
              
              server->authenticate (passwd, ACE_TRY_ENV);
              
              // Grab timestamp again.
              ACE_hrtime_t now = ACE_OS::gethrtime ();      
              
              // Record statistics.
              marker.sample (now - throughput_base,
                             now - latency_base,
                             2);
              
              ACE_CHECK;
              if (TAO_debug_level > 0 && i % 100 == 0)
                ACE_DEBUG ((LM_DEBUG, "(%P|%t) iteration = %d\n", i));
                          
            }
          marker.dump_stats ("Authenticate method  ", gsf, 2);
              
          ACE_CHECK;
          //  throughput.dump_results ("Aggregated", gsf);
        }
  ACE_CATCH (Test_Interceptors::Invalid, userex)
    {
      ACE_DEBUG ((LM_DEBUG, "Invalid passwd\n"));
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
  
      CORBA::Long result = server->update_records (id, 
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

      //  throughput.dump_results ("Aggregated", gsf);
              
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      PortableInterceptor::ClientRequestInterceptor_ptr interceptor = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Installing the Vault interceptor
      ACE_NEW_RETURN (interceptor,
                      Vault_Client_Request_Interceptor (orb.in ()),
                      -1);
      orb->_register_client_interceptor (interceptor);

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
