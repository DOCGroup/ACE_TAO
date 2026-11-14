#include "tao/IORManipulation/IORManip_IIOP_Filter.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/IIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/CDR.h"
#include <cstdlib>
#include <cstring>
#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IORManip_IIOP_Filter::~TAO_IORManip_IIOP_Filter () = default;

TAO_IORManip_IIOP_Filter::TAO_IORManip_IIOP_Filter () = default;

CORBA::Boolean
TAO_IORManip_IIOP_Filter::compare_profile_info (
    TAO_IORManip_IIOP_Filter::Profile_Info const &left,
    TAO_IORManip_IIOP_Filter::Profile_Info const &right)
{
  return (left.version_.major == right.version_.major &&
          left.version_.minor == right.version_.minor &&
          left.port_          == right.port_          &&
          left.host_name_     == right.host_name_);
}

CORBA::Boolean
TAO_IORManip_IIOP_Filter::profile_info_matches (
    TAO_IORManip_IIOP_Filter::Profile_Info const &)
{
  return true;
}

void
TAO_IORManip_IIOP_Filter::filter_and_add (TAO_Profile *profile,
                                          TAO_MProfile &new_profiles,
                                          TAO_Profile *guideline)
{
  TAO_IORManip_IIOP_Filter::Profile_Info ginfo;
  TAO_IORManip_IIOP_Filter::Profile_Info pinfo;
  TAO::IIOPEndpointSequence endpoints;

  this->fill_profile_info (guideline, ginfo);
  this->get_endpoints (profile, endpoints);
  if (endpoints.length () == 0)
    {
      this->fill_profile_info (profile, pinfo);
      CORBA::Boolean const matches = guideline == nullptr
        ? this->profile_info_matches (pinfo)
        : this->compare_profile_info (pinfo, ginfo);

      if (matches)
        {
          if (new_profiles.add_profile (profile) == -1)
            {
              throw CORBA::NO_MEMORY ();
            }
        }
    }
  else
    {
      // Create a new profile with just the
      // components we are looking for
      struct Profile_Deleter
      {
        void operator() (TAO_IIOP_Profile *profile) const
        {
          if (profile != nullptr)
            profile->_decr_refcnt ();
        }
      };
      std::unique_ptr<TAO_IIOP_Profile, Profile_Deleter> new_profile {
        this->create_profile (profile)
      };

      // Set pinfo to match the current profile
      this->fill_profile_info (profile, pinfo);

      // Add each endpoint if it matches.  Begin from the end
      // of the sequence to preserve endpoint order, since <add_endpoint>
      // method reverses the order of endpoints in the list.
      for (CORBA::Long i = endpoints.length () - 1; i >= 0; --i)
        {
          // Set pinfo host name to the current endpoint host name
          pinfo.host_name_ = endpoints[i].host.in ();
          pinfo.port_ = endpoints[i].port;

          CORBA::Boolean const matches = guideline == nullptr
            ? this->profile_info_matches (pinfo)
            : this->compare_profile_info (pinfo, ginfo);

          if (matches)
            {
              // Set the main endpoint on the profile
              if (i == 0)
                {
                  TAO_IIOP_Endpoint * const ep = dynamic_cast<TAO_IIOP_Endpoint *> (
                    new_profile->endpoint ());
                  if (ep == nullptr)
                    {
                      return;
                    }
                  ep->host (endpoints[i].host);
                  ep->port (endpoints[i].port);
                  ep->priority (endpoints[i].priority);
                }
              else
                {
                  std::unique_ptr<TAO_IIOP_Endpoint> endpoint {
                    new (std::nothrow) TAO_IIOP_Endpoint (endpoints[i].host,
                                                          endpoints[i].port,
                                                          endpoints[i].priority)
                  };
                  if (!endpoint)
                    {
                      return;
                    }

                  new_profile->add_endpoint (endpoint.release ());
                }
            }
        }

      if (new_profiles.add_profile (new_profile.get ()) == -1)
        {
          throw CORBA::NO_MEMORY ();
        }

      new_profile->encode_endpoints ();

      // The unique pointer releases the initial reference after MProfile has
      // retained its own reference.
    }
}

void
TAO_IORManip_IIOP_Filter::fill_profile_info (
    TAO_Profile *profile,
    TAO_IORManip_IIOP_Filter::Profile_Info &pinfo)
{
  static constexpr int host_length = 384;
  if (profile != nullptr)
    {
      char host[host_length] {};
      if (profile->endpoint ()->addr_to_string (host, host_length) != -1)
        {
          char * const delim = std::strchr (host, ':');
          if (delim != nullptr)
            {
              *delim = '\0';
              pinfo.port_ = static_cast<CORBA::UShort> (std::atoi (delim + 1));
            }
        }

      pinfo.host_name_ = host;
      pinfo.version_ = profile->version ();
    }
}

void
TAO_IORManip_IIOP_Filter::get_endpoints (TAO_Profile *profile,
                                         TAO::IIOPEndpointSequence &endpoints)
{
  // Reset the endpoints
  endpoints.length (0);

  // Get the endpoints tagged component
  TAO_Tagged_Components const &comps = profile->tagged_components ();
  IOP::TaggedComponent tagged_component {};
  tagged_component.tag = TAO_TAG_ENDPOINTS;

  if (comps.get_component (tagged_component))
    {
      // Prepare the CDR for endpoint extraction
      CORBA::Octet const * const buf =
        tagged_component.component_data.get_buffer ();

      TAO_InputCDR in_cdr (reinterpret_cast<char const *> (buf),
                          tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order {};
      if (!(in_cdr >> ACE_InputCDR::to_boolean (byte_order)))
        return;

      in_cdr.reset_byte_order (static_cast<int> (byte_order));

      // Extract endpoints sequence.
      if (!(in_cdr >> endpoints))
        return;
    }
  else
    {
      return;
    }
}

TAO_IIOP_Profile *
TAO_IORManip_IIOP_Filter::create_profile (TAO_Profile *profile)
{
  ACE_INET_Addr const addr;
  TAO_IIOP_Profile *new_profile {};
  ACE_NEW_THROW_EX (new_profile,
                   TAO_IIOP_Profile (addr,
                                     profile->object_key (),
                                     profile->version (),
                                     profile->orb_core ()),
                   CORBA::NO_MEMORY (
                     CORBA::SystemException::_tao_minor_code (
                       0,
                       ENOMEM),
                     CORBA::COMPLETED_NO));

  struct Profile_Deleter
  {
    void operator() (TAO_IIOP_Profile *profile) const
    {
      if (profile != nullptr)
        profile->_decr_refcnt ();
    }
  };
  std::unique_ptr<TAO_IIOP_Profile, Profile_Deleter> profile_owner {new_profile};

  // Copy all of the tagged components
  TAO_Tagged_Components const &comps = profile->tagged_components ();
  new_profile->tagged_components () = comps;

  // Set the endpoints component to an empty list
  IOP::TaggedComponent tagged_component {};
  tagged_component.tag = TAO_TAG_ENDPOINTS;
  new_profile->tagged_components ().set_component (tagged_component);

  return profile_owner.release ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
