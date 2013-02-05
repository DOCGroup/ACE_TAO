// $Id$

#include "tao/Strategies/DIOP_Profile.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/IIOP_EndpointsC.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_netdb.h"

static const char the_prefix[] = "diop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char TAO_DIOP_Profile::object_key_delimiter_ = '/';

char
TAO_DIOP_Profile::object_key_delimiter (void) const
{
  return TAO_DIOP_Profile::object_key_delimiter_;
}


TAO_DIOP_Profile::TAO_DIOP_Profile (const ACE_INET_Addr &addr,
                                    const TAO::ObjectKey &object_key,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_DIOP_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (addr,
               orb_core->orb_params ()->use_dotted_decimal_addresses ()),
    count_ (1)
{
}

TAO_DIOP_Profile::TAO_DIOP_Profile (const char* host,
                                    CORBA::UShort port,
                                    const TAO::ObjectKey &object_key,
                                    const ACE_INET_Addr &addr,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_DIOP_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (host, port, addr),
    count_ (1)
{
}

TAO_DIOP_Profile::TAO_DIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_DIOP_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1)
{
}

TAO_DIOP_Profile::~TAO_DIOP_Profile (void)
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
TAO_DIOP_Profile::decode_profile (TAO_InputCDR& cdr)
{
  // @@ NOTE: This code is repeated thrice. Need to factor out in a
  // better manner.
  // Decode host and port into the <endpoint_>.
  if (cdr.read_string (this->endpoint_.host_.out ()) == 0
      || cdr.read_ushort (this->endpoint_.port_) == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - DIOP_Profile::decode_profile, ")
                    ACE_TEXT ("error while decoding host/port\n")));
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
TAO_DIOP_Profile::parse_string_i (const char *ior)
{
  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...
  const char *okd = ACE_OS::strchr (ior, this->object_key_delimiter_);

  if (okd == 0 || okd == ior)
    {
      // No object key delimiter or no hostname specified.
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO::VMCID,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  // Length of host string.
  CORBA::ULong length_host = 0;

  const char *cp_pos = ACE_OS::strchr (ior, ':');  // Look for a port
#if defined (ACE_HAS_IPV6)
  // IPv6 numeric address in host string?
  bool ipv6_in_host = false;

  // Check if this is a (possibly) IPv6 supporting profile containing a
  // decimal IPv6 address representation.
  if ((this->version ().major > TAO_MIN_IPV6_IIOP_MAJOR ||
       this->version ().minor >= TAO_MIN_IPV6_IIOP_MINOR) &&
      ior[0] == '[')
    {
      // In this case we have to find the end of the numeric address and
      // start looking for the port separator from there.
      const char *cp_pos_a = ACE_OS::strchr (ior, ']');
      if (cp_pos_a == 0)
        {
          // No valid IPv6 address specified.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("\nTAO (%P|%t) - DIOP_Profile::parse_string_i, ")
                          ACE_TEXT ("invalid IPv6 decimal address specified.\n")));
            }

          throw ::CORBA::INV_OBJREF (
                         CORBA::SystemException::_tao_minor_code (
                                                                  0,
                                                                  EINVAL),
                         CORBA::COMPLETED_NO);
        }
      else
        {
          if (cp_pos_a[1] == ':')    // Look for a port
            cp_pos = cp_pos_a + 1;
          else
            cp_pos = 0;
          ipv6_in_host = true; // host string contains full IPv6 numeric address
        }
    }
#endif /* ACE_HAS_IPV6 */

  if (cp_pos == ior)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                   ACE_TEXT ("\nTAO (%P|%t) DIOP_Profile: ")
                   ACE_TEXT ("Host address may be omited only when no port has been specified.\n")));
        }
      // No hostname specified!  It is required by the spec.
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
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

      if (ACE_OS::strspn (tmp.in (), "1234567890") == length_port)
        {
          this->endpoint_.port_ =
            static_cast<CORBA::UShort> (ACE_OS::atoi (tmp.in ()));
        }
      else
        {
          ACE_INET_Addr ia;
          if (ia.string_to_addr (tmp.in ()) == -1)
            {
              throw ::CORBA::INV_OBJREF (
                             CORBA::SystemException::_tao_minor_code (
                               0,
                               EINVAL),
                             CORBA::COMPLETED_NO);
            }
          else
            {
              this->endpoint_.port_ = ia.get_port_number ();
            }
        }

      length_host = cp_pos - ior;
    }
  else
    length_host = okd - ior;

#if defined (ACE_HAS_IPV6)
  if (ipv6_in_host)
    length_host -= 2; // don't store '[' and ']'
#endif /* ACE_HAS_IPV6 */

  CORBA::String_var tmp = CORBA::string_alloc (length_host);

#if defined (ACE_HAS_IPV6)
  if (ipv6_in_host)
    ACE_OS::strncpy (tmp.inout (), ior + 1, length_host);
  else
#endif /* ACE_HAS_IPV6 */
  // Skip the trailing '/'
  ACE_OS::strncpy (tmp.inout (), ior, length_host);
  tmp[length_host] = '\0';

  this->endpoint_.host_ = tmp._retn ();
#if defined (ACE_HAS_IPV6)
  this->endpoint_.is_ipv6_decimal_ = ipv6_in_host;
#endif /* ACE_HAS_IPV6 */

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
                        ACE_TEXT ("TAO (%P|%t) - ")
                        ACE_TEXT ("DIOP_Profile::parse_string_i, ")
                        ACE_TEXT ("%p\n\n"),
                        ACE_TEXT ("cannot determine hostname")));

          // @@ What's the right exception to throw here?
          throw ::CORBA::INV_OBJREF (
                       CORBA::SystemException::_tao_minor_code (
                         TAO::VMCID,
                         EINVAL),
                       CORBA::COMPLETED_NO);
        }
      else
        this->endpoint_.host_ = CORBA::string_dup (tmp_host);
    }

  TAO::ObjectKey ok;
  TAO::ObjectKey::decode_string_to_sequence (ok,
                                             okd + 1);

  (void) this->orb_core ()->object_key_table ().bind (ok,
                                                      this->ref_object_key_);
}

CORBA::Boolean
TAO_DIOP_Profile::do_is_equivalent (const TAO_Profile *other_profile)
{
  const TAO_DIOP_Profile *op =
    dynamic_cast<const TAO_DIOP_Profile *> (other_profile);

  // Make sure we have a TAO_DIOP_Profile.
  if (op == 0)
    return 0;

  // Check endpoints equivalence.
  const TAO_DIOP_Endpoint *other_endp = &op->endpoint_;
  for (TAO_DIOP_Endpoint *endp = &this->endpoint_;
       endp != 0;
       endp = endp->next_)
    {
      if (endp->is_equivalent (other_endp))
        other_endp = other_endp->next_;
      else
        return false;
    }

  return true;
}

CORBA::ULong
TAO_DIOP_Profile::hash (CORBA::ULong max)
{
  // Get the hashvalue for all endpoints.
  CORBA::ULong hashval = 0;
  for (TAO_DIOP_Endpoint *endp = &this->endpoint_;
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

  hashval += this->hash_service_i (max);

  return hashval % max;
}

TAO_Endpoint*
TAO_DIOP_Profile::endpoint (void)
{
  return &this->endpoint_;
}

CORBA::ULong
TAO_DIOP_Profile::endpoint_count (void) const
{
  return this->count_;
}

void
TAO_DIOP_Profile::add_endpoint (TAO_DIOP_Endpoint *endp)
{
  endp->next_ = this->endpoint_.next_;
  this->endpoint_.next_ = endp;

  this->count_++;
}

char *
TAO_DIOP_Profile::to_string (void) const
{
  // corbaloc:diop:1.2@host:port,diop:1.2@host:port,.../key

  CORBA::String_var key;
  TAO::ObjectKey::encode_sequence_to_string (key.inout (),
                                             this->ref_object_key_->object_key ());

  size_t buflen = (
       8 /* "corbaloc" */ +
       1 /* colon separator */ +
       1 /* object key separator */ +
       ACE_OS::strlen (key.in ()));
  size_t pfx_len = (
       ACE_OS::strlen (::the_prefix) /* "diop" */ +
       1 /* colon separator */);

 const TAO_DIOP_Endpoint *endp = 0;
 for (endp = &this->endpoint_; endp != 0; endp = endp->next_)
   {
      buflen += (
          pfx_len +
          1 /* major version */ +
          1 /* decimal point */ +
          1 /* minor version */ +
          1 /* `@' character */ +
          ACE_OS::strlen (endp->host ()) +
          1 /* colon separator */ +
          5 /* port number */ +
          1 /* comma */);
#if defined (ACE_HAS_IPV6)
      if (endp->is_ipv6_decimal_)
        buflen += 2; // room for '[' and ']'
#endif /* ACE_HAS_IPV6 */
   }

  static const char digits [] = "0123456789";

  char * buf = CORBA::string_alloc (static_cast<CORBA::ULong> (buflen));

  ACE_OS::strcpy (buf, "corbaloc:");

  for (endp = &this->endpoint_; endp != 0; endp = endp->next_)
    {
      if (&this->endpoint_ != endp)
      ACE_OS::strcat (buf, ",");

#if defined (ACE_HAS_IPV6)
      if (endp->is_ipv6_decimal_)
        {
          // Don't publish scopeid if included.
          ACE_CString tmp (endp->host ());
          ACE_CString::size_type pos = tmp.find ('%');
          if (pos != ACE_CString::npos)
            {
              tmp = tmp.substr (0, pos + 1);
              tmp[pos] = '\0';
            }
          ACE_OS::sprintf (buf + ACE_OS::strlen (buf),
                  "%s:%c.%c@[%s]:%d",
                  ::the_prefix,
                  digits [this->version_.major],
                  digits [this->version_.minor],
                  tmp.c_str (),
                  endp->port ());
        }
      else
#endif
      ACE_OS::sprintf (buf + ACE_OS::strlen (buf),
              "%s:%c.%c@%s:%d",
              ::the_prefix,
              digits [this->version_.major],
              digits [this->version_.minor],
              endp->host (),
              endp->port ());

  }
  ACE_OS::sprintf (buf + ACE_OS::strlen (buf),
                   "%c%s",
                   this->object_key_delimiter_,
                   key.in ());

  return buf;
}

const char *
TAO_DIOP_Profile::prefix (void)
{
  return ::the_prefix;
}

void
TAO_DIOP_Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING hostname from profile
#if defined (ACE_HAS_IPV6)
  // For IPv6 decimal addresses make sure the possibly included scopeid
  // is not published as this has only local meaning.
  const char* host;
  const char* pos;
  if (this->endpoint_.is_ipv6_decimal_ &&
      (pos = ACE_OS::strchr (host = this->endpoint_.host (), '%')) != 0)
    {
      ACE_CString tmp;
      size_t len = pos - host;
      tmp.set (this->endpoint_.host (), len, 1);
      encap.write_string (tmp.c_str ());
    }
  else
#endif /* ACE_HAS_IPV6 */
  encap.write_string (this->endpoint_.host ());

  // UNSIGNED SHORT port number
  encap.write_ushort (this->endpoint_.port ());

  // OCTET SEQUENCE for object key
  if (this->ref_object_key_)
    encap << this->ref_object_key_->object_key ();
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO (%P|%t) - DIOP_Profile::create_profile_body, "
                  "no object key marshalled\n"));
    }

  if (this->version_.major > 1
      || this->version_.minor > 0)
    this->tagged_components ().encode (encap);
}

int
TAO_DIOP_Profile::encode_endpoints (void)
{
  // Create a data structure and fill it with endpoint info for wire
  // transfer.
  // We include information for the head of the list
  // together with other endpoints because even though its addressing
  // info is transmitted using standard ProfileBody components, its
  // priority is not!

  TAO::IIOPEndpointSequence endpoints;
  endpoints.length (this->count_);

  const TAO_DIOP_Endpoint *endpoint = &this->endpoint_;
  for (CORBA::ULong i = 0;
       i < this->count_;
       ++i)
    {
#if defined (ACE_HAS_IPV6)
      if (endpoint->is_ipv6_decimal_)
        {
          // Don't publish scopeid if included.
          ACE_CString tmp (endpoint->host ());
          ACE_CString::size_type pos = tmp.find ('%');
          if (pos != ACE_CString::npos)
            {
              tmp = tmp.substr (0, pos + 1);
              tmp[pos] = '\0';
              endpoints[i].host = tmp.c_str ();
            }
          else
            endpoints[i].host = tmp.c_str ();
        }
      else
#endif /* ACE_HAS_IPV6 */
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

  this->set_tagged_components (out_cdr);

  return  0;
}

int
TAO_DIOP_Profile::decode_endpoints (void)
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
        return -1;
      in_cdr.reset_byte_order (static_cast<int> (byte_order));

      // Extract endpoints sequence.
      TAO::IIOPEndpointSequence endpoints;

      if (! (in_cdr >> endpoints))
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
          TAO_DIOP_Endpoint *endpoint = 0;
          ACE_NEW_RETURN (endpoint,
                          TAO_DIOP_Endpoint (endpoints[i].host,
                                             endpoints[i].port,
                                             endpoints[i].priority),
                          -1);

          this->add_endpoint (endpoint);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
