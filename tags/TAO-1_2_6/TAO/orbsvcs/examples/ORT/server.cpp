// $Id$

#include "sum_server_i.h"
#include "Server_IORInterceptor_ORBInitializer.h"

#include "tao/PortableInterceptorC.h"

#include "ace/Get_Opt.h"

const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
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

  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
#if TAO_HAS_INTERCEPTORS == 1

      PortableInterceptor::ORBInitializer_ptr orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (orb_initializer,
                      Server_IORInterceptor_ORBInitializer,
                      -1); // No CORBA exceptions yet!

      PortableInterceptor::ORBInitializer_var orb_initializer_var =
        orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer_var.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#endif /* TAO_HAS_INTERCEPTORS == 1 */

      /// The usual initialization stuff
      /// {@

      /// Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "server_sum_orb"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      /// Resolve reference to RootPOA
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Narrow it down correctly.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Check for nil references
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);

      /// Get poa_manager reference
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Activate it.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ///@}

      /// initialize the sum_server
      sum_server_i sum_server_impl;

      /// Activate
      obj = sum_server_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Narrow it down.
      ORT::sum_server_var sum_server =
        ORT::sum_server::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Check for nil reference
      if (CORBA::is_nil (sum_server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain reference to ORT::sum_server "
                           "object.\n"),
                          -1);

      /// Convert the object reference to a string format.
      CORBA::String_var ior =
        orb->object_to_string (sum_server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// If the ior_output_file exists, output the IOR to it.
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

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_INFO, "Successful.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ORT example server:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
