// This may look like C, but it's really -*- C++ -*-
// $Id$

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/UIOP_Profile.h"
#include "tao/GIOP.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"
#include "tao/debug.h"

ACE_RCSID(tao, UIOP_Profile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/UIOP_Profile.i"
#endif /* __ACE_INLINE__ */

static const char *prefix_ = "uiop:";

const char TAO_UIOP_Profile::object_key_delimiter = '|';

TAO_UIOP_Profile::TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                                    const TAO_ObjectKey &object_key,
                                    const TAO_GIOP_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    version_ (version),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    orb_core_ (orb_core)
{
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char *,
                                    const TAO_ObjectKey &object_key,
                                    const ACE_UNIX_Addr &addr,
                                    const TAO_GIOP_Version &version,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    version_ (version),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    orb_core_ (orb_core)
{
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const TAO_UIOP_Profile &pfile)
  : TAO_Profile (pfile.tag ()),
    version_ (pfile.version_),
    object_key_ (pfile.object_key_),
    object_addr_ (pfile.object_addr_),
    hint_ (pfile.hint_),
    orb_core_ (pfile.orb_core_)
{
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char *string,
                                    TAO_ORB_Core *orb_core,
                                    CORBA::Environment &ACE_TRY_ENV)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    orb_core_ (orb_core)
{
  parse_string (string, ACE_TRY_ENV);
  ACE_CHECK;
}

TAO_UIOP_Profile::TAO_UIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    orb_core_ (orb_core)
{
}

TAO_UIOP_Profile::~TAO_UIOP_Profile (void)
{
}

int
TAO_UIOP_Profile::parse_string (const char *string,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (!string || !*string)
    return 0;

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
      this->version_.minor  > TAO_DEF_GIOP_MINOR)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
    }

  // Pull off the "rendezvous point" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy = CORBA::string_dup (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, this->object_key_delimiter);

  if (cp == 0)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
      // No rendezvous point specified
    }

  char *rendezvous = 0;

  ACE_NEW_RETURN (rendezvous,
                  char[1 + cp - start],
                  -1);

  ACE_OS::strncpy (rendezvous, start, cp - start);
  rendezvous[cp - start] = '\0';

  (void) this->rendezvous_point (rendezvous);

  delete [] rendezvous;

  start = ++cp;  // increment past the object key separator

  TAO_POA::decode_string_to_sequence (this->object_key_, start);

  return 1;
}

CORBA::Boolean
TAO_UIOP_Profile::is_equivalent (const TAO_Profile *other_profile)
{

  if (other_profile->tag () != TAO_IOP_TAG_UNIX_IOP)
    return 0;

  const TAO_UIOP_Profile *op =
    ACE_dynamic_cast (const TAO_UIOP_Profile *, other_profile);

  ACE_ASSERT (op->object_key_.length () < UINT_MAX);

  return this->object_key_ == op->object_key_ &&
    ACE_OS::strcmp (this->rendezvous_point (),
                    op->rendezvous_point ()) == 0 &&
    this->version_ == op->version_;
}

CORBA::ULong
TAO_UIOP_Profile::hash (CORBA::ULong max,
                        CORBA::Environment &)
{
  CORBA::ULong hashval;

  // Just grab a bunch of convenient bytes and hash them; could do
  // more (rendezvous_point, full key, exponential hashing)
  // but no real need  to do so except if performance requires a more
  // costly hash.

  hashval = this->object_key_.length () *
    ACE_OS::atoi (this->rendezvous_point ());  // @@ Is this valid?
  hashval += this->version_.minor;

  if (this->object_key_.length () >= 4)
    {
      hashval += this->object_key_ [1];
      hashval += this->object_key_ [3];
    }

  return hashval % max;
}

int
TAO_UIOP_Profile::addr_to_string (char *buffer, size_t length)
{
  if (length < (ACE_OS::strlen (this->rendezvous_point ()) + 1))
    return -1;

  ACE_OS::strcpy (buffer, this->rendezvous_point ());

  return 0;
}

const char *
TAO_UIOP_Profile::rendezvous_point (const char *rendezvous)
{
  if (!rendezvous || !*rendezvous)
    return 0;

  this->object_addr_.set (rendezvous);

  size_t length = ACE_OS::strlen (this->rendezvous_point ());

  // Check if rendezvous point was truncated by ACE_UNIX_Addr since
  // most UNIX domain socket rendezvous points can only be less than
  // 108 characters long.
  if (length < ACE_OS::strlen (rendezvous))
    {
      ACE_DEBUG ((LM_WARNING,
                  "TAO (%P|%t) UIOP rendezvous point was truncated to <%s>\n"
                  "since it was longer than %d characters long.\n",
                  this->rendezvous_point (),
                  length));
    }

  return this->rendezvous_point ();
}

void
TAO_UIOP_Profile::reset_hint (void)
{
  if (this->hint_)
    {
      this->hint_->cleanup_hint ();
      this->hint_ = 0;
    }
}

TAO_UIOP_Profile &
TAO_UIOP_Profile::operator= (const TAO_UIOP_Profile &src)
{
  this->version_ = src.version_;

  this->object_key_ = src.object_key_;

  this->object_addr_.set (src.object_addr_);

  return *this;
}

CORBA::String
TAO_UIOP_Profile::to_string (CORBA::Environment &)
{
  CORBA::String_var key;
  TAO_POA::encode_sequence_to_string (key.inout(),
                                      this->object_key ());

  u_int buflen = (ACE_OS::strlen (::prefix_) +
                  2 /* double-slash separator */ +
                  1 /* major version */ +
                  1 /* decimal point */ +
                  1 /* minor version */ +
                  1 /* `@' character */ +
                  ACE_OS::strlen (this->rendezvous_point ()) +
                  1 /* object key separator */ +
                  ACE_OS::strlen (key) +
                  1 /* zero terminator */);

  CORBA::String buf = CORBA::string_alloc (buflen);

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "%s//%c.%c@%s%c%s",
                   ::prefix_,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->rendezvous_point (),
                   this->object_key_delimiter,
                   key.in ());
  return buf;
}

const char *
TAO_UIOP_Profile::prefix (void)
{
  return ::prefix_;
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_UIOP_Profile::decode (TAO_InputCDR& cdr)
{
  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring UIOP
  // profiles whose versions we don't understand.
  // FIXME:  Version question again,  what do we do about them for this
  //         protocol?

  if (!(cdr.read_octet (this->version_.major)
        && this->version_.major == TAO_DEF_GIOP_MAJOR
        && cdr.read_octet (this->version_.minor)
        && this->version_.minor <= TAO_DEF_GIOP_MINOR))
  {
    ACE_DEBUG ((LM_DEBUG,
                "detected new v%d.%d UIOP profile\n",
                this->version_.major,
                this->version_.minor));
    return -1;
  }

  char *rendezvous = 0;

  // Get rendezvous_point
  if (cdr.read_string (rendezvous) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "error decoding UIOP rendezvous_point"));
      return -1;
    }

  // We could use this->rendezvous_point(rendezvous) to set and check the
  // rendezvous point.  However, it is safe to assume that it is valid 
  // since it should only have been encoded if it was valid.
  this->object_addr_.set (rendezvous);

  // Clean up
  delete [] rendezvous;

  // ... and object key.

  if ((cdr >> this->object_key_) == 0)
    return -1;

  if (this->version_.major > 1
      || this->version_.minor > 0)
    if (this->tagged_components_.decode (cdr) == 0)
      return -1;

  if (cdr.length () != 0 && TAO_debug_level)
    {
      // If there is extra data in the profile we are supposed to
      // ignore it, but print a warning just in case...
      ACE_DEBUG ((LM_DEBUG,
                  "%d bytes out of %d left after UIOP profile data\n",
                  cdr.length (),
                  encap_len));
    }

  if (cdr.good_bit ())
    return 1;

  return -1;
}

int
TAO_UIOP_Profile::encode (TAO_OutputCDR &stream) const
{
  // UNSIGNED LONG, tag for this protocol profile;
  // @@ it seems like this is not a good separation of concerns, why
  // do we write the TAG here? That's generic code and should be
  // handled by the object reference writer (IMHO).
  stream.write_ulong (TAO_IOP_TAG_UNIX_IOP);

  // Create the encapsulation....
  TAO_OutputCDR encap (ACE_CDR::DEFAULT_BUFSIZE,
                       TAO_ENCAP_BYTE_ORDER,
                       this->orb_core_->output_cdr_buffer_allocator (),
                       this->orb_core_->output_cdr_dblock_allocator (),
                       this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                       this->orb_core_->to_iso8859 (),
                       this->orb_core_->to_unicode ());

  // CHAR describing byte order, starting the encapsulation
  encap.write_octet (TAO_ENCAP_BYTE_ORDER);

  // The GIOP version
  encap.write_octet (this->version_.major);
  encap.write_octet (this->version_.minor);

  // STRING rendezvous_pointname from profile
  encap.write_string (this->rendezvous_point ());

  // OCTET SEQUENCE for object key
  encap << this->object_key_;

  if (this->version_.major > 1
      || this->version_.minor > 0)
    this->tagged_components_.encode (encap);

  // write the encapsulation as an octet sequence...
  stream << CORBA::ULong (encap.total_length ());
  stream.write_octet_array_mb (encap.begin ());

  return 1;
}

#endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
