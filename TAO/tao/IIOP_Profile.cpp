// -*- C++ -*-
// $Id$

#include "tao/IIOP_Profile.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/IIOP_EndpointsC.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/os_include/os_netdb.h"

static const char the_prefix[] = "iiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

//@@ TAO_PROFILE_SPL_COPY_HOOK_START

const char TAO_IIOP_Profile::object_key_delimiter_ = '/';

char
TAO_IIOP_Profile::object_key_delimiter (void) const
{
  return TAO_IIOP_Profile::object_key_delimiter_;
}

//@@ TAO_PROFILE_SPL_COPY_HOOK_END

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr &addr,
                                    const TAO::ObjectKey &object_key,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (IOP::TAG_INTERNET_IOP,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (addr,
               orb_core->orb_params ()->use_dotted_decimal_addresses ()),
    last_endpoint_ (&this->endpoint_),
    count_ (1)
{
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const char* host,
                                    CORBA::UShort port,
                                    const TAO::ObjectKey &object_key,
                                    const ACE_INET_Addr &addr,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (IOP::TAG_INTERNET_IOP,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (host, port, addr),
    last_endpoint_ (&this->endpoint_),
    count_ (1)
{
}

TAO_IIOP_Profile::TAO_IIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (IOP::TAG_INTERNET_IOP,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR,
                                           TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    last_endpoint_ (&this->endpoint_),
    count_ (1)
{
}

//@@ TAO_PROFILE_SPL_COPY_HOOK_START
int
TAO_IIOP_Profile::decode_profile (TAO_InputCDR& cdr)
{
  // Decode host and port into the <endpoint_>.
  // it is necessary to do it indirectly so that IPv6 host addresses
  // can be evaluated correctly.
  CORBA::String_var host;
  CORBA::UShort port;

  if (cdr.read_string(host.out()) == 0 ||
      cdr.read_ushort (port) == 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) IIOP_Profile::decode - ")
                    ACE_TEXT ("error while decoding host/port\n")));
      return -1;
    }

  this->endpoint_.host(host.in());
  this->endpoint_.port(port);

  if (cdr.good_bit ())
    {
      // Invalidate the object_addr_ until first access.
      this->endpoint_.object_addr_.set_type (-1);

      const char* csv = this->orb_core()->orb_params()->preferred_interfaces();
      if (csv != 0 && *csv != '\0')
        {
          bool const enforce =
            this->orb_core()->orb_params()->enforce_pref_interfaces();
          this->count_ += this->endpoint_.preferred_interfaces(csv, enforce, *this);
        }
      return 1;
    }

  return -1;
}

void
TAO_IIOP_Profile::parse_string_i (const char *ior)
{
  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...

  const char *okd = ACE_OS::strchr (ior, this->object_key_delimiter_);

  if (okd == 0 || okd == ior)
    {
      // No object key delimiter or no hostname specified.
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     0,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  // Length of host string.
  CORBA::ULong length_host = 0;

  const char *cp_pos_overrun = ACE_OS::strchr (ior, ':');  // Look for a port
  const char *cp_pos = (cp_pos_overrun < okd) ? cp_pos_overrun : 0; // but before object key
#if defined (ACE_HAS_IPV6)
  // IPv6 numeric address in host string?
  bool ipv6_in_host = false;

  // Check if this is a (possibly) IPv6 supporting profile containing a
  // decimal IPv6 address representation.
  if ((this->version().major > TAO_MIN_IPV6_IIOP_MAJOR ||
        this->version().minor >= TAO_MIN_IPV6_IIOP_MINOR) &&
      ior[0] == '[')
    {
      // In this case we have to find the end of the numeric address and
      // start looking for the port separator from there.
      const char *cp_pos_a_overrun = ACE_OS::strchr(ior, ']');
      const char *cp_pos_a = (cp_pos_a_overrun < okd) ? cp_pos_a_overrun : 0; // before object key
      if (cp_pos_a == 0)
        {
          // No valid IPv6 address specified.
          if (TAO_debug_level > 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                       ACE_TEXT ("\nTAO (%P|%t) IIOP_Profile: ")
                       ACE_TEXT ("Invalid IPv6 decimal address specified.\n")));
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
      // No hostname, however one is required by the spec when specifying a port.
      // See formal-04-03-01, section 13.6.10.3
      if (TAO_debug_level > 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                   ACE_TEXT ("\nTAO (%P|%t) IIOP_Profile: ")
                   ACE_TEXT ("Host address may be omited only when no port has been specified.\n")));
        }

      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     0,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }
  else if (cp_pos != 0)
    {
      // A port number or port name was specified.
      CORBA::ULong length_port = okd - cp_pos - 1;
      CORBA::String_var tmp = CORBA::string_alloc (length_port);

      if (tmp.in() != 0)
        {
          ACE_OS::strncpy (tmp.inout (), cp_pos + 1, length_port);
          tmp[length_port] = '\0';
        }

      if (length_port == 0)
        {
          this->endpoint_.port_ = 2809; // default IIOP port for
          // parsing corbaloc strings
        }
      else if (tmp.in () != 0 && ACE_OS::strspn (tmp.in (), "1234567890") == length_port)
        {
          this->endpoint_.port_ =
            static_cast<CORBA::UShort> (ACE_OS::atoi (tmp.in ()));
        }
      else
        {
          ACE_INET_Addr ia;
          if (tmp.in () == 0 || ia.string_to_addr (tmp.in ()) == -1)
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
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) ")
                        ACE_TEXT ("IIOP_Profile::parse_string ")
                        ACE_TEXT ("- %p\n\n"),
                        ACE_TEXT ("cannot determine hostname")));

          // @@ What's the right exception to throw here?
          throw ::CORBA::INV_OBJREF (
                       CORBA::SystemException::_tao_minor_code (
                         0,
                         EINVAL),
                       CORBA::COMPLETED_NO);
        }

      this->endpoint_.host_ = CORBA::string_dup (tmp_host);
      const char* csv = this->orb_core()->orb_params()->preferred_interfaces();
      bool const enforce =
        this->orb_core()->orb_params()->enforce_pref_interfaces();
      this->endpoint_.preferred_interfaces (csv, enforce, *this);
    }

  TAO::ObjectKey ok;

  TAO::ObjectKey::decode_string_to_sequence (ok,
                                             okd + 1);

  (void) this->orb_core ()->object_key_table ().bind (ok,
                                                      this->ref_object_key_);
}

CORBA::Boolean
TAO_IIOP_Profile::do_is_equivalent (const TAO_Profile *other_profile)
{
  if (other_profile == this)
    return true;

  const TAO_IIOP_Profile *op =
    dynamic_cast<const TAO_IIOP_Profile *> (other_profile);

  // Make sure we have a TAO_IIOP_Profile.
  if (op == 0)
    return false;

  if (this->count_ == 0 && op->count_ == 0)
    return true;
  if (this->count_ != op->count_)
    return false;
  // Check endpoints equivalence.
  const TAO_IIOP_Endpoint *other_endp = &op->endpoint_;
  for (TAO_IIOP_Endpoint *endp = &this->endpoint_;
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
TAO_IIOP_Profile::hash (CORBA::ULong max)
{
  // Get the hash value for all endpoints.
  CORBA::ULong hashval = 0;
  for (TAO_IIOP_Endpoint *endp = &this->endpoint_;
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

  hashval += TAO_Profile::hash_service_i (max);

  return hashval % max;
}

TAO_Endpoint*
TAO_IIOP_Profile::endpoint (void)
{
  return &this->endpoint_;
}

TAO_Endpoint *
TAO_IIOP_Profile::base_endpoint (void)
{
  // do not call endpoint(), return the value directly. This is to
  // avoid calling a derived implementation of endpoint().
  return &this->endpoint_;
}

CORBA::ULong
TAO_IIOP_Profile::endpoint_count (void) const
{
  return this->count_;
}

void
TAO_IIOP_Profile::add_endpoint (TAO_IIOP_Endpoint *endp)
{
  this->last_endpoint_->next_ = endp;
  this->last_endpoint_ = endp;

  ++this->count_;
}

void
TAO_IIOP_Profile::remove_endpoint (TAO_IIOP_Endpoint *endp)
{
  if (endp == 0)
    return;

  // special handling for the target matching the base endpoint
  if (endp == &this->endpoint_)
    {
      if (--this->count_ > 0)
        {
          TAO_IIOP_Endpoint* n = this->endpoint_.next_;
          this->endpoint_ = *n;
          // since the assignment operator does not copy the next_
          // pointer, we must do it by hand
          this->endpoint_.next_ = n->next_;
          if (this->last_endpoint_ == n)
            {
              this->last_endpoint_ = &this->endpoint_;
            }
          delete n;
        }
      return;
    }

  TAO_IIOP_Endpoint* prev = &this->endpoint_;
  TAO_IIOP_Endpoint* cur = this->endpoint_.next_;

  while (cur != 0)
  {
    if (cur == endp)
      break;
    prev = cur;
    cur = cur->next_;
  }

  if (cur != 0)
  {
    prev->next_ = cur->next_;
    cur->next_ = 0;
    --this->count_;
    if (this->last_endpoint_ == cur)
      {
        this->last_endpoint_ = prev;
      }
    delete cur;
  }
}

void
TAO_IIOP_Profile::remove_generic_endpoint (TAO_Endpoint *ep)
{
  this->remove_endpoint(dynamic_cast<TAO_IIOP_Endpoint *>(ep));
}

void
TAO_IIOP_Profile::add_generic_endpoint (TAO_Endpoint *endp)
{
  TAO_IIOP_Endpoint *iep = dynamic_cast<TAO_IIOP_Endpoint *>(endp);
  if (iep != 0)
    {
      TAO_IIOP_Endpoint *clone;
      ACE_NEW (clone, TAO_IIOP_Endpoint(*iep));
      this->add_endpoint(clone);
    }
}

char *
TAO_IIOP_Profile::to_string (void) const
{
  // corbaloc:iiop:1.2@host:port,iiop:1.2@host:port,.../key

  CORBA::String_var key;
  TAO::ObjectKey::encode_sequence_to_string (key.inout(),
                                             this->ref_object_key_->object_key ());

  size_t buflen = (
       8 /* "corbaloc" */ +
       1 /* colon separator */ +
       1 /* object key separator */ +
       ACE_OS::strlen (key.in ()));
  size_t const pfx_len = (
       ACE_OS::strlen (::the_prefix) /* "iiop" */ +
       1 /* colon separator */);

 const TAO_IIOP_Endpoint *endp = 0;
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

  ACE_OS::strcpy(buf, "corbaloc:");

  for (endp = &this->endpoint_; endp != 0; endp = endp->next_)
    {
      if(&this->endpoint_ != endp)
      ACE_OS::strcat(buf, ",");

#if defined (ACE_HAS_IPV6)
      if (endp->is_ipv6_decimal_)
        {
          // Don't publish scopeid if included.
          ACE_CString tmp(endp->host ());
          ACE_CString::size_type pos = tmp.find('%');
          if (pos != ACE_CString::npos)
            {
              tmp = tmp.substr(0, pos + 1);
              tmp[pos] = '\0';
            }
          ACE_OS::sprintf (buf + ACE_OS::strlen(buf),
                  "%s:%c.%c@[%s]:%d",
                  ::the_prefix,
                  digits [this->version_.major],
                  digits [this->version_.minor],
                  tmp.c_str (),
                  endp->port () );
        }
      else
#endif
      ACE_OS::sprintf (buf + ACE_OS::strlen(buf),
              "%s:%c.%c@%s:%d",
              ::the_prefix,
              digits [this->version_.major],
              digits [this->version_.minor],
              endp->host (),
              endp->port () );

  }
  ACE_OS::sprintf (buf + ACE_OS::strlen(buf),
          "%c%s",
          this->object_key_delimiter_,
          key.in ());

  return buf;
}

const char *
TAO_IIOP_Profile::prefix (void)
{
  return ::the_prefix;
}

void
TAO_IIOP_Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING hostname from profile
#if defined (ACE_HAS_IPV6)
  // For IPv6 decimal addresses make sure the possibly included scopeid
  // is not published as this has only local meaning.
  const char* host = 0;
  const char* pos = 0;
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
      TAOLIB_ERROR ((LM_ERROR,
                  "(%P|%t) TAO - IIOP_Profile::create_profile_body "
                  "no object key marshalled\n"));
    }

  if (this->version_.major > 1 || this->version_.minor > 0)
    this->tagged_components ().encode (encap);
}

int
TAO_IIOP_Profile::encode_alternate_endpoints (void)
{
  // encode IOP::TAG_ALTERNATE_IIOP_ADDRESS tags if there are more
  // than one endpoints to listen to.
  const TAO_IIOP_Endpoint *endpoint = &this->endpoint_;
  for (CORBA::ULong i = 1;
       i < this->count_;
       ++i)
    {
      // The first endpoint is the actual endpoint. The rest of the
      // endpoints are the alternate endpoints. So, neglect the first
      // endpoint for TAG_ALTERNATE_IIOP_ADDRESS
      endpoint = endpoint->next_;

      if (!endpoint->is_encodable_)
        continue;

      // Encode the data structure. - The CORBA specification does not
      // mandate a particular container for the endpoints, only that
      // it is encoded as host first, then port.
      TAO_OutputCDR out_cdr;

#if defined (ACE_HAS_IPV6)
      // For IPv6 decimal addresses make sure the possibly included scopeid
      // is not published as this has only local meaning.
      const char* host = 0;
      const char* pos = 0;
      if (endpoint->is_ipv6_decimal_ &&
          (pos = ACE_OS::strchr (host = endpoint->host (), '%')) != 0)
        {
          ACE_CString tmp;
          size_t len = pos - host;
          tmp.set (endpoint->host (), len, 1);
          if (!(out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
              || !(out_cdr << tmp.c_str ())
              || !(out_cdr << endpoint->port ()))
            return -1;
          out_cdr.write_string (len, endpoint->host ());
        }
      else
#endif /* ACE_HAS_IPV6 */
      if (!(out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
          || !(out_cdr << endpoint->host ())
          || !(out_cdr << endpoint->port ()))
        return -1;

      IOP::TaggedComponent tagged_component;
      tagged_component.tag = IOP::TAG_ALTERNATE_IIOP_ADDRESS;

      size_t length = out_cdr.total_length ();
      tagged_component.component_data.length
        (static_cast<CORBA::ULong>(length));
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
    }
  return  0;
}

int
TAO_IIOP_Profile::encode_endpoints (void)
{
  CORBA::ULong actual_count = 0;

  const TAO_IIOP_Endpoint *endpoint = &this->endpoint_;

  // Count the number of endpoints that needs to be encoded
  for (CORBA::ULong c = 0;
       c != this->count_;
       ++c)
    {
      if (endpoint->is_encodable_)
        ++actual_count;

      endpoint = endpoint->next_;
    }

  // Create a data structure and fill it with endpoint info for wire
  // transfer.
  // We include information for the head of the list
  // together with other endpoints because even though its addressing
  // info is transmitted using standard ProfileBody components, its
  // priority is not!

  TAO::IIOPEndpointSequence endpoints;
  endpoints.length (actual_count);

  endpoint = &this->endpoint_;

  for (CORBA::ULong i = 0;
       i < actual_count;
       ++i)
    {
      if (endpoint->is_encodable_)
        {
#if defined (ACE_HAS_IPV6)
          if (endpoint->is_ipv6_decimal_)
            {
              // Don't publish scopeid if included.
              ACE_CString tmp(endpoint->host ());
              ACE_CString::size_type pos = tmp.find('%');
              if (pos != ACE_CString::npos)
                {
                  tmp = tmp.substr (0, pos + 1);
                  tmp[pos] = '\0';
                  endpoints[i].host = tmp.c_str();
                }
              else
                endpoints[i].host = tmp.c_str();
            }
          else
#endif /* ACE_HAS_IPV6 */
          endpoints[i].host = endpoint->host ();
          endpoints[i].port = endpoint->port ();
          endpoints[i].priority = endpoint->priority ();
        }
      endpoint = endpoint->next_;
    }

  // Encode the data structure.
  TAO_OutputCDR out_cdr;
  if (!(out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
      || !(out_cdr << endpoints))
    return -1;

  this->set_tagged_components (out_cdr);

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

      TAO_InputCDR in_cdr (reinterpret_cast<const char *> (buf),
                           tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if (!(in_cdr >> ACE_InputCDR::to_boolean (byte_order)))
        return -1;
      in_cdr.reset_byte_order (static_cast<int> (byte_order));

      // Extract endpoints sequence.
      TAO::IIOPEndpointSequence endpoints;

      if (!(in_cdr >> endpoints))
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

  // Now decode if there are any TAG_ALTERNATE_IIOP_ADDRESS
  // components.

  IOP::MultipleComponentProfile& tc = this->tagged_components_.components();
  for (CORBA::ULong index = 0; index < tc.length(); index++)
    {
      if (tc[index].tag != IOP::TAG_ALTERNATE_IIOP_ADDRESS)
        continue;
      const CORBA::Octet *buf =
        tc[index].component_data.get_buffer ();

      TAO_InputCDR in_cdr (reinterpret_cast<const char*>(buf),
                           tc[index].component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if (!(in_cdr >> ACE_InputCDR::to_boolean (byte_order)))
        return -1;

      in_cdr.reset_byte_order (static_cast<int>(byte_order));

      CORBA::String_var host;
      CORBA::Short port;

      if (!(in_cdr >> host.out()) || !(in_cdr >> port))
        return -1;

      TAO_IIOP_Endpoint *endpoint = 0;
      ACE_NEW_RETURN (endpoint,
                      TAO_IIOP_Endpoint (host.in(),
                                         port,
                                         TAO_INVALID_PRIORITY),
                      -1);

      this->add_endpoint (endpoint);
    }

  return 0;
}
//@@ TAO_PROFILE_SPL_COPY_HOOK_END

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
