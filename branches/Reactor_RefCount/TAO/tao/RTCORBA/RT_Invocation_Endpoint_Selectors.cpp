#include "RT_Invocation_Endpoint_Selectors.h"

#if !defined (__ACE_INLINE__)
#include "RT_Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "tao/Invocation.h"
#include "RT_Policy_i.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "RT_Stub.h"
#include "Private_Transport_Descriptor.h"
#include "tao/Base_Transport_Property.h"
#include "RT_Endpoint_Utils.h"
#include "RT_Protocols_Hooks.h"
#include "tao/debug.h"


ACE_RCSID (RTCORBA,
           RT_Invocation_Endpoint_Selectors,
           "$Id$")


void
TAO_RT_Invocation_Endpoint_Selector::select_endpoint (
  TAO_GIOP_Invocation *invocation
  ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var client_protocol_policy_base =
    TAO_RT_Endpoint_Utils::client_protocol_policy (invocation
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (client_protocol_policy_base.ptr () == 0)
    {
      this->TAO_Default_Endpoint_Selector::select_endpoint (
        invocation
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      RTCORBA::ClientProtocolPolicy_var client_protocol_policy =
        RTCORBA::ClientProtocolPolicy::_narrow (
          client_protocol_policy_base.in ()
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      /// Cast to TAO_ClientProtocolPolicy
      TAO_ClientProtocolPolicy *tao_client_protocol_policy =
        ACE_static_cast (TAO_ClientProtocolPolicy *,
                         client_protocol_policy.in ());

      /// Get the ProtocolList
      RTCORBA::ProtocolList &client_protocols =
        tao_client_protocol_policy->protocols_rep ();

      this->select_endpoint_based_on_client_protocol_policy (
        invocation,
        client_protocol_policy.in (),
        client_protocols
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_RT_Invocation_Endpoint_Selector::select_endpoint_based_on_client_protocol_policy (
  TAO_GIOP_Invocation *invocation,
  RTCORBA::ClientProtocolPolicy_ptr client_protocol_policy,
  RTCORBA::ProtocolList &client_protocols
  ACE_ENV_ARG_DECL)
{
  CORBA::Boolean valid_profile_found = 0;

  // Even though cycling through all the protocols is the correct
  // things to do to find a match, starting from the start of the
  // profile list is not.  In addition, this code is also ignoring the
  // forwarded reference (if it exists).  This behavior is caused by
  // problems with the profile management in TAO which are documented
  // in bugzilla bugs 1237, 1238, and 1239.  Once the above problems
  // are fixed, this behavior should be fixed to do the right thing.
  for (CORBA::ULong protocol_index = 0;
       protocol_index < client_protocols.length ();
       ++protocol_index)
    {
      // Find the profiles that match the current protocol.
      TAO_Profile *profile = 0;
      TAO_MProfile &mprofile = invocation->stub ()->base_profiles ();

      for (TAO_PHandle i = 0;
           i < mprofile.profile_count ();
           ++i)
        {
          profile = mprofile.get_profile (i);

          if (profile->tag () == client_protocols[protocol_index].protocol_type)
            {
              valid_profile_found = 1;

              invocation->profile (profile);
              invocation->endpoint (invocation->profile ()->endpoint ());

              int status =
                this->endpoint_from_profile (invocation
                                             ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              if (status == 1)
                return;
              // @@ Else we should check for potential forwarding here.
            }
        }
    }

  // We have tried all the profiles specified in the client protocol
  // policy with no success.  Throw exception.
  if (!valid_profile_found)
    {
      if (invocation->inconsistent_policies ().ptr ())
        {
          invocation->inconsistent_policies ()->length (1);
          invocation->inconsistent_policies ()[0u] =
            CORBA::Policy::_duplicate (client_protocol_policy);
        }
      ACE_THROW (CORBA::INV_POLICY ());
    }

  // If we get here, we found at least one pertinent profile, but no
  // usable endpoints.
  ACE_THROW (CORBA::TRANSIENT (CORBA::OMGVMCID | 2,
                               CORBA::COMPLETED_NO));

}

int
TAO_RT_Invocation_Endpoint_Selector::endpoint_from_profile (
  TAO_GIOP_Invocation *invocation
  ACE_ENV_ARG_DECL)
{
  // Narrow to the RT Stub.
  TAO_RT_Stub *rt_stub =
    ACE_dynamic_cast (TAO_RT_Stub *, invocation->stub ());

  if (rt_stub == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Unexpected error narrowing stub to TAO_RT_Stub"));

      ACE_THROW_RETURN (CORBA::INTERNAL (
                           CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              EINVAL),
                           CORBA::COMPLETED_NO),
                        0);
    }

  // Get the priority model policy.
  CORBA::Policy_var priority_model_policy =
    rt_stub->exposed_priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Get the bands policy.
  CORBA::Policy_var bands_policy =
    TAO_RT_Endpoint_Utils::priority_bands_policy (invocation
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  int all_endpoints_are_valid = 0;
  int match_priority = 0;
  int match_bands = 0;
  CORBA::Short client_thread_priority = 0;
  CORBA::Short min_priority = 0;
  CORBA::Short max_priority = 0;

  // If the priority model policy is not set.
  if (priority_model_policy.ptr () == 0)
    {
      // Bands without priority model do not make sense.
      if (bands_policy.ptr () != 0)
        {
          if (invocation->inconsistent_policies ().ptr ())
            {
              invocation->inconsistent_policies ()->length (1);
              invocation->inconsistent_policies ()[0u] =
                CORBA::Policy::_duplicate (bands_policy.in ());
            }
          // Indicate error.
          ACE_THROW_RETURN (CORBA::INV_POLICY (), 0);
        }

      // No priority model policy (and no bands policy): all endpoints
      // are fair game.
      all_endpoints_are_valid = 1;
    }
  // If the priority model policy is set.
  else
    {
      // Get the protocol hooks.
      TAO_Protocols_Hooks *protocol_hooks =
        invocation->orb_core ()->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Short server_priority = 0;
      CORBA::Boolean is_client_propagated = 0;

      // Check the priority model policy to see if it is client
      // propagated.
      protocol_hooks->get_selector_hook (priority_model_policy.in (),
                                         is_client_propagated,
                                         server_priority);

      if (!is_client_propagated)
        {
          // Server declared: all endpoints are fair game.
          all_endpoints_are_valid = 1;
        }
      // Client propagated.
      else
        {
          // Get client thread priority.
          int status =
            protocol_hooks->get_thread_CORBA_priority (
              client_thread_priority  // side effect
              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          if (status == -1)
            {
              ACE_THROW_RETURN (CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 1,
                                                        CORBA::COMPLETED_NO),
                                0);
            }

          // If there are no bands.
          if (bands_policy.ptr () == 0)
            {
              // Match the priority of the client thread with the
              // endpoint.
              match_priority = 1;
            }
          // There are bands.
          else
            {
              // Check which band range we fall in.
              int in_range = 0;
              protocol_hooks->get_selector_bands_policy_hook (
                bands_policy.in (),
                client_thread_priority,
                min_priority,
                max_priority,
                in_range);

              // If priority doesn't fall into any of the bands.
              if (!in_range)
                {
                  if (invocation->inconsistent_policies ().ptr ())
                    {
                      invocation->inconsistent_policies ()->length (2);
                      invocation->inconsistent_policies ()[0u] =
                        CORBA::Policy::_duplicate (bands_policy.in ());
                      invocation->inconsistent_policies ()[1u] =
                        CORBA::Policy::_duplicate (
                          priority_model_policy.in ());
                    }

                  // Indicate error.
                  ACE_THROW_RETURN (CORBA::INV_POLICY (),
                                    0);
                }

              // Match the priority of the band with the endpoint.
              match_bands = 1;
            }
        }
    }

  while (invocation->endpoint () != 0)
    {
      // Get the priority of the endpoint.
      CORBA::Short endpoint_priority =
        invocation->endpoint ()->priority ();

      // If <all_endpoints_are_valid> or match the priority of the
      // client thread or match the priority of the band or
      // profile contains just one endpoint.  This happens when:
      //    a) we are talking to a nonTAO server (which doesn't have
      //       the concept of multiple endpoints per profile)
      //    or
      //    b) we have TAO server with a non-lane threadpool, in which
      //    case there is only one acceptor
      // In both cases we should use the endpoint regardless of its priority.

      if (all_endpoints_are_valid ||
          (match_priority &&
           client_thread_priority == endpoint_priority) ||
          (match_bands &&
           endpoint_priority <= max_priority &&
           endpoint_priority >= min_priority) ||
          invocation->profile ()->endpoint_count () == 1 &&
          endpoint_priority == TAO_INVALID_PRIORITY)
        {
          int status;
          if (rt_stub->private_connection ())
            {
              TAO_Private_Transport_Descriptor private_desc (
                invocation->endpoint (),
                ACE_reinterpret_cast (long, invocation->stub ()));

              status =
                invocation->perform_call (private_desc
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (-1);
            }
          else
            {
              TAO_Base_Transport_Property default_desc (
                invocation->endpoint ());

              status =
                invocation->perform_call (default_desc
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (-1);
            }

          // Check if the invocation has completed.
          if (status == 1)
            return 1;
        }

      // Go to the next endpoint in this profile.
      invocation->endpoint (invocation->endpoint()->next());
    }

  return 0;
}
