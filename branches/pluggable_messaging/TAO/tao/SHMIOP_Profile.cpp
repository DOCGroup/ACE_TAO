// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/SHMIOP_Profile.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/SHMIOP_Connect.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"
#include "tao/debug.h"

ACE_RCSID(tao, SHMIOP_Profile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/SHMIOP_Profile.i"
#endif /* __ACE_INLINE__ */

static const char prefix_[] = "shmiop";

const char TAO_SHMIOP_Profile::object_key_delimiter_ = '/';

char
TAO_SHMIOP_Profile::object_key_delimiter (void) const
{
  return TAO_SHMIOP_Profile::object_key_delimiter_;
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (const ACE_MEM_Addr &addr,
                                        const TAO_ObjectKey &object_key,
                                        const TAO_GIOP_Version &version,
                                        TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_SHMEM_PROFILE),
    host_ (),
    port_ (0),
    version_ (version),
    object_key_ (object_key),
    object_addr_ (addr.get_remote_addr ()),
    hint_ (0),
    orb_core_ (orb_core)
{
  this->set (addr.get_remote_addr ());
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (const char* host,
                                        CORBA::UShort port,
                                        const TAO_ObjectKey &object_key,
                                        const ACE_INET_Addr &addr,
                                        const TAO_GIOP_Version &version,
                                        TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_SHMEM_PROFILE),
    host_ (),
    port_ (port),
    version_ (version),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    orb_core_ (orb_core)
{
  if (host != 0)
    this->host_ = host;
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (const TAO_SHMIOP_Profile &pfile)
  : TAO_Profile (pfile.tag ()),
    host_ (pfile.host_),
    port_ (pfile.port_),
    version_ (pfile.version_),
    object_key_ (pfile.object_key_),
    object_addr_ (pfile.object_addr_),
    hint_ (pfile.hint_),
    orb_core_ (pfile.orb_core_)
{
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (const char *string,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Environment &ACE_TRY_ENV)
  : TAO_Profile (TAO_TAG_SHMEM_PROFILE),
    host_ (),
    port_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    orb_core_ (orb_core)
{
  parse_string (string, ACE_TRY_ENV);
  ACE_CHECK;
}

TAO_SHMIOP_Profile::TAO_SHMIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_TAG_SHMEM_PROFILE),
    host_ (),
    port_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    orb_core_ (orb_core)
{
}

int
TAO_SHMIOP_Profile::set (const ACE_INET_Addr &addr)
{
  this->port_ = addr.get_port_number();

  if (this->orb_core_->orb_params ()->use_dotted_decimal_addresses ())
    {
      const char *temp = addr.get_host_addr ();
      if (temp == 0)
        return -1;
      else
        this->host_ = temp;
    }
  else
    {
      char temphost[MAXHOSTNAMELEN + 1];

      if (addr.get_host_name (temphost,
                              sizeof temphost) != 0)
        return -1;

      this->host_ = CORBA::string_dup (temphost);
    }

  return 0;
}

TAO_SHMIOP_Profile::~TAO_SHMIOP_Profile (void)
{
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_SHMIOP_Profile::decode (TAO_InputCDR& cdr)
{
  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring SHMIOP
  // profiles whose versions we don't understand.
  //
  if (!(cdr.read_octet (this->version_.major)
        && this->version_.major == TAO_DEF_GIOP_MAJOR
        && cdr.read_octet (this->version_.minor)
        && this->version_.minor <= TAO_DEF_GIOP_MINOR))
  {
    if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) SHMIOP_Profile::decode - v%d.%d\n"),
                    this->version_.major,
                    this->version_.minor));
      }
  }

  // Get host and port
  if (cdr.read_string (this->host_.out ()) == 0
      || cdr.read_ushort (this->port_) == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("TAO (%P|%t) SHMIOP_Profile::decode - ")
                      ASYS_TEXT ("error while decoding host/port")));
        }
      return -1;
    }

  this->object_addr_.set (this->port_, this->host_.in ());

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
    {
      // If there is extra data in the profile we are supposed to
      // ignore it, but print a warning just in case...
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("%d bytes out of %d left after IIOP profile data\n"),
                  cdr.length (),
                  encap_len));
    }

  if (cdr.good_bit ())
    return 1;

  return -1;
}

int
TAO_SHMIOP_Profile::parse_string (const char *string,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  if (!string || !*string)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
        CORBA_SystemException::_tao_minor_code (
          TAO_NULL_POINTER_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO),
        -1);
    }

  // Remove the "N.n@" version prefix, if it exists, and verify the
  // version is one that we accept.

  // Check for version
  if (isdigit (string [0]) &&
      string[1] == '.' &&
      isdigit (string [2]) &&
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
      this->version_.minor >  TAO_DEF_GIOP_MINOR)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), -1);
    }

  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, ':');  // Look for a port

  if (cp == 0)
    {
      // No host/port delimiter!
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
        CORBA_SystemException::_tao_minor_code (
          TAO_NULL_POINTER_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO),
        -1);
    }

  char *okd = ACE_OS::strchr (start, this->object_key_delimiter_);

  if (okd == 0)
    {
      // No object key delimiter!
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
        CORBA_SystemException::_tao_minor_code (
          TAO_NULL_POINTER_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO),
        -1);
    }

  // Don't increment the pointer 'cp' directly since we still need
  // to use it immediately after this block.

  CORBA::ULong length = okd - (cp + 1);
  // Don't allocate space for the colon ':'.

  CORBA::String_var tmp = CORBA::string_alloc (length);

  ACE_OS::strncpy (tmp.inout (), cp + 1, length);
  tmp[length] = '\0';

  this->port_ = (CORBA::UShort) ACE_OS::atoi (tmp.in ());

  length = cp - start;

  tmp = CORBA::string_alloc (length);

  ACE_OS::strncpy (tmp.inout (), start, length);
  tmp[length] = '\0';

  this->host_ = tmp._retn ();

  this->object_addr_.set (this->port_, this->host_.in ());

  start = ++okd;  // increment past the object key separator

  TAO_POA::decode_string_to_sequence (this->object_key_, start);

  return 1;
}

CORBA::Boolean
TAO_SHMIOP_Profile::is_equivalent (const TAO_Profile *other_profile)
{

  if (other_profile->tag () != TAO_TAG_SHMEM_PROFILE)
    return 0;

  const TAO_SHMIOP_Profile *op =
    ACE_dynamic_cast (const TAO_SHMIOP_Profile *, other_profile);

  ACE_ASSERT (op->object_key_.length () < UINT_MAX);

  return this->port_ == op->port_
    && this->object_key_ == op->object_key_
    && ACE_OS::strcmp (this->host_.in (), op->host_.in ()) == 0
    && this->version_ == op->version_;
}

CORBA::ULong
TAO_SHMIOP_Profile::hash (CORBA::ULong max,
                          CORBA::Environment &)
{
  CORBA::ULong hashval;

  // Just grab a bunch of convenient bytes and hash them; could do
  // more (hostname, full key, exponential hashing) but no real need
  // to do so except if performance requires a more costly hash.

  hashval = this->object_key_.length () * this->port_;
  hashval += this->version_.minor;

  if (this->object_key_.length () >= 4)
    {
      hashval += this->object_key_ [1];
      hashval += this->object_key_ [3];
    }

  return hashval % max;
}

int
TAO_SHMIOP_Profile::addr_to_string (char *buffer, size_t length)
{
  size_t actual_len =
    ACE_OS::strlen (this->host_.in ()) // chars in host name
    + sizeof (':')                     // delimiter
    + ACE_OS::strlen ("65536")         // max port
    + sizeof ('\0');

  if (length < actual_len)
    return -1;

  ACE_OS::sprintf (buffer, "%s:%d",
                   this->host_.in (), this->port_);

  return 0;
}

const char *
TAO_SHMIOP_Profile::host (const char *h)
{
  this->host_ = h;

  return this->host_.in ();
}

void
TAO_SHMIOP_Profile::reset_hint (void)
{
  if (this->hint_)
    this->hint_->cleanup_hint ((void **) &this->hint_);
}

TAO_SHMIOP_Profile &
TAO_SHMIOP_Profile::operator= (const TAO_SHMIOP_Profile &src)
{
  this->version_ = src.version_;

  this->object_key_ = src.object_key_;

  this->object_addr_.set (src.object_addr_);

  this->port_ = src.port_;

  this->host_ = src.host_;

  return *this;
}

char *
TAO_SHMIOP_Profile::to_string (CORBA::Environment &)
{
  CORBA::String_var key;
  TAO_POA::encode_sequence_to_string (key.inout(),
                                      this->object_key ());

  u_int buflen = (ACE_OS::strlen (::prefix_) +
                  3 /* "loc" */ +
                  1 /* colon separator */ +
                  2 /* double-slash separator */ +
                  1 /* major version */ +
                  1 /* decimal point */ +
                  1 /* minor version */ +
                  1 /* `@' character */ +
                  ACE_OS::strlen (this->host_.in ()) +
                  1 /* colon separator */ +
                  5 /* port number */ +
                  1 /* object key separator */ +
                  ACE_OS::strlen (key.in ()));

  char * buf = CORBA::string_alloc (buflen);

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "%sloc://%c.%c@%s:%d%c%s",
                   ::prefix_,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->host_.in (),
                   this->port_,
                   this->object_key_delimiter_,
                   key.in ());
  return buf;
}

const char *
TAO_SHMIOP_Profile::prefix (void)
{
  return ::prefix_;
}

int
TAO_SHMIOP_Profile::encode (TAO_OutputCDR &stream) const
{
  // UNSIGNED LONG, protocol tag
  stream.write_ulong (this->tag ());

  // Create the encapsulation....
  TAO_OutputCDR encap (ACE_CDR::DEFAULT_BUFSIZE,
                       TAO_ENCAP_BYTE_ORDER,
                       this->orb_core_->output_cdr_buffer_allocator (),
                       this->orb_core_->output_cdr_dblock_allocator (),
                       this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                       this->orb_core_->to_iso8859 (),
                       this->orb_core_->to_unicode ());

  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING hostname from profile
  encap.write_string (this->host_.in ());

  // UNSIGNED SHORT port number
  encap.write_ushort (this->port_);

  // OCTET SEQUENCE for object key
  encap << this->object_key_;

  if (this->version_.major > 1
      || this->version_.minor > 0)
    this->tagged_components ().encode (encap);

  // write the encapsulation as an octet sequence...
  stream << CORBA::ULong (encap.total_length ());
  stream.write_octet_array_mb (encap.begin ());

  return 1;
}

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
