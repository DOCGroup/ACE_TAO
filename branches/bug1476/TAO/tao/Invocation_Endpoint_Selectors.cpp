// -*- C++ -*-
//$Id$
#include "tao/Invocation_Endpoint_Selectors.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "Base_Transport_Property.h"
#include "Profile_Transport_Resolver.h"
#include "ORB_Constants.h"

#if !defined (__ACE_INLINE__)
#include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Invocation_Endpoint_Selectors,
           "$Id$")

TAO_Invocation_Endpoint_Selector::TAO_Invocation_Endpoint_Selector (void)
{
}

TAO_Invocation_Endpoint_Selector::~TAO_Invocation_Endpoint_Selector (void)
{
}

// ****************************************************************

TAO_Default_Endpoint_Selector::TAO_Default_Endpoint_Selector (void)
{
}

TAO_Default_Endpoint_Selector::~TAO_Default_Endpoint_Selector (void)
{
}

void
TAO_Default_Endpoint_Selector::select_endpoint (
    TAO::Profile_Transport_Resolver *r,
    ACE_Time_Value *max_wait_time,
    bool block
    ACE_ENV_ARG_DECL)
{
  do
    {
      r->profile (r->stub ()->profile_in_use ());

// need also a book heere
      // If we shouldn't block but the profile doesn't support this
      // we can't use this profile
// shouldn't we just loop and try the next?
      if (!block && !r->profile()->supports_non_blocking_oneways())
        {
          break;
        }

      const size_t endpoint_count =
        r->profile ()->endpoint_count ();

      TAO_Endpoint *ep =
        r->profile ()->endpoint ();

      for (size_t i = 0; i < endpoint_count; ++i)
        {
          TAO_Base_Transport_Property desc (ep);
// now back to the transport resolver
          bool retval =
            r->try_connect (&desc,
                            max_wait_time,
                            block
                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Check if the connect has completed.
          if (retval)
            return;

          // Go to the next endpoint in this profile.
          ep = ep->next ();
        }
    }
  while (r->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (CORBA::OMGVMCID | 2,
                               CORBA::COMPLETED_NO));
}
