// $Id$

#include "tao/Strategies/SHMIOP_Profile.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/IIOP_EndpointsC.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_netdb.h"
#include "ace/os_include/os_netdb.h"

static const char prefix_[] = "shmiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char TAO_SHMIOP_Profile::object_key_delimiter_ = '/';

char
TAO_SHMIOP_Profile::object_key_delimiter (void) const
{
  return TAO_SHMIOP_Profile::object_key_delimiter_;
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (const ACE_MEM_Addr &addr,
                                        const TAO::ObjectKey &object_key,
                                        const TAO_GIOP_Message_Version &version,
                                        TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_SHMEM_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (addr,
               orb_core->orb_params ()->use_dotted_decimal_addresses ()),
    count_ (1)
{
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (const char* host,
                                        CORBA::UShort port,
                                        const TAO::ObjectKey &object_key,
                                        const ACE_INET_Addr &addr,
                                        const TAO_GIOP_Message_Version &version,
                                        TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_SHMEM_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (host, port, addr),
    count_ (1)
{
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_SHMEM_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1)
{
}

TAO_SHMIOP_Profile::~TAO_SHMIOP_Profile (void)
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

TAO_Endpoint*
TAO_SHMIOP_Profile::endpoint (void)
{
  return &this->endpoint_;
}

CORBA::ULong
TAO_SHMIOP_Profile::endpoint_count (void) const
{
  return this->count_;
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_SHMIOP_Profile::decode_profile (TAO_InputCDR& cdr)
{
  // Decode host and port into the <endpoint_>.
  if (cdr.read_string (this->endpoint_.host_.out ()) == 0
      || cdr.read_ushort (this->endpoint_.port_) == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) SHMIOP_Profile::decode - ")
                      ACE_TEXT ("error while decoding host/port")));
        }
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
TAO_SHMIOP_Profile::parse_string_i (const char *string
                                    )
{
  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, ':');  // Look for a port

  if (cp == 0)
    {
      // No host/port delimiter!
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO::VMCID,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  char *okd = ACE_OS::strchr (start, this->object_key_delimiter_);

  if (okd == 0)
    {
      // No object key delimiter!
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO::VMCID,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  // Don't increment the pointer 'cp' directly since we still need
  // to use it immediately after this block.

  CORBA::ULong length = okd - (cp + 1);
  // Don't allocate space for the colon ':'.

  CORBA::String_var tmp = CORBA::string_alloc (length);

  ACE_OS::strncpy (tmp.inout (), cp + 1, length);
  tmp[length] = '\0';

  if (ACE_OS::strspn (tmp.in (), "1234567890") == length)
    {
      this->endpoint_.port_ =
        static_cast <CORBA::UShort> (ACE_OS::atoi (tmp.in ()));
    }
  else
    {
      ACE_INET_Addr ia;
      if (ia.string_to_addr (tmp.in ()) == -1)
        {
          throw ::CORBA::INV_OBJREF (
              CORBA::SystemException::_tao_minor_code (
                  TAO::VMCID,
                  EINVAL),
              CORBA::COMPLETED_NO);
        }
      else
        {
          this->endpoint_.port_ = ia.get_port_number ();
        }
    }

  length = cp - start;

  tmp = CORBA::string_alloc (length);

  ACE_OS::strncpy (tmp.inout (), start, length);
  tmp[length] = '\0';

  this->endpoint_.host_ = tmp._retn ();

  ACE_INET_Addr host_addr;

  if (ACE_OS::strcmp (this->endpoint_.host_.in (), "") == 0)
    {
      char tmp_host [MAXHOSTNAMELEN + 1];

      // If no host is specified: assign the default host : the local host.
      if (host_addr.get_host_name (tmp_host,
                                   sizeof (tmp_host)) != 0)
      {
        const char *tmp = host_addr.get_host_addr ();
        if (tmp == 0)
          {
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("\n\nTAO (%P|%t) ")
                          ACE_TEXT ("SHMIOP_Profile::parse_string ")
                          ACE_TEXT ("- %p\n\n"),
                          ACE_TEXT ("cannot determine hostname")));

            // @@ What's the right exception to throw here?
            throw ::CORBA::INV_OBJREF (
                         CORBA::SystemException::_tao_minor_code (
                           TAO::VMCID,
                           EINVAL),
                         CORBA::COMPLETED_NO);
          }
        else
          this->endpoint_.host_ = tmp;
      }
      else
        {
          this->endpoint_.host_ = (const char *) tmp_host;
        }
    }

  if (this->endpoint_.object_addr_.set (this->endpoint_.port_,
                                        this->endpoint_.host_.in ()) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) SHMIOP_Profile::parse_string () -\n")
                      ACE_TEXT ("TAO (%P|%t) ACE_INET_Addr::set () failed")));
        }

      // @@ What's the right exception to throw here?
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO::VMCID,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  TAO::ObjectKey ok;
  TAO::ObjectKey::decode_string_to_sequence (ok,
                                             okd + 1);  // increment past the object key separator

  (void) this->orb_core ()->object_key_table ().bind (ok,
                                                      this->ref_object_key_);
}

CORBA::Boolean
TAO_SHMIOP_Profile::do_is_equivalent (const TAO_Profile *other_profile)
{
  const TAO_SHMIOP_Profile *op =
    dynamic_cast <const TAO_SHMIOP_Profile *> (other_profile);

  if (op == 0)
    return 0;

  // Check endpoints equivalence.
  const TAO_SHMIOP_Endpoint *other_endp = &op->endpoint_;
  for (TAO_SHMIOP_Endpoint *endp = &this->endpoint_;
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
TAO_SHMIOP_Profile::hash (CORBA::ULong max
                          )
{
  // Get the hashvalue for all endpoints.
  CORBA::ULong hashval = 0;
  for (TAO_SHMIOP_Endpoint *endp = &this->endpoint_;
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

void
TAO_SHMIOP_Profile::add_endpoint (TAO_SHMIOP_Endpoint *endp)
{
  endp->next_ = this->endpoint_.next_;
  this->endpoint_.next_ = endp;

  this->count_++;
}

char *
TAO_SHMIOP_Profile::to_string (void)
{
  CORBA::String_var key;
  TAO::ObjectKey::encode_sequence_to_string (key.inout(),
                                             this->ref_object_key_->object_key ());

  size_t buflen = (8 /* corbaloc */ +
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

  char * buf = CORBA::string_alloc (static_cast <CORBA::ULong> (buflen));

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
TAO_SHMIOP_Profile::prefix (void)
{
  return ::prefix_;
}

void
TAO_SHMIOP_Profile::create_profile_body (TAO_OutputCDR &encap) const
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
  if (this->ref_object_key_)
    encap << this->ref_object_key_->object_key ();
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) TAO - UIOP_Profile::create_profile_body "
                  "no object key marshalled\n"));
    }

  if (this->version_.major > 1
      || this->version_.minor > 0)
    this->tagged_components ().encode (encap);
}

int
TAO_SHMIOP_Profile::encode_endpoints (void)
{
  // Create a data structure and fill it with endpoint info for wire
  // transfer.
  // We include information for the head of the list
  // together with other endpoints because even though its addressing
  // info is transmitted using standard ProfileBody components, its
  // priority is not!
  TAO::IIOPEndpointSequence endpoints;
  endpoints.length (this->count_);

  TAO_SHMIOP_Endpoint *endpoint = &this->endpoint_;
  for (CORBA::ULong i = 0;
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
  size_t length = out_cdr.total_length ();

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;
  tagged_component.component_data.length (static_cast <CORBA::ULong>(length));
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

  return 0;
}

int
TAO_SHMIOP_Profile::decode_endpoints (void)
{
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;

  if (this->tagged_components_.get_component (tagged_component))
    {
      const CORBA::Octet *buf =
        tagged_component.component_data.get_buffer ();

      TAO_InputCDR in_cdr (reinterpret_cast <const char*> (buf),
                           tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return -1;
      in_cdr.reset_byte_order (static_cast<int> (byte_order));

      // Extract endpoints sequence.
      TAO::IIOPEndpointSequence endpoints;

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
          TAO_SHMIOP_Endpoint *endpoint = 0;
          ACE_NEW_RETURN (endpoint,
                          TAO_SHMIOP_Endpoint (endpoints[i].host,
                                               endpoints[i].port,
                                               endpoints[i].priority),
                          -1);

          this->add_endpoint (endpoint);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
