// $Id$

#include "tao/Strategies/COIOP_Profile.h"

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Strategies/COIOP_EndpointsC.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

static const char the_prefix[] = "niop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char TAO_COIOP_Profile::object_key_delimiter_ = '/';

char
TAO_COIOP_Profile::object_key_delimiter (void) const
{
  return TAO_COIOP_Profile::object_key_delimiter_;
}

TAO_COIOP_Profile::TAO_COIOP_Profile (const ACE_Utils::UUID& uuid,
                                      const TAO::ObjectKey &object_key,
                                      const TAO_GIOP_Message_Version &version,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_COIOP_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (uuid),
    count_ (1)
{
}

TAO_COIOP_Profile::TAO_COIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_COIOP_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1)
{
}

TAO_COIOP_Profile::~TAO_COIOP_Profile (void)
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
TAO_COIOP_Profile::decode_profile (TAO_InputCDR& cdr)
{
  // @@ NOTE: This code is repeated thrice. Need to factor out in a
  // better manner.
  // Decode uuid into the <endpoint_>.
  CORBA::String_var uuid;
  if (cdr.read_string (uuid.out ()) == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) COIOP_Profile::decode - ")
                    ACE_TEXT ("error while uuid")));
      return -1;
    }

  if (cdr.good_bit ())
    {
      this->endpoint_.uuid_.from_string (uuid.in());

      return 1;
    }

  return -1;
}

void
TAO_COIOP_Profile::parse_string_i (const char *ior)
{
  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...
  const char *okd =
    ACE_OS::strchr (ior, this->object_key_delimiter_);

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
  CORBA::ULong length_host = okd - ior;

  CORBA::String_var tmp = CORBA::string_alloc (length_host);

  // Skip the trailing '/'
  ACE_OS::strncpy (tmp.inout (), ior, length_host);
  tmp[length_host] = '\0';

  this->endpoint_.uuid_.from_string (tmp._retn ());

  TAO::ObjectKey ok;
  TAO::ObjectKey::decode_string_to_sequence (ok,
                                             okd + 1);

  (void) this->orb_core ()->object_key_table ().bind (ok,
                                                      this->ref_object_key_);
}

CORBA::Boolean
TAO_COIOP_Profile::do_is_equivalent (const TAO_Profile *other_profile)
{
  const TAO_COIOP_Profile *op =
    dynamic_cast<const TAO_COIOP_Profile *> (other_profile);

  // Check endpoints equivalence.
  const TAO_COIOP_Endpoint *other_endp = &op->endpoint_;
  for (TAO_COIOP_Endpoint *endp = &this->endpoint_;
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
TAO_COIOP_Profile::hash (CORBA::ULong max)
{
  // Get the hashvalue for all endpoints.
  CORBA::ULong hashval = 0;
  for (TAO_COIOP_Endpoint *endp = &this->endpoint_;
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
TAO_COIOP_Profile::endpoint (void)
{
  return &this->endpoint_;
}

CORBA::ULong
TAO_COIOP_Profile::endpoint_count (void) const
{
  return this->count_;
}

void
TAO_COIOP_Profile::add_endpoint (TAO_COIOP_Endpoint *endp)
{
  endp->next_ = this->endpoint_.next_;
  this->endpoint_.next_ = endp;

  this->count_++;
}

char *
TAO_COIOP_Profile::to_string (void)
{
  CORBA::String_var key;
  TAO::ObjectKey::encode_sequence_to_string (key.inout(),
                                            this->ref_object_key_->object_key ());

  const ACE_CString * uuidstr = this->endpoint_.uuid_.to_string ();
  size_t buflen = (8 /* "corbaloc" */ +
                   1 /* colon separator */ +
                   ACE_OS::strlen (::the_prefix) +
                   1 /* colon separator */ +
                   1 /* major version */ +
                   1 /* decimal point */ +
                   1 /* minor version */ +
                   1 /* `@' character */ +
                   uuidstr->length() +
                   1 /* colon separator */ +
                   5 /* port number */ +
                   1 /* object key separator */ +
                   ACE_OS::strlen (key.in ()));

  char * buf = CORBA::string_alloc (static_cast<CORBA::ULong> (buflen));

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "corbaloc:%s:%c.%c@%s%c%s",
                   ::the_prefix,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   uuidstr->c_str (),
                   this->object_key_delimiter_,
                   key.in ());

  return buf;
}

const char *
TAO_COIOP_Profile::prefix (void)
{
  return ::the_prefix;
}

void
TAO_COIOP_Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING hostname from profile
  const ACE_CString * uuid_str = this->endpoint_.uuid_.to_string();
  encap.write_string (uuid_str->c_str ());

  // OCTET SEQUENCE for object key
  if (this->ref_object_key_)
    encap << this->ref_object_key_->object_key ();
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) TAO - COIOP_Profile::create_profile_body "
                  "no object key marshalled\n"));
    }

  if (this->version_.major > 1
      || this->version_.minor > 0)
    this->tagged_components ().encode (encap);
}

int
TAO_COIOP_Profile::encode_endpoints (void)
{
  // Create a data structure and fill it with endpoint info for wire
  // transfer.
  // We include information for the head of the list
  // together with other endpoints because even though its addressing
  // info is transmitted using standard ProfileBody components, its
  // priority is not!

  TAO::COIOPEndpointSequence endpoints;
  endpoints.length (this->count_);

  const TAO_COIOP_Endpoint *endpoint = &this->endpoint_;
  for (CORBA::ULong i = 0;
       i < this->count_;
       ++i)
    {
      const ACE_CString* uuidstr = endpoint->uuid_.to_string();
      CORBA::String_var uuid (uuidstr->c_str());
      endpoints[i].uuid = uuid;
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
TAO_COIOP_Profile::decode_endpoints (void)
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
      TAO::COIOPEndpointSequence endpoints;

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
          TAO_COIOP_Endpoint *endpoint = 0;
          CORBA::String_var strvar = CORBA::string_dup (endpoints[i].uuid);
          ACE_Utils::UUID uuid1 (strvar.in());
          ACE_NEW_RETURN (endpoint,
                          TAO_COIOP_Endpoint (uuid1),
                          -1);

          this->add_endpoint (endpoint);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */
