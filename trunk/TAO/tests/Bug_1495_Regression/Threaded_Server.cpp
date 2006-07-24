// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "test_i.h"
#include "server_interceptor.h"
#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (Service_Context_Manipulation,
           server,
           "$Id$" )

const char *ior_output_file = "test.ior";
const char *ior_input_file = "file://thr_server.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:o:");
  int c;


  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'i':
          ior_input_file = get_opts.opt_arg ();
          break;
        case 'o':
          ior_output_file = get_opts.opt_arg ();
          break;
        case '?':
          default:
          ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <iorfile>"
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Create the ORB initializer.
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);

      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                                            "Remote_Server_ORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA. \n" ),
                             1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      // Create the interceptor.
      Echo_Server_Request_Interceptor * server_interceptor =
        temp_initializer->server_interceptor();

      if (server_interceptor == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            "(%P|%t) Could not obtain reference to "
            "server request interceptor. \n"),
            -1);
        }

      // Pull in the ior from the remote server to use as the forward location.
      CORBA::Object_var forward_location = orb->string_to_object (ior_input_file
                                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (forward_location.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil \n",
                             ior_input_file),
                             1);
        }

      server_interceptor->forward_reference (forward_location.in ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Bug1495_i server_impl (orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&server_impl ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var test_obj =
        root_poa->id_to_reference (id.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Bug1495_Regression::Bug1495_var server =
        Bug1495_Regression::Bug1495::_narrow (test_obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Output the server IOR to a file
      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

          if (output_file == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot open output file for writing the "
                                 "server IOR: %s", ior_output_file),
                                 1);
            }

          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      ACE_Time_Value tv (15, 0);

      orb->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (server_interceptor->forward_location_done() == false)
        {
          ACE_ERROR ((LM_ERROR, "ERRROR: Forward location has not occured!"));
        }

      ACE_DEBUG ((LM_DEBUG, "Threaded Server event loop finished \n"));
      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught an exception in server:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "Threaded Server ready\n"));

  return 0;
}


