// -*- C++ -*-

#include "tao/Invocation_Endpoint_Selectors.h"

#if !defined (__ACE_INLINE__)
#include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "tao/Invocation.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "tao/Base_Transport_Property.h"

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
  TAO_GIOP_Invocation *invocation
  ACE_ENV_ARG_DECL)
{
  do
    {
      invocation->profile (invocation->stub ()->profile_in_use ());
      invocation->endpoint (invocation->profile ()->endpoint ());

      int status =
        this->endpoint_from_profile (invocation ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (status == 1)
        return;
    }
  while (invocation->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW (CORBA::TRANSIENT (CORBA::OMGVMCID | 2,
                               CORBA::COMPLETED_NO));
}

void
TAO_Default_Endpoint_Selector::forward (TAO_GIOP_Invocation *invocation,
                                        const TAO_MProfile &mprofile
                                        ACE_ENV_ARG_DECL)
{
  invocation->stub ()->add_forward_profiles (mprofile);
  // This has to be and is thread safe.
  // TAO_Stub::add_forward_profiles() already makes a copy of the
  // MProfile, so do not make a copy here.


  // We may not need to do this since TAO_GIOP_Invocations
  // get created on a per-call basis. For now we'll play it safe.
  if (invocation->stub ()->next_profile () == 0)
    ACE_THROW (CORBA::TRANSIENT (
      CORBA::SystemException::_tao_minor_code (
        TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
        errno),
      CORBA::COMPLETED_NO));
}

void
TAO_Default_Endpoint_Selector::success (TAO_GIOP_Invocation *invocation)
{
  invocation->stub ()->set_valid_profile ();
}

void
TAO_Default_Endpoint_Selector::close_connection (TAO_GIOP_Invocation *invocation)
{
  // Get rid of any forwarding profiles and reset
  // the profile list to point to the first profile!
  // FRED For now we will not deal with recursive forwards!
  invocation->stub ()->reset_profiles ();
}


int
TAO_Default_Endpoint_Selector::endpoint_from_profile (
    TAO_GIOP_Invocation *invocation
    ACE_ENV_ARG_DECL)
{
  size_t endpoint_count =
    invocation->profile ()->endpoint_count();

  for (size_t i = 0; i < endpoint_count; ++i)
    {
      // If known endpoint, select it.
      if (invocation->endpoint () != 0)
        {
          TAO_Base_Transport_Property desc (invocation->endpoint ());

          int status =
            invocation->perform_call (desc ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          // Check if the invocation has completed.
          if (status == 1)
            return 1;

          // Go to the next endpoint in this profile.
          invocation->endpoint (invocation->endpoint()->next());
        }
    }

  return 0;
}
