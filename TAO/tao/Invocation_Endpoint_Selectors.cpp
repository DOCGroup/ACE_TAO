// -*- C++ -*-
//$Id$
#include "tao/Invocation_Endpoint_Selectors.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "Base_Transport_Property.h"
#include "Profile_Transport_Resolver.h"
#include "ORB_Constants.h"
#include "SystemException.h"

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
    ACE_Time_Value *max_wait_time
    ACE_ENV_ARG_DECL)
{
  do
    {
      r->profile (r->stub ()->profile_in_use ());

      // Check whether we need to do a blocked wait or we have a
      // non-blocked wait and we support that.  If this is not the
      // case we can't use this profile so try the next.
      if (r->blocked () ||
         (!r->blocked () && r->profile ()->supports_non_blocking_oneways ()))
        {
          const size_t endpoint_count =
            r->profile ()->endpoint_count ();

          TAO_Endpoint *ep =
            r->profile ()->endpoint ();

          for (size_t i = 0; i < endpoint_count; ++i)
            {
              TAO_Base_Transport_Property desc (ep);
              const bool retval =
                r->try_connect (&desc,
                                max_wait_time
                                ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              // Check if the connect has completed.
              if (retval)
                return;

              // Go to the next endpoint in this profile.
              ep = ep->next ();
            }
        }
    }
  while (r->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (CORBA::OMGVMCID | 2,
                               CORBA::COMPLETED_NO));
}
