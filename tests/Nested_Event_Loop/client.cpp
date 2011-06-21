// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "test_i.h"

#include "tao/Strategies/advanced_resource.h"

// Name of file contains ior.
static const ACE_TCHAR *IOR = ACE_TEXT ("file://ior");

// Default event_loop_depth.
static CORBA::ULong event_loop_depth = 6;

// Default event_loop_iterations.
static CORBA::ULong event_loop_iterations = 6;

// Flag indicates whether to shutdown remote server or not upon client
// shutdown.
static int shutdown_server = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:d:i:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'i':
        event_loop_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        event_loop_depth = ACE_OS::atoi (get_opts.opt_arg ());
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      int parse_args_result = parse_args (argc, argv);
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
      object = orb->string_to_object (IOR);

      // Try to narrow the object reference to a <server> reference.
      server_var server_object = server::_narrow (object.in ());

      client_i * servant = 0;
      ACE_NEW_RETURN (servant,
                      client_i(server_object.in ()),
                      1);
      PortableServer::ServantBase_var client_transfer(servant);

      servant->loop (event_loop_depth,
                     event_loop_iterations);

      // Shutdown server.
      if (shutdown_server)
        {
          server_object->shutdown ();
        }

      root_poa->destroy (1,
                         1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }


  return 0;
}
