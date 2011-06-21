// -*- C++ -*-
// $Id$

#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter.h"
#include "tao/EndpointPolicy/EndpointPolicyC.h"
#include "tao/EndpointPolicy/Endpoint_Value_Impl.h"

#include "tao/Transport_Acceptor.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Endpoint_Acceptor_Filter::TAO_Endpoint_Acceptor_Filter (const EndpointPolicy::EndpointList & eps)
: endpoints_(eps)
{
}

int
TAO_Endpoint_Acceptor_Filter::fill_profile (const TAO::ObjectKey &object_key,
                                            TAO_MProfile &mprofile,
                                            TAO_Acceptor **acceptors_begin,
                                            TAO_Acceptor **acceptors_end,
                                            CORBA::Short priority)
{
  CORBA::ULong const num_endpoints = endpoints_.length ();

  for (TAO_Acceptor** acceptor = acceptors_begin;
       acceptor != acceptors_end;
       ++acceptor)
    {
      bool tagfound = false;
      for (CORBA::ULong epx = 0; !tagfound && epx < num_endpoints; epx++)
        {
          tagfound = (*acceptor)->tag () == endpoints_[epx]->protocol_tag();
        }
      if (!tagfound)
        continue;

      if ((*acceptor)->create_profile (object_key,
                                       mprofile,
                                       priority) == -1)
        return -1;
    }

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("(%P|%t) EndpointPolicy filtering acceptors")
                ACE_TEXT(" - mprofile has %d profiles,")
                ACE_TEXT(" endpoint list has %d entries\n"),
                mprofile.profile_count(), num_endpoints));

  for (TAO_PHandle pfile_ndx = 0;
       pfile_ndx < mprofile.profile_count ();
       ++pfile_ndx)
    {
      TAO_Profile * const pfile = mprofile.get_profile (pfile_ndx);
      TAO_Endpoint * ep_in_pfile = pfile->base_endpoint ();

      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) Testing profile %d - ")
                    ACE_TEXT("it contains %d endpoints\n"),
                    pfile_ndx, pfile->endpoint_count()));

      bool first_endpoint = true;
      // Iterate the endpoints in the profile.
      while (ep_in_pfile != 0 && pfile->endpoint_count() > 0)
        {
          // Iterate the endpoints in the endpoint policy to see if the endpoint
          // in the profile matches.
          CORBA::ULong ep_ndx = 0;
          bool epmatch = false;
          for (ep_ndx = 0; !epmatch && ep_ndx < num_endpoints; ++ep_ndx)
            {
              if (endpoints_[ep_ndx]->protocol_tag() != pfile->tag())
                continue;

              const EndpointPolicy::EndpointValueBase_ptr evb =
                endpoints_[ep_ndx];

              const TAO_Endpoint_Value_Impl *evi =
                dynamic_cast <const TAO_Endpoint_Value_Impl*>(evb);

              epmatch = evi->is_equivalent(ep_in_pfile);
            }

          // The endpoint in profile does not match the endpoint specified in
          // Endpoint policy, now remove the endpoint from the profile.
          if (!epmatch)
            {
              //Get next endpoint before removing current one.
              TAO_Endpoint * next = ep_in_pfile->next ();
              if (TAO_debug_level > 2)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) EndpointPolicy filter ")
                            ACE_TEXT ("removing endpoint\n")));
              pfile->remove_generic_endpoint (ep_in_pfile);
              ep_in_pfile = first_endpoint ? pfile->endpoint() : next;
            }
          else
            {
              if (TAO_debug_level > 2)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) EndpointPolicy filter ")
                            ACE_TEXT ("Endpoint matched policy value\n")));
              ep_in_pfile = ep_in_pfile->next();
              first_endpoint = false;
            }
        }

      CORBA::ULong const ep_count = pfile->endpoint_count ();

      // Remove the profiles that have no endpoints match the endpoints in
      // endpoint policy.
      if (ep_count == 0
          && mprofile.remove_profile (pfile) != -1)
        {
          --pfile_ndx; // Step back one.  We've just shifted the profile list.

          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("(%P|%t) EndpointPolicy filter ")
                        ACE_TEXT("removing profile\n")));
        }
      else if (ep_count != 0)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("(%P|%t) EndpointPolicy filter ")
                        ACE_TEXT("profile retained with %d endpoints\n"),
                        ep_count));
        }
    }

  if (mprofile.profile_count () == 0) {
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P|%t) EndpointPolicy filter ")
                  ACE_TEXT("eliminated all profiles\n")));

    return -1;
  }

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("(%P|%t) EndpointPolicy filter returning mprofile ")
                ACE_TEXT("with %d profiles\n"),
                mprofile.profile_count()));

  return 0;
}


TAO_END_VERSIONED_NAMESPACE_DECL
