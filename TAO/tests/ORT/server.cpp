// $Id$

#include "ORT_test_i.h"
#include "ORT_test_IORInterceptor_ORBInitializer.h"
#include "tao/PortableInterceptorC.h"

#include "tao/ORB_Core.h"

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

int main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
#if TAO_HAS_INTERCEPTORS == 1

      PortableInterceptor::ORBInitializer_ptr orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (orb_initializer,
                      ORT_test_IORInterceptor_ORBInitializer,
                      -1); // No CORBA exceptions yet!

      PortableInterceptor::ORBInitializer_var orb_initializer_var =
        orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer_var.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#endif /* TAO_HAS_INTERCEPTORS == 1 */

      /// Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Narrow
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Check for nil references
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);

      /// Get poa_manager reference
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Activate it.
      poa_manager->activate (TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ///@}

      /// First lets check if the new -ORBId, -ORBServerId options are
      /// working correctly.
      CORBA::String_var orb_id = orb->orb_core ()->orbid ();

      if (ACE_OS::strcmp (orb_id.in (), "ORT_test_ORB") != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "-ORBId option not working correctly"));
          return -1;
        }

      CORBA::String_var server_id = orb->orb_core ()->server_id ();

      if (ACE_OS::strcmp (server_id.in (), "ORT_test_server") != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "-ORBServerId option not working correctly"));
          return -1;
        }

      CORBA::PolicyList policies (0);
      policies.length (0);

      /// Lets create some POA's
      PortableServer::POA_var first_poa =
        root_poa->create_POA ("FIRST_POA",
                              poa_manager.in (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var second_poa =
        root_poa->create_POA ("SECOND_POA",
                              poa_manager.in (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var third_poa =
        first_poa->create_POA ("THIRD_POA",
                              poa_manager.in (),
                              policies
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var fourth_poa =
        second_poa->create_POA ("FOURTH_POA",
                                poa_manager.in (),
                                policies
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ORT_test_i ort_test_impl;

      /// Activate
      obj = ort_test_impl._this (TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Narrow it down.
      ObjectReferenceTemplate::ORT_test_var ort_test =
        ObjectReferenceTemplate::ORT_test::_narrow (obj.in ()
                                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Check for nil reference
      if (CORBA::is_nil (ort_test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain reference to ",
                           "ObjectReferenceTemplate::ort_test ",
                           "object.\n"),
                          -1);

      /// Convert the object reference to a string format.
      CORBA::String_var ior =
        orb->object_to_string (ort_test.in () TAO_ENV_ARG_PARAMETER);
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

      orb->run (TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
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
