// -*- C++ -*-

#include "ace/Get_Opt.h"

#include "HasherFactory.h"

//#include "PropertyManagerTest.h"

ACE_RCSID (LoadBalancer,
           server,
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

LoadBalancing::Criteria *
setup_criteria (HasherFactory &hasher_factory)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      const CORBA::UShort INIT_NUM_REPLICAS = 4;
      const CORBA::UShort MIN_NUM_REPLICAS = 3;

      // Criteria:
      //   InitialNumberReplicas
      //   MinimumNumberReplicas
      //   Factories
      const CORBA::ULong DEFAULT_PROPERTY_COUNT = 3;

      LoadBalancing::Criteria *tmp = 0;
      ACE_NEW_RETURN (tmp,
                      LoadBalancing::Criteria (DEFAULT_PROPERTY_COUNT),
                      0);

      LoadBalancing::Criteria_var criteria = tmp;
      
      criteria->length (DEFAULT_PROPERTY_COUNT);

      // Default initial number of replicas
      LoadBalancing::Property &initial_number_replicas =
        criteria[CORBA::ULong (0)];

      initial_number_replicas.nam.length (1);
      initial_number_replicas.nam[CORBA::ULong (0)].id =
        CORBA::string_dup ("InitialNumberReplicas");
      initial_number_replicas.val <<= INIT_NUM_REPLICAS;

      // Default minimum number of replicas
      LoadBalancing::Property &minimum_number_replicas =
        criteria[CORBA::ULong (1)];

      minimum_number_replicas.nam.length (1);
      minimum_number_replicas.nam[CORBA::ULong (0)].id =
        CORBA::string_dup ("MinimumNumberReplicas");
      minimum_number_replicas.val <<= MIN_NUM_REPLICAS;


      // Default factories (simulated locations)
      LoadBalancing::Property &factories =
        criteria[CORBA::ULong (2)];

      factories.nam.length (1);
      factories.nam[CORBA::ULong (0)].id = CORBA::string_dup ("Factories");

      // Implicitly activate the HasherFactory.
      CORBA::Object_var obj =
        hasher_factory._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      LoadBalancing::GenericFactory_var factory =
        LoadBalancing::GenericFactory::_narrow (obj.in (),
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      LoadBalancing::FactoryInfos factory_infos;
      factory_infos.length (INIT_NUM_REPLICAS);
      for (CORBA::ULong i = 0; i < factory_infos.length (); ++i)
        {
          LoadBalancing::FactoryInfo &factory_info =
            factory_infos[i];

          // For this test, the same GenericFactory is used to create
          // a replica at each logical location.  Of course, a
          // deployed application would most likely have a
          // GenericFactory at each physical location, or at least one
          // capable of creating replicas at multiple physical
          // locations.
          factory_info.the_factory =
            LoadBalancing::GenericFactory::_duplicate (factory.in ());

          factory_info.the_location.length (1);

          // Create a logical location for each factory.
          char location[BUFSIZ] = { 0 };
          ACE_OS::sprintf (location, "%u", i);
          factory_info.the_location[CORBA::ULong (0)].id =
            CORBA::string_dup (location);
          factory_info.the_location[CORBA::ULong (0)].kind =
            CORBA::string_dup ("location number");
        }

      factories.val <<= factory_infos;

      // @@ TODO: Add the following criteria to the sequence of
      //    default properties:
      //       ReplicationStyle
      //       MembershipStyle
      //       ConsistencyStyle          (?)
      //       LoadMonitoringStyle
      //       LoadMonitoringGranularity
      //       LoadMonitoringInterval
      //       CheckpointInterval        (?)

//       this->property_manager_->set_default_properties (criteria,
//                                                        ACE_TRY_ENV);
//       ACE_TRY_CHECK;

      return criteria._retn ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));

      return 0;
    }
  ACE_ENDTRY;


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
        orb->resolve_initial_references ("LoadBalancing",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      LoadBalancing::GenericFactory_var factory =
        LoadBalancing::GenericFactory::_narrow (lb.in (),
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "GenericFactory reference is nil.\n"),
                          -1);

//       LoadBalancing::PropertyManager_var property_manager =
//         LoadBalancing::PropertyManager::_narrow (lb.in (),
//                                                  ACE_TRY_ENV);
//       ACE_TRY_CHECK;

//       if (CORBA::is_nil (property_manager.in ()))
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "PropertManager reference is nil.\n"),
//                           -1);


      HasherFactory hasher_factory (orb.in (), root_poa.in ());

      // Set up the criteria to be used when creating the object
      // group, and activate the hasher factory.
      LoadBalancing::Criteria_var the_criteria =
        ::setup_criteria (hasher_factory);

      // The FactoryCreationId
      LoadBalancing::GenericFactory::FactoryCreationId_var
        factory_creation_id;

      // Create a replicated Hasherobject (object group)
      obj = factory->create_object (HasherFactory::replica_type_id (),
                                    the_criteria.in (),
                                    factory_creation_id.out (),
                                    ACE_TRY_ENV);
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

      // Run the ORB event loop.
      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;


      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
      // @@ Move this to a point just before shutdown!
      // Destroy the object group.
      factory->delete_object (factory_creation_id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;


      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
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
