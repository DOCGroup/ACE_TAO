// $Id$

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

ACE_RCSID(tao, RT_Invocation_Endpoint_Selectors, "$Id$")

// ****************************************************************

void
TAO_RT_Default_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation *invocation
                                                   ACE_ENV_ARG_DECL)
{
  TAO_RT_Stub *rt_stub =
    ACE_dynamic_cast (TAO_RT_Stub *,
                      invocation->stub ());

  do
    {
      invocation->profile (invocation->stub ()->profile_in_use ());
      invocation->endpoint (invocation->profile ()->endpoint ());

      // If known endpoint, select it.
      if (invocation->endpoint () != 0)
        {
          int status;
          if (rt_stub->private_connection ())
            {
              TAO_Private_Transport_Descriptor private_desc (invocation->endpoint (),
                                                             ACE_reinterpret_cast (long, invocation->stub ()));

              status = invocation->perform_call (private_desc ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              TAO_Base_Transport_Property default_desc (invocation->endpoint ());

              status = invocation->perform_call (default_desc ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }

          // Check if the invocation has completed.
          if (status == 1)
            return;
        }
    }
  while (invocation->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (TAO_OMG_VMCID | 2,
                               CORBA::COMPLETED_NO));
}

// ****************************************************************

TAO_Priority_Endpoint_Selector::~TAO_Priority_Endpoint_Selector (void)
{
}

void
TAO_Priority_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation *invocation
                                                 ACE_ENV_ARG_DECL)
{
  TAO_RT_Stub *rt_stub = ACE_dynamic_cast (TAO_RT_Stub *,
                                           invocation->stub ());
  CORBA::Short client_priority;

  // Get client priority.
  TAO_Protocols_Hooks *tph = invocation->orb_core ()->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (tph->get_thread_CORBA_priority (client_priority
                                      ACE_ENV_ARG_PARAMETER)
      == -1)
    ACE_THROW (CORBA::DATA_CONVERSION (1,
               CORBA::COMPLETED_NO));

  do
    {
      // Obtain profile.
      invocation->profile (invocation->stub ()->profile_in_use ());

      // Select an endpoint from the profile.

      if (invocation->profile ()->endpoint_count () == 0)
        {
          // Unknown protocol - move onto the next profile.
          continue;
        }

      else if (invocation->profile ()->endpoint_count () == 1)
        {
          // Profile contains just one endpoint.  This happens when:
          //    a) we are talking to a nonTAO server (which doesn't have
          //       the concept of multiple endpoints per profile)
          //    or
          //    b) we have TAO server with a non-lane threadpool, in which
          //    case there is only one acceptor
          // In both cases we should use the endpoint regardless of its priority.
          invocation->endpoint (invocation->profile ()->endpoint ());
        }
      else
        {
          // Profiles contains more than one endpoint.  Find one with the
          // right priority.

          TAO_Endpoint *endpoint = 0;
          for (TAO_Endpoint *endp = invocation->profile ()->endpoint ();
               endp != 0;
               endp = endp->next ())
            {
              if (endp->priority () == client_priority)
                {
                  endpoint = endp;
                  break;
                }
            }

          if (endpoint != 0)
            {
              // Found an Endpoint with correct priority.
              invocation->endpoint (endpoint);
            }
          else
            {
              // The profile didn't contain an endpoint with matching priority.
              // There are two possibilities:
              //   a) the server is a multi-homed host and this is the
              //      only reason for profile containing more than 1
              //      endpoint, i.e., case a) above.
              //   b) we have misconfiguration - threadpool with lanes on
              //      the server, and client priority not matching any of
              //      the lanes.
              if (this->is_multihomed (invocation->profile ()->endpoint ()))
                {
                  // case a
                  invocation->endpoint (invocation->profile ()->endpoint ());
                }
              else
                // case b
                {
                  if (invocation->inconsistent_policies ().ptr ())
                    {
                      CORBA::Policy_var priority_model_policy = rt_stub->exposed_priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
                      ACE_CHECK;

                      invocation->inconsistent_policies ()->length (1);
                      invocation->inconsistent_policies ()[0u] =
                        CORBA::Policy::_duplicate (priority_model_policy.in ());
                    }
                  ACE_THROW (CORBA::INV_POLICY ());
                }
            }
        }

      // Try to perform the invocation using this endpoint.
      int status;
      if (rt_stub->private_connection ())
        {
          TAO_Private_Transport_Descriptor private_desc (invocation->endpoint (),
                                                         ACE_reinterpret_cast (long, invocation->stub ()));

          status = invocation->perform_call (private_desc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          TAO_Base_Transport_Property default_desc (invocation->endpoint ());

          status = invocation->perform_call (default_desc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      // Check if the invocation has completed.
      if (status == 1)
        return;
    }
  while (invocation->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (TAO_OMG_VMCID | 2,
                               CORBA::COMPLETED_NO));

}

int
TAO_Priority_Endpoint_Selector::is_multihomed (TAO_Endpoint *endpoint)
{
  for (TAO_Endpoint *endp = endpoint;
       endp != 0;
       endp = endp->next ())
    {
      if (endp->next () != 0
          && endp->priority () != endp->next ()->priority ())
        return 0;
    }
  return 1;
}

// ****************************************************************

TAO_Bands_Endpoint_Selector::~TAO_Bands_Endpoint_Selector (void)
{
}

void
TAO_Bands_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation *invocation
                                              ACE_ENV_ARG_DECL)
{
  TAO_RT_Stub *rt_stub = ACE_dynamic_cast (TAO_RT_Stub *,
                                           invocation->stub ());
  TAO_Protocols_Hooks *protocol_hooks =
    invocation->orb_core ()->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Policy_var bands_policy = TAO_RT_Endpoint_Utils::priority_bands_policy (invocation
                                                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Figure out target priority.
  CORBA::Policy_var priority_model_policy =
    rt_stub->exposed_priority_model (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Short server_priority = 0;
  CORBA::Boolean is_client_propagated = 0;
  protocol_hooks->get_selector_hook (
                                     priority_model_policy.in (),  // input
                                     is_client_propagated, // side effect
                                     server_priority); // side effect


  CORBA::Short p;
  if (is_client_propagated)
    {
      // Get Client priority.

      int status =
        protocol_hooks->get_thread_CORBA_priority (p  // side effect
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (status == -1)
        {
          ACE_THROW (CORBA::DATA_CONVERSION (1,
                     CORBA::COMPLETED_NO));
        }
    }
  else
    p = server_priority;

  int in_range = 0;

  CORBA::Short min_priority;
  CORBA::Short max_priority;

  protocol_hooks->get_selector_bands_policy_hook (bands_policy.in (),
                                                  min_priority,
                                                  max_priority,
                                                  p,
                                                  in_range);

  // If priority doesn't fall into any of the bands
  if (!in_range)
    {
      if (invocation->inconsistent_policies ().ptr ())
        {
          invocation->inconsistent_policies ()->length (2);
          invocation->inconsistent_policies ()[0u] =
            CORBA::Policy::_duplicate (bands_policy.in ());
          invocation->inconsistent_policies ()[1u] =
            CORBA::Policy::_duplicate (priority_model_policy.in ());
        }
      ACE_THROW (CORBA::INV_POLICY ());
    }

  do
    {
      // Obtain profile.
      invocation->profile (invocation->stub ()->profile_in_use ());

      // Select an endpoint from the profile.

      if (invocation->profile ()->endpoint_count () == 0)
        {
          // Unknown protocol - move onto the next profile.
          continue;
        }

      else if (invocation->profile ()->endpoint_count () == 1)
        {
          // Profile contains just one endpoint.  This happens when:
          //    a) we are talking to a nonTAO server (which doesn't have
          //       the concept of multiple endpoints per profile)
          //    or
          //    b) we have TAO server with a non-lane threadpool, in which
          //    case there is only one acceptor
          // In both cases we should use the endpoint regardless of its priority.
          invocation->endpoint (invocation->profile ()->endpoint ());
        }
      else
        {
          // Profiles contains more than one endpoint.  Find one with the
          // right priority.

          TAO_Endpoint *endpoint = 0;
          for (TAO_Endpoint *endp = invocation->profile ()->endpoint ();
               endp != 0;
               endp = endp->next ())
            {
              if (endp->priority () <= max_priority
                  && endp->priority () >= min_priority)
                {
                  if (TAO_debug_level > 1)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "(%P|%t) TAO Endpoint Selection: priority = %d, band = [%d %d], endpoint = %d\n",
                                  p,
                                  min_priority,
                                  max_priority,
                                  endp->priority ()));
                    }
                  endpoint = endp;
                  break;
                }
           }

          if (endpoint != 0)
            {
              // Found an Endpoint with correct priority.
              invocation->endpoint (endpoint);
            }
          else
            {
              if (invocation->inconsistent_policies ().ptr ())
                {
                  invocation->inconsistent_policies ()->length (1);
                  invocation->inconsistent_policies ()[0u] =
                    CORBA::Policy::_duplicate (bands_policy.in ());
                }
          ACE_THROW (CORBA::INV_POLICY ());
            }
        }

      // Try to perform the invocation using this endpoint.
      int status;
      if (rt_stub->private_connection ())
        {
          TAO_Private_Transport_Descriptor private_desc (invocation->endpoint (),
                                                         ACE_reinterpret_cast (long, invocation->stub ()));

          status = invocation->perform_call (private_desc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          TAO_Base_Transport_Property default_desc (invocation->endpoint ());

          status = invocation->perform_call (default_desc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      // Check if the invocation has completed.
      if (status == 1)
        return;
    }
  while (invocation->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (TAO_OMG_VMCID | 2,
                               CORBA::COMPLETED_NO));


}

// ****************************************************************

TAO_Protocol_Endpoint_Selector::~TAO_Protocol_Endpoint_Selector (void)
{
}

void
TAO_Protocol_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation
                                                 *invocation
                                                 ACE_ENV_ARG_DECL)
{
  /// Narrow down to the right policy.
  CORBA::Policy_var cp =
    TAO_RT_Endpoint_Utils::client_protocol_policy (invocation ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::ClientProtocolPolicy_var cp_policy =
    RTCORBA::ClientProtocolPolicy::_narrow (cp.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  /// Cast to TAO_ClientProtocolPolicy
  TAO_ClientProtocolPolicy *client_protocol_policy =
    ACE_static_cast (TAO_ClientProtocolPolicy *,
                     cp_policy.in ());

  /// Get the ProtocolList
  RTCORBA::ProtocolList & protocols =
    client_protocol_policy->protocols_rep ();

  TAO_RT_Stub *rt_stub = ACE_dynamic_cast (TAO_RT_Stub *,
                                           invocation->stub ());

  CORBA::Boolean valid_endpoint_found = 0;

  for (CORBA::ULong protocol_index = 0;
       protocol_index < protocols.length ();
       protocol_index++)
    {
      // Find the profiles that match the current protocol.
      TAO_Profile *profile = 0;
      TAO_MProfile& mprofile = invocation->stub ()->base_profiles ();

      for (TAO_PHandle i = 0;
           i < mprofile.profile_count ();
           ++i)
        {
          profile = mprofile.get_profile (i);
          if (profile->tag ()
              == protocols[protocol_index].protocol_type)
            {
              // Save away the profile and endpoint that we found in the
              // invocation.
              invocation->profile (profile);
              invocation->endpoint (profile->endpoint());
              valid_endpoint_found = 1;

              // Try to perform the invocation using this endpoint.
              int status;
              if (rt_stub->private_connection ())
                {
                  TAO_Private_Transport_Descriptor private_desc (invocation->endpoint (),
                                                                 ACE_reinterpret_cast (long, invocation->stub ()));

                  status = invocation->perform_call (private_desc ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }
              else
                {
                  TAO_Base_Transport_Property default_desc (invocation->endpoint ());

                  status = invocation->perform_call (default_desc ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }

              // Check if the invocation has completed.
              if (status == 1)
                return;
            }
        }
    }

  // We have tried all the protocols specified in the client
  // protocol policy with no success.  Throw exception.

  // Figure out proper exception.
  if (!valid_endpoint_found)
    {
      if (invocation->inconsistent_policies ().ptr ())
        {
          invocation->inconsistent_policies ()->length (1);
          invocation->inconsistent_policies ()[0u] =
            CORBA::Policy::_duplicate (client_protocol_policy);
        }
      ACE_THROW (CORBA::INV_POLICY ());
    }

    // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (TAO_OMG_VMCID | 2,
                               CORBA::COMPLETED_NO));


}

void
TAO_Protocol_Endpoint_Selector::next (TAO_GIOP_Invocation
                                     *
                                     ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("You Are not Suppesed to call: TAO_Protocol_Endpoint_Selector::next!!!\n")));
  //  invocation->profile_index ()++;
  // If we ran out of profiles to try - this will be detected and
  // exception thrown once <endpoint> is called.
}

void
TAO_Protocol_Endpoint_Selector::forward (TAO_GIOP_Invocation
                                        *invocation,
                                         const TAO_MProfile& /* mprofile */
                                        ACE_ENV_ARG_DECL)
{
  // Location forwarding is currently not supported when using
  // RTCORBA::ClientProtocolPolicy.  With the current profile
  // management scheme being broken, it would be a pain to support here.
  // @@ Add support for location forwarding once profile management is
  // redesigned.

  // Currently location forward replies are ignored.  When we receive
  // a location forward reply, we act as if the profile we used
  // didn't work, and we try the next one.  Should we throw not
  // supported exception instead?
  this->next (invocation ACE_ENV_ARG_PARAMETER);
}

void
TAO_Protocol_Endpoint_Selector::success (TAO_GIOP_Invocation *invocation)
{
  invocation->stub ()->set_valid_profile ();
}

void
TAO_Protocol_Endpoint_Selector::close_connection (TAO_GIOP_Invocation *)
{
}

// ****************************************************************

TAO_Priority_Protocol_Selector::~TAO_Priority_Protocol_Selector (void)
{
}

int
TAO_Priority_Protocol_Selector::is_multihomed (TAO_Endpoint *endpoint)
{
  for (TAO_Endpoint *endp = endpoint;
       endp != 0;
       endp = endp->next ())
    {
      if (endp->next () != 0
          && endp->priority () != endp->next ()->priority ())
        return 0;
    }
  return 1;
}

// ****************************************************************

TAO_Bands_Protocol_Selector::~TAO_Bands_Protocol_Selector (void)
{
}
