// $Id$

// ==================================================
// = LIBRARY
//    TAO/examples/POA/Identity
//
// = FILENAME
//    Identity.cpp
//
// = DESCRIPTION
//    This program demonstrates how to move between id, reference, and
//    servants.
//
// = AUTHOR
//    Irfan Pyarali
// ==================================================

#include "testS.h"

ACE_RCSID(Identity, Identity, "$Id$")

class test_i : public POA_test
{
public:

  test_i (PortableServer::POA_ptr poa)
    : poa_ (PortableServer::POA::_duplicate (poa))
    {
    }

  PortableServer::POA_ptr _default_POA (CORBA::Environment &)
    {
      return PortableServer::POA::_duplicate (this->poa_.in ());
    }

protected:

  PortableServer::POA_var poa_;
};

void
create_poas (PortableServer::POA_ptr root_poa,
             PortableServer::LifespanPolicyValue lifespan_policy,
             PortableServer::POA_out first_poa,
             PortableServer::POA_out second_poa,
             PortableServer::POA_out third_poa,
             PortableServer::POA_out forth_poa,
             CORBA::Environment &ACE_TRY_ENV)
{
  // Policies for the new POAs
  CORBA::PolicyList policies (3);
  policies.length (3);

  policies[0] = root_poa->create_lifespan_policy (lifespan_policy,
                                                  ACE_TRY_ENV);
  ACE_CHECK;

  policies[1] = root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID,
                                                       ACE_TRY_ENV);
  ACE_CHECK;

  policies[2] = root_poa->create_id_assignment_policy (PortableServer::SYSTEM_ID,
                                                       ACE_TRY_ENV);
  ACE_CHECK;

  // Creation of the firstPOA
  ACE_CString name = "firstPOA";
  first_poa = root_poa->create_POA (name.c_str (),
                                    PortableServer::POAManager::_nil (),
                                    policies,
                                    ACE_TRY_ENV);
  ACE_CHECK;

  policies[1]->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  policies[1] = root_poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID,
                                                       ACE_TRY_ENV);
  ACE_CHECK;

  // Creation of the secondPOA
  name = "secondPOA";
  second_poa = root_poa->create_POA (name.c_str (),
                                     PortableServer::POAManager::_nil (),
                                     policies,
                                     ACE_TRY_ENV);

  policies[2]->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  policies[2] = root_poa->create_id_assignment_policy (PortableServer::USER_ID,
                                                       ACE_TRY_ENV);

  // Creation of the thirdPOA
  name = "thirdPOA";
  third_poa = root_poa->create_POA (name.c_str (),
                                    PortableServer::POAManager::_nil (),
                                    policies,
                                    ACE_TRY_ENV);
  ACE_CHECK;

  policies[1]->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  policies[1] = root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID,
                                                       ACE_TRY_ENV);

  // Creation of the forthPOA
  name = "forthPOA";
  forth_poa = root_poa->create_POA (name.c_str (),
                                    PortableServer::POAManager::_nil (),
                                    policies,
                                    ACE_TRY_ENV);

  // Creation of the new POAs over, so destroy the policies
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy (ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
test_poas (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr first_poa,
           PortableServer::POA_ptr second_poa,
           PortableServer::POA_ptr third_poa,
           PortableServer::POA_ptr forth_poa,
           int perform_deactivation_test,
           CORBA::Environment &ACE_TRY_ENV)
{
  {
    test_i servant (first_poa);

    CORBA::Object_var obj = first_poa->create_reference ("IDL:test:1.0",
                                                         ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::String_var string = orb->object_to_string (obj.in (),
                                                      ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::ObjectId_var id = first_poa->reference_to_id (obj.in (),
                                                                  ACE_TRY_ENV);
    ACE_CHECK;

    first_poa->activate_object_with_id (id.in (),
                                        &servant,
                                        ACE_TRY_ENV);
    ACE_CHECK;

    obj = first_poa->id_to_reference (id.in (),
                                      ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::Servant servant_from_reference = first_poa->reference_to_servant (obj.in (),
                                                                                      ACE_TRY_ENV);
    ACE_CHECK;

    PortableServer::Servant servant_from_id = first_poa->id_to_servant (id.in (),
                                                                        ACE_TRY_ENV);
    ACE_CHECK;

    ACE_ASSERT (servant_from_reference == servant_from_id);
    ACE_ASSERT (servant_from_reference == &servant);

    first_poa->deactivate_object (id.in (),
                                  ACE_TRY_ENV);
    ACE_CHECK;

    if (perform_deactivation_test)
      {
        first_poa->activate_object_with_id (id.in (),
                                            &servant,
                                            ACE_TRY_ENV);
        ACE_CHECK;

        servant_from_reference = first_poa->reference_to_servant (obj.in (),
                                                                  ACE_TRY_ENV);
        ACE_CHECK;

        ACE_ASSERT (servant_from_reference == &servant);

        first_poa->deactivate_object (id.in (),
                                      ACE_TRY_ENV);
        ACE_CHECK;
      }
  }

  {
    test_i servant (first_poa);

    PortableServer::ObjectId_var id = first_poa->activate_object (&servant,
                                                                  ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::Object_var obj = first_poa->id_to_reference (id.in (),
                                                        ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::String_var string = orb->object_to_string (obj.in (),
                                                      ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    obj = first_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0",
                                               ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    first_poa->deactivate_object (id.in (),
                                  ACE_TRY_ENV);
    ACE_CHECK;
  }

  {
    test_i servant (second_poa);

    CORBA::Object_var obj = second_poa->create_reference ("IDL:test:1.0",
                                                          ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::String_var string = orb->object_to_string (obj.in (),
                                                      ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::ObjectId_var id = second_poa->reference_to_id (obj.in (),
                                                                   ACE_TRY_ENV);
    ACE_CHECK;

    second_poa->activate_object_with_id (id.in (),
                                         &servant,
                                         ACE_TRY_ENV);
    ACE_CHECK;

    obj = second_poa->id_to_reference (id.in (),
                                       ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::Servant servant_from_reference = second_poa->reference_to_servant (obj.in (),
                                                                                       ACE_TRY_ENV);
    ACE_CHECK;

    PortableServer::Servant servant_from_id = second_poa->id_to_servant (id.in (),
                                                                         ACE_TRY_ENV);
    ACE_CHECK;

    ACE_ASSERT (servant_from_reference == servant_from_id);
    ACE_ASSERT (servant_from_reference == &servant);

    obj = second_poa->servant_to_reference (&servant,
                                            ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    obj = servant._this (ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::ObjectId_var id_from_servant = second_poa->servant_to_id (&servant,
                                                                              ACE_TRY_ENV);
    ACE_CHECK;

    ACE_ASSERT (id_from_servant.in () == id.in ());

    second_poa->deactivate_object (id.in (),
                                   ACE_TRY_ENV);
    ACE_CHECK;

    if (perform_deactivation_test)
      {
        second_poa->activate_object_with_id (id.in (),
                                             &servant,
                                             ACE_TRY_ENV);
        ACE_CHECK;

        servant_from_reference = second_poa->reference_to_servant (obj.in (),
                                                                   ACE_TRY_ENV);
        ACE_CHECK;

        ACE_ASSERT (servant_from_reference == &servant);

        second_poa->deactivate_object (id.in (),
                                       ACE_TRY_ENV);
        ACE_CHECK;
      }
  }

  {
    test_i servant (second_poa);

    PortableServer::ObjectId_var id = second_poa->activate_object (&servant,
                                                                   ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::Object_var obj = second_poa->id_to_reference (id.in (),
                                                         ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::String_var string = orb->object_to_string (obj.in (),
                                                      ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    obj = second_poa->create_reference_with_id (id.in (),
                                                "IDL:test:1.0",
                                                ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    second_poa->deactivate_object (id.in (),
                                   ACE_TRY_ENV);
    ACE_CHECK;
  }

  {
    test_i servant (third_poa);
    PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId ("hello");

    CORBA::Object_var obj = third_poa->create_reference_with_id (id.in (),
                                                                 "IDL:test:1.0",
                                                                 ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::String_var string = orb->object_to_string (obj.in (),
                                                      ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::ObjectId_var id_from_reference = third_poa->reference_to_id (obj.in (),
                                                                                 ACE_TRY_ENV);
    ACE_CHECK;

    string = PortableServer::ObjectId_to_string (id_from_reference.in ());
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    ACE_ASSERT (id_from_reference.in () == id.in ());

    third_poa->activate_object_with_id (id.in (),
                                        &servant,
                                        ACE_TRY_ENV);
    ACE_CHECK;

    obj = third_poa->id_to_reference (id.in (),
                                      ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::Servant servant_from_reference = third_poa->reference_to_servant (obj.in (),
                                                                                      ACE_TRY_ENV);
    ACE_CHECK;

    PortableServer::Servant servant_from_id = third_poa->id_to_servant (id.in (),
                                                                        ACE_TRY_ENV);
    ACE_CHECK;

    ACE_ASSERT (servant_from_reference == servant_from_id);
    ACE_ASSERT (servant_from_reference == &servant);

    obj = third_poa->servant_to_reference (&servant,
                                           ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    obj = servant._this (ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::ObjectId_var id_from_servant = third_poa->servant_to_id (&servant,
                                                                             ACE_TRY_ENV);
    ACE_CHECK;

    string = PortableServer::ObjectId_to_string (id_from_servant.in ());
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    ACE_ASSERT (id_from_servant.in () == id.in ());

    third_poa->deactivate_object (id.in (),
                                  ACE_TRY_ENV);
    ACE_CHECK;

    if (perform_deactivation_test)
      {
        third_poa->activate_object_with_id (id.in (),
                                            &servant,
                                            ACE_TRY_ENV);
        ACE_CHECK;

        servant_from_reference = third_poa->reference_to_servant (obj.in (),
                                                                  ACE_TRY_ENV);
        ACE_CHECK;

        ACE_ASSERT (servant_from_reference == &servant);

        third_poa->deactivate_object (id.in (),
                                      ACE_TRY_ENV);
        ACE_CHECK;
      }
  }

  {
    test_i servant (forth_poa);
    PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId ("hello");

    CORBA::Object_var obj = forth_poa->create_reference_with_id (id.in (),
                                                                 "IDL:test:1.0",
                                                                 ACE_TRY_ENV);
    ACE_CHECK;

    CORBA::String_var string = orb->object_to_string (obj.in (),
                                                      ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::ObjectId_var id_from_reference = forth_poa->reference_to_id (obj.in (),
                                                                                 ACE_TRY_ENV);
    ACE_CHECK;

    string = PortableServer::ObjectId_to_string (id_from_reference.in ());
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    ACE_ASSERT (id_from_reference.in () == id.in ());

    forth_poa->activate_object_with_id (id.in (),
                                        &servant,
                                        ACE_TRY_ENV);
    ACE_CHECK;

    obj = forth_poa->id_to_reference (id.in (),
                                      ACE_TRY_ENV);
    ACE_CHECK;

    string = orb->object_to_string (obj.in (),
                                    ACE_TRY_ENV);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

    PortableServer::Servant servant_from_reference = forth_poa->reference_to_servant (obj.in (),
                                                                                      ACE_TRY_ENV);
    ACE_CHECK;

    PortableServer::Servant servant_from_id = forth_poa->id_to_servant (id.in (),
                                                                        ACE_TRY_ENV);
    ACE_CHECK;

    ACE_ASSERT (servant_from_reference == servant_from_id);
    ACE_ASSERT (servant_from_reference == &servant);

    forth_poa->deactivate_object (id.in ());

    if (perform_deactivation_test)
      {
        forth_poa->activate_object_with_id (id.in (),
                                            &servant,
                                            ACE_TRY_ENV);
        ACE_CHECK;

        servant_from_reference = forth_poa->reference_to_servant (obj.in (),
                                                                  ACE_TRY_ENV);
        ACE_CHECK;

        ACE_ASSERT (servant_from_reference == &servant);

        forth_poa->deactivate_object (id.in (),
                                      ACE_TRY_ENV);
        ACE_CHECK;
      }
  }
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA",
                                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in (),
                                                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters
        = TAO_ORB_Core_instance ()->server_factory ()->active_object_map_creation_parameters ();

      int perform_deactivation_test = creation_parameters.allow_reactivation_of_system_ids_;

      PortableServer::POA_var first_poa;
      PortableServer::POA_var second_poa;
      PortableServer::POA_var third_poa;
      PortableServer::POA_var forth_poa;

      create_poas (root_poa.in (),
                   PortableServer::TRANSIENT,
                   first_poa.out (),
                   second_poa.out (),
                   third_poa.out (),
                   forth_poa.out (),
                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      test_poas (orb.in (),
                 first_poa.in (),
                 second_poa.in (),
                 third_poa.in (),
                 forth_poa.in (),
                 perform_deactivation_test,
                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      first_poa->destroy (1,
                          1,
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      second_poa->destroy (1,
                           1,
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      third_poa->destroy (1,
                          1,
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      forth_poa->destroy (1,
                          1,
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      create_poas (root_poa.in (),
                   PortableServer::PERSISTENT,
                   first_poa.out (),
                   second_poa.out (),
                   third_poa.out (),
                   forth_poa.out (),
                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      test_poas (orb.in (),
                 first_poa.in (),
                 second_poa.in (),
                 third_poa.in (),
                 forth_poa.in (),
                 perform_deactivation_test,
                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      root_poa->destroy (1,
                         1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception!");
    }
  ACE_ENDTRY;

  return 0;
}
