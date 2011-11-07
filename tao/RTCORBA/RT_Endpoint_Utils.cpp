// $Id$

#include "tao/RTCORBA/RT_Endpoint_Utils.h"
#include "tao/Profile_Transport_Resolver.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_Endpoint_Selector_Factory.h"
#include "tao/RTCORBA/RT_Invocation_Endpoint_Selectors.h"

#include "tao/RTCORBA/RT_Stub.h"

#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_RT_Endpoint_Utils::policy (TAO_Cached_Policy_Type type,
                               TAO::Profile_Transport_Resolver &r)
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  TAO_RT_Stub * const rt_stub =
    dynamic_cast<TAO_RT_Stub *> (r.stub ());

  if (!rt_stub)
    throw CORBA::INTERNAL ();

  try
    {
      policy = rt_stub->get_cached_policy (type);
    }
  catch (const ::CORBA::INV_POLICY&)
    {
      if (r.inconsistent_policies ())
        {
          CORBA::PolicyList *p = r.inconsistent_policies ();
          p->length (1);
          (*p)[0u] = rt_stub->TAO_Stub::get_cached_policy (type);
        }

      throw;
    }

  return policy;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
