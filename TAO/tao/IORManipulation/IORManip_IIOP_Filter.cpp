// $Id$

#include "tao/IORManipulation/IORManip_IIOP_Filter.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/IIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IORManip_IIOP_Filter::TAO_IORManip_IIOP_Filter (void)
{
}


TAO_IORManip_IIOP_Filter::~TAO_IORManip_IIOP_Filter (void)
{
}


CORBA::Boolean
TAO_IORManip_IIOP_Filter::compare_profile_info (
                        const TAO_IORManip_IIOP_Filter::Profile_Info& left,
                        const TAO_IORManip_IIOP_Filter::Profile_Info& right)
{
  return (left.version_.major == right.version_.major &&
          left.version_.minor == right.version_.minor &&
          left.port_          == right.port_          &&
          left.host_name_     == right.host_name_);
}


CORBA::Boolean
TAO_IORManip_IIOP_Filter::profile_info_matches (
                            const TAO_IORManip_IIOP_Filter::Profile_Info&)
{
  return true;
}


void
TAO_IORManip_IIOP_Filter::filter_and_add (TAO_Profile* profile,
                                          TAO_MProfile& new_profiles,
                                          TAO_Profile* guideline)
{
  TAO_IORManip_IIOP_Filter::Profile_Info ginfo;
  TAO_IORManip_IIOP_Filter::Profile_Info pinfo;
  TAO::IIOPEndpointSequence endpoints;

  this->fill_profile_info (guideline, ginfo);
  this->get_endpoints (profile, endpoints);
  if (endpoints.length () == 0)
    {
      CORBA::Boolean matches = false;
      this->fill_profile_info (profile, pinfo);

      if (guideline == 0)
        {
          matches = this->profile_info_matches (pinfo);
        }
      else
        {
          // Compare the current profile with the guideline profile
          matches = this->compare_profile_info (pinfo, ginfo);
        }

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
      TAO_IIOP_Profile* new_profile =
          this->create_profile (profile);

      // Set pinfo to match the current profile
      this->fill_profile_info (profile, pinfo);

      // Add each endpoint if it matches.  Begin from the end
      // of the sequence to preserve endpoint order, since <add_endpoint>
      // method reverses the order of endpoints in the list.
      for (CORBA::Long i = endpoints.length () - 1; i >= 0; i--) {
        // Set pinfo host name to the current endpoint host name
        pinfo.host_name_ = endpoints[i].host.in ();
        pinfo.port_ = endpoints[i].port;

        CORBA::Boolean matches = false;
        if (guideline == 0)
          {
            matches = this->profile_info_matches (pinfo);
          }
        else
          {
            // Compare the current profile with the guideline profile
            matches = this->compare_profile_info (pinfo, ginfo);
          }

        if (matches)
          {
            // Set the main endpoint on the profile
            if (i == 0)
              {
                TAO_IIOP_Endpoint* ep = dynamic_cast<TAO_IIOP_Endpoint*> (
                                          new_profile->endpoint ());
                if (ep == 0)
                  {
                    new_profile->_decr_refcnt ();
                    return;
                  }
                else
                  {
                    ep->host (CORBA::string_dup (endpoints[i].host));
                    ep->port (endpoints[i].port);
                    ep->priority (endpoints[i].priority);
                  }
              }
            else
              {
                TAO_IIOP_Endpoint *endpoint = 0;
                ACE_NEW (endpoint,
                         TAO_IIOP_Endpoint (endpoints[i].host,
                                            endpoints[i].port,
                                            endpoints[i].priority));
                if (endpoint == 0)
                  {
                    new_profile->_decr_refcnt ();
                    return;
                  }

                new_profile->add_endpoint (endpoint);
              }
          }
      }

      if (new_profiles.add_profile (new_profile) == -1)
        {
          throw CORBA::NO_MEMORY ();
        }

      new_profile->encode_endpoints ();

      // After adding the profile to the MProfile,
      // we must decrement the reference to avoid a memory leak
      new_profile->_decr_refcnt ();
    }
}


int
TAO_IORManip_IIOP_Filter::fill_profile_info (
                             TAO_Profile* profile,
                             TAO_IORManip_IIOP_Filter::Profile_Info& pinfo)
{
  static const int host_length = 384;
  int status = 0;
  if (profile != 0)
    {
      char host[host_length] = "";
      if (profile->endpoint ()->addr_to_string (host, host_length) != -1)
        {
          char* delim = ACE_OS::strchr (host, ':');
          if (delim != 0)
            {
              *delim = '\0';
              pinfo.port_ = ACE_OS::atoi (delim + 1);
              status = 1;
            }
        }

      pinfo.host_name_ = host;
      pinfo.version_ = profile->version ();
    }

  return status;
}


int
TAO_IORManip_IIOP_Filter::get_endpoints (TAO_Profile* profile,
                                         TAO::IIOPEndpointSequence& endpoints)
{
  // Reset the endpoints
  endpoints.length (0);

  // Get the endpoints tagged component
  const TAO_Tagged_Components& comps = profile->tagged_components ();
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;
  comps.get_component (tagged_component);

  // Prepare the CDR for endpoint extraction
  const CORBA::Octet *buf =
    tagged_component.component_data.get_buffer ();

  TAO_InputCDR in_cdr (reinterpret_cast<const char*> (buf),
                       tagged_component.component_data.length ());

  // Extract the Byte Order.
  CORBA::Boolean byte_order;
  if (!(in_cdr >> ACE_InputCDR::to_boolean (byte_order)))
    return 0;

  in_cdr.reset_byte_order (static_cast<int> (byte_order));

  // Extract endpoints sequence.
  if (!(in_cdr >> endpoints))
    return 0;

  return 1;
}


TAO_IIOP_Profile*
TAO_IORManip_IIOP_Filter::create_profile (TAO_Profile* profile)
{
  ACE_INET_Addr addr;
  TAO_IIOP_Profile* new_profile = 0;
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

  // Copy all of the tagged components
  const TAO_Tagged_Components& comps = profile->tagged_components ();
  new_profile->tagged_components () = comps;

  // Set the endpoints component to an empty list
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;
  new_profile->tagged_components ().set_component (tagged_component);

  return new_profile;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
