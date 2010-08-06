// -*- C++ -*-

#include "orbsvcs/FaultTolerance/FT_Invocation_Endpoint_Selectors.h"

#include "tao/Profile_Transport_Resolver.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "tao/Base_Transport_Property.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

ACE_RCSID (FaultTolerance,
           FT_Invocation_Endpoint_Selectors,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FT_Invocation_Endpoint_Selector::TAO_FT_Invocation_Endpoint_Selector (void)
  : TAO_Default_Endpoint_Selector ()
{
}

TAO_FT_Invocation_Endpoint_Selector::~TAO_FT_Invocation_Endpoint_Selector (void)
{
}

void
TAO_FT_Invocation_Endpoint_Selector::select_endpoint (
    TAO::Profile_Transport_Resolver *r,
    ACE_Time_Value *val)
{
  bool retval =
    this->select_primary (r,
                          val);

  if (retval)
    return;

  retval =
    this->select_secondary (r,
                            val);

  if (retval == false)
    {
      // If we get here, we completely failed to find an endpoint selector
      // that we know how to use, so throw an exception.
      throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }

  return;
}

bool
TAO_FT_Invocation_Endpoint_Selector::select_primary (
    TAO::Profile_Transport_Resolver *r,
    ACE_Time_Value *max_wait_time)
{
  // Grab the forwarded list
  TAO_MProfile *prof_list =
    const_cast<TAO_MProfile *> (r->stub ()->forward_profiles ());

  TAO_MProfile &basep = r->stub ()->base_profiles ();

  if (prof_list ==0)
    prof_list = &basep;

  if (prof_list == 0)
    return false;

  // Did not succeed. Try to look for primaries all over the place
  CORBA::ULong sz =
    prof_list->size ();

  // Iterate through the list in a circular fashion. Stop one before
  // the list instead of trying the same thing again.
  for (CORBA::ULong i = 0;
       i != sz;
       ++i)
    {
      TAO_Profile *tmp = prof_list->get_profile (i);

      bool retval =
        this->check_profile_for_primary (tmp);

      // Choose a non-primary
      if (retval == true && tmp != 0)
        {
          retval =
            this->try_connect (r,
                               tmp,
                               max_wait_time);

          if (retval == true)
            return true;
        }
    }

  return false;
}

bool
TAO_FT_Invocation_Endpoint_Selector::select_secondary (
    TAO::Profile_Transport_Resolver *r,
    ACE_Time_Value *max_wait_time)
{
  // Grab the forwarded list
  TAO_MProfile *prof_list =
    const_cast<TAO_MProfile *> (r->stub ()->forward_profiles ());

  TAO_MProfile &basep =
    r->stub ()->base_profiles ();

  if (prof_list ==0)
    prof_list = &basep;

  if (prof_list == 0)
    return false;

  CORBA::ULong sz =
    prof_list->size ();

  for (CORBA::ULong i = 0;
       i != sz;
       ++i)
    {
      TAO_Profile *tmp =
        prof_list->get_profile (i);

      bool retval =
        this->check_profile_for_primary (tmp);

      // Choose a non-primary
      if (retval == false && tmp != 0)
        {
          retval =
            this->try_connect (r,
                               tmp,
                               max_wait_time);

          if (retval == true)
            return true;
        }
    }

  return false;
}

bool
TAO_FT_Invocation_Endpoint_Selector::try_connect (
    TAO::Profile_Transport_Resolver *r,
    TAO_Profile *profile,
    ACE_Time_Value *max_wait_time)
{
  r->profile (profile);

  size_t endpoint_count =
    r->profile ()->endpoint_count ();

  TAO_Endpoint *ep =
    r->profile ()->endpoint ();

  for (size_t i = 0; i < endpoint_count; ++i)
    {
      TAO_Base_Transport_Property desc (ep);

      bool retval =
        r->try_connect (&desc,
                        max_wait_time);

      // @@ Good place to handle timeouts.. We can omit timeouts and
      // go ahead looking for other things... There are some small
      // issues that needs ironed out first in the invocation
      // classes.

      // Check if the connect has completed.
      if (retval)
        return true;

      // Go to the next endpoint in this profile.
      ep = ep->next ();
    }

  return false;
}

bool
TAO_FT_Invocation_Endpoint_Selector::check_profile_for_primary (
    TAO_Profile *pfile)
{
  if (pfile == 0)
    return false;

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_FT_PRIMARY;

  // Get the tagged component from the  profile
  TAO_Tagged_Components &pfile_tagged =
    pfile->tagged_components ();

  // Search for the TaggedComponent that we want
  if (pfile_tagged.get_component (tagged_component) != 1)
    return false;

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_FT (%P|%t) - Got a primary component\n")));
    }

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
