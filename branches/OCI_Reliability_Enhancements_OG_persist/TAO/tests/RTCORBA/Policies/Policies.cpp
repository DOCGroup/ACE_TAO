
//=============================================================================
/**
 *  @file     Policies.cpp
 *
 *  $Id$
 *
 *   This program tests the construction of RTCORBA policies, both
 *   through the generic ORB::create_policy interface and the
 *   RTCORBA specific interfaces.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "tao/ORB.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/AnyTypeCode/Any.h"

#include "ace/Log_Msg.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rtorb =
        RTCORBA::RTORB::_narrow (object.in ());

      /*
       * The following code should be reenabled once the OMG spec has
       * been fixed such that a RTCORBA::PriorityModelPolicy can be
       * created by using the ORB::create_policy interface.
       *
      {
        RTCORBA::PriorityModelPolicy_var policy1 =
          rtorb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                               RTCORBA::minPriority);

        CORBA::Any policy_value;
        policy_value <<= RTCORBA::CLIENT_PROPAGATED;
        policy_value <<= RTCORBA::minPriority;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE,
                              policy_value);

        RTCORBA::PriorityModelPolicy_var policy2 =
          RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->priority_model () == policy2->priority_model ());
        ACE_ASSERT (policy1->server_priority () == policy2->server_priority ());
      }

      */

      {
        RTCORBA::ThreadpoolId poolid = 0;

        RTCORBA::ThreadpoolPolicy_var policy1 =
          rtorb->create_threadpool_policy (poolid);

        CORBA::Any policy_value;
        policy_value <<= poolid;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::THREADPOOL_POLICY_TYPE,
                              policy_value);

        RTCORBA::ThreadpoolPolicy_var policy2 =
          RTCORBA::ThreadpoolPolicy::_narrow (policy.in ());

        ACE_ASSERT (policy1->threadpool () == policy2->threadpool ());
      }

      {
        RTCORBA::ProtocolList empty_protocols;

        RTCORBA::ServerProtocolPolicy_var policy1 =
          rtorb->create_server_protocol_policy (empty_protocols);

        CORBA::Any policy_value;
        policy_value <<= empty_protocols;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::SERVER_PROTOCOL_POLICY_TYPE,
                              policy_value);

        RTCORBA::ServerProtocolPolicy_var policy2 =
          RTCORBA::ServerProtocolPolicy::_narrow (policy.in ());

        RTCORBA::ProtocolList_var protocols1 =
          policy1->protocols ();
        RTCORBA::ProtocolList_var protocols2 =
          policy2->protocols ();

        ACE_ASSERT (protocols1->length () == protocols2->length ());
      }

      {
        RTCORBA::ProtocolList empty_protocols;

        RTCORBA::ClientProtocolPolicy_var policy1 =
          rtorb->create_client_protocol_policy (empty_protocols);

        CORBA::Any policy_value;
        policy_value <<= empty_protocols;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE,
                              policy_value);

        RTCORBA::ClientProtocolPolicy_var policy2 =
          RTCORBA::ClientProtocolPolicy::_narrow (policy.in ());

        RTCORBA::ProtocolList_var protocols1 =
          policy1->protocols ();
        RTCORBA::ProtocolList_var protocols2 =
          policy2->protocols ();

        ACE_ASSERT (protocols1->length () == protocols2->length ());
      }

      {
        RTCORBA::PrivateConnectionPolicy_var policy1 =
          rtorb->create_private_connection_policy ();

        CORBA::Any policy_value;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::PRIVATE_CONNECTION_POLICY_TYPE,
                              policy_value);

        RTCORBA::PrivateConnectionPolicy_var policy2 =
          RTCORBA::PrivateConnectionPolicy::_narrow (policy.in ());
      }

      {
        RTCORBA::PriorityBands empty_priority_bands;

        RTCORBA::PriorityBandedConnectionPolicy_var policy1 =
          rtorb->create_priority_banded_connection_policy (empty_priority_bands);

        CORBA::Any policy_value;
        policy_value <<= empty_priority_bands;

        CORBA::Policy_var policy =
          orb->create_policy (RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE,
                              policy_value);

        RTCORBA::PriorityBandedConnectionPolicy_var policy2 =
          RTCORBA::PriorityBandedConnectionPolicy::_narrow (policy.in ());

        RTCORBA::PriorityBands_var priority_bands1 =
          policy1->priority_bands ();
        RTCORBA::PriorityBands_var priority_bands2 =
          policy2->priority_bands ();

        ACE_ASSERT (priority_bands1->length () == priority_bands2->length ());
      }

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
