#include "RT_Endpoint_Utils.h"
#include "RT_Endpoint_Selector_Factory.h"
#include "RT_Invocation_Endpoint_Selectors.h"
#include "tao/Invocation.h"

#include "RT_Stub.h"

ACE_RCSID(tao, RT_Endpoint_Utils, "$Id$")

// ****************************************************************

CORBA::Policy *
TAO_RT_Endpoint_Utils::priority_bands_policy (TAO_GIOP_Invocation *invocation
                                              TAO_ENV_ARG_DECL)
{
  CORBA::Policy *bands_policy = CORBA::Policy::_nil ();
  TAO_RT_Stub *rt_stub =
    ACE_dynamic_cast (TAO_RT_Stub *, invocation->stub ());

  ACE_TRY
    {
      bands_policy =
        rt_stub->effective_priority_banded_connection (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (invocation->inconsistent_policies ().ptr ())
        {
          invocation->inconsistent_policies ()->length (1);
          invocation->inconsistent_policies ()[0u] =
            rt_stub->priority_banded_connection ();
        }
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return bands_policy;
}

CORBA::Policy *
TAO_RT_Endpoint_Utils::client_protocol_policy (TAO_GIOP_Invocation *invocation
                                               TAO_ENV_ARG_DECL)
{
  TAO_RT_Stub *rt_stub =
    ACE_dynamic_cast (TAO_RT_Stub *, invocation->stub ());

  CORBA::Policy *retval = 0;
  ACE_TRY
    {
      retval = rt_stub->effective_client_protocol (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (invocation->inconsistent_policies ().ptr ())
        {
          invocation->inconsistent_policies ()->length (1);
          invocation->inconsistent_policies ()[0u] =
            rt_stub->client_protocol ();
        }
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return retval;
}

