
#include "LB_test.h"

ACE_RCSID (LB_test, PropertyManager, "$Id$")

int
LB_test::set_default_properties (void)
{
  if (this->test_invalid_property () != 0
      || this->test_unsupported_property != 0)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      const int INIT_NUM_REPLICAS = 4;
      const int MIN_NUM_REPLICAS = 3;

      const int DEFAULT_PROPERTY_COUNT = 3;
      TAO_LoadBalancer::Properties properties;
      properties.length (DEFAULT_PROPERTY_COUNT);

      // Default initial number of replicas
      TAO_LoadBalancer::Property initial_number_replicas;
      property.nam.length (1);
      property.nam[0].id = CORBA::string_dup ("InitialNumberReplicas");
      property.val = INIT_NUM_REPLICAS;
      properties[0] = initial_number_replicas;

      //Default minimum number of replicas
      TAO_LoadBalancer::Property minimum_number_replicas;
      property.nam.length (1);
      property.nam[0].id = CORBA::string_dup ("MinimumNumberReplicas");
      property.val = MIN_NUM_REPLICAS;
      properties[1] = minimum_number_replicas;

      // Default factories (simulated locations)
      TAO_LoadBalancer::Property factories;
      property.nam.length (1);
      property.nam[0].id = CORBA::string_dup ("Factories");

      TAO_LoadBalancer::FactoryInfos factory_infos;
      factory_infos.length (INIT_NUM_REPLICAS);
      for (int i = 0; i < factory_infos.length (); ++i)
        {
          TAO_LoadBalancer::FactoryInfo &factory_info =
            factory_infos[i];

          TAO_LoadBalancer::GenericFactory_var factory =
            factory_servant[i]->_this (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          factory_info.factory = factory.in ();

          // Create a fake location for each factory.
          char location[BUFSIZ] = { 0 };
          ACE_OS::sprintf (location, "%d", i);
          factory_info.the_location.id =
            CORBA::string_dup (location);
          factory_info.the_location.kind =
            CORBA::string_dup ("location number");

          // @@ TODO: Add factory-specific Criteria.
          // factory_info.the_criteria ...
        }      property.val = factory_infos;
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

      this->load_balancer->set_default_properties (properties,
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
LB_test::test_invalid_property (void)
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

      this->load_balancer->set_default_properties (properties,
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (TAO_LoadBalancer::InvalidProperty, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Successfully caught %s exception\n"),
                  ex._id ()));

      for (int j = 0; j < ex.nam.length (); ++j)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("  nam[%d].id:   %s\n")
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
LB_test::test_unsupported_property (void)
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

      this->load_balancer->set_default_properties (properties,
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
LB_test::get_default_properties (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties properties =
        this->load_balancer->get_default_properties (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      for (int i = 0; i < properties.length (); ++i)
        {
          // Property values are not displayed since they are stored
          // as Anys.

          TAO_LoadBalancer::Property &property = properties[i];

          for (int j = 0; j < property.length (); ++j)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("properties[%d].nam[%d].id   = <%s>\n")
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
LB_test::remove_default_properties (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties current_properties =
        this->load_balancer->get_default_properties (ACE_TRY_ENV);
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

      this->load_balancer->remove_default_properties (removed_properties,
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      removed_properties =
        this->load_balancer->get_default_properties (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ TODO: The following is a weak test.  It needs improvement.
      //
      // Iterate through the sequence of id values in each property.
      // If one in the properties after removal matches the its
      // corresponding id in the property that was removed then no
      // removal occurred.
      for (int i = 0; i < removed_properties.length (); ++i)
        for (int j = 0; j < removed_property.length (); ++j)
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
LB_test::set_type_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties overrides;

      this->load_balancer->set_type_properties (type_id,
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
LB_test::get_type_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties =
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
LB_test::remove_type_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties properties;

      this->load_balancer->remove_type_properties (type_id,
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
LB_test::set_properties_dynamically (void)
{
  // @@ TODO: Implement

  if (this->test_object_group_not_found () != 0)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      this->load_balancer->set_properties_dynamically (object_group,
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
LB_test::test_object_group_not_found ()
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
LB_test::get_properties (void)
{
  // @@ TODO: Implement

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer::Properties properties =
        this->load_balancer->get_properties (object_group,
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
