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

ACE_RCSID (tao,
           Invocation_Endpoint_Selectors,
           "$Id$")


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
                                                ACE_Time_Value *max_wait_time
                                                ACE_ENV_ARG_DECL)
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

              TAO_Endpoint *ep =
                r->profile ()->endpoint ();

              TAO_Base_Transport_Property desc (ep);
              bool success =
                r->try_parallel_connect (&desc,
                                         max_wait_time
                                         ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              // Check if the connect has completed.
              if (success)
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
              bool retval =
                r->try_connect (&desc,
                                max_wait_time
                                ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              // Check if the connect has completed.
              if (retval)
                return;
            }

        }
    }
  while (r->stub ()->next_profile_retry () != 0);


  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (CORBA::OMGVMCID | 2,
                               CORBA::COMPLETED_NO));
}

TAO_END_VERSIONED_NAMESPACE_DECL
