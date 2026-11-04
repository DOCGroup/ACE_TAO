#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "test_i.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  int iterations {10};
  bool shutdown {};
  bool suicide {};
  bool self_shutdown {};
  bool abort_server {};
  bool crash_server {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("acszxk:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.abort_server = true;
        break;

      case 'c':
        options.crash_server = true;
        break;

      case 'x':
        options.shutdown = true;
        break;

      case 's':
        options.suicide = true;
        break;

      case 'z':
        options.self_shutdown = true;
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-a] [-c] [-x] [-s] [-z] "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior.c_str ()),
                            1);
        }

      Callback_i callback_impl (orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&callback_impl);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      Callback_var const callback =
        Callback::_narrow (object_act.in ());

      for (int i {}; i != options.iterations; ++i)
        {
          CORBA::Long const r =
            server->test_method (false, false, callback.in ());

          if (r != 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) unexpected result = %d for %d",
                          r, i));
            }
        }

      if (options.abort_server)
        {
          try
            {
              server->shutdown_now (false);
            }
          catch (CORBA::COMM_FAILURE const &)
            {
              // Expected exception, continue....
            }
        }
      else if (options.crash_server)
        {
          try
            {
              server->shutdown_now (true);
            }
          catch (CORBA::COMM_FAILURE const &)
            {
              // Expected exception, continue....
            }
        }
      else if (options.suicide)
        {
          (void) server->test_method (true, false, callback.in ());
          // The shutdown callback could arrive after this twoway invocation
          // returned. Wait for it shutdown callback and abort,
          // otherwise it will timeout (in run_test.pl).
          ACE_OS::sleep (120);
        }
      else if (options.self_shutdown)
        {
          (void) server->test_method (true, true, callback.in ());
        }

      if (options.shutdown)
        {
          server->shutdown ();
        }

      root_poa->destroy (true, true);
    }
  catch (CORBA::COMM_FAILURE const &x)
    {
      // For other case this is expected.
      if (!options.self_shutdown)
        {
          x._tao_print_exception ("ERROR: Unexpected exception\n");
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client:");
      return -1;
    }

  return 0;
}
