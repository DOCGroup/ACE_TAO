// $Id$

#include "RT_Endpoint_Selector_Factory.h"
#include "RT_Invocation_Endpoint_Selectors.h"
#include "tao/Invocation.h"
#include "RT_Endpoint_Utils.h"
#include "RT_Stub.h"

ACE_RCSID(tao, RT_Endpoint_Selector_Factory, "$Id$")

// ****************************************************************

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

  ACE_NEW (this->rt_default_endpoint_selector_,
           TAO_RT_Default_Endpoint_Selector);
}

RT_Endpoint_Selector_Factory::~RT_Endpoint_Selector_Factory (void)
{
  delete this->priority_endpoint_selector_;
  delete this->bands_endpoint_selector_;
  delete this->protocol_endpoint_selector_;
  delete this->priority_protocol_selector_;
  delete this->bands_protocol_selector_;
  delete this->rt_default_endpoint_selector_;
}

TAO_Invocation_Endpoint_Selector *
RT_Endpoint_Selector_Factory::get_selector (TAO_GIOP_Invocation *invocation
                                             TAO_ENV_ARG_DECL)
{
  // Initialize selection state with all RTCORBA policies affecting
  // endpoint selection.

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
    rt_stub->exposed_priority_model (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  //  state.private_connection_ =
  //   rt_stub->private_connection ();

  CORBA::Policy_var client_protocol_policy =
    TAO_RT_Endpoint_Utils::client_protocol_policy (invocation TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Policy_var bands_policy = TAO_RT_Endpoint_Utils::priority_bands_policy (invocation TAO_ENV_ARG_PARAMETER);
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
        return this->rt_default_endpoint_selector_;
      else
        return this->protocol_endpoint_selector_;
    }

  // @@ This is to be used in case 3:
  CORBA::Short server_priority = 0;
  CORBA::Boolean is_client_propagated = 0;

  TAO_Protocols_Hooks *tph = invocation->orb_core ()->get_protocols_hooks (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  tph->get_selector_hook (priority_model_policy.in (),
                          is_client_propagated,
                          server_priority);

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
        return this->rt_default_endpoint_selector_;
      else
        return this->protocol_endpoint_selector_;
    }
}


void
RT_Endpoint_Selector_Factory::
init_client_protocol (TAO_GIOP_Invocation *
                      TAO_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("RT_Endpoint_Selector_Factory:init_client_protocol - Is deprecated!")));
  /*
  TAO_RT_Stub *rt_stub =
    ACE_dynamic_cast (TAO_RT_Stub *, invocation->stub_);

  ACE_TRY
    {
      invocation->endpoint_selection_state_.client_protocol_policy_ =
        rt_stub->effective_client_protocol (TAO_ENV_SINGLE_ARG_PARAMETER);
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

// ****************************************************************

ACE_STATIC_SVC_DEFINE (RT_Endpoint_Selector_Factory,
                       ACE_TEXT ("RT_Endpoint_Selector_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (RT_Endpoint_Selector_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTCORBA, RT_Endpoint_Selector_Factory)
