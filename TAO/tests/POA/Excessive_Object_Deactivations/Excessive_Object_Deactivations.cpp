//=============================================================================
/**
 *  @file     Excessive_Object_Deactivations.cpp
 *
 *   This program tests for excessive deactivations of a servant.
 *   The test checks excessive deactivations in a POA with SYSTEM_ID
 *   and other POA with USER_ID.  The test also check for excessive
 *   deactivations during upcalls.
 *
 *  @author  Irfan Pyarali
 */
#include "testS.h"

class test_i : public POA_test
{
public:
  test_i () = default;
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  void deactivate_self () override;

  PortableServer::POA_ptr _default_POA () override;

  PortableServer::POA_var poa_ {};

  PortableServer::ObjectId id_ {};
};

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::deactivate_self ()
{
  this->poa_->deactivate_object (this->id_);

  // Exception flag
  bool expected_exception_raised {};

  try
    {
      this->poa_->deactivate_object (this->id_);
    }
  catch (PortableServer::POA::ObjectNotActive const &)
    {
      // This is the correct exception! Ignore
      expected_exception_raised = true;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Exception caught of incorrect type");
      ACE_ASSERT (0);
    }

  // Make sure an exception was raised and it was of the correct type.
  ACE_ASSERT (expected_exception_raised);

  ACE_UNUSED_ARG (expected_exception_raised);
}

void
test_object_deactivation (PortableServer::POA_ptr poa,
                          PortableServer::ObjectId const &id)
{
  test_i servant;
  bool expected_exception_raised {};

  PortableServer::ObjectId_var const invalid_id =
    PortableServer::string_to_ObjectId ("invalid id");

  try
    {
      poa->deactivate_object (invalid_id.in ());
    }
  catch (PortableServer::POA::ObjectNotActive const &)
    {
      // This is the correct exception! Ignore
      expected_exception_raised = true;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Exception caught of incorrect type");
      ACE_ASSERT (0);
    }

  // Make sure an exception was raised and it was of the correct
  // type.
  ACE_ASSERT (expected_exception_raised);
  poa->activate_object_with_id (id,
                                &servant);

  poa->deactivate_object (id);

  // Reset flag
  expected_exception_raised = false;

  try
    {
      poa->deactivate_object (id);
    }
  catch (PortableServer::POA::ObjectNotActive const &)
    {
      // This is the correct exception! Ignore
      expected_exception_raised = true;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Exception caught of incorrect type");
      ACE_ASSERT (0);
    }

  // Make sure an exception was raised and it was of the correct
  // type.
  ACE_ASSERT (expected_exception_raised);

  poa->activate_object_with_id (id,
                                &servant);

  servant.poa_ =
    PortableServer::POA::_duplicate (poa);

  servant.id_ = id;

  CORBA::Object_var const object = poa->id_to_reference (id);

  test_var const test =
    test::_narrow (object.in ());

  test->deactivate_self ();

  // ACE_ASSERT dissappears in non-debug builds
  ACE_UNUSED_ARG (expected_exception_raised);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList const empty_policies;
      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              empty_policies);

      poa_manager->activate ();

      obj =
        root_poa->create_reference ("IDL:test:1.0");

      PortableServer::ObjectId_var id =
        root_poa->reference_to_id (obj.in ());

      test_object_deactivation (root_poa.in (),
                                id.in ());

      id = PortableServer::string_to_ObjectId ("good id");

      test_object_deactivation (child_poa.in (),
                                id.in ());

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
