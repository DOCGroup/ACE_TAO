#include "tao/RTCORBA/RT_Endpoint_Utils.h"
#include "tao/Profile_Transport_Resolver.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Endpoint_Selector_Factory.h"
#include "tao/RTCORBA/RT_Invocation_Endpoint_Selectors.h"

#include "tao/RTCORBA/RT_Stub.h"

ACE_RCSID (RTCORBA,
           RT_Endpoint_Utils,
           "$Id$")

// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy *
TAO_RT_Endpoint_Utils::policy (TAO_Cached_Policy_Type type,
                               TAO::Profile_Transport_Resolver &r
                               ACE_ENV_ARG_DECL)
{
  CORBA::Policy *policy = CORBA::Policy::_nil ();

  TAO_RT_Stub *rt_stub = dynamic_cast<TAO_RT_Stub *> (r.stub ());

  ACE_TRY
    {
      policy =
        rt_stub->get_cached_policy (type
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (r.inconsistent_policies ())
        {
          CORBA::PolicyList *p = r.inconsistent_policies ();
          p->length (1);
          (*p)[0u] = rt_stub->TAO_Stub::get_cached_policy (type
                                                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return policy;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
