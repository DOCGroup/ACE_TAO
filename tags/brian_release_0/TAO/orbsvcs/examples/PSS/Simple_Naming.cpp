// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/examples/PSS
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//      This class implements a simple CORBA server which uses the
//      simple_naming.idl to bind and find a object reference and uses
//      Persistent State Service to save the information persistently.
//      Serves like a Simple naming service.
//
// = AUTHOR
//     Priyanka Gontla <gontla_p@ociweb.com>
// ============================================================================

#include "Simple_Naming_i.h"
#include "ace/Get_Opt.h"

ACE_RCSID (PSS, client, "$Id$")

const char *ior_output_file = "simple_naming.ior";

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
                           "usage:  %s "
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        return -1;

      // Get a reference to the RootPOA
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow down to the correct reference
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set a POA Manager
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate the POA Manager
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);

      // Id Assignment policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Lifespan policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // We use a different POA, otherwise the user would have to change
      // the object key each time it invokes the server.
      PortableServer::POA_var poa =
        root_poa->create_POA ("Simple_Naming",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      Naming_Context_i simple_naming_i (orb.in ());

      // Activate it to obtain the reference
      Simple_Naming::Naming_Context_var simple_naming =
        simple_naming_i._this ();

      CORBA::String_var string_obj_ref =
        orb->object_to_string (simple_naming.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", string_obj_ref.in ());
      ACE_OS::fclose (output_file);

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected excpeption in PSS Test");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
