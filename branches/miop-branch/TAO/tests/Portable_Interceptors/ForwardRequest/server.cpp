// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "Server_ORBInitializer.h"
#include "Server_Request_Interceptor.h"

ACE_RCSID (ForwardRequest,
           server,
           "$Id$")

const char *ior1_file = 0;
const char *ior2_file = 0;

int
parse_args (int argc, char *argv[])
{
  if (argc != 5)  // foo -o IOR_1 -o IOR_2
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Wrong number of arguments.\n"),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  int ior_count = 1;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':

        if (ior_count == 1)
          {
            ior1_file = get_opts.opt_arg ();
            ++ior_count;
          }
        else if (ior_count == 2)
          {
            ior2_file = get_opts.opt_arg ();
            ++ior_count;
          }

        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o IOR_1 -o IOR_2\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      Server_ORBInitializer *temp_initializer = 0;
      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (::parse_args (argc, argv) != 0)
        return -1;

      CORBA::PolicyList policies;  // Empty policy list.

      // Servant 1
      test_i servant1 (1, orb.in ());

      PortableServer::POA_var first_poa =
        root_poa->create_POA ("first POA",
                              poa_manager.in (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var oid1 =
        first_poa->activate_object (&servant1
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj1 =
        first_poa->servant_to_reference (&servant1
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior1 =
        orb->object_to_string (obj1.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "ForwardRequestTest::test servant 1: <%s>\n",
                  ior1.in ()));

      // Servant 2
      test_i servant2 (2, orb.in ());

      PortableServer::POA_var second_poa =
        root_poa->create_POA ("second POA",
                              poa_manager.in (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var oid2 =
        second_poa->activate_object (&servant2
                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj2 =
        second_poa->servant_to_reference (&servant2
                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior2 =
        orb->object_to_string (obj2.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "ForwardRequestTest::test servant 2: <%s>\n",
                  ior2.in ()));

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set the forward references in the server request interceptor.
      PortableInterceptor::ServerRequestInterceptor_var
        server_interceptor = temp_initializer->server_interceptor ();

      ForwardRequestTest::ServerRequestInterceptor_var interceptor =
        ForwardRequestTest::ServerRequestInterceptor::_narrow (
           server_interceptor.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (interceptor.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Could not obtain reference to "
                           "server request interceptor.\n"),
                          -1);

      interceptor->forward_references (obj1.in (),
                                       obj2.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Write each IOR to a file.

      // IOR 1
      FILE *output_file= ACE_OS::fopen (ior1_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file <%s> for writing "
                           "IOR: %s",
                           ior1.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior1.in ());
      ACE_OS::fclose (output_file);

      // IOR 2
      output_file= ACE_OS::fopen (ior2_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior2.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior2.in ());
      ACE_OS::fclose (output_file);

      // Run the ORB event loop.
      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
