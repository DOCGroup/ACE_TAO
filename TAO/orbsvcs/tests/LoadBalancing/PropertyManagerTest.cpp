
#include "PropertyManagerTest.h"

ACE_RCSID (LoadBalancing,
           PropertyManagerTest,
           "$Id$")

PropertyManagerTest::PropertyManagerTest (
  TAO_LoadBalancer::PropertyManager_ptr property_manager)
  : property_manager_ (TAO_LoadBalancer::PropertyManager::duplicate (
                         property_manager)),
    hasher_factory_ ()
{
}

int
PropertyManagerTest::run (void)
{
  int result = 0;

  if (this->set_default_properties () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "set_default_properties() FAILED\n"));
      result = -1;
    }

  if (this->test_invalid_property () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "test_invalid_property() FAILED\n"));
      result = -1;
    }

  if (this->test_unsupported_property () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "test_unsupported_property() FAILED\n"));
      result = -1;
    }

  if (this->get_default_properties () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "get_default_properties() FAILED\n"));
      result = -1;
    }

  if (this->remove_default_properties () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "remove_default_properties() FAILED\n"));
      result = -1;
    }

  if (this->set_type_properties () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "set_type_properties() FAILED\n"));
      result = -1;
    }

  if (this->get_type_properties () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "get_type_properties() FAILED\n"));
      result = -1;
    }

  if (this->remove_type_properties () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "remove_type_properties() FAILED\n"));
      result = -1;
    }

  if (this->set_properties_dynamically () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "set_properties_dynamically() FAILED\n"));
      result = -1;
    }

  if (this->test_object_group_not_found () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "test_object_group_not_found() FAILED\n"));
      result = -1;
    }

  if (this->get_properties () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "get_properties() FAILED\n"));
      result = -1;
    }

  return result;
}

int
PropertyManagerTest::set_default_properties (void)
{
  if (this->test_invalid_property () != 0
      || this->test_unsupported_property != 0)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      const CORBA::UShort INIT_NUM_REPLICAS = 4;
      const CORBA::UShort MIN_NUM_REPLICAS = 3;

      // Properties:
      //   InitialNumberReplicas
      //   MinimumNumberReplicas
      //   Factories
      const CORBA::ULong DEFAULT_PROPERTY_COUNT = 3;

      TAO_LoadBalancer::Properties properties;
      properties.length (DEFAULT_PROPERTY_COUNT);

      // Default initial number of replicas
      TAO_LoadBalancer::Property initial_number_replicas;
      initial_number_replicas.nam.length (1);
      initial_number_replicas.nam[0].id =
        CORBA::string_dup ("InitialNumberReplicas");
      initial_number_replicas.val = INIT_NUM_REPLICAS;
      properties[0] = initial_number_replicas;

      // Default minimum number of replicas
      TAO_LoadBalancer::Property minimum_number_replicas;
      minimum_number_replicas.nam.length (1);
      minimum_number_replicas.nam[0].id =
        CORBA::string_dup ("MinimumNumberReplicas");
      minimum_number_replicas.val = MIN_NUM_REPLICAS;
      properties[1] = minimum_number_replicas;

      // Default factories (simulated locations)
      TAO_LoadBalancer::Property factories;
      factories.nam.length (1);
      factories.nam[0].id = CORBA::string_dup ("Factories");

      // Implicitly activate the HasherFactory.
      CORBA::Object_var obj =
        this->hasher_factory_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      TAO_LoadBalancer::GenericFactory_var factory =
        TAO_LoadBalancer::GenericFactory::_narrow (obj.in (),
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      TAO_LoadBalancer::FactoryInfos factory_infos;
      factory_infos.length (INIT_NUM_REPLICAS);
      for (CORBA::ULong i = 0; i < factory_infos.length (); ++i)
        {
          TAO_LoadBalancer::FactoryInfo &factory_info =
            factory_infos[i];

          // For this test, the same GenericFactory is used to create
          // a replica at each logical location.  Of course, a
          // deployed application would most likely have a
          // GenericFactory at each physical location, or at least one
          // capable of creating replicas at multiple physical
          // locations.
          factory_info.the_factory =
            TAO_LoadBalancer::GenericFactory::duplicate (factory.in ());

          // Create a logical location for each factory.
          char location[BUFSIZ] = { 0 };
          ACE_OS::sprintf (location, "%u", i);
          factory_info.the_location.id =
            CORBA::string_dup (location);
          factory_info.the_location.kind =
            CORBA::string_dup ("location number");

          // @@ TODO: Add factory-specific Criteria.
          // factory_info.the_criteria ...
        }

      factories.val = factory_infos;
      properties[2] = factories;

      // @@ TODO: Add the following properties to the sequence of
      //    default properties:
      //       ReplicationStyle
      //       MembershipStyle
      //       ConsistencyStyle          (?)
      //       LoadMonitoringStyle
      //       LoadMonitoringGranularity
      //       LoadMonitoringInterval
      //       CheckpointInterval        (?)

      this->property_manager_->set_default_properties (properties,
                                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;


  return 0;
}

int
PropertyManagerTest::test_invalid_property (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Property property;
      property.nam.length (1);
      property.nam[0].id = CORBA::string_dup ("FOO_BAR_BAZ");

      TAO_LoadBalancer::Properties properties;
      properties.length (1);
      properties[0] = property;

      this->property_manager_->set_default_properties (properties,
                                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (TAO_LoadBalancer::InvalidProperty, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Successfully caught %s exception\n"),
                  ex._id ()));

      for (CORBA::ULong j = 0; j < ex.nam.length (); ++j)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("  nam[%u].id:   %s\n")
                    ACE_TEXT ("          kind: %s\n"),
                    j,
                    ex.nam[j].id,
                    ex.nam[j].kind));
      // @@ What's the best way to print the contents of a ex.val,
      //    i.e. an Any?
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::test_unsupported_property (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Property property;
      property.nam.length (1);
      property.nam[0].id = CORBA::string_dup ("FOO_BAR_BAZ");

      TAO_LoadBalancer::Properties properties;
      properties.length (1);
      properties[0] = property;

      this->property_manager_->set_default_properties (properties,
                                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (TAO_LoadBalancer::UnsupportedProperty, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Successfully caught %s exception\n"),
                  ex._id ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::get_default_properties (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties_var properties =
        this->property_manager_->get_default_properties (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      for (CORBA::ULong i = 0; i < properties->length (); ++i)
        {
          // Property values are not displayed since they are stored
          // as Anys.

          TAO_LoadBalancer::Property &property = properties[i];

          for (CORBA::ULong j = 0; j < property.length (); ++j)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("properties[%u].nam[%u].id   = <%s>\n")
                        ACE_TEXT ("                       kind = <%s>\n"),
                        i,
                        j,
                        property[j].id,
                        property[j].kind));
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }

  return 0;
}

int
PropertyManagerTest::remove_default_properties (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties current_properties =
        this->property_manager_->get_default_properties (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Make sure some default properties are already set.
      if (current_properties.length () == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("No default properties set")),
                          -1);

      // Remove the first default property.
      TAO_LoadBalancer::Property &removed_property =
        current_properties[0];

      TAO_LoadBalancer::Properties removed_properties;
      removed_properties.length (1);
      removed_properties[0] = removed_property;

      this->property_manager_->remove_default_properties (removed_properties,
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      removed_properties =
        this->property_manager_->get_default_properties (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ TODO: The following is a weak test.  It needs improvement.
      //
      // Iterate through the sequence of id values in each property.
      // If one in the properties after removal matches its
      // corresponding id in the property that was removed then no
      // removal occurred.
      for (CORBA::ULong i = 0; i < removed_properties.length (); ++i)
        for (CORBA::ULong j = 0; j < removed_property.length (); ++j)
          if (ACE_OS::strcmp (removed_property[j].id,
                              removed_properties[i][j].id) == 0)
            return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::set_type_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      const char *type_id =
        HasherFactory::repository_type_id ();

      TAO_LoadBalancer::Properties overrides;

      this->property_manager_->set_type_properties (type_id,
                                                    overrides,
                                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::get_type_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties_var properties =
        this->get_type_properties (type_id, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::remove_type_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties properties;

      this->property_manager_->remove_type_properties (type_id,
                                                       properties,
                                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::set_properties_dynamically (void)
{
  // @@ TODO: Implement

  if (this->test_object_group_not_found () != 0)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->property_manager_->set_properties_dynamically (object_group,
                                                           overrides,
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::test_object_group_not_found (void)
{
  // @@ TODO:: Implement
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_TRY_CHECK;
    }
  ACE_CATCH (TAO_LoadBalancer::ObjectGroupNotFound, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Successfully caught %s exception\n"),
                  ex._id ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
PropertyManagerTest::get_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties_var properties =
        this->property_manager_->get_properties (object_group,
                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));
 
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
