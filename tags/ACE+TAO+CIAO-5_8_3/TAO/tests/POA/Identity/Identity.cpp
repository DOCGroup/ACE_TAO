// $Id$

// ==================================================
// = LIBRARY
//    TAO/test/POA/Identity
//
// = FILENAME
//    Identity.cpp
//
// = DESCRIPTION
//    This program demonstrates and tests how to move between id,
//    reference, and servants.
//
// = AUTHOR
//    Irfan Pyarali
// ==================================================

#include "testS.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"

class test_i : public POA_test
{
public:

  test_i (PortableServer::POA_ptr poa)
    : poa_ (PortableServer::POA::_duplicate (poa))
    {
    }

  PortableServer::POA_ptr _default_POA (void)
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
             PortableServer::POA_out forth_poa)
{
  // Policies for the new POAs
  CORBA::PolicyList policies (3);
  policies.length (3);

  policies[0] = root_poa->create_lifespan_policy (lifespan_policy);

  policies[1] = root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

  policies[2] = root_poa->create_id_assignment_policy (PortableServer::SYSTEM_ID);

  // Creation of the firstPOA
  ACE_CString name = "firstPOA";
  first_poa = root_poa->create_POA (name.c_str (),
                                    PortableServer::POAManager::_nil (),
                                    policies);

  policies[1]->destroy ();

  policies[1] = root_poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID);

  // Creation of the secondPOA
  name = "secondPOA";
  second_poa = root_poa->create_POA (name.c_str (),
                                     PortableServer::POAManager::_nil (),
                                     policies);

  policies[2]->destroy ();

  policies[2] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);

  // Creation of the thirdPOA
  name = "thirdPOA";
  third_poa = root_poa->create_POA (name.c_str (),
                                    PortableServer::POAManager::_nil (),
                                    policies);

  policies[1]->destroy ();

  policies[1] = root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

  // Creation of the forthPOA
  name = "forthPOA";
  forth_poa = root_poa->create_POA (name.c_str (),
                                    PortableServer::POAManager::_nil (),
                                    policies);

  // Creation of the new POAs over, so destroy the policies
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy ();
    }
}

void
test_poas (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr root_poa,
           PortableServer::POA_ptr first_poa,
           PortableServer::POA_ptr second_poa,
           PortableServer::POA_ptr third_poa,
           PortableServer::POA_ptr forth_poa,
           int perform_deactivation_test)
{
  {
    test_i servant (root_poa);

    CORBA::Object_var obj = root_poa->create_reference ("IDL:test:1.0");

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id = root_poa->reference_to_id (obj.in ());

    root_poa->activate_object_with_id (id.in (),
                                       &servant);

    obj = root_poa->id_to_reference (id.in ());

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ServantBase_var servant_from_reference =
      root_poa->reference_to_servant (obj.in ());

    PortableServer::ServantBase_var servant_from_id =
      root_poa->id_to_servant (id.in ());

    if (servant_from_reference.in () != servant_from_id.in ()
        || servant_from_reference.in () != &servant)
      {
        ACE_ERROR ((LM_ERROR,
                    "Mismatched servant_from_reference, "
                    "servant_from_id and &servant\n"));
      }

    obj = root_poa->servant_to_reference (&servant);

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    obj = servant._this ();

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id_from_servant = root_poa->servant_to_id (&servant);

    ACE_ASSERT (id_from_servant.in () == id.in ());

    root_poa->deactivate_object (id.in ());

    if (perform_deactivation_test)
      {
        root_poa->activate_object_with_id (id.in (),
                                           &servant);

        servant_from_reference = root_poa->reference_to_servant (obj.in ());

        ACE_ASSERT (servant_from_reference.in () == &servant);

        root_poa->deactivate_object (id.in ());
      }
  }

  {
    test_i servant (root_poa);

    PortableServer::ObjectId_var id = root_poa->activate_object (&servant);

    CORBA::Object_var obj = root_poa->id_to_reference (id.in ());

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    obj = root_poa->create_reference_with_id (id.in (),
                                              "IDL:test:1.0");

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    root_poa->deactivate_object (id.in ());
  }

  {
    test_i servant (first_poa);

    CORBA::Object_var obj = first_poa->create_reference ("IDL:test:1.0");

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id = first_poa->reference_to_id (obj.in ());

    first_poa->activate_object_with_id (id.in (),
                                        &servant);

    obj = first_poa->id_to_reference (id.in ());

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ServantBase_var servant_from_reference =
      first_poa->reference_to_servant (obj.in ());

    PortableServer::ServantBase_var servant_from_id =
      first_poa->id_to_servant (id.in ());

    if (servant_from_reference .in () != servant_from_id.in ()
        || servant_from_reference.in () != &servant)
      {
        ACE_ERROR ((LM_ERROR,
                    "Mismatched servant_from_reference, "
                    "servant_from_id and &servant\n"));
      }

    first_poa->deactivate_object (id.in ());

    if (perform_deactivation_test)
      {
        first_poa->activate_object_with_id (id.in (),
                                            &servant);

        servant_from_reference = first_poa->reference_to_servant (obj.in ());

        ACE_ASSERT (servant_from_reference.in () == &servant);

        first_poa->deactivate_object (id.in ());
      }
  }

  {
    test_i servant (first_poa);

    PortableServer::ObjectId_var id = first_poa->activate_object (&servant);

    CORBA::Object_var obj = first_poa->id_to_reference (id.in ());

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    obj = first_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    first_poa->deactivate_object (id.in ());
  }

  {
    test_i servant (second_poa);

    CORBA::Object_var obj = second_poa->create_reference ("IDL:test:1.0");

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id = second_poa->reference_to_id (obj.in ());

    second_poa->activate_object_with_id (id.in (),
                                         &servant);

    obj = second_poa->id_to_reference (id.in ());

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ServantBase_var servant_from_reference =
      second_poa->reference_to_servant (obj.in ());

    PortableServer::ServantBase_var servant_from_id =
      second_poa->id_to_servant (id.in ());

    if (servant_from_reference.in () != servant_from_id.in ()
        || servant_from_reference.in () != &servant)
      {
        ACE_ERROR ((LM_ERROR,
                    "Mismatched servant_from_reference, "
                    "servant_from_id and &servant\n"));
      }

    obj = second_poa->servant_to_reference (&servant);

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    obj = servant._this ();

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id_from_servant = second_poa->servant_to_id (&servant);

    ACE_ASSERT (id_from_servant.in () == id.in ());

    second_poa->deactivate_object (id.in ());

    if (perform_deactivation_test)
      {
        second_poa->activate_object_with_id (id.in (),
                                             &servant);

        servant_from_reference = second_poa->reference_to_servant (obj.in ());

        ACE_ASSERT (servant_from_reference.in () == &servant);

        second_poa->deactivate_object (id.in ());
      }
  }

  {
    test_i servant (second_poa);

    PortableServer::ObjectId_var id = second_poa->activate_object (&servant);

    CORBA::Object_var obj = second_poa->id_to_reference (id.in ());

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    obj = second_poa->create_reference_with_id (id.in (),
                                                "IDL:test:1.0");

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    second_poa->deactivate_object (id.in ());
  }

  {
    test_i servant (third_poa);
    PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId ("hello");

    CORBA::Object_var obj = third_poa->create_reference_with_id (id.in (),
                                                                 "IDL:test:1.0");

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id_from_reference = third_poa->reference_to_id (obj.in ());

    string = PortableServer::ObjectId_to_string (id_from_reference.in ());
    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    ACE_ASSERT (id_from_reference.in () == id.in ());

    third_poa->activate_object_with_id (id.in (),
                                        &servant);

    obj = third_poa->id_to_reference (id.in ());

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ServantBase_var servant_from_reference =
      third_poa->reference_to_servant (obj.in ());

    PortableServer::ServantBase_var servant_from_id =
      third_poa->id_to_servant (id.in ());

    if (servant_from_reference.in () != servant_from_id.in ()
        || servant_from_reference.in () != &servant)
      {
        ACE_ERROR ((LM_ERROR,
                    "Mismatched servant_from_reference, "
                    "servant_from_id and &servant\n"));
      }

    obj = third_poa->servant_to_reference (&servant);

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    obj = servant._this ();

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id_from_servant = third_poa->servant_to_id (&servant);

    string = PortableServer::ObjectId_to_string (id_from_servant.in ());
    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    ACE_ASSERT (id_from_servant.in () == id.in ());

    third_poa->deactivate_object (id.in ());

    if (perform_deactivation_test)
      {
        third_poa->activate_object_with_id (id.in (),
                                            &servant);

        servant_from_reference = third_poa->reference_to_servant (obj.in ());

        ACE_ASSERT (servant_from_reference.in () == &servant);

        third_poa->deactivate_object (id.in ());
      }
  }

  {
    test_i servant (forth_poa);
    PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId ("hello");

    CORBA::Object_var obj = forth_poa->create_reference_with_id (id.in (),
                                                                 "IDL:test:1.0");

    CORBA::String_var string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ObjectId_var id_from_reference = forth_poa->reference_to_id (obj.in ());

    string = PortableServer::ObjectId_to_string (id_from_reference.in ());
    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    ACE_ASSERT (id_from_reference.in () == id.in ());

    forth_poa->activate_object_with_id (id.in (),
                                        &servant);

    obj = forth_poa->id_to_reference (id.in ());

    string = orb->object_to_string (obj.in ());

    ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

    PortableServer::ServantBase_var servant_from_reference =
      forth_poa->reference_to_servant (obj.in ());

    PortableServer::ServantBase_var servant_from_id =
      forth_poa->id_to_servant (id.in ());

    if (servant_from_reference.in () != servant_from_id.in ()
        || servant_from_reference.in () != &servant)
      {
        ACE_ERROR ((LM_ERROR,
                    "Mismatched servant_from_reference, "
                    "servant_from_id and &servant\n"));
      }

    forth_poa->deactivate_object (id.in ());

    if (perform_deactivation_test)
      {
        forth_poa->activate_object_with_id (id.in (),
                                            &servant);

        servant_from_reference = forth_poa->reference_to_servant (obj.in ());

        ACE_ASSERT (servant_from_reference.in () == &servant);

        forth_poa->deactivate_object (id.in ());
      }
  }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

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
                   forth_poa.out ());

      test_poas (orb.in (),
                 root_poa.in (),
                 first_poa.in (),
                 second_poa.in (),
                 third_poa.in (),
                 forth_poa.in (),
                 perform_deactivation_test);

      first_poa->destroy (1,
                          1);

      second_poa->destroy (1,
                           1);

      third_poa->destroy (1,
                          1);

      forth_poa->destroy (1,
                          1);

      create_poas (root_poa.in (),
                   PortableServer::PERSISTENT,
                   first_poa.out (),
                   second_poa.out (),
                   third_poa.out (),
                   forth_poa.out ());

      test_poas (orb.in (),
                 root_poa.in (),
                 first_poa.in (),
                 second_poa.in (),
                 third_poa.in (),
                 forth_poa.in (),
                 perform_deactivation_test);

      root_poa->destroy (1,
                         1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception!");
      return -1;
    }

  return 0;
}
