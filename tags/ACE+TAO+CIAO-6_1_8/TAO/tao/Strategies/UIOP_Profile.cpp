// $Id$

#include "tao/Strategies/UIOP_Profile.h"

#if TAO_HAS_UIOP == 1

#include "tao/Strategies/uiop_endpointsC.h"

#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"

static const char prefix_[] = "uiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char TAO_UIOP_Profile::object_key_delimiter_ = '|';

char
TAO_UIOP_Profile::object_key_delimiter (void) const
{
  return TAO_UIOP_Profile::object_key_delimiter_;
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                                    const TAO::ObjectKey &object_key,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIOP_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (addr),
    count_ (1)
{
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char *,
                                    const TAO::ObjectKey &object_key,
                                    const ACE_UNIX_Addr &addr,
                                    const TAO_GIOP_Message_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIOP_PROFILE,
                 orb_core,
                 object_key,
                 version),
    endpoint_ (addr),
    count_ (1)
{
}

TAO_UIOP_Profile::TAO_UIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIOP_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR,
                                           TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1)
{
}

TAO_UIOP_Profile::~TAO_UIOP_Profile (void)
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
TAO_UIOP_Profile::endpoint (void)
{
  return &this->endpoint_;
}

CORBA::ULong
TAO_UIOP_Profile::endpoint_count (void) const
{
  return this->count_;
}

void
TAO_UIOP_Profile::parse_string_i (const char *string)
{
  if (!string || !*string)
    {
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     0,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  // Remove the "N.n@" version prefix, if it exists, and verify the
  // version is one that we accept.

  // Check for version
  if (ACE_OS::ace_isdigit (string [0]) &&
      string[1] == '.' &&
      ACE_OS::ace_isdigit (string [2]) &&
      string[3] == '@')
    {
      // @@ This may fail for non-ascii character sets [but take that
      // with a grain of salt]
      this->version_.set_version ((char) (string [0] - '0'),
                                  (char) (string [2] - '0'));
      string += 4;
      // Skip over the "N.n@"
    }

  if (this->version_.major != TAO_DEF_GIOP_MAJOR ||
      this->version_.minor  > TAO_DEF_GIOP_MINOR)
    {
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     0,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }


  // Pull off the "rendezvous point" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, this->object_key_delimiter_);

  if (cp == 0)
    {
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO::VMCID,
                     EINVAL),
                   CORBA::COMPLETED_NO);
      // No rendezvous point specified
    }

  CORBA::ULong length = cp - start;

  CORBA::String_var rendezvous = CORBA::string_alloc (length);

  ACE_OS::strncpy (rendezvous.inout (), start, length);
  rendezvous[length] = '\0';

  if (this->endpoint_.object_addr_.set (rendezvous.in ()) != 0)
    {
      throw ::CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO::VMCID,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  start = ++cp;  // increment past the object key separator

  TAO::ObjectKey ok;
  TAO::ObjectKey::decode_string_to_sequence (ok,
                                             start);

  (void) this->orb_core ()->object_key_table ().bind (ok,
                                                      this->ref_object_key_);
}

CORBA::Boolean
TAO_UIOP_Profile::do_is_equivalent (const TAO_Profile *other_profile)
{
  const TAO_UIOP_Profile *op =
    dynamic_cast <const TAO_UIOP_Profile *> (other_profile);

  if (op == 0)
    return false;

  // Check endpoints equivalence.
  const TAO_UIOP_Endpoint *other_endp = &op->endpoint_;
  for (TAO_UIOP_Endpoint *endp = &this->endpoint_;
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
TAO_UIOP_Profile::hash (CORBA::ULong max)
{
  // Get the hashvalue for all endpoints.
  CORBA::ULong hashval = 0;
  for (TAO_UIOP_Endpoint *endp = &this->endpoint_;
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
TAO_UIOP_Profile::add_endpoint (TAO_UIOP_Endpoint *endp)
{
  endp->next_ = this->endpoint_.next_;
  this->endpoint_.next_ = endp;

  this->count_++;
}


char *
TAO_UIOP_Profile::to_string (void) const
{
  CORBA::String_var key;
  TAO::ObjectKey::encode_sequence_to_string (key.inout(),
                                            this->ref_object_key_->object_key ());

  u_int buflen = (8 /* "corbaloc" */ +
                  1 /* colon separator */ +
                  ACE_OS::strlen (::prefix_) +
                  1 /* colon separator */ +
                  1 /* major version */ +
                  1 /* decimal point */ +
                  1 /* minor version */ +
                  1 /* `@' character */ +
                  ACE_OS::strlen (this->endpoint_.rendezvous_point ()) +
                  1 /* object key separator */ +
                  ACE_OS::strlen (key.in ()));

  char * buf = CORBA::string_alloc (buflen);

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "corbaloc:%s:%c.%c@%s%c%s",
                   ::prefix_,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->endpoint_.rendezvous_point (),
                   this->object_key_delimiter_,
                   key.in ());
  return buf;
}

const char *
TAO_UIOP_Profile::prefix (void)
{
  return ::prefix_;
}

int
TAO_UIOP_Profile::decode_profile (TAO_InputCDR& cdr)
{
  char *rendezvous = 0;

  // Get rendezvous_point
  if (cdr.read_string (rendezvous) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "error decoding UIOP rendezvous_point"));
      return -1;
    }

  if (this->endpoint_.object_addr_.set (rendezvous) == -1)
    {
      // In the case of an ACE_UNIX_Addr, this should call should
      // never fail!
      //
      // If the call fails, allow the profile to be created, and rely
      // on TAO's connection handling to throw the appropriate
      // exception.
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) UIOP_Profile::decode - ")
                      ACE_TEXT ("ACE_UNIX_Addr::set() failed\n")));
        }
    }

  // Clean up
  delete [] rendezvous;

  return 1;
}

void
TAO_UIOP_Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  // CHAR describing byte order, starting the encapsulation
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING rendezvous_pointname from profile
  encap.write_string (this->endpoint_.rendezvous_point ());

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
TAO_UIOP_Profile::encode_endpoints (void)
{
  // Create a data structure and fill it with endpoint info for wire
  // transfer.
  // We include information for the head of the list
  // together with other endpoints because even though its addressing
  // info is transmitted using standard ProfileBody components, its
  // priority is not!
  TAO_UIOPEndpointSequence endpoints;
  endpoints.length (this->count_);

  TAO_UIOP_Endpoint *endpoint = &this->endpoint_;
  for (size_t i = 0;
       i < this->count_;
       ++i)
    {
      endpoints[i].rendezvous_point = endpoint->rendezvous_point ();
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
TAO_UIOP_Profile::decode_endpoints (void)
{
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;

  if (this->tagged_components_.get_component (tagged_component))
    {
      const CORBA::Octet *buf =
        tagged_component.component_data.get_buffer ();

      TAO_InputCDR in_cdr (reinterpret_cast <const char*>(buf),
                           tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return -1;
      in_cdr.reset_byte_order (static_cast<int>(byte_order));

      // Extract endpoints sequence.
      TAO_UIOPEndpointSequence endpoints;

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
          TAO_UIOP_Endpoint *endpoint = 0;
          ACE_NEW_RETURN (endpoint,
                          TAO_UIOP_Endpoint,
                          -1);
          this->add_endpoint (endpoint);
          if (endpoint->object_addr_.set
              (endpoints[i].rendezvous_point)
              == -1)
            {
              // In the case of an ACE_UNIX_Addr, this should call should
              // never fail!
              // If the call fails, allow the profile to be created, and rely
              // on TAO's connection handling to throw the appropriate
              // exception.
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) UIOP_Profile::decode_endpoints - ")
                              ACE_TEXT ("ACE_UNIX_Addr::set() failed\n")));
                }

            }
          endpoint->priority (endpoints[i].priority);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_UIOP == 1 */
