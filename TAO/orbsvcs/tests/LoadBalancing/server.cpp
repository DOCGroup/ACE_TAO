// -*- C++ -*-
//
// $Id$

#include "ace/Get_Opt.h"

#include "orbsvcs/LoadBalancingC.h"

#include "PropertyManagerTest.h"

ACE_RCSID (LoadBalancer,
           server.cpp,
           "$Id$")

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
//       case 'i':
//         // Infrastructure-controlled membership test
//         break;
//       case 'a':
//         // Application-controlled membership test
//         break;
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "test_orb",
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      // Obtain Reference to the TAO LoadBalancer ReplicationManager
      CORBA::Object_var lb =
        orb->resolve_initial_references ("TAO_LoadBalancer",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      LoadBalancing::GenericFactory_var factory =
        TAO_LoadBalancer::GenericFactory::_narrow (lb.in (),
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "GenericFactory reference is nil.\n"),
                          -1);

      TAO_LoadBalancer::PropertyManager_var property_manager =
        TAO_LoadBalancer::PropertyManager::_narrow (lb.in (),
                                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (property_manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "PropertManager reference is nil.\n"),
                          -1);

      // Run the PropertyManager tests, in addition to setting up the
      // Load Balancer for the remainder of this test.

      // The FactoryCreationId
      TAO_LoadBalancer::FactoryCreationId_var factory_creation_id;

      // Create a replicated Hasherobject (object group)
      obj = factory->create_object (HasherFactory::repository_type_id (),
                                    the_criteria,
                                    factory_creation_id.out (),
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ Move this to a point just before shutdown!
      // Destroy the object group.
      factory->delete_object (factory_creation_id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Application-Controlled MembershipStyle

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Replica group reference is nil.\n"),
                          -1);

      CORBA::String_var ior =
        orb->object_to_string (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_INFO, "Hasher: <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the IOR to it.
      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing "
                               "IOR: %s",
                               ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (&ACE_ANY_EXCEPTION,
                           "Caught exception:");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
