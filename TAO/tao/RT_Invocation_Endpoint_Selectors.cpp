// $Id$

#include "tao/RT_Invocation_Endpoint_Selectors.h"

#if !defined (__ACE_INLINE__)
#include "tao/RT_Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "tao/Invocation.h"
#include "tao/RT_Policy_i.h"
#include "tao/Client_Priority_Policy.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "tao/RT_Stub.h"
#include "tao/Private_Transport_Descriptor.h"
#include "tao/Base_Transport_Property.h"
#include "tao/RT_Endpoint_Utils.h"
#include "RT_Protocols_Hooks.h"

ACE_RCSID(tao, RT_Invocation_Endpoint_Selectors, "$Id$")

// ****************************************************************

#if (TAO_HAS_RT_CORBA == 1)

TAO_Priority_Endpoint_Selector::~TAO_Priority_Endpoint_Selector (void)
{
}

void
TAO_Priority_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation *invocation,
                                                 CORBA::Environment &ACE_TRY_ENV)
{
  TAO_RT_Stub *rt_stub = ACE_dynamic_cast (TAO_RT_Stub *,
                                           invocation->stub ());
  CORBA::Short client_priority;

  // Get client priority.
  if (invocation->orb_core ()->get_protocols_hooks ()
      ->get_thread_priority (invocation->orb_core (),
                             client_priority,
                             ACE_TRY_ENV)
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
                      CORBA::Policy_var priority_model_policy = rt_stub->exposed_priority_model (ACE_TRY_ENV);
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

          status = invocation->perform_call (private_desc, ACE_TRY_ENV);
          ACE_CHECK;
        }
      else
        {
          TAO_Base_Transport_Property default_desc (invocation->endpoint ());

          status = invocation->perform_call (default_desc, ACE_TRY_ENV);
          ACE_CHECK;
        }

      // Check if the invocation has completed.
      if (status == 1)
        return;
    }
  while (invocation->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (
                               CORBA_SystemException::_tao_minor_code (
                                    TAO_INVOCATION_CONNECT_MINOR_CODE,
                                    errno),
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
TAO_Bands_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation *invocation,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  TAO_RT_Stub *rt_stub = ACE_dynamic_cast (TAO_RT_Stub *,
                                           invocation->stub ());
  TAO_Protocols_Hooks *protocol_hooks =
    invocation->orb_core ()->get_protocols_hooks ();


  CORBA::Policy_var bands_policy = TAO_RT_Endpoint_Utils::priority_bands_policy (invocation,
                                                                              ACE_TRY_ENV);
  ACE_CHECK;

  // Figure out target priority.
  CORBA::Policy_var priority_model_policy =
    rt_stub->exposed_priority_model (ACE_TRY_ENV);
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

      int status = protocol_hooks->get_thread_priority (invocation->orb_core (),
                                                        p, // side effect
                                                        ACE_TRY_ENV);
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

          status = invocation->perform_call (private_desc, ACE_TRY_ENV);
          ACE_CHECK;
        }
      else
        {
          TAO_Base_Transport_Property default_desc (invocation->endpoint ());

          status = invocation->perform_call (default_desc, ACE_TRY_ENV);
          ACE_CHECK;
        }

      // Check if the invocation has completed.
      if (status == 1)
        return;
    }
  while (invocation->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (
                               CORBA_SystemException::_tao_minor_code (
                                    TAO_INVOCATION_CONNECT_MINOR_CODE,
                                    errno),
                               CORBA::COMPLETED_NO));


//   // Obtain profile.
//   invocation->profile_ = invocation->stub_->profile_in_use ();

//   // Select an endpoint from the profile.

//   if (invocation->profile_->endpoint_count () == 0)
//     {
//     // Unknown protocol - move onto the next profile.
//       this->next (invocation, ACE_TRY_ENV);
//       ACE_CHECK;
//       this->select_endpoint (invocation, ACE_TRY_ENV);
//       ACE_CHECK;
//     }
//   else
//     {

//       // Find the endpoint for the band of interest.
//       TAO_Endpoint *endpoint = 0;
//       for (TAO_Endpoint *endp = invocation->profile_->endpoint ();
//            endp != 0;
//            endp = endp->next ())
//         {
//           if (endp->priority ()
//               <= invocation->endpoint_selection_state_.max_priority_
//               && endp->priority ()
//               >= invocation->endpoint_selection_state_.min_priority_)
//             {
//               endpoint = endp;
//               break;
//             }
//         }

//       if (endpoint != 0)
//         {
//           // Found an Endpoint with priority in the range of the band.
//           invocation->endpoint_ = endpoint;
//         }
//       else
//         {
//           // The profile didn't contain an endpoint with priority
//           // matching the band.
//           // There are two possibilities:
//           //   a) we are talking to non-TAO server.
//           //   b) we have misconfiguration - bands were set on the
//           //      client and do not match server configuration.
//           // In both cases throw exception.  (We are throwing
//           // exception for case 'a' because the current implementation of
//           // bands is not interoperable.)
//           if (invocation->inconsistent_policies_.ptr ())
//             {
//               invocation->inconsistent_policies_->length (1);
//               invocation->inconsistent_policies_[0u] =
//                 CORBA::Policy::_duplicate (invocation->
//                      endpoint_selection_state_.bands_policy_);
//             }
//           ACE_THROW (CORBA::INV_POLICY ());
//         }
//     }
}

// ****************************************************************

TAO_Protocol_Endpoint_Selector::~TAO_Protocol_Endpoint_Selector (void)
{
}

void
TAO_Protocol_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation
                                                 *invocation,
                                                 CORBA::Environment &ACE_TRY_ENV)
{
  /// Narrow down to the right policy.
  CORBA::Policy_var cp =
    TAO_RT_Endpoint_Utils::client_protocol_policy (invocation, ACE_TRY_ENV);
  ACE_CHECK;

  RTCORBA::ClientProtocolPolicy_var cp_policy =
    RTCORBA::ClientProtocolPolicy::_narrow (cp.in (), ACE_TRY_ENV);
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

                  status = invocation->perform_call (private_desc, ACE_TRY_ENV);
                  ACE_CHECK;
                }
              else
                {
                  TAO_Base_Transport_Property default_desc (invocation->endpoint ());

                  status = invocation->perform_call (default_desc, ACE_TRY_ENV);
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
  ACE_THROW (CORBA::TRANSIENT (
                               CORBA_SystemException::_tao_minor_code (
                                    TAO_INVOCATION_CONNECT_MINOR_CODE,
                                    errno),
                               CORBA::COMPLETED_NO));


  // if (protocols.length () == protocol_index)
//     // We have tried all the protocols specified in the client
//     // protocol policy with no success.  Throw exception.
//     {
//       // Figure out proper exception.
//       if (!invocation->endpoint_selection_state_.valid_endpoint_found_)
//         {
//           if (invocation->inconsistent_policies_.ptr ())
//             {
//               invocation->inconsistent_policies_->length (1);
//               invocation->inconsistent_policies_[0u] =
//                 CORBA::Policy::_duplicate (invocation->
//                      endpoint_selection_state_.client_protocol_policy_);
//             }
//           ACE_THROW (CORBA::INV_POLICY ());
//         }
//       else
//         // At least one satisfactory endpoint was found, but
//         // connection could not be established.
//         ACE_THROW (CORBA::COMM_FAILURE ());
//     }

//   // Find a Profile for the next protocol we would like to try.
//   TAO_Profile *profile = 0;
//   TAO_MProfile& mprofile = invocation->stub ()->base_profiles ();

//   for (TAO_PHandle i = 0;
//        i < mprofile.profile_count ();
//        ++i)
//     {
//       TAO_Profile *pf = mprofile.get_profile (i);
//       if (pf->tag ()
//           == protocols[protocol_index].protocol_type)
//         {
//           profile = pf;
//           break;
//         }
//     }

//   if (profile == 0
//       || profile->endpoint_count () == 0)
//     {
//       // If either no profile for the protocol of interest were found
//       // or profile was found but client ORB doesn't understand the
//       // protocol, try another protocol.
//       invocation->endpoint_selection_state_.client_protocol_index_++;
//       this->select_endpoint (invocation, ACE_TRY_ENV);
//       ACE_CHECK;
//     }
//   else
//     {
//       // Found the profile - get the endpoint.
//       invocation->profile_ = profile;
//       this->endpoint (invocation, ACE_TRY_ENV);
//       ACE_CHECK;
//     }
}

// void
// TAO_Protocol_Endpoint_Selector::endpoint (TAO_GIOP_Invocation *invocation,
//                                           CORBA::Environment& /*ACE_TRY_ENV*/)
// {
//   // invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
//   invocation->endpoint_ = invocation->profile_->endpoint ();
// }

void
TAO_Protocol_Endpoint_Selector::next (TAO_GIOP_Invocation
                                     *,
                                     CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("You Are not Suppesed to call: TAO_Protocol_Endpoint_Selector::next!!!\n")));
  //  invocation->profile_index ()++;
  // If we ran out of profiles to try - this will be detected and
  // exception thrown once <endpoint> is called.
}

void
TAO_Protocol_Endpoint_Selector::forward (TAO_GIOP_Invocation
                                        *invocation,
                                         const TAO_MProfile& /* mprofile */,
                                        CORBA::Environment &ACE_TRY_ENV)
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
  this->next (invocation, ACE_TRY_ENV);
}

void
TAO_Protocol_Endpoint_Selector::success (TAO_GIOP_Invocation *invocation)
{
  invocation->stub ()->set_valid_profile ();
}

void
TAO_Protocol_Endpoint_Selector::close_connection (TAO_GIOP_Invocation *)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("You Are not Suppesed to call: TAO_Protocol_Endpoint_Selector::next!!!\n")));
  //invocation->endpoint_selection_state_.client_protocol_index_ = 0;
}

// ****************************************************************

TAO_Client_Priority_Policy_Selector::~TAO_Client_Priority_Policy_Selector (void)
{
}

void
TAO_Client_Priority_Policy_Selector::select_endpoint (TAO_GIOP_Invocation
                                                      *invocation,
                                                      CORBA::Environment &ACE_TRY_ENV)
{
  TAO_RT_Stub *rt_stub = ACE_dynamic_cast (TAO_RT_Stub *,
                                           invocation->stub ());

  TAO_MProfile& mprofile = invocation->stub ()->base_profiles ();
  invocation->endpoint (0);

    // Figure out target priority.
  CORBA::Policy_var priority_model_policy =
    rt_stub->exposed_priority_model (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Short min_priority;
  CORBA::Short max_priority;
  CORBA::Policy_var bands_policy = TAO_RT_Endpoint_Utils::priority_bands_policy (invocation,
                                                                              ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Short p;
  CORBA::Short server_priority = 0;
  CORBA::Boolean is_client_propagated = 0;

  TAO_Protocols_Hooks *protocol_hooks = invocation->orb_core ()->get_protocols_hooks ();
  protocol_hooks->get_selector_hook (
                                     priority_model_policy.in (),  // input
                                     is_client_propagated, // side effect
                                     server_priority); // side effect


  if (is_client_propagated)
    {
      // Get Client priority.

      int status = protocol_hooks->get_thread_priority (invocation->orb_core (),
                                                        p, // side effect
                                                        ACE_TRY_ENV);
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

  protocol_hooks->get_selector_bands_policy_hook (bands_policy.in (),
                                                  min_priority,
                                                  max_priority,
                                                  p,
                                                  in_range);

  CORBA::Boolean valid_endpoint_found = 0;

  do
    {
      for (TAO_PHandle i = invocation->profile_index ();
           i < mprofile.profile_count ();
           ++i)
        {
          TAO_Profile *profile = mprofile.get_profile (i);

          // Check if this profile contains any endpoints of the
          // right priority.
          for (TAO_Endpoint *endp = profile->endpoint ();
               endp != 0;
               endp = endp->next ())
            {
              CORBA::Short priority = endp->priority ();
              if (priority >= min_priority && priority <= max_priority)
                {
                  invocation->profile (profile);
                  invocation->endpoint (endp);
                  valid_endpoint_found = 1;
                  invocation->profile_index () = i;
                  break;
                }
            }
          if (invocation->endpoint () != 0)
            break;
        }

      if (invocation->endpoint () == 0)
        break;

      // Try to perform the invocation using this endpoint.
      int status;
      valid_endpoint_found = 1;
      if (rt_stub->private_connection ())
        {
          TAO_Private_Transport_Descriptor private_desc (invocation->endpoint (),
                                                         ACE_reinterpret_cast (long, invocation->stub ()));

          status = invocation->perform_call (private_desc, ACE_TRY_ENV);
          ACE_CHECK;
        }
      else
        {
          TAO_Base_Transport_Property default_desc (invocation->endpoint ());

          status = invocation->perform_call (default_desc, ACE_TRY_ENV);
          ACE_CHECK;
        }

      // Check if the invocation has completed.
      if (status == 1)
        return;
    }
  while (++invocation->profile_index () < mprofile.profile_count ());

  // Figure out proper exception.
  if (!valid_endpoint_found)
    {
      if (invocation->inconsistent_policies ().ptr ())
        {
          invocation->inconsistent_policies ()->length (1);
          invocation->inconsistent_policies ()[0u] =
            invocation->stub ()->get_client_policy
            (TAO_CLIENT_PRIORITY_POLICY_TYPE,
             ACE_TRY_ENV);
        }
      ACE_THROW (CORBA::INV_POLICY ());
    }

  ACE_THROW (CORBA::TRANSIENT (
                               CORBA_SystemException::_tao_minor_code (
                                    TAO_INVOCATION_CONNECT_MINOR_CODE,
                                    errno),
                               CORBA::COMPLETED_NO));



//   for (TAO_PHandle i = profile_index;
//        i < mprofile.profile_count ();
//        ++i)
//     {
//       TAO_Profile *profile = mprofile.get_profile (i);

//       // Check if this profile contains any endpoints of the
//       // right priority.
//       for (TAO_Endpoint *endp = profile->endpoint ();
//            endp != 0;
//            endp = endp->next ())
//         {
//           CORBA::Short priority = endp->priority ();
//           if (priority >= min && priority <= max)
//             {
//               invocation->profile (profile);
//               invocation->endpoint (endp);
//               invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
//               invocation->endpoint_selection_state_.client_protocol_index_ = i;
//               break;
//             }
//         }

//       if (invocation->endpoint () != 0)
//         break;
//     }

//   // We were not able to find profile with the endpoint of the
//   // right priority.
//   if (invocation->endpoint_ == 0)
//     // Figure out proper exception.
//     if (!invocation->endpoint_selection_state_.valid_endpoint_found_)
//       {
//         if (invocation->inconsistent_policies_.ptr ())
//           {
//             invocation->inconsistent_policies_->length (1);
//             invocation->inconsistent_policies_[0u] =
//               invocation->stub_->get_client_policy
//               (TAO_CLIENT_PRIORITY_POLICY_TYPE,
//                ACE_TRY_ENV);
//           }
//         ACE_THROW (CORBA::INV_POLICY ());
//       }
//     else
//       // At least one satisfactory endpoint was found, but
//       // connection could not be established.
//       ACE_THROW (CORBA::COMM_FAILURE ());
}

void
TAO_Client_Priority_Policy_Selector::next (TAO_GIOP_Invocation
                                           *invocation,
                                           CORBA::Environment &)
{
  invocation->profile_index ()++;
  // If we ran out of profiles to try - this will be detected and
  // exception thrown once <endpoint> is called.
}

void
TAO_Client_Priority_Policy_Selector::forward (TAO_GIOP_Invocation
                                              *invocation,
                                              const TAO_MProfile & /* mprofile */,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  // Location forwarding is not supported when using
  // TAO::Client_Priority_Policy.  Location forward replies are
  // ignored.  When we receive a location forward reply, we act as if
  // the profile we used didn't work, and we try the next one.
  this->next (invocation, ACE_TRY_ENV);
}

void
TAO_Client_Priority_Policy_Selector::success (TAO_GIOP_Invocation *invocation)
{
  invocation->stub ()->set_valid_profile ();
}

void
TAO_Client_Priority_Policy_Selector::close_connection (TAO_GIOP_Invocation *invocation)
{
  invocation->profile_index () = 0;
}

// ****************************************************************

TAO_Priority_Protocol_Selector::~TAO_Priority_Protocol_Selector (void)
{
}

// void
// TAO_Priority_Protocol_Selector::endpoint (TAO_GIOP_Invocation *invocation,
//                                           CORBA::Environment
//                                           &ACE_TRY_ENV)
// {
//   if (invocation->profile_->endpoint_count () == 1)
//     {
//       // Profile contains just one endpoint.  This happens when:
//       //    a) we are talking to a nonTAO server (which doesn't have
//       //       the concept of multiple endpoints per profile)
//       //    or
//       //    b) we have TAO server with a non-lane threadpool, in which
//       //    case there is only one acceptor
//       // In both cases we should use the endpoint regardless of its priority.
//       invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
//       invocation->endpoint_ = invocation->profile_->endpoint ();
//     }
//   else
//     {
//       // Profiles contains more than one endpoint.  Find one with the
//       // right priority.
//       TAO_Endpoint *endpoint = 0;
//       for (TAO_Endpoint *endp = invocation->profile_->endpoint ();
//            endp != 0;
//            endp = endp->next ())
//         {
//           if (endp->priority ()
//               == invocation->endpoint_selection_state_.client_priority_)
//             {
//               endpoint = endp;
//               break;
//             }
//         }

//       if (endpoint != 0)
//         {
//           // Found an Endpoint with correct priority.
//           invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
//           invocation->endpoint_ = endpoint;
//         }
//       else
//         {
//           // The profile didn't contain an endpoint with matching priority.
//           // There are two possibilities:
//           //   a) the server is a multi-homed host and this is the
//           //      only reason for profile containing more than 1
//           //      endpoint, i.e., case a) above.
//           //   b) we have misconfiguration - threadpool with lanes on
//           //      the server, and client priority not matching any of
//           //      the lanes.
//           if (this->is_multihomed (invocation->profile_->endpoint ()))
//             {
//               // case a
//               invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
//               invocation->endpoint_ = invocation->profile_->endpoint ();
//             }
//           else
//             // case b
//             {
//               if (invocation->inconsistent_policies_.ptr ())
//                 {
//                   invocation->inconsistent_policies_->length (1);
//                   invocation->inconsistent_policies_[0u] =
//                     CORBA::Policy::_duplicate (invocation->
//                          endpoint_selection_state_.priority_model_policy_);
//                 }
//               ACE_THROW (CORBA::INV_POLICY ());
//             }
//         }
//     }
// }

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

// void
// TAO_Bands_Protocol_Selector::endpoint (TAO_GIOP_Invocation *invocation,
//                                        CORBA::Environment
//                                        &ACE_TRY_ENV)
// {
//   // Find the endpoint for the band of interest.
//   TAO_Endpoint *endpoint = 0;
//   for (TAO_Endpoint *endp = invocation->profile_->endpoint ();
//        endp != 0;
//        endp = endp->next ())
//     {
//       if (endp->priority ()
//           <= invocation->endpoint_selection_state_.max_priority_
//           && endp->priority ()
//           >= invocation->endpoint_selection_state_.min_priority_)
//         {
//           endpoint = endp;
//           break;
//         }
//     }

//   if (endpoint != 0)
//     {
//       // Found an Endpoint with priority in the range of the band.
//       invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
//       invocation->endpoint_ = endpoint;
//     }
//   else
//     {
//       // The profile didn't contain an endpoint with priority
//       // matching the band.
//       // There are two possibilities:
//       //   a) we are talking to non-TAO server.
//       //   b) we have misconfiguration - bands were set on the
//       //      client and do not match server configuration.
//       // In both cases throw exception.  (We are throwing
//       // exception for case 'a' because the current implementation of
//       // bands is not interoperable.)
//       if (invocation->inconsistent_policies_.ptr ())
//         {
//           invocation->inconsistent_policies_->length (1);
//           invocation->inconsistent_policies_[0u] =
//             CORBA::Policy::_duplicate (invocation->
//                  endpoint_selection_state_.bands_policy_);
//         }
//       ACE_THROW (CORBA::INV_POLICY ());
//     }
// }

#endif /* TAO_HAS_RT_CORBA == 1 */
