
#include "LB_test.h"

ACE_RCSID (LB_test, GenericFactory, "$Id$")

int
LB_test::create_object (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->object_group_ =
        this->load_balancer->create_object (type_id,
                                            the_criteria,
                                            this->factory_creation_id_.out (),
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
LB_test::test_no_factory (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // @@ TODO: Implement
    }
  ACE_CATCH (TAO_LoadBalancer::NoFactory, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Successfully caught %s exception\n"),
                  ex._id ()));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("  Replica TypeId:            %s\n"),
                  ex.type_id));

      TAO_LoadBalancer::Location &location =
        ex.the_location;

      for (int i = 0; i < location.length (); ++i)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("          Location[%d].id:   %s\n")
                    ACE_TEXT ("                       kind: %s\n"),
                    location[i].id,
                    location[i].kind));
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
LB_test::test_object_not_created (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // @@ TODO: Implement
    }
  ACE_CATCH (TAO_LoadBalancer::ObjectNotCreated, ex)
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
LB_test::test_invalid_criteria (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // @@ TODO: Implement
    }
  ACE_CATCH (TAO_LoadBalancer::InvalidCriteria, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Successfully caught %s exception\n"),
                  ex._id ()));

      TAO_LoadBalancer::Criteria &invalid_criteria =
        ex.invalid_criteria;

      for (int i = 0; i < properties.length (); ++i)
        {
          // Property values are not displayed since they are stored
          // as Anys.

          TAO_LoadBalancer::Property &invalid_property =
            invalid_criteria[i];

          for (int j = 0; j < invalid_property.length (); ++j)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("  invalid_criteria[%d].nam[%d].id:   %s\n")
                        ACE_TEXT ("                               kind: %s\n"),
                        i,
                        j,
                        invalid_property[j].id,
                        invalid_property[j].kind));
        }
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
LB_test::test_cannot_meet_criteria (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // @@ TODO: Implement
    }
  ACE_CATCH (TAO_LoadBalancer::CannotMeetCriteria, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Successfully caught %s exception\n"),
                  ex._id ()));


      TAO_LoadBalancer::Criteria &invalid_criteria =
        ex.invalid_criteria;

      for (int i = 0; i < properties.length (); ++i)
        {
          // Property values are not displayed since they are stored
          // as Anys.

          TAO_LoadBalancer::Property &unmet_property =
            unmet_criteria[i];

          for (int j = 0; j < unmet_property.length (); ++j)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("  unmet_criteria[%d].nam[%d].id:   %s\n")
                        ACE_TEXT ("                             kind: %s\n"),
                        i,
                        j,
                        unmet_property[j].id,
                        unmet_property[j].kind));
        }
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

int LB_test::delete_object (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->load_balancer->delete_object (this->factory_creation_id_.in ()
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
LB_test::test_object_not_found (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // @@ TODO: Implement
    }
  ACE_CATCH (TAO_LoadBalancer::ObjectNotFound, ex)
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
