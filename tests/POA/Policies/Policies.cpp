
//=============================================================================
/**
 *  @file     Policies.cpp
 *
 *  $Id$
 *
 *   This program tests the construction of POA policies, both
 *   through the generic ORB::create_policy interface and the
 *   PortableServer specific interfaces.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "tao/ORB.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PI_Server/PI_Server.h"

#include "ace/Log_Msg.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow to POA.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the POA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

      {
        PortableServer::ThreadPolicy_var policy1 =
          root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::ORB_CTRL_MODEL;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::THREAD_POLICY_ID,
                              policy_value);

        PortableServer::ThreadPolicy_var policy2 =
          PortableServer::ThreadPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

#endif /* TAO_HAS_MINIMUM_POA == 0 */

      {
        PortableServer::LifespanPolicy_var policy1 =
          root_poa->create_lifespan_policy (PortableServer::TRANSIENT);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::TRANSIENT;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::LIFESPAN_POLICY_ID,
                              policy_value);

        PortableServer::LifespanPolicy_var policy2 =
          PortableServer::LifespanPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::IdUniquenessPolicy_var policy1 =
          root_poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::UNIQUE_ID;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::ID_UNIQUENESS_POLICY_ID,
                              policy_value);

        PortableServer::IdUniquenessPolicy_var policy2 =
          PortableServer::IdUniquenessPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::IdAssignmentPolicy_var policy1 =
          root_poa->create_id_assignment_policy (PortableServer::USER_ID);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::USER_ID;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::ID_ASSIGNMENT_POLICY_ID,
                              policy_value);

        PortableServer::IdAssignmentPolicy_var policy2 =
          PortableServer::IdAssignmentPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

      {
        PortableServer::ImplicitActivationPolicy_var policy1 =
          root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::IMPLICIT_ACTIVATION;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::IMPLICIT_ACTIVATION_POLICY_ID,
                              policy_value);

        PortableServer::ImplicitActivationPolicy_var policy2 =
          PortableServer::ImplicitActivationPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::ServantRetentionPolicy_var policy1 =
          root_poa->create_servant_retention_policy (PortableServer::RETAIN);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::RETAIN;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::SERVANT_RETENTION_POLICY_ID,
                              policy_value);

        PortableServer::ServantRetentionPolicy_var policy2 =
          PortableServer::ServantRetentionPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

      {
        PortableServer::RequestProcessingPolicy_var policy1 =
          root_poa->create_request_processing_policy (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);

        CORBA::Any policy_value;
        policy_value <<= PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY;

        CORBA::Policy_var policy =
          orb->create_policy (PortableServer::REQUEST_PROCESSING_POLICY_ID,
                              policy_value);

        PortableServer::RequestProcessingPolicy_var policy2 =
          PortableServer::RequestProcessingPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->value () == policy2->value ());
      }

#endif /* TAO_HAS_MINIMUM_POA == 0 */

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
