// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Excessive_Object_Deactivations
//
// = FILENAME
//     Excessive_Object_Deactivations.cpp
//
// = DESCRIPTION
//     This program tests for excessive deactivations of a servant.
//     The test checks excessive deactivations in a POA with SYSTEM_ID
//     and other POA with USER_ID.  The test also check for excessive
//     deactivations during upcalls.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"

class test_i : public POA_test
{
public:
  void deactivate_self (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL);

  PortableServer::POA_var poa_;

  PortableServer::ObjectId id_;
};

PortableServer::POA_ptr
test_i::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::deactivate_self (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->poa_->deactivate_object (this->id_
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Exception flag
  int expected_exception_raised = 0;

  ACE_TRY
    {
      this->poa_->deactivate_object (this->id_
                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ex)
    {
      // This is the correct exception! Ignore
      expected_exception_raised = 1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught of incorrect type");
      ACE_ASSERT (0);
    }
  ACE_ENDTRY;
  ACE_CHECK;

  // Make sure an exception was raised and it was of the correct type.
  ACE_ASSERT (expected_exception_raised);

  ACE_UNUSED_ARG (expected_exception_raised);
}

void
test_object_deactivation (PortableServer::POA_ptr poa,
                          const PortableServer::ObjectId &id
                          TAO_ENV_ARG_DECL)
{
  test_i servant;
  int expected_exception_raised = 0;

  PortableServer::ObjectId_var invalid_id =
    PortableServer::string_to_ObjectId ("invalid id");

  ACE_TRY_EX (invalid_id)
    {
      poa->deactivate_object (invalid_id.in ()
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (invalid_id);
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ex)
    {
      // This is the correct exception! Ignore
      expected_exception_raised = 1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught of incorrect type");
      ACE_ASSERT (0);
    }
  ACE_ENDTRY;
  ACE_CHECK;

  // Make sure an exception was raised and it was of the correct
  // type.
  ACE_ASSERT (expected_exception_raised);



  poa->activate_object_with_id (id,
                                &servant
                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Reset flag
  expected_exception_raised = 0;

  ACE_TRY_EX (double_deactivate)
    {
      poa->deactivate_object (id
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (double_deactivate);
    }
  ACE_CATCH (PortableServer::POA::ObjectNotActive, ex)
    {
      // This is the correct exception! Ignore
      expected_exception_raised = 1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught of incorrect type");
      ACE_ASSERT (0);
    }
  ACE_ENDTRY;
  ACE_CHECK;

  // Make sure an exception was raised and it was of the correct
  // type.
  ACE_ASSERT (expected_exception_raised);

  poa->activate_object_with_id (id,
                                &servant
                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  servant.poa_ =
    PortableServer::POA::_duplicate (poa);

  servant.id_ = id;

  test_var test =
    servant._this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  test->deactivate_self (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ACE_ASSERT dissappears in non-debug builds
  ACE_UNUSED_ARG (expected_exception_raised);

}

int
main (int argc, char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList empty_policies;
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              empty_policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj =
        root_poa->create_reference ("IDL:test:1.0"
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        root_poa->reference_to_id (obj.in ()
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_object_deactivation (root_poa.in (),
                                id.in ()
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      id = PortableServer::string_to_ObjectId ("good id");

      test_object_deactivation (child_poa.in (),
                                id.in ()
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
