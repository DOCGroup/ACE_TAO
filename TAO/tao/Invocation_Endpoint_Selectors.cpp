// $Id$

#include "tao/Invocation_Endpoint_Selectors.h"

#if !defined (__ACE_INLINE__)
#include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "tao/Invocation.h"
#include "tao/Client_Priority_Policy.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "tao/RT_Invocation_Endpoint_Selectors.h"

ACE_RCSID(tao, Invocation_Endpoint_Selectors, "$Id$")

// ****************************************************************

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

  // CASE 1: TAO::Client_Priority_Policy is enabled.
  if (invocation->endpoint_selector_ != 0)
    return;

  // Initialize selection state with all RTCORBA policies affecting
  // endpoint selection.
  TAO_Endpoint_Selection_State &state =
    invocation->endpoint_selection_state_;

  state.priority_model_policy_ =
    invocation->stub_->exposed_priority_model (ACE_TRY_ENV);
  ACE_CHECK;

  state.private_connection_ =
    invocation->stub_->private_connection ();

  this->init_client_protocol (invocation, ACE_TRY_ENV);
  ACE_CHECK;
  this->init_bands (invocation, ACE_TRY_ENV);
  ACE_CHECK;

  //
  // Look at RTCORBA policies to decide on appropriate selector.
  //

  // CASE 2: No PriorityModelPolicy set.
  if (state.priority_model_policy_ == 0)
    {
     // Bands without priority model do not make sense.
      if (state.bands_policy_ != 0)
        {
          if (invocation->inconsistent_policies_.ptr ())
            {
              invocation->inconsistent_policies_->length (1);
              invocation->inconsistent_policies_[0u] =
                CORBA::Policy::_duplicate (state.bands_policy_);
            }
          ACE_THROW (CORBA::INV_POLICY ());
        }

      if (state.client_protocol_policy_ == 0)
        invocation->endpoint_selector_ =
          invocation->orb_core_->default_endpoint_selector ();
      else
        {
          invocation->endpoint_selector_ =
            invocation->orb_core_->protocol_endpoint_selector ();
        }
      return;
    }

  CORBA::Boolean is_client_propagated = 0;

  // @@ This is to be used in case 3:
  CORBA::Short server_priority = 0;

  invocation->orb_core_->get_protocols_hooks () ->get_selector_hook (
                                               state.priority_model_policy_,
                                               is_client_propagated,
                                               server_priority);

  if (is_client_propagated)
    {
      // Get client priority.
      if (invocation->orb_core_->get_protocols_hooks ()
          ->get_thread_priority (invocation->orb_core_,
                                 state.client_priority_,
                                 ACE_TRY_ENV)
          == -1)
        ACE_THROW (CORBA::DATA_CONVERSION (1,
                                           CORBA::COMPLETED_NO));
    }

  // CASE 3: PriorityBandedConnection Policy is set.
  if (state.bands_policy_ != 0)
    {
      // Figure out target priority.
      CORBA::Short p;
      if (is_client_propagated)
        p = state.client_priority_;
      else
        p = server_priority;

      int in_range = 0;

      invocation->orb_core_->get_protocols_hooks
        ()->get_selector_bands_policy_hook (state.bands_policy_,
                                            state.min_priority_,
                                            state.max_priority_,
                                            p,
                                            in_range);

      // If priority doesn't fall into any of the bands
      if (!in_range)
        {
          if (invocation->inconsistent_policies_.ptr ())
            {
              invocation->inconsistent_policies_->length (2);
              invocation->inconsistent_policies_[0u] =
                CORBA::Policy::_duplicate (state.bands_policy_);
              invocation->inconsistent_policies_[1u] =
                CORBA::Policy::_duplicate (state.priority_model_policy_);
            }
          ACE_THROW (CORBA::INV_POLICY ());
        }

      // Matching band found.  Instantiate appropriate selector.
      if (state.client_protocol_policy_ == 0)
        invocation->endpoint_selector_ =
          invocation->orb_core_->bands_endpoint_selector ();
      else
          invocation->endpoint_selector_ =
            invocation->orb_core_->bands_protocol_selector ();

      return;
    }

  // CASE 4: CLIENT_PROPAGATED priority model, no bands.
  if (is_client_propagated)
    {
      if (state.client_protocol_policy_ == 0)
        invocation->endpoint_selector_ =
          invocation->orb_core_->priority_endpoint_selector ();
      else
        invocation->endpoint_selector_ =
          invocation->orb_core_->priority_protocol_selector ();
    }
  else
    {
      // CASE 5: SERVER_DECLARED priority model, no bands.
      if (state.client_protocol_policy_ == 0)
        invocation->endpoint_selector_ =
          invocation->orb_core_->default_endpoint_selector ();
      else
        invocation->endpoint_selector_ =
          invocation->orb_core_->protocol_endpoint_selector ();
    }
#else /* TAO_HAS_RT_CORBA == 1 */

  ACE_UNUSED_ARG (ACE_TRY_ENV); // FUZZ: ignore check_for_ace_check

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
              if (invocation->orb_core_->get_protocols_hooks
                  ()->get_thread_priority (invocation->orb_core_,
                                           priority,
                                           ACE_TRY_ENV)
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
      ACE_UNUSED_ARG (ACE_TRY_ENV); // FUZZ: ignore check_for_ace_check

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */
}

void
TAO_Endpoint_Selector_Factory::
init_client_protocol (TAO_GIOP_Invocation *invocation,
                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      invocation->endpoint_selection_state_.client_protocol_policy_ =
        invocation->stub_->effective_client_protocol (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (invocation->inconsistent_policies_.ptr ())
        {
          invocation->inconsistent_policies_->length (1);
          invocation->inconsistent_policies_[0u] =
            invocation->stub_->client_protocol ();
        }
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
TAO_Endpoint_Selector_Factory::
init_bands (TAO_GIOP_Invocation *invocation,
            CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      invocation->endpoint_selection_state_.bands_policy_ =
        invocation->stub_->effective_priority_banded_connection (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (invocation->inconsistent_policies_.ptr ())
        {
          invocation->inconsistent_policies_->length (1);
          invocation->inconsistent_policies_[0u] =
            invocation->stub_->priority_banded_connection ();
        }
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

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
      ACE_CHECK;
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
