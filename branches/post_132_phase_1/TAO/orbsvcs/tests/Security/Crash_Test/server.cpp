// $Id$

#include "ace/Get_Opt.h"
#include "test_i.h"

ACE_RCSID(Send_File, server, "$Id$")

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
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): usage:  %s "
                           "-o <iorfile> -w <wait-time>"
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
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var persistent_poa =
        root_poa->create_POA ("PersistentPOA",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("object1");
      ACE_TRY_CHECK;

      persistent_poa->activate_object_with_id (oid.in (),
                                               &server_impl
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::Object_var server =
        persistent_poa->create_reference_with_id (oid.in (),
                                                  "IDL:Simple_Server:1.0"
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "SERVER (%P): Cannot open output file "
                               "for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as file://%s\n",
                      ior_output_file));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as <%s>\n",
                      ior.in ()));
        }


      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "SERVER (%P): Event loop finished.\n"));

      persistent_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::sleep (1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "SERVER (%P): Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
