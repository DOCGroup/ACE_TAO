// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/RTCORBA/Policies
//
// = FILENAME
//     Policies.cpp
//
// = DESCRIPTION
//     This program tests the construction of RTCORBA policies, both
//     through the generic ORB::create_policy interface and the
//     RTCORBA specific interfaces.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "tao/corba.h"
#include "tao/RTCORBA/RTCORBA.h"

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
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rtorb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /*
       * The following code should be reenabled once the OMG spec has
       * been fixed such that a RTCORBA::PriorityModelPolicy can be
       * created by using the ORB::create_policy interface.
       *
      {
        RTCORBA::PriorityModelPolicy_var policy1 =
          rtorb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                               RTCORBA::minPriority
                                               ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::Any policy_value;
        policy_value <<= RTCORBA::CLIENT_PROPAGATED;
        policy_value <<= RTCORBA::minPriority;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::PriorityModelPolicy_var policy2 =
          RTCORBA::PriorityModelPolicy::_narrow (policy.in ()
                                                 ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_ASSERT (policy1->priority_model () == policy2->priority_model ());
        ACE_ASSERT (policy1->server_priority () == policy2->server_priority ());
      }

      */

      {
        RTCORBA::ThreadpoolId poolid = 0;

        RTCORBA::ThreadpoolPolicy_var policy1 =
          rtorb->create_threadpool_policy (poolid
                                           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::Any policy_value;
        policy_value <<= poolid;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::THREADPOOL_POLICY_TYPE,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::ThreadpoolPolicy_var policy2 =
          RTCORBA::ThreadpoolPolicy::_narrow (policy.in ()
                                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_ASSERT (policy1->threadpool () == policy2->threadpool ());
      }

      {
        RTCORBA::ProtocolList empty_protocols;

        RTCORBA::ServerProtocolPolicy_var policy1 =
          rtorb->create_server_protocol_policy (empty_protocols
                                                ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::Any policy_value;
        policy_value <<= empty_protocols;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::SERVER_PROTOCOL_POLICY_TYPE,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::ServerProtocolPolicy_var policy2 =
          RTCORBA::ServerProtocolPolicy::_narrow (policy.in ()
                                                  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::ProtocolList_var protocols1 =
          policy1->protocols ();
        RTCORBA::ProtocolList_var protocols2 =
          policy2->protocols ();

        ACE_ASSERT (protocols1->length () == protocols2->length ());
      }

      {
        RTCORBA::ProtocolList empty_protocols;

        RTCORBA::ClientProtocolPolicy_var policy1 =
          rtorb->create_client_protocol_policy (empty_protocols
                                                ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::Any policy_value;
        policy_value <<= empty_protocols;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::ClientProtocolPolicy_var policy2 =
          RTCORBA::ClientProtocolPolicy::_narrow (policy.in ()
                                                  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::ProtocolList_var protocols1 =
          policy1->protocols ();
        RTCORBA::ProtocolList_var protocols2 =
          policy2->protocols ();

        ACE_ASSERT (protocols1->length () == protocols2->length ());
      }

      {
        RTCORBA::PrivateConnectionPolicy_var policy1 =
          rtorb->create_private_connection_policy (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::Any policy_value;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::PrivateConnectionPolicy_var policy2 =
          RTCORBA::PrivateConnectionPolicy::_narrow (policy.in ()
                                                     ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      {
        RTCORBA::PriorityBands empty_priority_bands;

        RTCORBA::PriorityBandedConnectionPolicy_var policy1 =
          rtorb->create_priority_banded_connection_policy (empty_priority_bands
                                                           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::Any policy_value;
        policy_value <<= empty_priority_bands;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE,
                              policy_value
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::PriorityBandedConnectionPolicy_var policy2 =
          RTCORBA::PriorityBandedConnectionPolicy::_narrow (policy.in ()
                                                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        RTCORBA::PriorityBands_var priority_bands1 =
          policy1->priority_bands ();
        RTCORBA::PriorityBands_var priority_bands2 =
          policy2->priority_bands ();

        ACE_ASSERT (priority_bands1->length () == priority_bands2->length ());
      }

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
  ACE_CHECK_RETURN (-1);

  return 0;
}
