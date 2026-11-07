#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "test_i.h"

#include "tao/Strategies/advanced_resource.h"

struct Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://ior")};
  CORBA::ULong event_loop_depth {6};
  CORBA::ULong event_loop_iterations {6};
  bool shutdown_server {};
};

static int
parse_args (int argc, ACE_TCHAR **argv, Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:d:i:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.event_loop_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        options.event_loop_depth = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        options.shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-i event loop iterations "
                           "-d event loop depth "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      Options options {};
      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Get an object reference from the argument string.
      object = orb->string_to_object (options.ior);

      // Try to narrow the object reference to a <server> reference.
      server_var server_object = server::_narrow (object.in ());

      client_i *servant {};
      ACE_NEW_RETURN (servant,
                      client_i (server_object.in ()),
                      1);
      PortableServer::ServantBase_var client_transfer (servant);

      servant->loop (options.event_loop_depth,
                     options.event_loop_iterations);

      // Shutdown server.
      if (options.shutdown_server)
        {
          server_object->shutdown ();
        }

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
