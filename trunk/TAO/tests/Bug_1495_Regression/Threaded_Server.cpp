// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "test_i.h"
#include "server_interceptor.h"
#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
const ACE_TCHAR *ior_input_file = ACE_TEXT("file://thr_server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:o:"));
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Create the ORB initializer.
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);

      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      // Now initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                                            "Remote_Server_ORB");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n" ),
                             1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

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
            "server request interceptor.\n"),
            -1);
        }

      // Pull in the ior from the remote server to use as the forward location.
      CORBA::Object_var forward_location = orb->string_to_object (ior_input_file);

      if (CORBA::is_nil (forward_location.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior_input_file),
                             1);
        }

      server_interceptor->forward_reference (forward_location.in ());

      Bug1495_i server_impl (orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var test_obj =
        root_poa->id_to_reference (id.in ());

      Bug1495_Regression::Bug1495_var server =
        Bug1495_Regression::Bug1495::_narrow (test_obj.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

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

      orb->run (tv);

      if (server_interceptor->forward_location_done() == false)
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Forward location has not occured!\n"));
        }

      ACE_DEBUG ((LM_DEBUG, "Threaded Server event loop finished\n"));
      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught an exception in server:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "Threaded Server ready\n"));

  return 0;
}


