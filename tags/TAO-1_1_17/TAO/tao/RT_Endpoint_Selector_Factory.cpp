// $Id$

#include "tao/RT_Endpoint_Selector_Factory.h"
#include "tao/RT_Invocation_Endpoint_Selectors.h"
#include "tao/Client_Priority_Policy.h"
#include "tao/Invocation.h"
#include "tao/RT_Endpoint_Utils.h"

#if (TAO_HAS_RT_CORBA == 1)

# include "tao/RT_Stub.h"

#endif /* TAO_HAS_RT_CORBA */

ACE_RCSID(tao, RT_Endpoint_Selector_Factory, "$Id$")

// ****************************************************************
#if (TAO_HAS_RT_CORBA == 1)

RT_Endpoint_Selector_Factory::RT_Endpoint_Selector_Factory (void)
{
  ACE_NEW (this->priority_endpoint_selector_,
           TAO_Priority_Endpoint_Selector);

  ACE_NEW (this->bands_endpoint_selector_,
           TAO_Bands_Endpoint_Selector);

  ACE_NEW (this->protocol_endpoint_selector_,
           TAO_Protocol_Endpoint_Selector);

  ACE_NEW (this->priority_protocol_selector_,
           TAO_Priority_Protocol_Selector);

  ACE_NEW (this->bands_protocol_selector_,
           TAO_Bands_Protocol_Selector);

  ACE_NEW (this->client_priority_policy_selector_,
           TAO_Client_Priority_Policy_Selector);
}

RT_Endpoint_Selector_Factory::~RT_Endpoint_Selector_Factory (void)
{
  delete this->priority_endpoint_selector_;
  delete this->bands_endpoint_selector_;
  delete this->protocol_endpoint_selector_;
  delete this->priority_protocol_selector_;
  delete this->bands_protocol_selector_;
  delete this->client_priority_policy_selector_;
}

TAO_Invocation_Endpoint_Selector *
RT_Endpoint_Selector_Factory::get_selector (TAO_GIOP_Invocation *invocation,
                                             CORBA::Environment &ACE_TRY_ENV)
{
  // Obtains correct selector if TAO::Client_Priority_Policy is
  // enabled.
  TAO_Invocation_Endpoint_Selector *selector =
                  this->check_client_priority_policy (invocation,
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // CASE 1: TAO::Client_Priority_Policy is enabled.
  if (selector != 0)
    return selector;

  // Initialize selection state with all RTCORBA policies affecting
  // endpoint selection.
  //  TAO_Endpoint_Selection_State &state =
  //invocation->endpoint_selection_state_;

  TAO_RT_Stub *rt_stub =
    ACE_dynamic_cast (TAO_RT_Stub *, invocation->stub ());

  if (rt_stub == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Unexpected error narrowing stub to TAO_RT_Stub"));

      ACE_THROW_RETURN (CORBA::INTERNAL (
                           CORBA_SystemException::_tao_minor_code (
                                                                   TAO_DEFAULT_MINOR_CODE,
                                                                   EINVAL),
                           CORBA::COMPLETED_NO),
                        0);
    }

  CORBA::Policy_var priority_model_policy =
    rt_stub->exposed_priority_model (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  //  state.private_connection_ =
  //   rt_stub->private_connection ();

  CORBA::Policy_var client_protocol_policy =
    TAO_RT_Endpoint_Utils::client_protocol_policy (invocation, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::Policy_var bands_policy = TAO_RT_Endpoint_Utils::priority_bands_policy (invocation, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  //
  // Look at RTCORBA policies to decide on appropriate selector.
  //

  // CASE 2: No PriorityModelPolicy set.
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
          ACE_THROW_RETURN (CORBA::INV_POLICY (), 0);
        }

      if (client_protocol_policy.ptr () == 0)
        return this->default_endpoint_selector_;
      else
        return this->protocol_endpoint_selector_;
    }

  CORBA::Boolean is_client_propagated = 0;

  // @@ This is to be used in case 3:
  CORBA::Short server_priority = 0;

  invocation->orb_core ()->get_protocols_hooks () ->get_selector_hook (
                                               priority_model_policy.in (),
                                               is_client_propagated,
                                               server_priority);

//   if (is_client_propagated)
//     {
//       // Get client priority.
//       if (invocation->orb_core_->get_protocols_hooks ()
//           ->get_thread_priority (invocation->orb_core_,
//                                  state.client_priority_,
//                                  ACE_TRY_ENV)
//           == -1)
//         ACE_THROW_RETURN (CORBA::DATA_CONVERSION (1,
//                                                   CORBA::COMPLETED_NO),
//                           0);
//     }

  // CASE 3: PriorityBandedConnection Policy is set.
  if (bands_policy.ptr () != 0)
    {
      // Matching band found.  Instantiate appropriate selector.
      if (client_protocol_policy.ptr () == 0)
        return this->bands_endpoint_selector_;
      else
        return this->bands_protocol_selector_;
    }

  // CASE 4: CLIENT_PROPAGATED priority model, no bands.
  if (is_client_propagated)
    {
      if (client_protocol_policy.ptr () == 0)
        return this->priority_endpoint_selector_;
      else
        return this->priority_protocol_selector_;
    }
  else
    {
      // CASE 5: SERVER_DECLARED priority model, no bands.
      if (client_protocol_policy.ptr () == 0)
        return this->default_endpoint_selector_;
      else
        return this->protocol_endpoint_selector_;
    }
}

TAO_Invocation_Endpoint_Selector *
RT_Endpoint_Selector_Factory::
check_client_priority_policy (TAO_GIOP_Invocation *invocation,
                              CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *policy =
    invocation->stub_->client_priority ();

  // Automatically release the policy
  CORBA::Object_var auto_release = policy;

  if (policy == 0)
    return this->default_endpoint_selector_;

  else
    {
      // Policy is set.
      TAO::PrioritySpecification priority_spec =
        policy->priority_specification (ACE_TRY_ENV);
      ACE_CHECK;

      TAO::PrioritySelectionMode mode = priority_spec.mode;

      if (mode == TAO::USE_NO_PRIORITY)
        // Don't care about priority - use default selector.
        return this->default_endpoint_selector_;
      else
        // Care about priority - use client priority policy selector.
        {
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

          return this->client_priority_policy_selector_;
        }
    }

#else

      ACE_UNUSED_ARG (invocation);
      ACE_UNUSED_ARG (ACE_TRY_ENV); // FUZZ: ignore check_for_ace_check

      return 0;
#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */
}

void
RT_Endpoint_Selector_Factory::
init_client_protocol (TAO_GIOP_Invocation *,
                      CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("RT_Endpoint_Selector_Factory:init_client_protocol - Is deprecated!")));
  /*
  TAO_RT_Stub *rt_stub =
    ACE_dynamic_cast (TAO_RT_Stub *, invocation->stub_);

  ACE_TRY
    {
      invocation->endpoint_selection_state_.client_protocol_policy_ =
        rt_stub->effective_client_protocol (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (invocation->inconsistent_policies_.ptr ())
        {
          invocation->inconsistent_policies_->length (1);
          invocation->inconsistent_policies_[0u] =
            rt_stub->client_protocol ();
        }
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  */
}

#endif /* TAO_HAS_RT_CORBA == 1 */
