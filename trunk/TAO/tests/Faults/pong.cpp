// $Id$

#include "ace/Get_Opt.h"
#include "ping_i.h"

ACE_RCSID(Faults, server, "$Id$")

int iterations = 60;
int period = 10;
int kill_on = 30;
const char* ior = "file://ping.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:p:k:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'p':
        period = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'k':
        ior = get_opts.optarg;
        break;

      case 't':
        kill_on = ACE_OS::atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <iterations> "
                           "-p <period> (milliseconds) "
                           "-k ior "
                           "-t <rate> (terminate server every n calls) "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
run_client (ORB_ptr orb,
            Ping_ptr server,
            Ping_ptr callback,
            CORBA::Environment &ACE_TRY_ENV)
{
  // Run the client requests in a separate routine to isolate the
  // exceptions...

  for (int i = 0; i != iterations; ++i)
    {
      ACE_Time_Value tv (0, 1000 * period);
      const char* reason = "";
      ACE_TRY
        {
          reason = "Exception during ping call";
          server->ping (callback, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Run for <period> milliseconds, to receive the reply
          orb->run (tv);

          // Terminate the client if:
          //   - This is the last iteration
          //   - The iteration is 0 modulo <kill_on>
          if (i + 1 == iterations
              || (kill_on != 0 && i > 0 && i % kill_on == 0))
            {
              ACE_DEBUG ((LM_DEBUG, "Shutting down server\n"));
              reason = "Exception during server shutdown";
              server->shutdown (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCH (CORBA::TRANSIENT, t)
        {
          ACE_DEBUG ((LM_DEBUG, "Ignoring transient exception\n"));
          ACE_OS::sleep (tv);
        }
      ACE_CATCH (CORBA::COMM_FAILURE, f)
        {
          ACE_DEBUG ((LM_DEBUG, "Ignoring comm failure exception\n"));
          ACE_OS::sleep (tv);
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, reason);
          ACE_OS::sleep (tv);
        }
      ACE_ENDTRY;
    }
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // create child poa with PERSISTENT policy
      ACE_TRY_CHECK;
      CORBA::PolicyList policies;
      policies.length (2);
      policies[0] =
        root_poa->create_lifespan_policy(PortableServer::PERSISTENT,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      policies[1] =
        root_poa->create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION,
                                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var persistent_poa =
        root_poa->create_POA("persistent",
                             poa_manager.in (),
                             policies,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      policies[0]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      policies[1]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Ping_var server =
        Ping::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      Ping_i callback_impl (orb.in (),
                            persistent_poa.in ());

      Ping_var callback =
        callback_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;


      // If the ior_output_file exists, output the ior to it
      run_client (orb.in (),
                  server.in (),
                  callback.in (),
                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      persistent_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1, ACE_TRY_ENV);
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
