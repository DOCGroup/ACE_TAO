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

class test_i : public POA_test
{
};

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow to POA.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Policies for the new POA.
      CORBA::PolicyList policies (3);
      policies.length (3);

      // Request Processing Policy.
      policies[0] =
        root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Id Uniqueness Policy.
      policies[1] =
        root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Servant Retention Policy.
      policies[2] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create POA to host default servant.
      ACE_CString name = "Default Servant";
      PortableServer::POA_var default_servant_poa =
        root_poa->create_POA (name.c_str (),
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy policies.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Activate POA manager.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test servant.
      test_i test;

      // Register default servant.
      default_servant_poa->set_servant (&test
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create dummy id.
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("id");

      // Create dummy object.
      object =
        default_servant_poa->create_reference ("IDL:test:1.0"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::Servant servant = 0;

      // Invoke id_to_servant(). Should retrieve default servant.
      servant =
        default_servant_poa->id_to_servant (id.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Assert correctness.
      ACE_ASSERT (&test == servant);

      // Invoke reference_to_servant(). Should retrieve default servant.
      servant =
        default_servant_poa->reference_to_servant (object.in ()
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Assert correctness.
      ACE_ASSERT (&test == servant);

      // Report success.
      ACE_DEBUG ((LM_DEBUG,
                  "Default_Servant test successful\n"));

      // Destroy the ORB.
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
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
