// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Profile.h"
#include "orbsvcs/PortableGroup/miopconf.h"
#include "tao/CDR.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/target_specification.h"
#include "ace/OS_NS_ctype.h"

#include "orbsvcs/miopC.h"
#include "orbsvcs/PortableGroupC.h"

static const char the_prefix[] = "miop";

// UIPMC doesn't support object keys, so send profiles by default in the GIOP 1.2 target
// specification.
static const CORBA::Short default_addressing_mode_ = TAO_Target_Specification::Profile_Addr;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char TAO_UIPMC_Profile::object_key_delimiter_ = '/';

char
TAO_UIPMC_Profile::object_key_delimiter (void) const
{
  return TAO_UIPMC_Profile::object_key_delimiter_;
}

TAO_UIPMC_Profile::TAO_UIPMC_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (IOP::TAG_UIPMC,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    tagged_profile_ ()
{
    addressing_mode_ = default_addressing_mode_;
}

TAO_UIPMC_Profile::TAO_UIPMC_Profile (const ACE_INET_Addr &addr,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (IOP::TAG_UIPMC,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (addr),
    tagged_profile_ ()
{
    addressing_mode_ = default_addressing_mode_;
}

TAO_UIPMC_Profile::TAO_UIPMC_Profile (const CORBA::Octet class_d_address[4],
                                      CORBA::UShort port,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (IOP::TAG_UIPMC,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (class_d_address, port),
    tagged_profile_ ()
{
    addressing_mode_ = default_addressing_mode_;
}

/*

TAO_UIPMC_Profile::TAO_UIPMC_Profile (const char *string,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIPMC_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    tagged_profile_ ()
{
  this->add_group_component ();
  this->parse_string (string);
  addressing_mode_ = default_addressing_mode_;
}

*/

TAO_UIPMC_Profile::~TAO_UIPMC_Profile (void)
{
}

int
TAO_UIPMC_Profile::decode (TAO_InputCDR& cdr)
{
  // The following is a selective reproduction of TAO_Profile::decode

  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring profiles
  // whose versions we don't understand.
  if (!(cdr.read_octet (this->version_.major)
        && this->version_.major == TAO_DEF_GIOP_MAJOR
        && cdr.read_octet (this->version_.minor)
        && this->version_.minor <= TAO_DEF_GIOP_MINOR))
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Profile::decode - v%d.%d\n"),
                      this->version_.major,
                      this->version_.minor));
        }

      return -1;
    }

  // Transport specific details
  if (this->decode_profile (cdr) < 0)
    {
      return -1;
    }

  // UIPMC profiles must have tagged components.
  if (this->tagged_components_.decode (cdr) == 0)
    {
      return -1;
    }

  if (cdr.length () != 0 && TAO_debug_level)
    {
      // If there is extra data in the profile we are supposed to
      // ignore it, but print a warning just in case...
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%d bytes out of %d left after profile data\n"),
                  cdr.length (),
                  encap_len));
    }

  // We don't call ::decode_endpoints because it is implemented
  // as ACE_NOTSUP_RETURN (-1) for this profile

  return 1;
}

int
TAO_UIPMC_Profile::decode_endpoints (void)
{
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_UIPMC_Profile::decode_profile (TAO_InputCDR& cdr)
{
  CORBA::UShort port = 0;
  ACE_CString address;
  if (!(cdr.read_string (address)
        && cdr.read_ushort (port)))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - UIPMC_Profile::decode - ")
                    ACE_TEXT ("Couldn't unmarshal address and port!\n")));
      return -1;
    }

  if (cdr.good_bit ())
    {
      // If everything was successful, update the endpoint's address
      // and port with the new data.
      ACE_INET_Addr addr (port, address.c_str ());
      this->endpoint_.object_addr (addr);
      return 1;
    }

  return -1;
}

void
TAO_UIPMC_Profile::parse_string (const char *string)
{
  this->parse_string_i (string);
}

void
TAO_UIPMC_Profile::parse_string_i (const char *string)
{
  // Remove the "N.n@" version prefix, if it exists, and verify the
  // version is one that we accept.

  // Check for MIOP version
  if (ACE_OS::ace_isdigit (string [0]) &&
      string[1] == '.' &&
      ACE_OS::ace_isdigit (string [2]) &&
      string[3] == '@')
    {
      if (string[0] != '1' ||
          string[2] != '0')
        {
          throw CORBA::INV_OBJREF (
            CORBA::SystemException::_tao_minor_code (
              TAO::VMCID,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      string += 4;
      // Skip over the "N.n@"
    }

  // UIPMC profiles always use GIOP 1.2
  this->version_.set_version (1, 2);

  //
  // Parse the group_id.
  //

  // Parse the group component version.
  if (ACE_OS::ace_isdigit (string [0]) &&
      string[1] == '.' &&
      ACE_OS::ace_isdigit (string [2]) &&
      string[3] == '-')
    {
      CORBA::Char major;
      CORBA::Char minor;

      major = (char) (string [0] - '0');
      minor = (char) (string [2] - '0');

      // Verify that a supported version of MIOP is specified.
      if (major != TAO_DEF_MIOP_MAJOR ||
          minor >  TAO_DEF_MIOP_MINOR)
        {
          throw CORBA::INV_OBJREF (
            CORBA::SystemException::_tao_minor_code (
              TAO::VMCID,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      // Skip over "N.n-"
      string += 4;
    }
  else
    {
      // The group component version is mandatory.
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  // Parse the group_domain_id.
  // The Domain ID is terminated with a '-'.

  // Look for the group domain delimitor.
  const char *pos = ACE_OS::strchr (string, '-');

  if (pos == 0)
    {
      // The group_domain_id is mandatory, so throw an
      // exception if it isn't found.
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  // Save the group_domain_id.
  ACE_CString group_domain_id (string, pos - string);

  // Parse the group_id.
  // The group_id is terminated with a '-' or a '/'.

  // Skip past the last '-'.
  string = pos + 1;
  pos = ACE_OS::strchr (string, '-');

  CORBA::Boolean parse_group_ref_version_flag = 0;

  if (pos != 0)
    {
      // String was terminated by a '-', so there's a group
      // reference version to be parsed.
      parse_group_ref_version_flag = 1;
    }
  else
    {
      // Look for a slash as the separator.
      pos = ACE_OS::strchr (string, '/');

      if (pos == 0)
        {
          // The Group ID is mandatory, so throw an exception.
          throw CORBA::INV_OBJREF (
            CORBA::SystemException::_tao_minor_code (
              TAO::VMCID,
              EINVAL),
            CORBA::COMPLETED_NO);
        }
    }

  if (ACE_OS::strspn (string, "0123456789") !=
      static_cast<size_t> (pos - string))
    {
      // Throw an exception if it's not a proper number
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  // Get the group_id.
  ACE_CString str_group_id (string, pos - string);

  // Convert the group_id into numerical form.
  PortableGroup::ObjectGroupId group_id =
    ACE_OS::strtoull (str_group_id.c_str (), 0, 10);

  this->has_ref_version_ = false;
  PortableGroup::ObjectGroupRefVersion ref_version = 0;
  if (parse_group_ref_version_flag)
    {
      // Try to find the group version.  It is terminated by a '/'.
      string = pos + 1;
      pos = ACE_OS::strchr (string, '/');
      if (pos == 0)
        {
          // The group version was expected but not found,
          // so throw an exception.
          throw CORBA::INV_OBJREF (
            CORBA::SystemException::_tao_minor_code (
              TAO::VMCID,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      if (ACE_OS::strspn (string, "0123456789") !=
          static_cast<size_t> (pos - string))
        {
          // Throw an exception if it's not a proper number
          throw CORBA::INV_OBJREF (
            CORBA::SystemException::_tao_minor_code (
              TAO::VMCID,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      ACE_CString str_group_ref_ver (string, pos - string);

      ref_version =
        ACE_OS::strtoul (str_group_ref_ver.c_str (), 0, 10);
      this->has_ref_version_ = true;
    }

  // Parse the group multicast address.
  // The multicast address is terminated by a ':'.
  string = pos + 1;
  pos = ACE_OS::strchr (string, ':');

  if (pos == 0)
    {
      // The multicast address is mandatory, so throw an exception,
      // since it wasn't found.
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  ACE_CString mcast_addr;

#if defined (ACE_HAS_IPV6)
  // Check if this is a (possibly) IPv6 supporting profile containing a
  // decimal IPv6 address representation.
  if ((this->version ().major > TAO_MIN_IPV6_IIOP_MAJOR ||
        this->version ().minor >= TAO_MIN_IPV6_IIOP_MINOR) &&
      string[0] == '[')
    {
      // In this case we have to find the end of the numeric address and
      // start looking for the port separator from there.
      pos = ACE_OS::strchr (string, ']');
      if (pos == 0)
        {
          // No valid IPv6 address specified.
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("\nTAO (%P|%t) - UIPMC_Profile: ")
                          ACE_TEXT ("Invalid IPv6 decimal address specified.\n")));
            }

          throw CORBA::INV_OBJREF (
            CORBA::SystemException::_tao_minor_code (
              0,
              EINVAL),
            CORBA::COMPLETED_NO);
        }
      else
        {
          ++string;
          mcast_addr = ACE_CString (string, pos - string);
          string = pos + 2;
        }
    }
  else
    {
#endif /* ACE_HAS_IPV6 */
      mcast_addr = ACE_CString (string, pos - string);
      string = pos + 1;
#if defined (ACE_HAS_IPV6)
    }
#endif /* ACE_HAS_IPV6 */

  size_t mcast_addr_len = mcast_addr.length ();
  if (ACE_OS::strspn (mcast_addr.c_str (),
                      ".:0123456789ABCDEFabcdef") != mcast_addr_len)
    {
      // Throw an exception if it's not a proper IPv4/IPv6 address
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  // Parse the multicast port number.

  // First check that there's something left in the string.
  if (string[0] == '\0')
    {
      // The multicast port is mandatory, so throw an exception,
      // since it wasn't found.
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  // Port can have name thus letters and '-' are allowed.
  const char port_chars[] =
    "-0123456789ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  size_t port_len = ACE_OS::strlen (string);
  if (ACE_OS::strspn (string, port_chars) != port_len)
    {
      // Throw an exception if it's not a proper port
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  ACE_INET_Addr ia;
  if (ia.string_to_addr (string) == -1)
    {
      throw CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  u_short mcast_port = ia.get_port_number ();

  //
  // Finally, set all of the fields of the profile.
  //

  ACE_INET_Addr addr (mcast_port, mcast_addr.c_str ());
  this->endpoint_.object_addr (addr);

  this->set_group_info (group_domain_id.c_str (),
                        group_id,
                        ref_version);
}

CORBA::Boolean
TAO_UIPMC_Profile::do_is_equivalent (const TAO_Profile *other_profile)
{
  const TAO_UIPMC_Profile *op =
    dynamic_cast<const TAO_UIPMC_Profile *> (other_profile);

  if (op == 0)
    return 0;

  return this->endpoint_.is_equivalent (&op->endpoint_);
}

CORBA::ULong
TAO_UIPMC_Profile::hash (CORBA::ULong max)
{
  // Get the hashvalue for all endpoints.
  CORBA::ULong hashval = this->endpoint_.hash ();

  hashval += this->version_.minor;
  hashval += this->tag ();

  return hashval % max;
}

TAO_Endpoint*
TAO_UIPMC_Profile::endpoint (void)
{
  return &this->endpoint_;
}

int
TAO_UIPMC_Profile::encode_endpoints (void)
{
  return 1;
}

CORBA::ULong
TAO_UIPMC_Profile::endpoint_count (void) const
{
  return 1;
}

char *
TAO_UIPMC_Profile::to_string (void)
{
  // corbaloc:miop:1.2@1.0-group_id-1-1/host:port

  size_t buflen = (8 /* "corbaloc" */ +
                   1 /* colon separator */ +
                   ACE_OS::strlen (::the_prefix) + /* "miop" */
                   1 /* colon separator */ +
                   1 /* major version */ +
                   1 /* decimal point */ +
                   1 /* minor version */ +
                   1 /* `@' character */ +
                   1 /* component major version */ +
                   1 /* decimal point */ +
                   1 /* component minor version */ +
                   1 /* `-' character */ +
                   this->group_domain_id_.length () + /* domain id */
                   1 /* `-' character */ +
                   20 /* group id */ +
                   1 /* `-' character */ +
                   10 /* group reference version */ +
                   1 /* `/' character */ +
                   39 /* IPv4/IPv6 address */ +
                   1 /* colon separator */ +
                   5 /* port number */);
#if defined (ACE_HAS_IPV6)
  if (this->endpoint_.object_addr ().get_type () == AF_INET6)
    buflen += 2; // room for '[' and ']'
#endif /* ACE_HAS_IPV6 */

  static const char digits [] = "0123456789";

  char * buf = CORBA::string_alloc (static_cast<CORBA::ULong> (buflen));

  ACE_OS::sprintf (buf,
                   "corbaloc:%s:%c.%c@%c.%c-%s-%u",
                   ::the_prefix,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   digits [TAO_DEF_MIOP_MAJOR],
                   digits [TAO_DEF_MIOP_MINOR],
                   this->group_domain_id_.c_str (),
                   this->group_id_);

  if (this->has_ref_version_)
    {
      ACE_OS::sprintf (&buf[ACE_OS::strlen (buf)],
                       "-%u",
                       this->ref_version_);
    }

#if defined (ACE_HAS_IPV6)
  if (this->endpoint_.object_addr ().get_type () == AF_INET6)
    {
      ACE_OS::sprintf (&buf[ACE_OS::strlen (buf)],
                       "/[%s]:%d",
                       this->endpoint_.host (),
                       this->endpoint_.port ());
    }
  else
#endif /* ACE_HAS_IPV6 */
  ACE_OS::sprintf (&buf[ACE_OS::strlen (buf)],
                   "/%s:%d",
                   this->endpoint_.host (),
                   this->endpoint_.port ());

  return buf;
}

const char *
TAO_UIPMC_Profile::prefix (void)
{
  return ::the_prefix;
}

IOP::TaggedProfile &
TAO_UIPMC_Profile::create_tagged_profile (void)
{
  // Check whether we have already created the TaggedProfile
  if (this->tagged_profile_.profile_data.length () == 0)
    {
      // As we have not created we will now create the TaggedProfile
      this->tagged_profile_.tag = IOP::TAG_UIPMC;

      // Create the encapsulation....
      TAO_OutputCDR encap;

      // Create the profile body
      this->create_profile_body (encap);

      CORBA::ULong length =
        static_cast<CORBA::ULong> (encap.total_length ());

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
TAO_UIPMC_Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  // Note: Only GIOP 1.2 and above are supported currently for MIOP.
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // Address.
  encap.write_string (this->endpoint_.host ());

  // Port number.
  encap.write_ushort (this->endpoint_.port ());

  // UIPMC is only supported by versions of GIOP that have tagged components,
  // so unconditionally encode the components.
  this->tagged_components ().encode (encap);
}

/*
int
TAO_UIPMC_Profile::decode_endpoints (void)
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
      TAO_UIPMCEndpointSequence endpoints;

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
          TAO_UIPMC_Endpoint *endpoint = 0;
          ACE_NEW_RETURN (endpoint,
                          TAO_UIPMC_Endpoint (endpoints[i].host,
                                             endpoints[i].port,
                                             endpoints[i].priority),
                          -1);

          this->add_endpoint (endpoint);
        }
    }

  return 0;
}
*/

void
TAO_UIPMC_Profile::set_group_info (const char *domain_id,
                                   PortableGroup::ObjectGroupId group_id,
                                   PortableGroup::ObjectGroupRefVersion ref_version)
{
  // First, record the group information.
  this->group_domain_id_.set (domain_id);
  this->group_id_ = group_id;
  this->ref_version_ = ref_version;

  // Update the cached version of the group component.
  this->update_cached_group_component ();
}

void
TAO_UIPMC_Profile::update_cached_group_component (void)
{
  PortableGroup::TagGroupTaggedComponent group;

  // Encode the data structure.
  group.component_version.major = TAO_DEF_MIOP_MAJOR;
  group.component_version.minor = TAO_DEF_MIOP_MINOR;

  group.group_domain_id = CORBA::string_dup (this->group_domain_id_.c_str ());
  group.object_group_id = this->group_id_;
  group.object_group_ref_version = this->ref_version_;

  TAO_OutputCDR out_cdr;

  // Write the byte order.
  out_cdr << ACE_OutputCDR::from_boolean (ACE_CDR_BYTE_ORDER);

  // Write the group information.
  if ((out_cdr << group) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error marshaling group component!"));
      return;
    }

  size_t length = out_cdr.total_length ();

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_GROUP;
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
  this->tagged_components_.set_component (tagged_component);
}

void
TAO_UIPMC_Profile::request_target_specifier (
                      TAO_Target_Specification &target_spec,
                      TAO_Target_Specification::TAO_Target_Address required_type)
{
  // Fill out the target specifier based on the required type.
  switch (required_type)
    {
    case TAO_Target_Specification::Profile_Addr:

      // Only using a profile as the target specifier is supported
      // at this time.  Object keys are strictly not supported since
      // UIPMC profiles do not have object keys.
      target_spec.target_specifier (
            this->create_tagged_profile ());
      break;

    case TAO_Target_Specification::Key_Addr:
    case TAO_Target_Specification::Reference_Addr:
    default:
      // Unsupported or unknown required type.  Throw an exception.
      throw CORBA::MARSHAL ();
    }
}

int
TAO_UIPMC_Profile::supports_multicast (void) const
{
  // Yes!  We support multicast!
  return 1;
}

void
TAO_UIPMC_Profile::addressing_mode (CORBA::Short addr_mode)
{
  // ** See race condition note about addressing mode in Profile.h **
  switch (addr_mode)
    {
    case TAO_Target_Specification::Profile_Addr:
    case TAO_Target_Specification::Reference_Addr:
      this->addressing_mode_ = addr_mode;
      break;

    case TAO_Target_Specification::Key_Addr:
      // There is no object key, so it is not supported.

    default:
      throw CORBA::BAD_PARAM (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);
    }
}

int
TAO_UIPMC_Profile::extract_group_component (const IOP::TaggedProfile &profile,
                                            PortableGroup::TagGroupTaggedComponent &group)
{
  // Create the decoding stream from the encapsulation in the buffer,
//#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
//  TAO_InputCDR cdr (profile.profile_data.mb ());
//#else
  TAO_InputCDR cdr (reinterpret_cast<const char*> (profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
//#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  // Extract the Byte Order.
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;
  cdr.reset_byte_order (static_cast<int> (byte_order));

  // Read and verify major, minor versions, ignoring UIPMC profiles
  // whose versions we don't understand.
  CORBA::Octet major;
  CORBA::Octet minor = CORBA::Octet();

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major)
        && cdr.read_octet (minor)))
  {
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - UIPMC_Profile::extract_group_component - v%d.%d\n"),
                    major,
                    minor));
      }
    return -1;
  }

  // Decode the endpoint.
  ACE_CString address;
  CORBA::UShort port;

  if (!(cdr.read_string (address)
        && cdr.read_ushort (port)))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - UIPMC_Profile::extract_group_component - Couldn't unmarshal address and port!\n")));
      return -1;
    }

  TAO_Tagged_Components tagged_components;
  if (tagged_components.decode (cdr) == 0)
    return -1;

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_GROUP;

  // Try to find it.
  if (tagged_components.get_component (tagged_component) == 0)
    return -1;

  // Found it.
  const CORBA::Octet *buf =
    tagged_component.component_data.get_buffer ();

  TAO_InputCDR in_cdr (reinterpret_cast<const char*> (buf),
                       tagged_component.component_data.length ());

  // Extract the Byte Order.
  if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;
  in_cdr.reset_byte_order (static_cast<int> (byte_order));

  if ((in_cdr >> group) == 0)
    return -1;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
