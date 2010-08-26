// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Default_Servant
//
// = FILENAME
//     Default_Servant.cpp
//
// = DESCRIPTION
//     This program tests the behavior of POA::id_to_servant() and
//     POA::reference_to_servant() with the use of default servants.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"
#include "ace/SString.h"
#include "tao/PortableServer/ServantManagerC.h"

class test_i : public POA_test
{
};

void
test_get_servant_manager (PortableServer::POA_ptr poa)
{
  bool succeed = false;
  try
    {
      // Getting the servant manager should give a wrong policy exception
      // exception
      PortableServer::ServantManager_ptr servant_manager =
        poa->get_servant_manager ();

      ACE_UNUSED_ARG (servant_manager);
    }
  catch (const PortableServer::POA::WrongPolicy& )
    {
      succeed = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, get servant manager failed, should give an exception\n"));
  }
}

void
test_set_servant_manager (PortableServer::POA_ptr poa)
{
  bool succeed = false;
  try
    {
      // Setting the servant manager should give a wrong policy exception
      // exception
      poa->set_servant_manager (PortableServer::ServantManager::_nil());
    }
  catch (const PortableServer::POA::WrongPolicy& )
    {
      succeed = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, set servant manager failed, should give an exception\n"));
  }
}

void
test_get_servant_with_no_set (PortableServer::POA_ptr poa)
{
  bool succeed = false;
  try
    {
      // Getting the default servant without one set whould give a NoServant
      // exception
      PortableServer::Servant servant =
        poa->get_servant ();

      ACE_UNUSED_ARG (servant);
    }
  catch (const PortableServer::POA::NoServant& )
    {
      succeed = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, get servant without one set failed\n"));
  }
}

void
test_reference_to_servant_active_object(PortableServer::POA_ptr root_poa)
{
  test_i test;
  CORBA::ULong expected_refcount = 1;

  PortableServer::ObjectId_var id =
    root_poa->activate_object (&test);
  expected_refcount++;

  CORBA::Object_var object =
    root_poa->id_to_reference (id.in ());

  PortableServer::ServantBase_var servant =
    root_poa->reference_to_servant (object.in ());
  ++expected_refcount;

  root_poa->deactivate_object (id.in ());
  --expected_refcount;

  CORBA::ULong refcount =
    test._refcount_value ();

  ACE_UNUSED_ARG (refcount);
  ACE_UNUSED_ARG (expected_refcount);
  ACE_ASSERT (expected_refcount == refcount);
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      // Narrow to POA.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
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
      ACE_CString name = "Default Servant";
      PortableServer::POA_var default_servant_poa =
        root_poa->create_POA (name.c_str (),
                              poa_manager.in (),
                              policies);

      // Destroy policies.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      // Activate POA manager.
      poa_manager->activate ();

      test_reference_to_servant_active_object(root_poa.in ());

      // Test servant.
      test_i test;
      CORBA::ULong expected_refcount = 1;

      (void) test_get_servant_with_no_set (default_servant_poa.in());

      (void) test_get_servant_manager (default_servant_poa.in());

      (void) test_set_servant_manager (default_servant_poa.in());

      // Register default servant.
      default_servant_poa->set_servant (&test);
      expected_refcount++;

      // Create dummy id.
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("id");

      // Create dummy object.
      object =
        default_servant_poa->create_reference ("IDL:test:1.0");

      // Invoke id_to_servant(). Should retrieve default servant.
      PortableServer::ServantBase_var servant =
        default_servant_poa->id_to_servant (id.in ());
      expected_refcount++;

      // Assert correctness.
      ACE_ASSERT (&test == servant.in());

      // Invoke reference_to_servant(). Should retrieve default servant.
      servant =
        default_servant_poa->reference_to_servant (object.in ());
      expected_refcount++;

      // Assert correctness.
      ACE_ASSERT (&test == servant.in());

      // Report success.
      ACE_DEBUG ((LM_DEBUG,
                  "Default_Servant test successful\n"));

      CORBA::ULong refcount =
        test._refcount_value ();

      ACE_UNUSED_ARG (refcount);
      ACE_UNUSED_ARG (expected_refcount);
      ACE_ASSERT (expected_refcount == refcount);

      // Destroy the ORB.
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
