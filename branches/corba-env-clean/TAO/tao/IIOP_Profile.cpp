// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "IIOP_Profile.h"
#include "CDR.h"
#include "Environment.h"
#include "ORB.h"
#include "ORB_Core.h"
#include "debug.h"
#include "iiop_endpoints.h"

ACE_RCSID(TAO,
          IIOP_Profile,
          "$Id$")

#if !defined (__ACE_INLINE__)
# include "IIOP_Profile.i"
#endif /* __ACE_INLINE__ */

static const char prefix_[] = "iiop";

const char TAO_IIOP_Profile::object_key_delimiter_ = '/';

char
TAO_IIOP_Profile::object_key_delimiter (void) const
{
  return TAO_IIOP_Profile::object_key_delimiter_;
}



TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr &addr,
                                    const TAO_ObjectKey &object_key,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_IIOP_PROFILE, orb_core, version),
    endpoint_ (addr,
               orb_core->orb_params ()->use_dotted_decimal_addresses ()),
    count_ (1),
    object_key_ (object_key),
    tagged_profile_ ()
{
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const char* host,
                                    CORBA::UShort port,
                                    const TAO_ObjectKey &object_key,
                                    const ACE_INET_Addr &addr,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_IIOP_PROFILE, orb_core, version),
    endpoint_ (host, port, addr),
    count_ (1),
    object_key_ (object_key),
    tagged_profile_ ()
{
}

TAO_IIOP_Profile::TAO_IIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_IIOP_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1),
    object_key_ (),
    tagged_profile_ ()
{
}

TAO_IIOP_Profile::~TAO_IIOP_Profile (void)
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
TAO_IIOP_Profile::decode (TAO_InputCDR& cdr)
{
  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring IIOP profiles
  // whose versions we don't understand.
  CORBA::Octet major = 0, minor = 0;

  if (!(cdr.read_octet (major)
        && major == TAO_DEF_GIOP_MAJOR
        && cdr.read_octet (minor)))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) IIOP_Profile::decode - v%d.%d\n"),
                    major,
                    minor));
      return -1;
    }

  this->version_.major = major;

  if (minor <= TAO_DEF_GIOP_MINOR)
    this->version_.minor = minor;

  // Decode host and port into the <endpoint_>.
  if (cdr.read_string (this->endpoint_.host_.out ()) == 0
      || cdr.read_ushort (this->endpoint_.port_) == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) IIOP_Profile::decode - ")
                    ACE_TEXT ("error while decoding host/port")));
      return -1;
    }

  // ... and object key.

  if ((cdr >> this->object_key_) == 0)
    return -1;

  // Tagged Components *only* exist after version 1.0!
  // For GIOP 1.2, IIOP and GIOP have same version numbers!
  if (this->version_.major > 1
      || this->version_.minor > 0)
    if (this->tagged_components_.decode (cdr) == 0)
      return -1;

  if (cdr.length () != 0 && TAO_debug_level)
    // If there is extra data in the profile we are supposed to
    // ignore it, but print a warning just in case...
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%d bytes out of %d left after IIOP profile data\n"),
                cdr.length (),
                encap_len));

  // Decode any additional endpoints per profile.  (At the present,
  // only RTCORBA takes advantage of this feature.)
  if (this->decode_endpoints () == -1)
    return -1;

  if (cdr.good_bit ())
    {
      // Invalidate the object_addr_ until first access.
      this->endpoint_.object_addr_.set_type (-1);

      return 1;
    }

  return -1;
}

void
TAO_IIOP_Profile::parse_string (const char *ior
                                TAO_ENV_ARG_DECL)
{
  if (!ior || !*ior)
    {
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA_SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }

  // Remove the "N.n@" version prefix, if it exists, and verify the
  // version is one that we accept.

  // Check for version
  if (isdigit (ior [0]) &&
      ior[1] == '.' &&
      isdigit (ior [2]) &&
      ior[3] == '@')
    {
      // @@ This may fail for non-ascii character sets [but take that
      // with a grain of salt]
      this->version_.set_version ((char) (ior[0] - '0'),
                                  (char) (ior[2] - '0'));
      ior += 4;
      // Skip over the "N.n@"
    }
  else
    {
      // CORBA spec requires 1.0 if a version isn't specified.
      this->version_.set_version (1, 0);
    }

  if (this->version_.major != TAO_DEF_GIOP_MAJOR ||
      this->version_.minor >  TAO_DEF_GIOP_MINOR)
    {
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA_SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }

  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...

  const char *okd = ACE_OS::strchr (ior, this->object_key_delimiter_);

  if (okd == 0 || okd == ior)
    {
      // No object key delimiter or no hostname specified.
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA_SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }

  // Length of host string.
  CORBA::ULong length_host = 0;

  const char *cp_pos = ACE_OS::strchr (ior, ':');  // Look for a port

  if (cp_pos == ior)
    {
      // No hostname specified!  It is required by the spec.
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA_SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }
  else if (cp_pos != 0)
    {
      // A port number or port name was specified.
      CORBA::ULong length_port = okd - cp_pos - 1;

      CORBA::String_var tmp = CORBA::string_alloc (length_port);

      ACE_OS::strncpy (tmp.inout (), cp_pos + 1, length_port);
      tmp[length_port] = '\0';

      this->endpoint_.port_ =
        ACE_static_cast (CORBA::UShort, ACE_OS::atoi (tmp.in ()));

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
      ACE_INET_Addr host_addr;

      char tmp_host [MAXHOSTNAMELEN + 1];

      // If no host is specified: assign the default host, i.e. the
      // local host.
      if (host_addr.get_host_name (tmp_host,
                                   sizeof (tmp_host)) != 0)
        {
          // Can't get the IP address since the INET_Addr wasn't
          // initialized.  Just throw an exception.

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) ")
                        ACE_TEXT ("IIOP_Profile::parse_string ")
                        ACE_TEXT ("- %p\n\n"),
                        ACE_TEXT ("cannot determine hostname")));

          // @@ What's the right exception to throw here?
          ACE_THROW (CORBA::INV_OBJREF (
                       CORBA_SystemException::_tao_minor_code (
                         TAO_DEFAULT_MINOR_CODE,
                         EINVAL),
                       CORBA::COMPLETED_NO));
        }
      else
        this->endpoint_.host_ = CORBA::string_dup (tmp_host);
    }

  TAO_ObjectKey::decode_string_to_sequence (this->object_key_, okd + 1);
}

CORBA::Boolean
TAO_IIOP_Profile::is_equivalent (const TAO_Profile *other_profile)
{

  if (other_profile->tag () != TAO_TAG_IIOP_PROFILE)
    return 0;

  const TAO_IIOP_Profile *op =
    ACE_dynamic_cast (const TAO_IIOP_Profile *, other_profile);


  if (!(this->object_key_ == op->object_key_
        && this->version_ == op->version_
        && this->count_ == op->count_))
    return 0;

  // Check endpoints equivalence.
  const TAO_IIOP_Endpoint *other_endp = &op->endpoint_;
  for (TAO_IIOP_Endpoint *endp = &this->endpoint_;
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
TAO_IIOP_Profile::hash (CORBA::ULong max
                        TAO_ENV_ARG_DECL_NOT_USED)
{
  // Get the hashvalue for all endpoints.
  CORBA::ULong hashval = 0;
  for (TAO_IIOP_Endpoint *endp = &this->endpoint_;
       endp != 0;
       endp = endp->next_)
    {
      hashval += endp->hash ();
    }

  hashval += this->version_.minor;
  hashval += this->tag ();

  if (this->object_key_.length () >= 4)
    {
      hashval += this->object_key_ [1];
      hashval += this->object_key_ [3];
    }

  return hashval % max;
}

TAO_Endpoint*
TAO_IIOP_Profile::endpoint (void)
{
  return &this->endpoint_;
}

size_t
TAO_IIOP_Profile::endpoint_count (void)
{
  return this->count_;
}

void
TAO_IIOP_Profile::add_endpoint (TAO_IIOP_Endpoint *endp)
{
  endp->next_ = this->endpoint_.next_;
  this->endpoint_.next_ = endp;

  this->count_++;
}

char *
TAO_IIOP_Profile::to_string (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  CORBA::String_var key;
  TAO_ObjectKey::encode_sequence_to_string (key.inout(),
                                            this->object_key_);

  u_int buflen = (8 /* "corbaloc" */ +
                  1 /* colon separator */ +
                  ACE_OS::strlen (::prefix_) +
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

  char * buf = CORBA::string_alloc (buflen);

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "corbaloc:%s:%c.%c@%s:%d%c%s",
                   ::prefix_,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->endpoint_.host (),
                   this->endpoint_.port (),
                   this->object_key_delimiter_,
                   key.in ());

  return buf;
}



const char *
TAO_IIOP_Profile::prefix (void)
{
  return ::prefix_;
}

int
TAO_IIOP_Profile::encode (TAO_OutputCDR &stream) const
{
  // UNSIGNED LONG, protocol tag
  stream.write_ulong (this->tag ());

  // Create the encapsulation....
  TAO_OutputCDR encap (ACE_CDR::DEFAULT_BUFSIZE,
                       TAO_ENCAP_BYTE_ORDER,
                       this->orb_core ()->output_cdr_buffer_allocator (),
                       this->orb_core ()->output_cdr_dblock_allocator (),
                       this->orb_core ()->output_cdr_msgblock_allocator (),
                       this->orb_core ()->orb_params ()->cdr_memcpy_tradeoff (),
                       TAO_DEF_GIOP_MAJOR,
                       TAO_DEF_GIOP_MINOR,
                       this->orb_core ()->to_iso8859 (),
                       this->orb_core ()->to_unicode ());


  // Create the profile body
  this->create_profile_body (encap);

  // write the encapsulation as an octet sequence...

  stream << CORBA::ULong (encap.total_length ());
  stream.write_octet_array_mb (encap.begin ());

  return 1;
}


IOP::TaggedProfile &
TAO_IIOP_Profile::create_tagged_profile (void)
{
  // Check whether we have already created the TaggedProfile
  if (this->tagged_profile_.profile_data.get_buffer () == 0)
    {
      // As we have not created we will now create the TaggedProfile
      this->tagged_profile_.tag = TAO_TAG_IIOP_PROFILE;

      // Create the encapsulation....
      TAO_OutputCDR encap (ACE_CDR::DEFAULT_BUFSIZE,
                           TAO_ENCAP_BYTE_ORDER,
                           this->orb_core ()->output_cdr_buffer_allocator (),
                           this->orb_core ()->output_cdr_dblock_allocator (),
                           this->orb_core ()->output_cdr_msgblock_allocator (),
                           this->orb_core ()->orb_params ()->cdr_memcpy_tradeoff (),
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR,
                           this->orb_core ()->to_iso8859 (),
                           this->orb_core ()->to_unicode ());

      // Create the profile body
      this->create_profile_body (encap);

      CORBA::ULong length =
        ACE_static_cast(CORBA::ULong,encap.total_length ());

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      // Place the message block in to the Sequence of Octets that we
      // have
      this->tagged_profile_.profile_data.replace (length,
                                                  encap.begin ());
#else
      this->tagged_profile_.profile_data.length (length);
      CORBA::Octet *buffer =
        this->tagged_profile_.profile_data.get_buffer ();
      for (const ACE_Message_Block *i = encap.begin ();
           i != encap.end ();
           i = i->next ())
        {
          ACE_OS::memcpy (buffer, i->rd_ptr (), i->length ());
          buffer += i->length ();
        }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
    }

  return this->tagged_profile_;
}

void
TAO_IIOP_Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING hostname from profile
  encap.write_string (this->endpoint_.host ());

  // UNSIGNED SHORT port number
  encap.write_ushort (this->endpoint_.port ());

  // OCTET SEQUENCE for object key
  encap << this->object_key_;

  if (this->version_.major > 1
      || this->version_.minor > 0)
    this->tagged_components ().encode (encap);
}

int
TAO_IIOP_Profile::encode_endpoints (void)
{
  // Create a data structure and fill it with endpoint info for wire
  // transfer.
  // We include information for the head of the list
  // together with other endpoints because even though its addressing
  // info is transmitted using standard ProfileBody components, its
  // priority is not!

  TAO_IIOPEndpointSequence endpoints;
  endpoints.length (this->count_);

  const TAO_IIOP_Endpoint *endpoint = &this->endpoint_;
  for (size_t i = 0;
       i < this->count_;
       ++i)
    {
      endpoints[i].host = endpoint->host ();
      endpoints[i].port = endpoint->port ();
      endpoints[i].priority = endpoint->priority ();

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
  tagged_component.tag = TAO_TAG_ENDPOINTS;
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

  return  0;
}

int
TAO_IIOP_Profile::decode_endpoints (void)
{
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;

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
      TAO_IIOPEndpointSequence endpoints;

      if ((in_cdr >> endpoints) == 0)
        return -1;

      // Get the priority of the first endpoint (head of the list.
      // It's other data is extracted as part of the standard profile
      // decoding.
      this->endpoint_.priority (endpoints[0].priority);

      // Use information extracted from the tagged component to
      // populate the profile.  Skip the first endpoint, since it is
      // always extracted through standard profile body.  Also, begin
      // from the end of the sequence to preserve endpoint order,
      // since <add_endpoint> method reverses the order of endpoints
      // in the list.
      for (CORBA::ULong i = endpoints.length () - 1;
           i > 0;
           --i)
        {
          TAO_IIOP_Endpoint *endpoint = 0;
          ACE_NEW_RETURN (endpoint,
                          TAO_IIOP_Endpoint (endpoints[i].host,
                                             endpoints[i].port,
                                             endpoints[i].priority),
                          -1);

          this->add_endpoint (endpoint);
        }
    }

  return 0;
}
