// @(#) $Id$

#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter.h"
#include "tao/EndpointPolicy/EndpointPolicyC.h"
#include "tao/EndpointPolicy/Endpoint_Value_Impl.h"

#include "tao/Transport_Acceptor.h"
#include "tao/MProfile.h"
#include "tao/IIOP_Profile.h"

ACE_RCSID(EndpointPolicy,
          Endpoint_Acceptor_Filter,
          "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Endpoint_Acceptor_Filter::TAO_Endpoint_Acceptor_Filter (EndpointPolicy::Policy_ptr p)
: endpoints_(p->value())
{
}

int
TAO_Endpoint_Acceptor_Filter::fill_profile (const TAO::ObjectKey &object_key,
                                            TAO_MProfile &mprofile,
                                            TAO_Acceptor **acceptors_begin,
                                            TAO_Acceptor **acceptors_end,
                                            CORBA::Short priority)
{
  CORBA::ULong num_endpoints = endpoints_->length ();

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("(%P|%t) EndpointPolicy filtering acceptors")
                ACE_TEXT(" - mprofile has %d profiles,")
                ACE_TEXT(" endpoint list has %d entries\n"),
                mprofile.profile_count(), num_endpoints));

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

  for (TAO_PHandle i = 0; i < mprofile.profile_count (); ++i)
    {
      TAO_Profile *pfile =mprofile.get_profile (i);

      TAO_Endpoint *ep_in_pfile = pfile->endpoint ();

      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) Testing profile %d - ")
                    ACE_TEXT("it contains %d endpoints\n"),
                    i, pfile->endpoint_count()));

      bool first_endpoint = true;
      // Iterate the endpoints in the profile.
      while (ep_in_pfile != 0 && pfile->endpoint_count() > 0)
        {
          // Iterate the endpoints in the endpoint policy to see if the endpoint
          // in the profile matches.
          CORBA::ULong j = 0;
          bool epmatch = false;
          for (j = 0; !epmatch && j < num_endpoints; ++j)
            {
              if (endpoints_[j]->protocol_tag() != pfile->tag())
                continue;

              const EndpointPolicy::EndpointValueBase *evb = endpoints_[i];

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
              ep_in_pfile = ep_in_pfile->next();
              first_endpoint = false;
            }
        }
      // Remove the profiles that have no endpoints match the endpoints in
      // endpoint policy.
      if (pfile->endpoint_count () == 0)
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("(%P|%t) EndpointPolicy filter removing profile\n")));
          mprofile.remove_profile (pfile);
          --i; // step back one, since we've just shifted the profile list.
        }
    }

  if (mprofile.profile_count () == 0) {
    if (TAO_debug_level > 2)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P|%t) EndpointPolicy filter eliminated all profiles\n")));

    return -1;
  }
  return 0;
}


TAO_END_VERSIONED_NAMESPACE_DECL
