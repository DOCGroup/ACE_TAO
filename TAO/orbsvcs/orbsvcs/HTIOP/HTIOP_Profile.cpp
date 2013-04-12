// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/HTIOP/HTIOP_Profile.h"
#include "orbsvcs/HTIOP/htiop_endpointsC.h"

#include "ace/os_include/os_netdb.h"

#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

static const char the_prefix[] = "htiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char TAO::HTIOP::Profile::object_key_delimiter_ = '/';

char
TAO::HTIOP::Profile::object_key_delimiter (void) const
{
  return TAO::HTIOP::Profile::object_key_delimiter_;
}

TAO::HTIOP::Profile::Profile (const ACE::HTBP::Addr &addr,
                                      const TAO::ObjectKey &object_key,
                                      const TAO_GIOP_Message_Version &version,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (OCI_TAG_HTIOP_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (addr,
               orb_core->orb_params ()->use_dotted_decimal_addresses ()),
    count_ (1)
{
}

TAO::HTIOP::Profile::Profile (const char* host,
                                      CORBA::UShort port,
                                      const char* htid,
                                      const TAO::ObjectKey &object_key,
                                      const ACE::HTBP::Addr &addr,
                                      const TAO_GIOP_Message_Version &version,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (OCI_TAG_HTIOP_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (host, port, htid, addr),
    count_ (1)
{
}

TAO::HTIOP::Profile::Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (OCI_TAG_HTIOP_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1)
{
}

TAO::HTIOP::Profile::~Profile (void)
{
  // Clean up the list of endpoints since we own it.
  // Skip the head, since it is not dynamically allocated.
  TAO_Endpoint *tmp = 0;

  for (TAO_Endpoint *next = this->endpoint ()->next ();
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
TAO::HTIOP::Profile::decode_profile (TAO_InputCDR& cdr)
{

  // Decode host and port into the <endpoint_>.
  if (cdr.read_string (this->endpoint_.host_.out ()) == 0
      || cdr.read_ushort (this->endpoint_.port_) == 0
      || cdr.read_string (this->endpoint_.htid_.out()) == 0)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) TAO::HTIOP::Profile::decode - ")
                    ACE_TEXT ("error while decoding host/port")));
      return -1;
    }

  if (cdr.good_bit ())
    {
      // Invalidate the object_addr_ until first access.
      this->endpoint_.object_addr_.set_type (-1);

      return 1;
    }
  return -1;
}

void
TAO::HTIOP::Profile::parse_string_i (const char *ior)
{
  // Pull off the "hostname:port#token/" part of the objref
  // Copy the string because we are going to modify it...

  const char *okd = ACE_OS::strchr (ior, this->object_key_delimiter_);

  if (okd == 0 || okd == ior)
    {
      // No object key delimiter or no hostname specified.
      throw CORBA::INV_OBJREF(
        CORBA::SystemException::_tao_minor_code(
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  // Length of host string.
  CORBA::ULong length_host = 0;

  const char *cp_pos = ACE_OS::strchr (ior, ':');  // Look for a port

  if (cp_pos == ior)
    {
      // No hostname specified!  It is required by the spec.
      throw CORBA::INV_OBJREF(
        CORBA::SystemException::_tao_minor_code(
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }
  else if (cp_pos != 0)
    {
      // A port number or port name was specified.
      CORBA::ULong length_port = okd - cp_pos - 1;

      CORBA::String_var tmp = CORBA::string_alloc (length_port);

      ACE_OS::strncpy (tmp.inout (), cp_pos + 1, length_port);
      tmp[length_port] = '\0';

      this->endpoint_.port_ =
        static_cast<CORBA::UShort> (ACE_OS::atoi (tmp.in ()));

      length_host = cp_pos - ior;
    }
  else
    length_host = okd - ior;

  CORBA::String_var tmp = CORBA::string_alloc (length_host);

  // Skip the trailing '/'
  ACE_OS::strncpy (tmp.inout (), ior, length_host);
  tmp[length_host] = '\0';

  this->endpoint_.host_ = tmp._retn ();

  if (ACE_OS::strcmp (this->endpoint_.host_.in (), "") == 0)
    {
      ACE::HTBP::Addr host_addr;

      char tmp_host [MAXHOSTNAMELEN + 1];

      // If no host is specified: assign the default host, i.e. the
      // local host.
      if (host_addr.get_host_name (tmp_host,
                                   sizeof (tmp_host)) != 0)
        {
          // Can't get the IP address since the INET_Addr wasn't
          // initialized.  Just throw an exception.

          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) ")
                        ACE_TEXT ("TAO::HTIOP::Profile::parse_string ")
                        ACE_TEXT ("- %p\n\n"),
                        ACE_TEXT ("cannot determine hostname")));

          // @@ What's the right exception to throw here?
          throw CORBA::INV_OBJREF(
            CORBA::SystemException::_tao_minor_code(
              TAO::VMCID,
              EINVAL),
            CORBA::COMPLETED_NO);
        }
      else
        this->endpoint_.host_ = CORBA::string_dup (tmp_host);
    }

  TAO::ObjectKey ok;
  TAO::ObjectKey::decode_string_to_sequence (ok, okd + 1);

  (void) this->orb_core ()->object_key_table ().bind (ok,
                                                      this->ref_object_key_);
}

CORBA::Boolean
TAO::HTIOP::Profile::do_is_equivalent (const TAO_Profile *other_profile)
{
  const TAO::HTIOP::Profile *op =
    dynamic_cast<const TAO::HTIOP::Profile *> (other_profile);

  if (op == 0)
    return 0;

  // Check endpoints equivalence.
  const TAO::HTIOP::Endpoint *other_endp = &op->endpoint_;
  for (TAO::HTIOP::Endpoint *endp = &this->endpoint_;
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

CORBA::ULong
TAO::HTIOP::Profile::hash (CORBA::ULong max)
{
  // Get the hashvalue for all endpoints.
  CORBA::ULong hashval = 0;
  for (TAO::HTIOP::Endpoint *endp = &this->endpoint_;
       endp != 0;
       endp = endp->next_)
    {
      hashval += endp->hash ();
    }

  hashval += this->version_.minor;
  hashval += this->tag ();

  const TAO::ObjectKey &ok =
    this->ref_object_key_->object_key ();

  if (ok.length () >= 4)
    {
      hashval += ok[1];
      hashval += ok[3];
    }

  return hashval % max;
}

TAO_Endpoint*
TAO::HTIOP::Profile::endpoint (void)
{
  return &this->endpoint_;
}

CORBA::ULong
TAO::HTIOP::Profile::endpoint_count (void) const
{
  return this->count_;
}

void
TAO::HTIOP::Profile::add_endpoint (TAO::HTIOP::Endpoint *endp)
{
  endp->next_ = this->endpoint_.next_;
  this->endpoint_.next_ = endp;

  this->count_++;
}

char *
TAO::HTIOP::Profile::to_string (void) const
{
  CORBA::String_var key;
  TAO::ObjectKey::encode_sequence_to_string (key.inout(),
                                             this->ref_object_key_->object_key ());

  size_t buflen = (8 /* "corbaloc" */ +
                   1 /* colon separator */ +
                   ACE_OS::strlen (::the_prefix) +
                   1 /* colon separator */ +
                   1 /* major version */ +
                   1 /* decimal point */ +
                   1 /* minor version */ +
                   1 /* `@' character */ +
                   ACE_OS::strlen (this->endpoint_.host ()) +
                   1 /* colon separator */ +
                   5 /* port number */ +
                   1 /* object key separator */ +
                   ACE_OS::strlen (key.in ()));

  char * buf = CORBA::string_alloc (static_cast<CORBA::ULong> (buflen));

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "corbaloc:%s:%c.%c@%s:%d%c%s",
                   ::the_prefix,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->endpoint_.host (),
                   this->endpoint_.port (),
                   this->object_key_delimiter_,
                   key.in ());

  return buf;
}



const char *
TAO::HTIOP::Profile::prefix (void)
{
  return ::the_prefix;
}

void
TAO::HTIOP::Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING hostname from profile
  encap.write_string (this->endpoint_.host ());

  // UNSIGNED SHORT port number
  encap.write_ushort (this->endpoint_.port ());

  // STRING htid
  encap.write_string (this->endpoint_.htid ());

  // OCTET SEQUENCE for object key
  if (this->ref_object_key_)
    encap << this->ref_object_key_->object_key ();
  else
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  "(%P|%t) TAO - IIOP_Profile::create_profile_body "
                  "no object key marshalled\n"));
    }

  if (this->version_.major > 1
      || this->version_.minor > 0)
    this->tagged_components ().encode (encap);
}

int
TAO::HTIOP::Profile::encode_endpoints ()
{
  // Create a data structure and fill it with endpoint info for wire
  // transfer.
  // We include information for the head of the list
  // together with other endpoints because even though its addressing
  // info is transmitted using standard ProfileBody components, its
  // priority is not!

  HTIOPEndpointSequence endpoints;
  endpoints.length (this->count_);

  const TAO::HTIOP::Endpoint *endpoint = &this->endpoint_;
  for (CORBA::ULong i = 0;
       i < this->count_;
       ++i)
    {
      endpoints[i].host = endpoint->host ();
      endpoints[i].port = endpoint->port ();
      endpoints[i].htid = endpoint->htid ();

      endpoint = endpoint->next_;
    }

  // Encode the data structure.
  TAO_OutputCDR out_cdr;
  if ((out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
       == 0)
      || (out_cdr << endpoints) == 0)
    return -1;
  size_t length = out_cdr.total_length ();

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;
  tagged_component.component_data.length (static_cast<CORBA::ULong> (length));
  CORBA::Octet *buf =
    tagged_component.component_data.get_buffer ();

  for (const ACE_Message_Block *iterator = out_cdr.begin ();
       iterator != 0;
       iterator = iterator->cont ())
    {
      size_t i_length = iterator->length ();
      ACE_OS::memcpy (buf, iterator->rd_ptr (), i_length);

      buf += i_length;
    }

  // Add component with encoded endpoint data to this profile's
  // TaggedComponents.
  tagged_components_.set_component (tagged_component);

  return  0;
}

int
TAO::HTIOP::Profile::decode_endpoints (void)
{
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;

  if (this->tagged_components_.get_component (tagged_component))
    {
      const CORBA::Octet *buf =
        tagged_component.component_data.get_buffer ();

      TAO_InputCDR in_cdr (reinterpret_cast<const char*> (buf),
                           tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"boolean byte_order extraction failed\n"),
                          -1);
      in_cdr.reset_byte_order (static_cast<int> (byte_order));

      // Extract endpoints sequence.
      HTIOPEndpointSequence endpoints;

      if ((in_cdr >> endpoints) == 0)
        ORBSVCS_ERROR_RETURN ((LM_ERROR,"endpoint sequence extraction failed\n"),
                          -1);

      // Use information extracted from the tagged component to
      // populate the profile.  Skip the first endpoint, since it is
      // always extracted through standard profile body.  Also, begin
      // from the end of the sequence to preserve endpoint order,
      // since <add_endpoint> method reverses the order of endpoints
      // in the list.
      for (CORBA::ULong i = endpoints.length() -1;
           i > 0;
           --i)
        {
          TAO::HTIOP::Endpoint *endpoint = 0;
          ACE_NEW_RETURN (endpoint,
                          TAO::HTIOP::Endpoint (endpoints[i].host,
                                              endpoints[i].port,
                                              endpoints[i].htid),
                          -1);
          this->add_endpoint (endpoint);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
