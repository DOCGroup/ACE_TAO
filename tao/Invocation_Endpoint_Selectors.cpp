//$Id$

#include "tao/Invocation_Endpoint_Selectors.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/ORB_Core.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Invocation_Endpoint_Selector::~TAO_Invocation_Endpoint_Selector (void)
{
}

// ****************************************************************

TAO_Default_Endpoint_Selector::~TAO_Default_Endpoint_Selector (void)
{
}

void
TAO_Default_Endpoint_Selector::select_endpoint (TAO::Profile_Transport_Resolver *r,
                                                ACE_Time_Value *max_wait_time)
{
  do
    {
      r->profile (r->stub ()->profile_in_use ());

      // Check whether we need to do a blocked wait or we have a
      // non-blocked wait and we support that.  If this is not the
      // case we can't use this profile so try the next.
      if (r->blocked_connect () ||
          (!r->blocked_connect () &&
           r->profile ()->supports_non_blocking_oneways ()))
        {
          if (r->profile ()->endpoint_count () > 1 &&
              r->use_parallel_connect())
            {
              TAO_Endpoint *ep = r->profile ()->endpoint ();

              TAO_Base_Transport_Property desc (ep);

              // Check if the connect has completed.
              if (r->try_parallel_connect (&desc, max_wait_time))
                return;
              // The default implementation of try_parallel_connect returns
              // a not supported errno. In this case, allow the ordinary
              // connection mechanism to be tried. OTOH, if the connection
              // attempt failed for some other reason, then abandon this
              // profile and try the next one in the list.
              else if (errno != ENOTSUP)
                continue;
            }

          for (TAO_Endpoint *ep = r->profile ()->first_filtered_endpoint ();
               ep != 0;
               ep = r->profile ()->next_filtered_endpoint (ep))
            {
              TAO_Base_Transport_Property desc (ep);

              // Check if the connect has completed.
              if (r->try_connect (&desc, max_wait_time))
                return;
            }
        }
    }
  while (r->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint
  // that we know how to use. We used to throw an exception
  // but that would prevent any request interception points
  // being called. They may know how to fix the problem so
  // we wait to throw the exception in
  // Synch_Twoway_Invocation::remote_twoway and
  // Synch_Oneway_Invocation::remote_oneway instead.
}

TAO_END_VERSIONED_NAMESPACE_DECL
