// -*- C++ -*-
//$Id$
#include "tao/Invocation_Endpoint_Selectors.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/ORB_Core.h"
#include "tao/IIOP_Endpoint.h"
#include "Base_Transport_Property.h"
#include "Profile_Transport_Resolver.h"
#include "ORB_Constants.h"
#include "SystemException.h"

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
#if defined (ACE_HAS_IPV6)
  // See if we have IIOP selector preference parameters set.
  // If so we need to walk a different path in selecting an endpoint.
  if (r->stub ()->orb_core ()->orb_params ()->prefer_ipv6_interfaces () ||
      r->stub ()->orb_core ()->orb_params ()->connect_ipv6_only ())
    {
      bool test_ipv6 = true;  // first round try IPv6

      for (unsigned test_count = 2; test_count > 0 ;--test_count)
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
                  const size_t endpoint_count =
                    r->profile ()->endpoint_count ();

                  TAO_Endpoint *ep =
                    r->profile ()->endpoint ();

                  for (size_t i = 0; i < endpoint_count; ++i)
                    {
                      // in case we're running the first round:
                      //   try this endpoint if it either *not* an IIOP endpoint OR
                      //   if it is an IIOP IPv6 endpoint.
                      // else
                      //   only try when it's an IPv4 IIOP endpoint
                      bool try_ep = test_ipv6;

                      if (r->profile ()->tag () == IOP::TAG_INTERNET_IOP)
                        {
                          TAO_IIOP_Endpoint *iep =
                            dynamic_cast<TAO_IIOP_Endpoint *> (ep);
                          if (!iep->is_ipv6_decimal ())
                            {
                              ACE_INET_Addr addr(iep->port (),
                                                 iep->host ());
                              if (test_ipv6)
                                try_ep =
                                  addr.get_type () == AF_INET6 &&
                                    !addr.is_ipv4_mapped_ipv6();
                              else
                                try_ep =
                                  addr.get_type () == AF_INET ||
                                    addr.is_ipv4_mapped_ipv6();
                            }
                        }

                      if (try_ep)
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
                        }

                      // Go to the next endpoint in this profile.
                      ep = ep->next ();
                    }
                }
            }
          while (r->stub ()->next_profile_retry () != 0);

          // If we get here we did not find any suitable non-IPv4 endpoint so
          // now try those if allowed
          if (!r->stub ()->orb_core ()->orb_params ()->connect_ipv6_only ())
            test_ipv6 = false;
          else
            break;  // Do not test remaining (if any) IPv4 endpoints
        }
    }
  else
#endif /* ACE_HAS_IPV6 */
  do
    {
      r->profile (r->stub ()->profile_in_use ());

      // Check whether we need to do a blocked wait or we have a
      // non-blocked wait and we support that.  If this is not the
      // case we can't use this profile so try the next.
      if (r->blocked_connect () ||
         (!r->blocked_connect () && r->profile ()->supports_non_blocking_oneways ()))
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
