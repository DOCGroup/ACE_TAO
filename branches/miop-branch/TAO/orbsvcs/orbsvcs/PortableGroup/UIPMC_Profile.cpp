// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "UIPMC_Profile.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/target_specification.h"

#include "orbsvcs/miopC.h"
#include "orbsvcs/PortableGroupC.h"

ACE_RCSID(tao, UIPMC_Profile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "UIPMC_Profile.i"
#endif /* __ACE_INLINE__ */

static const char prefix_[] = "uipmc";

// UIPMC doesn't support object keys, so send profiles by default in the GIOP 1.2 target
// specification.
static const CORBA::Short default_addressing_mode_ = TAO_Target_Specification::Profile_Addr;

const char TAO_UIPMC_Profile::object_key_delimiter_ = '/';

char
TAO_UIPMC_Profile::object_key_delimiter (void) const
{
  return TAO_UIPMC_Profile::object_key_delimiter_;
}


TAO_UIPMC_Profile::TAO_UIPMC_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIPMC_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1),
    tagged_profile_ ()
{
    addressing_mode_ = default_addressing_mode_;
}

TAO_UIPMC_Profile::TAO_UIPMC_Profile (const ACE_INET_Addr &addr,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIPMC_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (addr),
    count_ (1),
    tagged_profile_ ()
{
    addressing_mode_ = default_addressing_mode_;
}

TAO_UIPMC_Profile::TAO_UIPMC_Profile (const CORBA::Octet class_d_address[4],
                                      CORBA::UShort port,
                                      TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIPMC_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (class_d_address, port),
    count_ (1),
    tagged_profile_ ()
{
    addressing_mode_ = default_addressing_mode_;
}

/*

TAO_UIPMC_Profile::TAO_UIPMC_Profile (const char *string,
                                      TAO_ORB_Core *orb_core
                                      TAO_ENV_ARG_DECL)
  : TAO_Profile (TAO_TAG_UIPMC_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1),
    tagged_profile_ ()
{
  this->add_group_component ();
  this->parse_string (string TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  addressing_mode_ = default_addressing_mode_;
}

TAO_UIPMC_Profile::TAO_UIPMC_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_UIPMC_PROFILE,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    endpoint_ (),
    count_ (1),
    tagged_profile_ ()
{
  addressing_mode_ = default_addressing_mode_;
}
*/

TAO_UIPMC_Profile::~TAO_UIPMC_Profile (void)
{
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.

int
TAO_UIPMC_Profile::decode (TAO_InputCDR& cdr)
{
  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring UIPMC profiles
  // whose versions we don't understand.
  CORBA::Octet major = 0, minor = 0;

  if (!(cdr.read_octet (major)
        && major == TAO_DEF_MIOP_MAJOR
        && cdr.read_octet (minor)))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) UIPMC_Profile::decode - v%d.%d\n"),
                    major,
                    minor));
      return -1;
    }

  this->version_.major = major;

  if (minor <= TAO_DEF_MIOP_MINOR)
    this->version_.minor = minor;

  // Decode the endpoint.
  ACE_CString address;
  CORBA::UShort port;
  
  if (!(cdr.read_string (address)
        && cdr.read_ushort (port)))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) UIPMC_Profile::decode - Couldn't unmarshal address and port!\n")));
      return -1;
    }

  if (this->tagged_components_.decode (cdr) == 0)
    return -1;

  if (cdr.length () != 0 && TAO_debug_level)
    // If there is extra data in the profile we are supposed to
    // ignore it, but print a warning just in case...
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%d bytes out of %d left after UIPMC profile data\n"),
                cdr.length (),
                encap_len));

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
TAO_UIPMC_Profile::parse_string (const char *string
                                 TAO_ENV_ARG_DECL)
{
  // Remove the "N.n@" version prefix, if it exists, and verify the
  // version is one that we accept.

  // Check for MIOP version
  if (isdigit (string [0]) &&
      string[1] == '.' &&
      isdigit (string [2]) &&
      string[3] == '@')
    {
      if (string[0] != '1' ||
          string[2] != '0') 
        {
          ACE_THROW (CORBA::INV_OBJREF (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO));
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
  if (isdigit (string [0]) &&
      string[1] == '.' &&
      isdigit (string [2]) &&
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
          ACE_THROW (CORBA::INV_OBJREF (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO));
        }

      // Skip over "N.n-"
      string += 4;
    }
  else
    {
      // The group component version is mandatory.
      ACE_THROW (CORBA::INV_OBJREF (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        EINVAL),
                      CORBA::COMPLETED_NO));
    }

  // Parse the group_domain_id.
  // The Domain ID is terminated with a '-'.

  // Wrap the string in a ACE_CString
  ACE_CString ace_str (string, 0, 0);

  // Look for the group domain delimitor.
  int pos = ace_str.find ('-');

  if (pos == ACE_CString::npos)
    {
      // The group_domain_id is mandatory, so throw an 
      // exception if it isn't found.
      ACE_THROW (CORBA::INV_OBJREF (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        EINVAL),
                      CORBA::COMPLETED_NO));
    }

  // Save the group_domain_id.
  ACE_CString group_domain_id = ace_str.substring (0, pos);

  // Parse the group_id.
  // The group_id is terminated with a '-' or a '/'.
  
  // Skip past the last '-'.
  pos++;  
  int end_pos = ace_str.find ('-',pos);

  CORBA::Boolean parse_group_ref_version_flag = 0;

  if (end_pos != ACE_CString::npos)
    {
      // String was terminated by a '-', so there's a group
      // reference version to be parsed.
      parse_group_ref_version_flag = 1;
    }
  else
    {
      // Look for a slash as the separator.
      end_pos = ace_str.find ('/', pos);

      if (end_pos == ACE_CString::npos)
        {
          // The Group ID is mandatory, so throw an exception.
          ACE_THROW (CORBA::INV_OBJREF (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO));
        }
    }

  // Get the domain_id.
  ACE_CString str_domain_id = ace_str.substring (pos, end_pos - pos);

  // Convert the domain_id into numerical form.
  // @@ group_id is actually 64 bits, but strtoul only can parse 32 bits.
  // @@ Need a 64 bit strtoul...
  PortableGroup::ObjectGroupId group_id = 
    ACE_OS::strtoul (str_domain_id.c_str (), 0, 10);

  PortableGroup::ObjectGroupRefVersion ref_version = 0;
  if (parse_group_ref_version_flag)
    {
      // Try to find the group version.  It is terminated by a '/'.
      pos = end_pos + 1;
      end_pos = ace_str.find ('/', pos);
      if (end_pos == ACE_CString::npos)
        {
          // The group version was expected but not found,
          // so throw an exception.
          ACE_THROW (CORBA::INV_OBJREF (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO));
        }

      ACE_CString str_group_ref_ver = ace_str.substring (pos, end_pos - pos);

      ref_version = 
        ACE_OS::strtoul (str_group_ref_ver.c_str (), 0, 10);
    }

  // Parse the group multicast address.
  // The multicast address is terminated by a ':'.
  pos = end_pos + 1;
  end_pos = ace_str.find (':', pos);

  if (end_pos == ACE_CString::npos)
    {
      // The multicast address is mandatory, so throw an exception,
      // since it wasn't found.
      ACE_THROW (CORBA::INV_OBJREF (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        EINVAL),
                      CORBA::COMPLETED_NO));
    }

  ACE_CString mcast_addr = ace_str.substring (pos, end_pos - pos);

  // Parse the multicast port number.

  // First check that there's something left in the string.
  pos = end_pos + 1;
  if (ace_str[pos] == '\0')
    {
      // The multicast port is mandatory, so throw an exception,
      // since it wasn't found.
      ACE_THROW (CORBA::INV_OBJREF (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        EINVAL),
                      CORBA::COMPLETED_NO));
    }

  CORBA::UShort mcast_port = 
      ACE_static_cast (CORBA::UShort,
        ACE_OS::strtoul (ace_str.c_str () + pos, 0, 10));

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
TAO_UIPMC_Profile::is_equivalent (const TAO_Profile *other_profile)
{

  if (other_profile->tag () != TAO_TAG_UIPMC_PROFILE)
    return 0;

  const TAO_UIPMC_Profile *op =
    ACE_dynamic_cast (const TAO_UIPMC_Profile *, other_profile);

  if (!(this->version_ == op->version_
        && this->endpoint_.is_equivalent (&op->endpoint_)))
    return 0;

  return 1;
}

CORBA::ULong
TAO_UIPMC_Profile::hash (CORBA::ULong max
                         TAO_ENV_ARG_DECL_NOT_USED)
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

size_t
TAO_UIPMC_Profile::endpoint_count (void)
{
  return 1;
}

char *
TAO_UIPMC_Profile::to_string (TAO_ENV_ARG_DECL_NOT_USED)
{
  // @@ Frank: Update to pull out GroupID information...

  u_int buflen = (ACE_OS::strlen (::prefix_) +
                  3 /* "loc" */ +
                  1 /* colon separator */ +
                  2 /* double-slash separator */ +
                  1 /* major version */ +
                  1 /* decimal point */ +
                  1 /* minor version */ +
                  1 /* `@' character */ +
                  15 /* dotted decimal IPv4 address */ +
                  1 /* colon separator */ +
                  5 /* port number */);

  char * buf = CORBA::string_alloc (buflen);

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "%sloc://%c.%c@%s:%d%c%s",
                   ::prefix_,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->endpoint_.get_host_addr (),
                   this->endpoint_.port ());
  return buf;
}

const char *
TAO_UIPMC_Profile::prefix (void)
{
  return ::prefix_;
}

int
TAO_UIPMC_Profile::encode (TAO_OutputCDR &stream) const
{
  // UNSIGNED LONG, protocol tag
  stream.write_ulong (this->tag ());

  // Create the encapsulation....
  TAO_OutputCDR encap;


  // Create the profile body
  this->create_profile_body (encap);

  // write the encapsulation as an octet sequence...

  stream << CORBA::ULong (encap.total_length ());
  stream.write_octet_array_mb (encap.begin ());

  return 1;
}


IOP::TaggedProfile &
TAO_UIPMC_Profile::create_tagged_profile (void)
{
  // Check whether we have already created the TaggedProfile
  if (this->tagged_profile_.profile_data.length () == 0)
    {
      // As we have not created we will now create the TaggedProfile
      this->tagged_profile_.tag = TAO_TAG_UIPMC_PROFILE;

      // Create the encapsulation....
      TAO_OutputCDR encap;

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
TAO_UIPMC_Profile::create_profile_body (TAO_OutputCDR &encap) const
{
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  // Note: Only GIOP 1.2 and above are supported currently for MIOP.
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // Address.
  encap.write_string (this->endpoint_.get_host_addr ());

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

      TAO_InputCDR in_cdr (ACE_reinterpret_cast (const char*, buf),
                           tagged_component.component_data.length ());

      // Extract the Byte Order.
      CORBA::Boolean byte_order;
      if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        return -1;
      in_cdr.reset_byte_order (ACE_static_cast(int, byte_order));

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
  if ((out_cdr << group) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, 
                  "Error marshaling group component!"));
      return;
    }

  CORBA::ULong length = out_cdr.total_length ();

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_GROUP;
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
  this->tagged_components_.set_component (tagged_component);
}

void 
TAO_UIPMC_Profile::request_target_specifier (
                      TAO_Target_Specification &target_spec,
                      TAO_Target_Specification::TAO_Target_Address required_type
                      TAO_ENV_ARG_DECL)
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
      ACE_THROW (CORBA::MARSHAL ());
    }
}

int
TAO_UIPMC_Profile::supports_multicast (void) const
{
  // Yes!  We support multicast!
  return 1;
}

void
TAO_UIPMC_Profile::addressing_mode (CORBA::Short addr
                                    TAO_ENV_ARG_DECL)
{
  // ** See race condition note about addressing mode in Profile.h **
  switch (addr) 
    {
    case TAO_Target_Specification::Profile_Addr:
    case TAO_Target_Specification::Reference_Addr:
      this->addressing_mode_ = addr;
      break;

    case TAO_Target_Specification::Key_Addr:
      // There is no object key, so it is not supported.

    default:
      ACE_THROW (CORBA::BAD_PARAM (
             CORBA_SystemException::_tao_minor_code (
               TAO_DEFAULT_MINOR_CODE,
               EINVAL),
             CORBA::COMPLETED_NO));
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
  TAO_InputCDR cdr (ACE_reinterpret_cast(char*,profile.profile_data.get_buffer ()),
                    profile.profile_data.length ());
//#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  CORBA::ULong encap_len = cdr.length ();

  // Extract the Byte Order.
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;
  cdr.reset_byte_order (ACE_static_cast(int, byte_order));

  // Read and verify major, minor versions, ignoring UIPMC profiles
  // whose versions we don't understand.
  CORBA::Octet major, minor;

  // Read the version. We just read it here. We don't*do any*
  // processing.
  if (!(cdr.read_octet (major)
        && cdr.read_octet (minor)))
  {
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) UIPMC_Profile::extract_group_component - v%d.%d\n"),
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
                    ACE_TEXT ("TAO (%P|%t) UIPMC_Profile::extract_group_component - Couldn't unmarshal address and port!\n")));
      return -1;
    }

  TAO_Tagged_Components tagged_components;
  if (tagged_components.decode (cdr) == 0)
    return -1;

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_GROUP;

  // Try to find it.
  if (tagged_components.get_component (tagged_component) == 0)
    return -1;

  // Found it.
  const CORBA::Octet *buf =
    tagged_component.component_data.get_buffer ();

  TAO_InputCDR in_cdr (ACE_reinterpret_cast (const char*, buf),
                       tagged_component.component_data.length ());

  // Extract the Byte Order.
  if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;
  in_cdr.reset_byte_order (ACE_static_cast(int, byte_order));

  if ((in_cdr >> group) == 0)
    return -1;

  return 0;
}
