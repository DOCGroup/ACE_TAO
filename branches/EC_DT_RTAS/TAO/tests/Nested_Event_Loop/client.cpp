// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "test_i.h"

#include "tao/Strategies/advanced_resource.h"

ACE_RCSID(Nested_Event_Loop, client, "$Id$")

// Name of file contains ior.
static const char *IOR = "file://ior";

// Default event_loop_depth.
static CORBA::ULong event_loop_depth = 6;

// Default event_loop_iterations.
static CORBA::ULong event_loop_iterations = 6;

// Flag indicates whether to shutdown remote server or not upon client
// shutdown.
static int shutdown_server = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:d:i:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'i':
        event_loop_iterations = ::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        event_loop_depth = ::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
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

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize options based on command-line arguments.
      int parse_args_result = parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get an object reference from the argument string.
      object = orb->string_to_object (IOR
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Try to narrow the object reference to a <server> reference.
      server_var server_object = server::_narrow (object.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client_i servant (server_object.in ());

      servant.loop (event_loop_depth,
                    event_loop_iterations
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Shutdown server.
      if (shutdown_server)
        {
          server_object->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      root_poa->destroy (1,
                         1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);

  return 0;
}
