// $Id$

#include "tao/Invocation_Endpoint_Selectors.h"

#if !defined (__ACE_INLINE__)
#include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "tao/Invocation.h"
#include "tao/RT_Policy_i.h"
#include "tao/Client_Priority_Policy.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"

ACE_RCSID(tao, Invocation_Endpoint_Selectors, "$Id$")



void
TAO_Endpoint_Selector_Factory::get_selector (TAO_GIOP_Invocation
                                             *invocation,
                                             CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_RT_CORBA == 1)

  // Obtains correct selector if TAO::Client_Priority_Policy is
  // enabled.
  this->check_client_priority_policy (invocation,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  if (invocation->endpoint_selector_ != 0)
    return;

  // Initialize Invocation's endpoint selection state.
  this->init_state (invocation, ACE_TRY_ENV);
  ACE_CHECK;

  // Look at RTCORBA policies to decide on appropriate selector.

  TAO_PriorityModelPolicy *priority_policy =
    invocation->endpoint_selection_state_.priority_model_policy_;

  if (priority_policy == 0)
    // Priority model not set.
    {
     // Bands without priority model do not make sense.
      if (invocation->endpoint_selection_state_.bands_policy_
          != 0)
        ACE_THROW (CORBA::INV_POLICY ());

      if (invocation->endpoint_selection_state_.client_protocol_policy_
          == 0)
        invocation->endpoint_selector_ =
          invocation->orb_core_->default_endpoint_selector ();
      else
        {
          invocation->endpoint_selector_ =
            invocation->orb_core_->protocol_endpoint_selector ();
        }
      return;
    }

  if (priority_policy->get_priority_model ()
      == RTCORBA::CLIENT_PROPAGATED)
    // CLIENT_PROPAGATED priority model.
    {
      // Get client priority.
      if (invocation->orb_core_->get_thread_priority
          (invocation->endpoint_selection_state_.client_priority_)
          == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (1,
                                           CORBA::COMPLETED_NO));
      if (invocation->endpoint_selection_state_.client_protocol_policy_
          == 0)
        invocation->endpoint_selector_ =
          invocation->orb_core_->priority_endpoint_selector ();
      else
        invocation->endpoint_selector_ =
          invocation->orb_core_->priority_protocol_selector ();
    }
  else
    {
      // SERVER_DECLARED priority model.
      if (invocation->endpoint_selection_state_.client_protocol_policy_
          == 0)
        invocation->endpoint_selector_ =
          invocation->orb_core_->default_endpoint_selector ();
      else
        invocation->endpoint_selector_ =
          invocation->orb_core_->protocol_endpoint_selector ();
    }

  // Check that we are in range with Priority Bands.
  if (invocation->endpoint_selection_state_.bands_policy_)
    {
      CORBA::Short p;

      if (priority_policy->get_priority_model ()
          == RTCORBA::CLIENT_PROPAGATED)
        p = invocation->endpoint_selection_state_.client_priority_;
      else
        p = priority_policy->server_priority ();

      if (!this->is_in_bands_range (
              invocation->endpoint_selection_state_.bands_policy_, p))
        ACE_THROW (CORBA::INV_POLICY ());
    }

#else /* TAO_HAS_RT_CORBA == 1 */

  ACE_UNUSED_ARG (ACE_TRY_ENV);

  invocation->endpoint_selector_ =
    invocation->orb_core_->default_endpoint_selector ();

#endif /* TAO_HAS_RT_CORBA == 1 */
}

#if (TAO_HAS_RT_CORBA == 1)

void
TAO_Endpoint_Selector_Factory::
check_client_priority_policy (TAO_GIOP_Invocation *invocation,
                              CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *policy =
    invocation->stub_->client_priority ();

  // Automatically release the policy
  CORBA::Object_var auto_release = policy;

  if (policy == 0)
    invocation->endpoint_selector_ =
      invocation->orb_core_->default_endpoint_selector ();

  else
    {
      // Policy is set.
      TAO::PrioritySpecification priority_spec =
        policy->priority_specification (ACE_TRY_ENV);
      ACE_CHECK;

      TAO::PrioritySelectionMode mode = priority_spec.mode;

      if (mode == TAO::USE_NO_PRIORITY)
      // Don't care about priority - use default selector.
        invocation->endpoint_selector_ =
              invocation->orb_core_->default_endpoint_selector ();
      else
        // Care about priority - use client priority policy selector.
        {
          invocation->endpoint_selector_ =
            invocation->orb_core_->client_priority_policy_selector ();

          // Initialize endpoint selection state.
          if (mode == TAO::USE_PRIORITY_RANGE)
            {
              invocation->endpoint_selection_state_.min_priority_ =
                priority_spec.min_priority;
              invocation->endpoint_selection_state_.max_priority_ =
                priority_spec.max_priority;
            }
          else
            // mode == TAO::USE_THREAD_PRIORITY
            {
              CORBA::Short priority;
              if (invocation->orb_core_->get_thread_priority (priority)
                  == -1)
                ACE_THROW (CORBA::DATA_CONVERSION (1,
                                                   CORBA::COMPLETED_NO));
              invocation->endpoint_selection_state_.min_priority_ =
                priority;
              invocation->endpoint_selection_state_.max_priority_ =
                priority;
            }
        }
    }

#else

      ACE_UNUSED_ARG (invocation);
      ACE_UNUSED_ARG (ACE_TRY_ENV);

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */
}

void
TAO_Endpoint_Selector_Factory::
init_state (TAO_GIOP_Invocation *invocation,
            CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize selection state with all RTCORBA policies affecting
  // endpoint selection.

  invocation->endpoint_selection_state_.priority_model_policy_ =
    invocation->stub_->exposed_priority_model ();

  invocation->endpoint_selection_state_.client_protocol_policy_ =
    invocation->stub_->effective_client_protocol (ACE_TRY_ENV);
  ACE_CHECK;

  invocation->endpoint_selection_state_.private_connection_ =
    invocation->stub_->private_connection ();

  invocation->endpoint_selection_state_.bands_policy_ =
    invocation->stub_->effective_priority_banded_connection (ACE_TRY_ENV);
  ACE_CHECK;
}


int
TAO_Endpoint_Selector_Factory::is_in_bands_range
                  (TAO_PriorityBandedConnectionPolicy *bands_policy,
                   CORBA::Short priority)
{
  int in_range = 0;
  RTCORBA::PriorityBands &bands = bands_policy->priority_bands_rep ();

  if (bands.length () == 0)
    return 1;

  for (CORBA::ULong i = 0; i < bands.length (); ++i)
    if (bands[i].low <= priority
        && bands[i].high >= priority)
      {
        in_range = 1;
        break;
      }

  return in_range;
}


// ****************************************************************

TAO_Endpoint_Selection_State::~TAO_Endpoint_Selection_State (void)
{
  CORBA::release (this->priority_model_policy_);
  CORBA::release (this->client_protocol_policy_);
  CORBA::release (this->bands_policy_);
  CORBA::release (this->private_connection_);
}

#endif /* TAO_HAS_RT_CORBA == 1 */

// ****************************************************************

TAO_Invocation_Endpoint_Selector::~TAO_Invocation_Endpoint_Selector (void)
{
}

// ****************************************************************

TAO_Default_Endpoint_Selector::~TAO_Default_Endpoint_Selector (void)
{
}

void
TAO_Default_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation
                                                *invocation,
                                                CORBA::Environment &ACE_TRY_ENV)
{
  invocation->profile_ = invocation->stub_->profile_in_use ();
  invocation->endpoint_ = invocation->profile_->endpoint ();

  if (invocation->endpoint_ == 0)
    {
      // Unknown protocol - move onto the next profile.
      this->next (invocation, ACE_TRY_ENV);
      ACE_CHECK;
      this->select_endpoint (invocation, ACE_TRY_ENV);
    }
}

void
TAO_Default_Endpoint_Selector::next (TAO_GIOP_Invocation
                                     *invocation,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  if (invocation->stub_->next_profile_retry () == 0)
    ACE_THROW (CORBA::TRANSIENT (
                                 CORBA_SystemException::_tao_minor_code (
                                      TAO_INVOCATION_CONNECT_MINOR_CODE,
                                      errno),
                                 CORBA::COMPLETED_NO));
}

void
TAO_Default_Endpoint_Selector::forward (TAO_GIOP_Invocation
                                        *invocation,
                                        const TAO_MProfile &mprofile,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  invocation->stub_->add_forward_profiles (mprofile);
  // This has to be and is thread safe.
  // TAO_Stub::add_forward_profiles() already makes a copy of the
  // MProfile, so do not make a copy here.


  // We may not need to do this since TAO_GIOP_Invocations
  // get created on a per-call basis. For now we'll play it safe.
  if (invocation->stub_->next_profile () == 0)
    ACE_THROW (CORBA::TRANSIENT (
                                 CORBA_SystemException::_tao_minor_code (
                                       TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
                                       errno),
                                 CORBA::COMPLETED_NO));
}

void
TAO_Default_Endpoint_Selector::success (TAO_GIOP_Invocation *invocation)
{
  invocation->stub_->set_valid_profile ();
}

void
TAO_Default_Endpoint_Selector::close_connection (TAO_GIOP_Invocation *invocation)
{
  // Get rid of any forwarding profiles and reset
  // the profile list to point to the first profile!
  // FRED For now we will not deal with recursive forwards!
  invocation->stub_->reset_profiles ();
}

// ****************************************************************

#if (TAO_HAS_RT_CORBA == 1)

TAO_Priority_Endpoint_Selector::~TAO_Priority_Endpoint_Selector (void)
{
}

void
TAO_Priority_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation
                                                 *invocation,
                                                 CORBA::Environment &ACE_TRY_ENV)
{
  // Obtain profile.
  invocation->profile_ = invocation->stub_->profile_in_use ();

  // Select an endpoint from the profile.

  if (invocation->profile_->endpoint_count () == 0)
    {
    // Unknown protocol - move onto the next profile.
      this->next (invocation, ACE_TRY_ENV);
      ACE_CHECK;
      this->select_endpoint (invocation, ACE_TRY_ENV);
    }

  else if (invocation->profile_->endpoint_count () == 1)
    {
      // Profile contains just one endpoint.  This happens when:
      //    a) we are talking to a nonTAO server (which doesn't have
      //       the concept of multiple endpoints per profile)
      //    or
      //    b) we have TAO server with a non-lane threadpool, in which
      //    case there is only one acceptor
      // In both cases we should use the endpoint regardless of its priority.
      invocation->endpoint_ = invocation->profile_->endpoint ();
    }
  else
    {
      // Profiles contains more than one endpoint.  Find one with the
      // right priority.
      TAO_Endpoint *endpoint = 0;
      for (TAO_Endpoint *endp = invocation->profile_->endpoint ();
           endp != 0;
           endp = endp->next ())
        {
          if (endp->priority ()
              == invocation->endpoint_selection_state_.client_priority_)
            {
              endpoint = endp;
              break;
            }
        }

      if (endpoint != 0)
        {
          // Found an Endpoint with correct priority.
          invocation->endpoint_ = endpoint;
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
          if (this->is_multihomed (invocation->profile_->endpoint ()))
            {
              // case a
              invocation->endpoint_ = invocation->profile_->endpoint ();
            }
          else
            // case b
            ACE_THROW (CORBA::INV_POLICY ());
        }
    }
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

TAO_Protocol_Endpoint_Selector::~TAO_Protocol_Endpoint_Selector (void)
{
}

void
TAO_Protocol_Endpoint_Selector::select_endpoint (TAO_GIOP_Invocation
                                                 *invocation,
                                                 CORBA::Environment &ACE_TRY_ENV)
{
  RTCORBA::ProtocolList & protocols =
    invocation->endpoint_selection_state_.
    client_protocol_policy_->protocols_rep ();

  CORBA::ULong protocol_index =
    invocation->endpoint_selection_state_.client_protocol_index_;

  if (protocols.length () == protocol_index)
    // We have tried all the protocols specified in the client
    // protocol policy with no success.  Throw exception.
    {
      // Figure out proper exception.
      if (!invocation->endpoint_selection_state_.valid_endpoint_found_)
        ACE_THROW (CORBA::INV_POLICY ());
      else
        // At least one satisfactory endpoint was found, but
        // connection could not be established.
        ACE_THROW (CORBA::COMM_FAILURE ());
    }

  // Find a Profile for the next protocol we would like to try.
  TAO_Profile *profile = 0;
  TAO_MProfile& mprofile = invocation->stub_->base_profiles ();

  for (TAO_PHandle i = 0;
       i < mprofile.profile_count ();
       ++i)
    {
      TAO_Profile *pf = mprofile.get_profile (i);
      if (pf->tag ()
          == protocols[protocol_index].protocol_type)
        {
          profile = pf;
          break;
        }
    }

  if (profile == 0
      || profile->endpoint_count () == 0)
    {
      // If either no profile for the protocol of interest were found
      // or profile was found but client ORB doesn't understand the
      // protocol, try another protocol.
      invocation->endpoint_selection_state_.client_protocol_index_++;
      this->select_endpoint (invocation, ACE_TRY_ENV);
    }
  else
    {
      // Found the profile - get the endpoint.
      invocation->profile_ = profile;
      this->endpoint (invocation, ACE_TRY_ENV);
    }
}

void
TAO_Protocol_Endpoint_Selector::endpoint (TAO_GIOP_Invocation *invocation,
                                          CORBA::Environment& /*ACE_TRY_ENV*/)
{
  invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
  invocation->endpoint_ = invocation->profile_->endpoint ();
}

void
TAO_Protocol_Endpoint_Selector::next (TAO_GIOP_Invocation
                                     *invocation,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  invocation->endpoint_selection_state_.client_protocol_index_++;
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
  invocation->stub_->set_valid_profile ();
}

void
TAO_Protocol_Endpoint_Selector::close_connection (TAO_GIOP_Invocation *invocation)
{
  invocation->endpoint_selection_state_.client_protocol_index_ = 0;
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
  TAO_MProfile& mprofile = invocation->stub_->base_profiles ();
  invocation->endpoint_ = 0;
  CORBA::ULong profile_index =
    invocation->endpoint_selection_state_.client_protocol_index_;

    if (mprofile.profile_count () == profile_index)
      // We have tried all the profiles with no success.  Throw exception.
    {
      // Figure out proper exception.
      if (!invocation->endpoint_selection_state_.valid_endpoint_found_)
        ACE_THROW (CORBA::INV_POLICY ());
      else
        // At least one satisfactory endpoint was found, but
        // connection could not be established.
        ACE_THROW (CORBA::COMM_FAILURE ());
    }

    CORBA::Short max =
      invocation->endpoint_selection_state_.max_priority_;
    CORBA::Short min =
      invocation->endpoint_selection_state_.min_priority_;

    for (TAO_PHandle i = profile_index;
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
          if (priority >= min && priority <= max)
            {
              invocation->profile_ = profile;
              invocation->endpoint_ = endp;
              invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
              invocation->endpoint_selection_state_.client_protocol_index_ = i;
              break;
            }
        }

      if (invocation->endpoint_ != 0)
        break;
    }

  // We were not able to find profile with the endpoint of the
  // right priority.
  if (invocation->endpoint_ == 0)
    // Figure out proper exception.
    if (!invocation->endpoint_selection_state_.valid_endpoint_found_)
      ACE_THROW (CORBA::INV_POLICY ());
    else
      // At least one satisfactory endpoint was found, but
      // connection could not be established.
      ACE_THROW (CORBA::COMM_FAILURE ());
}

void
TAO_Client_Priority_Policy_Selector::next (TAO_GIOP_Invocation
                                           *invocation,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  invocation->endpoint_selection_state_.client_protocol_index_++;
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
  invocation->stub_->set_valid_profile ();
}

void
TAO_Client_Priority_Policy_Selector::close_connection (TAO_GIOP_Invocation *invocation)
{
  invocation->endpoint_selection_state_.client_protocol_index_ = 0;
}

// ****************************************************************

TAO_Priority_Protocol_Selector::~TAO_Priority_Protocol_Selector (void)
{
}

void
TAO_Priority_Protocol_Selector::endpoint (TAO_GIOP_Invocation *invocation,
                                          CORBA::Environment
                                          &ACE_TRY_ENV)
{
  if (invocation->profile_->endpoint_count () == 1)
    {
      // Profile contains just one endpoint.  This happens when:
      //    a) we are talking to a nonTAO server (which doesn't have
      //       the concept of multiple endpoints per profile)
      //    or
      //    b) we have TAO server with a non-lane threadpool, in which
      //    case there is only one acceptor
      // In both cases we should use the endpoint regardless of its priority.
      invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
      invocation->endpoint_ = invocation->profile_->endpoint ();
    }
  else
    {
      // Profiles contains more than one endpoint.  Find one with the
      // right priority.
      TAO_Endpoint *endpoint = 0;
      for (TAO_Endpoint *endp = invocation->profile_->endpoint ();
           endp != 0;
           endp = endp->next ())
        {
          if (endp->priority ()
              == invocation->endpoint_selection_state_.client_priority_)
            {
              endpoint = endp;
              break;
            }
        }

      if (endpoint != 0)
        {
          // Found an Endpoint with correct priority.
          invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
          invocation->endpoint_ = endpoint;
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
          if (this->is_multihomed (invocation->profile_->endpoint ()))
            {
              // case a
              invocation->endpoint_selection_state_.valid_endpoint_found_ = 1;
              invocation->endpoint_ = invocation->profile_->endpoint ();
            }
          else
            // case b
            ACE_THROW (CORBA::INV_POLICY ());
        }
    }
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

#endif /* TAO_HAS_RT_CORBA == 1 */
