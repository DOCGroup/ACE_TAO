// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/wait_for_completion
//
// = FILENAME
//     wait_for_completion.cpp
//
// = DESCRIPTION
//     This program tests the wait_for_completion feature of a POA.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"

class test_i : public POA_test
{
public:
  void destroy_poa (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void test_poa (PortableServer::POA_ptr poa);

  PortableServer::POA_var poa_;
};

void
test_i::test_poa (PortableServer::POA_ptr poa)
{
  this->poa_ = PortableServer::POA::_duplicate (poa);
}

void
test_i::destroy_poa (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Boolean etherealize_objects = 1;
  CORBA::Boolean wait_for_completion = 1;
  this->poa_->destroy (etherealize_objects,
                       wait_for_completion
                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::POA_ptr
init_orb (int argc,
          char **argv,
          const char *orb_name
          ACE_ENV_ARG_DECL)
{
  // Initialize the ORB first.
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        orb_name
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Obtain the RootPOA.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  return root_poa._retn ();
}

int
main (int argc,
      char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      PortableServer::POA_var first_poa =
        init_orb (argc,
                  argv,
                  "first ORB"
                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var second_poa =
        init_orb (argc,
                  argv,
                  "second ORB"
                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i servant;
      test_var test_object = servant._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int expected_exception_raised = 0;

      ACE_TRY_EX (first_poa)
        {
          servant.test_poa (first_poa.in ());

          test_object->destroy_poa (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (first_poa);
        }
      ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
        {
          // This is the correct exception! Ignore
          expected_exception_raised = 1;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught of incorrect type");
          return -1;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);

      // Make sure an exception was raised and it was of the correct
      // type.
      ACE_ASSERT (expected_exception_raised);

      // In non-debug compiles, asserts will disappear.
      ACE_UNUSED_ARG (expected_exception_raised);

      servant.test_poa (second_poa.in ());

      test_object->destroy_poa (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      first_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Unexpected exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
