// $Id$

#include "McastHello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/PortableGroup/MIOP.h"
#include "orbsvcs/PortableGroup/GOA.h"

ACE_RCSID (McastHello,
           server,
           "$Id$")

static const char *ior_output_file = "test.ior";

// Use a multicast address in the administrative "site local" range, 239.255.0.0 to
// 239.255.255.255.  The range 224.255.0.0 to 238.255.255.255 should also be valid
// too.
//static const char *group_ior = "corbaloc:miop:1.0@1.0-TestDomain-1/239.255.0.1:16000";
static const char *group_ior = "corbaloc:miop:1.0@1.0-TestDomain-1/224.1.239.2:1234";

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

      case 'g':
        group_ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-g <group ior corbaloc>"
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableGroup::GOA_var root_poa =
        PortableGroup::GOA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the group IOR.
      CORBA::String_var ior = CORBA::string_dup (group_ior);


      CORBA::Object_var group1 =
        orb->string_to_object (ior.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Output the Group IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      PortableServer::ObjectId_var id =
        root_poa->create_id_for_reference (group1.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create and activate an instance of our servant.
      McastHello server_impl (orb.in (), 0);

      root_poa->activate_object_with_id (id.in (),
                                         &server_impl
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Validate that our servants got the right requests.
      if (server_impl.get_status () == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Multicast Servant did not receive expected requests!\n"),
                          1);
      else
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Success!\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
