// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "SSLIOP_Profile.h"
#include "ssl_endpoints.h"
#include "tao/CDR.h"
#include "tao/Environment.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Profile,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "SSLIOP_Profile.i"
#endif /* __ACE_INLINE__ */

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (const ACE_INET_Addr &addr,
                                        const TAO_ObjectKey &object_key,
                                        const TAO_GIOP_Message_Version &version,
                                        TAO_ORB_Core *orb_core,
                                        const SSLIOP::SSL *ssl_component)
  : TAO_IIOP_Profile (addr,
                      object_key,
                      version,
                      orb_core),
  ssl_endpoint_ (ssl_component, 0)
{
  this->ssl_endpoint_.iiop_endpoint (&this->endpoint_, 1);
}

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (const char* host,
                                        CORBA::UShort port,
                                        const TAO_ObjectKey &object_key,
                                        const ACE_INET_Addr &addr,
                                        const TAO_GIOP_Message_Version &version,
                                        TAO_ORB_Core *orb_core,
                                        const SSLIOP::SSL *ssl_component)
  : TAO_IIOP_Profile (host,
                      port,
                      object_key,
                      addr,
                      version,
                      orb_core),
  ssl_endpoint_ (ssl_component, 0)
{
  this->ssl_endpoint_.iiop_endpoint (&this->endpoint_, 1);
}

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (TAO_ORB_Core *orb_core,
                                        const SSLIOP::SSL *ssl_component)
  : TAO_IIOP_Profile (orb_core),
    ssl_endpoint_ (ssl_component, 0)
{
  this->ssl_endpoint_.iiop_endpoint (&this->endpoint_, 1);
}

TAO_SSLIOP_Profile::TAO_SSLIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_IIOP_Profile (orb_core),
    ssl_endpoint_ (0, 0)
{
  this->ssl_endpoint_.iiop_endpoint (&this->endpoint_, 1);
}

TAO_SSLIOP_Profile::~TAO_SSLIOP_Profile (void)
{
  // Clean up the list of endpoints since we own it.
  // Skip the head, since it is not dynamically allocated.
  TAO_Endpoint *tmp = 0;

  for (TAO_Endpoint *next = this->ssl_endpoint_.next ();
       next != 0;
       next = tmp)
    {
      tmp = next->next ();
      delete next;
    }
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_SSLIOP_Profile::decode (TAO_InputCDR& cdr)
{
  int r = this->TAO_IIOP_Profile::decode (cdr);
  if (r != 1)
    return r;

  // Attempt to decode SSLIOP::SSL tagged component.  It may not be
  // there if we are dealing with pure IIOP profile.
  int ssl_component_found = 0;
  IOP::TaggedComponent component;
  component.tag = SSLIOP::TAG_SSL_SEC_TRANS;

  if (this->tagged_components ().get_component (component))
    {
      TAO_InputCDR cdr (ACE_reinterpret_cast (
                          const char*,
                          component.component_data.get_buffer ()),
                        component.component_data.length ());
      CORBA::Boolean byte_order;
      if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return -1;
      cdr.reset_byte_order (ACE_static_cast(int, byte_order));

      cdr >> this->ssl_endpoint_.ssl_component_;

      ssl_component_found = 1;
    }

  // Since IIOP portion of the profile has already been decoded, we
  // know how many endpoints it should contain and can finish
  // initialization accordingly.
  if (this->count_ < 2)
    {
      // This profile contains only one endpoint.  Finish initializing
      // it.
      this->ssl_endpoint_.iiop_endpoint (&this->endpoint_, 1);
      this->ssl_endpoint_.priority (this->endpoint_.priority ());
      return 1;
    }
  else
    {
      // This profile contains more than one endpoint.
      if (ssl_component_found)
        {
          // It is true ssl profile, i.e., not just IIOP, so must have
          // ssl endpoints encoded.

          if (this->decode_endpoints () == -1)
            return -1;

          return 1;
        }
      else
        {
          // IIOP profile - doesn't have ssl endpoints encoded.  We
          // must create 'dummy' ssl endpoint list anyways, in order to
          // make iiop endpoints accessable and usable.
          for (size_t i = 0;
               i < this->count_;
               ++i)
            {
              TAO_SSLIOP_Endpoint *endpoint = 0;
              ACE_NEW_RETURN (endpoint,
                              TAO_SSLIOP_Endpoint (0, 0),
                              -1);
              this->add_endpoint (endpoint);
            }

          // Now that we have a complete list of ssl endpoins, we can
          // connect them with their iiop counterparts.
          TAO_IIOP_Endpoint *iiop_endp = &this->endpoint_;

          for (TAO_SSLIOP_Endpoint * ssl_endp = &this->ssl_endpoint_;
               ssl_endp != 0;
               ssl_endp = ssl_endp->next_)
            {
              ssl_endp->iiop_endpoint (iiop_endp, 1);
              ssl_endp->priority (iiop_endp->priority ());
              iiop_endp = iiop_endp->next_;
            }

          return 1;
        }
    }
}

CORBA::Boolean
TAO_SSLIOP_Profile::is_equivalent (const TAO_Profile *other_profile)
{
  const TAO_SSLIOP_Profile *op =
    ACE_dynamic_cast (const TAO_SSLIOP_Profile *, other_profile);


  // Check endpoints equivalence.
  const TAO_SSLIOP_Endpoint *other_endp = &op->ssl_endpoint_;
  for (TAO_SSLIOP_Endpoint *endp = &this->ssl_endpoint_;
       endp != 0;
       endp = endp->next_)
    {
      if (endp->is_equivalent (other_endp))
        other_endp = other_endp->next_;
      else
        return 0;
    }

  return 1;
}

TAO_Endpoint*
TAO_SSLIOP_Profile::endpoint (void)
{
  return &this->ssl_endpoint_;
}

void
TAO_SSLIOP_Profile::add_endpoint (TAO_SSLIOP_Endpoint *endp)
{
  endp->next_ = this->ssl_endpoint_.next_;
  this->ssl_endpoint_.next_ = endp;

  // We do not want to add our iiop endpoint counterpart when we are
  // decoding a profile, and iiop endpoints have been added before we
  // even get to SSLIOP-specific decoding.
  if (endp->iiop_endpoint () != 0)
    this->TAO_IIOP_Profile::add_endpoint (endp->iiop_endpoint ());
}

int
TAO_SSLIOP_Profile::encode_endpoints (void)
{
  // If we have more than one endpoint, we encode info about others
  // into a tagged component for wire transfer.
  if (this->count_ > 1)
    {
      // Encode all endpoints except the first one, since it is always
      // transferred through standard profile component.

      // Create a data structure and fill it with endpoint info for wire
      // transfer.
      TAO_SSLEndpointSequence endpoints;
      endpoints.length (this->count_ - 1);

      const TAO_SSLIOP_Endpoint *endpoint = this->ssl_endpoint_.next_;
      for (size_t i = 0;
           i < this->count_ - 1;
           ++i)
        {
          endpoints[i] = endpoint->ssl_component_;
          endpoint = endpoint->next_;
        }

      // Encode the data structure.
      TAO_OutputCDR out_cdr;
      if ((out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
           == 0)
          || (out_cdr << endpoints) == 0)
        return -1;
      CORBA::ULong length = out_cdr.total_length ();

      IOP::TaggedComponent tagged_component;
      tagged_component.tag = TAO_TAG_SSL_ENDPOINTS;
      tagged_component.component_data.length (length);
      CORBA::Octet *buf =
        tagged_component.component_data.get_buffer ();

      for (const ACE_Message_Block *iterator = out_cdr.begin ();
           iterator != 0;
           iterator = iterator->cont ())
        {
          CORBA::ULong i_length = iterator->length ();
          ACE_OS::memcpy (buf, iterator->rd_ptr (), i_length);

          buf += i_length;
        }

      // Add component with encoded endpoint data to this profile's
      // TaggedComponents.
      tagged_components_.set_component (tagged_component);
    }

  return this->TAO_IIOP_Profile::encode_endpoints ();
}

int
TAO_SSLIOP_Profile::decode_endpoints (void)
{
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_SSL_ENDPOINTS;

  if (this->tagged_components_.get_component (tagged_component))
    {
      const CORBA::Octet *buf =
        tagged_component.component_data.get_buffer ();

      TAO_InputCDR in_cdr (ACE_reinterpret_cast (const char*, buf),
                           tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return -1;
      in_cdr.reset_byte_order (ACE_static_cast(int, byte_order));

      // Extract endpoints sequence.
      TAO_SSLEndpointSequence endpoints;
      if ((in_cdr >> endpoints) == 0)
        return -1;

      // Use information extracted from the tagged component to
      // populate the profile.  Begin from the end of the sequence to
      // preserve endpoint order, since <add_endpoint> method reverses
      // the order of endpoints in the list.
      for (CORBA::ULong i = endpoints.length () - 1;
           (i + 1) != 0;
           --i)
        {
          TAO_SSLIOP_Endpoint *endpoint = 0;
          ACE_NEW_RETURN (endpoint,
                          TAO_SSLIOP_Endpoint (0, 0),
                          -1);
          endpoint->ssl_component_ = endpoints[i];
          this->add_endpoint (endpoint);
        }

      // Now that we have a complete list of ssl endpoins, we can
      // connect them with their iiop counterparts, which have been
      // extracted/chained during the IIOP profile decoding.
      TAO_IIOP_Endpoint *iiop_endp =
        &this->endpoint_;

      for (TAO_SSLIOP_Endpoint * ssl_endp = &this->ssl_endpoint_;
           ssl_endp != 0;
           ssl_endp = ssl_endp->next_)
        {
          ssl_endp->iiop_endpoint (iiop_endp, 1);
          ssl_endp->priority (iiop_endp->priority ());
          iiop_endp = iiop_endp->next_;
        }

      return 0;
    }

  // Since this method is only called if we are expecting
  // TAO_TAG_SSL_ENDPOINTS component, failure to find it is an error.
  return -1;
}

void
TAO_SSLIOP_Profile::parse_string (const char *ior
                                  ACE_ENV_ARG_DECL)
{
   TAO_IIOP_Profile::parse_string (ior
                                   ACE_ENV_ARG_PARAMETER);

    this->ssl_endpoint_.iiop_endpoint(&this->endpoint_, 1);
}
