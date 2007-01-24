// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
const char *ior_output_file = 0;
const char *key = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'k':
        key = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
               "-k <key>"
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

      // We do the command line parsing first
      if (parse_args (argc, argv) != 0)
        return 1;
      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies;
      policies.length (3);
      policies[0] = root_poa->create_id_assignment_policy (
          PortableServer::USER_ID ACE_ENV_ARG_PARAMETER);
      policies[1] = root_poa->create_implicit_activation_policy (
          PortableServer::NO_IMPLICIT_ACTIVATION ACE_ENV_ARG_PARAMETER);
      policies[2] = root_poa->create_lifespan_policy (
          PortableServer::PERSISTENT ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var poa = root_poa->create_POA (
          "PERS_POA", poa_manager.in (), policies ACE_ENV_ARG_PARAMETER);

      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          policies[i]->destroy ();
        }

      // Instantiate the LCD_Display implementation class
      Simple_Server_i display_impl (orb.in (), key);
      PortableServer::ObjectId_var id =
          PortableServer::string_to_ObjectId ("IOGR_OID");

      poa->activate_object_with_id (id.in(), &display_impl ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var server =
          poa->id_to_reference (id.in () ACE_ENV_ARG_PARAMETER);

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      orb->run ();

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
