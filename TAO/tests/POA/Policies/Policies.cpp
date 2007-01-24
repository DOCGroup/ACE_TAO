// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Policies
//
// = FILENAME
//     Policies.cpp
//
// = DESCRIPTION
//     This program tests the construction of POA policies, both
//     through the generic ORB::create_policy interface and the
//     PortableServer specific interfaces.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "tao/ORB.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PI_Server/PI_Server.h"

#include "ace/Log_Msg.h"

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      // Narrow to POA.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);

      // Get the POAManager of the POA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

      {
        PortableServer::ThreadPolicy_var policy1 =
          root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL
                                          ACE_ENV_ARG_PARAMETER);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::ORB_CTRL_MODEL;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::THREAD_POLICY_ID,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::ThreadPolicy_var policy2 =
          PortableServer::ThreadPolicy::_narrow (policy.in ()
                                                 ACE_ENV_ARG_PARAMETER);

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

#endif /* TAO_HAS_MINIMUM_POA == 0 */

      {
        PortableServer::LifespanPolicy_var policy1 =
          root_poa->create_lifespan_policy (PortableServer::TRANSIENT
                                            ACE_ENV_ARG_PARAMETER);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::TRANSIENT;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::LIFESPAN_POLICY_ID,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::LifespanPolicy_var policy2 =
          PortableServer::LifespanPolicy::_narrow (policy.in ()
                                                   ACE_ENV_ARG_PARAMETER);

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::IdUniquenessPolicy_var policy1 =
          root_poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID
                                                 ACE_ENV_ARG_PARAMETER);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::UNIQUE_ID;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::ID_UNIQUENESS_POLICY_ID,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::IdUniquenessPolicy_var policy2 =
          PortableServer::IdUniquenessPolicy::_narrow (policy.in ()
                                                       ACE_ENV_ARG_PARAMETER);

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::IdAssignmentPolicy_var policy1 =
          root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                                 ACE_ENV_ARG_PARAMETER);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::USER_ID;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::ID_ASSIGNMENT_POLICY_ID,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::IdAssignmentPolicy_var policy2 =
          PortableServer::IdAssignmentPolicy::_narrow (policy.in ()
                                                       ACE_ENV_ARG_PARAMETER);

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

      {
        PortableServer::ImplicitActivationPolicy_var policy1 =
          root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                       ACE_ENV_ARG_PARAMETER);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::IMPLICIT_ACTIVATION;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::IMPLICIT_ACTIVATION_POLICY_ID,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::ImplicitActivationPolicy_var policy2 =
          PortableServer::ImplicitActivationPolicy::_narrow (policy.in ()
                                                             ACE_ENV_ARG_PARAMETER);

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::ServantRetentionPolicy_var policy1 =
          root_poa->create_servant_retention_policy (PortableServer::RETAIN
                                                     ACE_ENV_ARG_PARAMETER);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::RETAIN;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::SERVANT_RETENTION_POLICY_ID,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::ServantRetentionPolicy_var policy2 =
          PortableServer::ServantRetentionPolicy::_narrow (policy.in ()
                                                           ACE_ENV_ARG_PARAMETER);

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::RequestProcessingPolicy_var policy1 =
          root_poa->create_request_processing_policy (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY
                                                      ACE_ENV_ARG_PARAMETER);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::REQUEST_PROCESSING_POLICY_ID,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);

        PortableServer::RequestProcessingPolicy_var policy2 =
          PortableServer::RequestProcessingPolicy::_narrow (policy.in ()
                                                            ACE_ENV_ARG_PARAMETER);

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

#endif /* TAO_HAS_MINIMUM_POA == 0 */

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
