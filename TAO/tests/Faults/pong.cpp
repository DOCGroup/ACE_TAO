#include "ping_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

struct Pong_Options
{
  int iterations {60};
  int period {10};
  int kill_on {30};
  ACE_TString ior {ACE_TEXT ("file://ping.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Pong_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:p:k:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        options.period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 't':
        options.kill_on = ACE_OS::atoi (get_opts.opt_arg ());
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
  // Indicates successful parsing of the command line
  return 0;
}

void
run_client (CORBA::ORB_ptr orb,
            PingObject_ptr server,
            PingObject_ptr callback,
            Pong_Options const &options)
{
  // Run the client requests in a separate routine to isolate the
  // exceptions...

  for (int i {}; i != options.iterations; ++i)
    {
      ACE_Time_Value tv (0, 1000 * options.period);
      char const *reason = "";
      try
        {
          reason = "Exception during ping call";
          server->ping (callback);

          // Run for <period> milliseconds, to receive the reply
          orb->run (tv);

          // Terminate the client if:
          //   - This is the last iteration
          //   - The iteration is 0 modulo <kill_on>
          if (i + 1 == options.iterations
              || (options.kill_on != 0 && i > 0 && i % options.kill_on == 0))
            {
              ACE_DEBUG ((LM_DEBUG, "Shutting down server\n"));
              reason = "Exception during server shutdown";
              server->shutdown ();
            }
        }
      catch (CORBA::TRANSIENT const &)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "Ignoring transient exception\n"));
          // ACE_PRINT_EXCEPTION (t, reason);
          ACE_OS::sleep (tv);
        }
      catch (CORBA::COMM_FAILURE const &)
        {
          ACE_DEBUG ((LM_DEBUG, "Ignoring comm failure exception\n"));
          // ACE_PRINT_EXCEPTION (f, reason);
          ACE_OS::sleep (tv);
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (reason);
          ACE_OS::sleep (tv);
        }
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Pong_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // create child poa with PERSISTENT policy
      CORBA::PolicyList policies {};
      policies.length (2);
      policies[0] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
      policies[1] =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

      PortableServer::POA_var persistent_poa =
        root_poa->create_POA ("persistent",
                             poa_manager.in (),
                             policies);
      policies[0]->destroy ();
      policies[1]->destroy ();

      poa_manager->activate ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      PingObject_var const server =
        PingObject::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ())),
                            1);
        }

      PingObject_i callback_impl (orb.in (), persistent_poa.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&callback_impl);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      PingObject_var const callback =
        PingObject::_narrow (object_act.in ());

      run_client (orb.in (),
                  server.in (),
                  callback.in (),
                  options);

      persistent_poa->destroy (true, true);

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
