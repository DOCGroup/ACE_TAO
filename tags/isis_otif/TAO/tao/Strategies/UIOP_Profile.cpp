// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "UIOP_Profile.h"

#if TAO_HAS_UIOP == 1

#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "uiop_endpoints.h"

ACE_RCSID (Strategies,
           UIOP_Profile,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "UIOP_Profile.i"
#endif /* __ACE_INLINE__ */

static const char prefix_[] = "uiop";

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

size_t
TAO_UIOP_Profile::endpoint_count (void)
{
  return this->count_;
}

void
TAO_UIOP_Profile::parse_string_i (const char *string
                                ACE_ENV_ARG_DECL)
{
  // Pull off the "rendezvous point" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, this->object_key_delimiter_);

  if (cp == 0)
    {
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
      // No rendezvous point specified
    }

  CORBA::ULong length = cp - start;

  CORBA::String_var rendezvous = CORBA::string_alloc (length);

  ACE_OS::strncpy (rendezvous.inout (), start, length);
  rendezvous[length] = '\0';

  if (this->endpoint_.object_addr_.set (rendezvous.in ()) != 0)
    {
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }

  start = ++cp;  // increment past the object key separator

  TAO::ObjectKey::decode_string_to_sequence (this->object_key_, start);
}

CORBA::Boolean
TAO_UIOP_Profile::is_equivalent (const TAO_Profile *other_profile)
{

  if (other_profile->tag () != TAO_TAG_UIOP_PROFILE)
    return 0;

  const TAO_UIOP_Profile *op =
    ACE_dynamic_cast (const TAO_UIOP_Profile *, other_profile);

  if (!(this->object_key_ == op->object_key_
        && this->version_ == op->version_
        && this->count_ == op->count_))
    return 0;

  // Check endpoints equivalence.
  const TAO_UIOP_Endpoint *other_endp = &op->endpoint_;
  for (TAO_UIOP_Endpoint *endp = &this->endpoint_;
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
TAO_UIOP_Profile::hash (CORBA::ULong max
                        ACE_ENV_ARG_DECL_NOT_USED)
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

  if (this->object_key_.length () >= 4)
    {
      hashval += this->object_key_ [1];
      hashval += this->object_key_ [3];
    }

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
TAO_UIOP_Profile::to_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  CORBA::String_var key;
  TAO::ObjectKey::encode_sequence_to_string (key.inout(),
                                            this->object_key_);

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
  encap << this->object_key_;

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

      TAO_InputCDR in_cdr (ACE_reinterpret_cast (const char*, buf),
                           tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return -1;
      in_cdr.reset_byte_order (ACE_static_cast(int, byte_order));

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

#endif  /* TAO_HAS_UIOP == 1 */
