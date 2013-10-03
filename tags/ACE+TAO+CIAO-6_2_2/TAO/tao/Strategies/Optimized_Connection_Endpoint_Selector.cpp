// -*- C++ -*-
// $Id$

#include "tao/Strategies/Optimized_Connection_Endpoint_Selector.h"

#include "tao/debug.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "tao/Base_Transport_Property.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_Time_Value TAO_Optimized_Connection_Endpoint_Selector::timeout_;

TAO_Optimized_Connection_Endpoint_Selector::
TAO_Optimized_Connection_Endpoint_Selector (const ACE_Time_Value &tv)
{
  TAO_Optimized_Connection_Endpoint_Selector::timeout_ = tv;
  if (TAO_debug_level)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO(%P|%t) Optimized Connection Enpoint Selector: ")
                  ACE_TEXT ("Initializing timeout hook tv = %d sec, %d usec\n"),
              tv.sec(), tv.usec()));
    }
  if (tv > ACE_Time_Value::zero)
    {
      TAO_ORB_Core::connection_timeout_hook
        (TAO_Optimized_Connection_Endpoint_Selector::hook);
    }
}

TAO_Optimized_Connection_Endpoint_Selector::
~TAO_Optimized_Connection_Endpoint_Selector (void)
{
}


void
TAO_Optimized_Connection_Endpoint_Selector::hook (TAO_ORB_Core *,
                                                  TAO_Stub *,
                                                  bool &has_timeout,
                                                  ACE_Time_Value &tv)
{
  has_timeout =
    TAO_Optimized_Connection_Endpoint_Selector::
    timeout_ > ACE_Time_Value::zero;
  if (has_timeout)
    tv = TAO_Optimized_Connection_Endpoint_Selector::timeout_;
}

int
TAO_Optimized_Connection_Endpoint_Selector::check_profile (TAO_Profile *p,
                                                           TAO::Profile_Transport_Resolver *r)
{
  TAO_Endpoint *effective_endpoint = 0;

  r->profile(p);
  effective_endpoint = p->endpoint ();
  size_t endpoint_count = p->endpoint_count();
  for (size_t i = 0; i < endpoint_count; ++i)
    {
      TAO_Base_Transport_Property desc (effective_endpoint);
      if (r->find_transport(&desc))
        return 1;
      // Go to the next endpoint in this profile
      effective_endpoint = effective_endpoint->next();
    }
  return 0;
}

void
TAO_Optimized_Connection_Endpoint_Selector::select_endpoint
  (TAO::Profile_Transport_Resolver *r,
   ACE_Time_Value *max_wait_time)
{
  TAO_Stub *stub = r->stub();
  TAO_Profile *p = stub->profile_in_use();

  // first, look for the endpoints for the current profile in use.
  // if that is available then go for it.

  if (this->check_profile (p, r) != 0)
    return;

  // next, look for any other profiles. If the stub has any forward profiles,
  // use those, otherwise look at the base profiles. This separation is
  // necessary to avoid re-using a corbaloc or other previously forwarded
  // profile.

  const TAO_MProfile *profiles = stub->forward_profiles();
  if (profiles != 0)
    {
      for (CORBA::ULong count = 0; count <  profiles->profile_count(); count++)
        {
          p = const_cast<TAO_Profile *>(profiles->get_profile(count));
          if (this->check_profile (p, r) != 0)
            {
              if (stub->profile_in_use() != p)
                {
                  // thread-safe way to coerse stub to this profile.
                  stub->reset_profiles();
                  while (stub->profile_in_use() != p)
                    if (stub->next_profile_retry() == 0)
                      break;
                }
              return;
            }
        }
    }
  else
    {
      do
        {
          p = stub->profile_in_use();
          if (this->check_profile(p, r) != 0)
            return;
        }
      while (stub->next_profile_retry () != 0);
    }



  // at this point, we do not have an existing transport, so we must
  // reset the profile list and try establishing connections via the
  // connector(s).

  do
    {
      r->profile (r->stub ()->profile_in_use ());

      // Check whether we need to do a blocked wait or we have a
      // non-blocked wait and we support that.  If this is not the
      // case we can't use this profile so try the next.
      if (r->blocked_connect () ||
         (!r->blocked_connect () && r->profile ()->supports_non_blocking_oneways ()))
        {
          const size_t endpoint_count = r->profile ()->endpoint_count ();

          TAO_Endpoint *ep = r->profile ()->endpoint ();

          for (size_t i = 0; i < endpoint_count; ++i)
            {
              TAO_Base_Transport_Property desc (ep);
              const bool retval = r->try_connect (&desc, max_wait_time);

              // Check if the connect has completed.
              if (retval)
                return;

              // Go to the next endpoint in this profile.
              ep = ep->next ();
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
