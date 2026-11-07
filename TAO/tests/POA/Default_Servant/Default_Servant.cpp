//=============================================================================
/**
 *  @file     Default_Servant.cpp
 *
 *   This program tests the behavior of POA::id_to_servant() and
 *   POA::reference_to_servant() with the use of default servants.
 *
 *  @author  Irfan Pyarali
 */
#include "testS.h"
#include "tao/PortableServer/ServantManagerC.h"

class test_i : public POA_test
{
public:
  test_i () = default;
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;
};

void
test_get_servant_manager (PortableServer::POA_ptr poa)
{
  bool succeed {};
  try
    {
      // Getting the servant manager should give a wrong policy exception
      // exception
      PortableServer::ServantManager_ptr const servant_manager =
        poa->get_servant_manager ();

      ACE_UNUSED_ARG (servant_manager);
    }
  catch (PortableServer::POA::WrongPolicy const &)
    {
      succeed = true;
    }
  catch (CORBA::Exception const &)
    {
    }

  if (!succeed)
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, get servant manager failed, should give an exception\n"));
}

void
test_set_servant_manager (PortableServer::POA_ptr poa)
{
  bool succeed {};
  try
    {
      // Setting the servant manager should give a wrong policy exception
      // exception
      poa->set_servant_manager (PortableServer::ServantManager::_nil ());
    }
  catch (PortableServer::POA::WrongPolicy const &)
    {
      succeed = true;
    }
  catch (CORBA::Exception const &)
    {
    }

  if (!succeed)
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, set servant manager failed, should give an exception\n"));
}

void
test_get_servant_with_no_set (PortableServer::POA_ptr poa)
{
  bool succeed {};
  try
    {
      // Getting the default servant without one set whould give a NoServant
      // exception
      PortableServer::Servant servant =
        poa->get_servant ();

      ACE_UNUSED_ARG (servant);
    }
  catch (PortableServer::POA::NoServant const &)
    {
      succeed = true;
    }
  catch (CORBA::Exception const &)
    {
    }

  if (!succeed)
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, get servant without one set failed\n"));
}

void
test_reference_to_servant_active_object (PortableServer::POA_ptr root_poa)
{
  test_i test;
  CORBA::ULong expected_refcount {1};

  PortableServer::ObjectId_var const id =
    root_poa->activate_object (&test);
  expected_refcount++;

  CORBA::Object_var const object =
    root_poa->id_to_reference (id.in ());

  PortableServer::ServantBase_var const servant =
    root_poa->reference_to_servant (object.in ());
  ++expected_refcount;

  root_poa->deactivate_object (id.in ());
  --expected_refcount;

  CORBA::ULong const refcount =
    test._refcount_value ();

  ACE_UNUSED_ARG (refcount);
  ACE_UNUSED_ARG (expected_refcount);
  ACE_ASSERT (expected_refcount == refcount);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      // Narrow to POA.
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Policies for the new POA.
      CORBA::PolicyList policies (3);
      policies.length (3);

      // Request Processing Policy.
      policies[0] =
        root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT);

      // Id Uniqueness Policy.
      policies[1] =
        root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

      // Servant Retention Policy.
      policies[2] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

      // Create POA to host default servant.
      PortableServer::POA_var const default_servant_poa =
        root_poa->create_POA ("Default Servant",
                              poa_manager.in (),
                              policies);

      // Destroy policies.
      for (CORBA::ULong i {};
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr const policy = policies[i];
          policy->destroy ();
        }

      // Activate POA manager.
      poa_manager->activate ();

      test_reference_to_servant_active_object (root_poa.in ());

      // Test servant.
      test_i test;
      CORBA::ULong expected_refcount {1};

      test_get_servant_with_no_set (default_servant_poa.in ());

      test_get_servant_manager (default_servant_poa.in ());

      test_set_servant_manager (default_servant_poa.in ());

      // Register default servant.
      default_servant_poa->set_servant (&test);
      expected_refcount++;

      // Create dummy id.
      PortableServer::ObjectId_var const id =
        PortableServer::string_to_ObjectId ("id");

      // Create dummy object.
      object =
        default_servant_poa->create_reference ("IDL:test:1.0");

      // Invoke id_to_servant(). Should retrieve default servant.
      PortableServer::ServantBase_var servant =
        default_servant_poa->id_to_servant (id.in ());
      expected_refcount++;

      // Assert correctness.
      ACE_ASSERT (&test == servant.in ());

      // Invoke reference_to_servant(). Should retrieve default servant.
      servant =
        default_servant_poa->reference_to_servant (object.in ());
      expected_refcount++;

      // Assert correctness.
      ACE_ASSERT (&test == servant.in ());

      // Report success.
      ACE_DEBUG ((LM_DEBUG,
                  "Default_Servant test successful\n"));

      CORBA::ULong const refcount =
        test._refcount_value ();

      ACE_UNUSED_ARG (refcount);
      ACE_UNUSED_ARG (expected_refcount);
      ACE_ASSERT (expected_refcount == refcount);

      // Destroy the ORB.
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
