#include "RT_Endpoint_Utils.h"
#include "tao/Profile_Transport_Resolver.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "RT_Endpoint_Selector_Factory.h"
#include "RT_Invocation_Endpoint_Selectors.h"

#include "RT_Stub.h"

ACE_RCSID (RTCORBA, 
           RT_Endpoint_Utils, 
           "$Id$")

// ****************************************************************

CORBA::Policy *
TAO_RT_Endpoint_Utils::priority_bands_policy (TAO::Profile_Transport_Resolver &r
                                              ACE_ENV_ARG_DECL)
{
  CORBA::Policy *bands_policy = CORBA::Policy::_nil ();

  TAO_RT_Stub *rt_stub = dynamic_cast<TAO_RT_Stub *> (r.stub ());

  ACE_TRY
    {
      bands_policy =
        rt_stub->effective_priority_banded_connection (
                     ACE_ENV_SINGLE_ARG_PARAMETER
                   );
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (r.inconsistent_policies ())
        {
          CORBA::PolicyList *p = r.inconsistent_policies ();
          p->length (1);
          (*p)[0u] = rt_stub->priority_banded_connection ();
        }

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return bands_policy;
}

/*static*/ CORBA::Policy *
TAO_RT_Endpoint_Utils::client_protocol_policy (
    TAO::Profile_Transport_Resolver &r
    ACE_ENV_ARG_DECL)
{
  CORBA::Policy *retval = 0;

  TAO_RT_Stub *rt_stub = dynamic_cast<TAO_RT_Stub *> (r.stub ());

  if (rt_stub == 0)
    {
      return 0;
    }

  ACE_TRY
    {
      retval =
        rt_stub->effective_client_protocol (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (r.inconsistent_policies ())
        {
          CORBA::PolicyList *p = r.inconsistent_policies ();
          p->length (1);
          (*p)[0u] = rt_stub->client_protocol ();
        }

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return retval;
}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
