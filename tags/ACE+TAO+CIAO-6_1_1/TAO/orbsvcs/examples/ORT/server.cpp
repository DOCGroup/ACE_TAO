// $Id$

#include "sum_server_i.h"
#include "Server_IORInterceptor_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-o <iorfile>"
                           "\n",
                           argv[0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
#if TAO_HAS_INTERCEPTORS == 1

      PortableInterceptor::ORBInitializer_ptr orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (orb_initializer,
                      Server_IORInterceptor_ORBInitializer,
                      -1); // No CORBA exceptions yet!

      PortableInterceptor::ORBInitializer_var orb_initializer_var =
        orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer_var.in ());

#endif /* TAO_HAS_INTERCEPTORS == 1 */

      // The usual initialization stuff

      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "server_sum_orb");

      if (parse_args (argc, argv) != 0)
        return -1;

      // Resolve reference to RootPOA
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow it down correctly.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Check for nil references
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);

      // Get poa_manager reference
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Activate it.
      poa_manager->activate ();

      // initialize the sum_server
      sum_server_i sum_server_impl;

      // Activate
      obj = sum_server_impl._this ();

      // Narrow it down.
      ORT::sum_server_var sum_server =
        ORT::sum_server::_narrow (obj.in ());

      // Check for nil reference
      if (CORBA::is_nil (sum_server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain reference to ORT::sum_server "
                           "object.\n"),
                          -1);

      // Convert the object reference to a string format.
      CORBA::String_var ior =
        orb->object_to_string (sum_server.in ());

      // If the ior_output_file exists, output the IOR to it.
      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing "
                               "IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run ();

      ACE_DEBUG ((LM_INFO, "Successful.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ORT example server:");
      return -1;
    }

  return 0;
}
