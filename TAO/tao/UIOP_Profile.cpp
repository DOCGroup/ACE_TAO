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
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    orb_core_ (orb_core)
{
  this->set (addr);
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char *rendezvous_point,
                                    const TAO_ObjectKey &object_key,
                                    const ACE_UNIX_Addr &addr,
                                    TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    orb_core_ (orb_core)
{
  if (rendezvous_point)
    {
      ACE_NEW (this->rendezvous_point_,
               char[ACE_OS::strlen (rendezvous_point) + 1]);
      ACE_OS::strcpy (this->rendezvous_point_, rendezvous_point);
    }
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const TAO_UIOP_Profile &pfile)
  : TAO_Profile (pfile.tag ()),
    rendezvous_point_(0),
    version_(pfile.version_),
    object_key_(pfile.object_key_),
    object_addr_(pfile.object_addr_),
    hint_(0),
    orb_core_ (pfile.orb_core_)
{
  ACE_NEW (this->rendezvous_point_,
           char[ACE_OS::strlen (pfile.rendezvous_point_) + 1]);
  ACE_OS::strcpy (this->rendezvous_point_, pfile.rendezvous_point_);
  hint_ = pfile.hint_;
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char *string,
                                    TAO_ORB_Core *orb_core,
                                    CORBA::Environment &env)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    orb_core_ (orb_core)
{
  parse_string (string, env);
}

TAO_UIOP_Profile::TAO_UIOP_Profile (TAO_ORB_Core *orb_core)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    orb_core_ (orb_core)
{
}

int
TAO_UIOP_Profile::set (const ACE_UNIX_Addr& addr)
{
  char temp_rendezvous_point[MAXPATHLEN + 1];

  addr.addr_to_string (temp_rendezvous_point, sizeof (temp_rendezvous_point));

  ACE_NEW_RETURN (this->rendezvous_point_,
                  char[ACE_OS::strlen (temp_rendezvous_point) + 1],
                  -1);

  ACE_OS::strcpy (this->rendezvous_point_, temp_rendezvous_point);

  return 0;
}

TAO_UIOP_Profile::~TAO_UIOP_Profile (void)
{
  delete [] this->rendezvous_point_;
  this->rendezvous_point_ = 0;
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

  if (this->version_.major != TAO_UIOP_Profile::DEF_UIOP_MAJOR ||
      this->version_.minor  > TAO_UIOP_Profile::DEF_UIOP_MINOR)
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

  if (this->rendezvous_point_)
    {
      delete [] this->rendezvous_point_;
      this->rendezvous_point_ = 0;
    }

  ACE_NEW_RETURN (this->rendezvous_point_,
                  char[1 + cp - start],
                  -1);

  ACE_OS::strncpy (this->rendezvous_point_, start, cp - start);
  this->rendezvous_point_[cp - start] = '\0';

  this->object_addr_.set (this->rendezvous_point_);

  start = ++cp;  // increment past the object key separator

  TAO_POA::decode_string_to_sequence (this->object_key_, start);

  return 1;
}

CORBA::Boolean
TAO_UIOP_Profile::is_equivalent (TAO_Profile *other_profile,
                                 CORBA::Environment &env)
{
  env.clear ();

  if (other_profile->tag () != TAO_IOP_TAG_UNIX_IOP)
    return 0;

  TAO_UIOP_Profile *op =
    ACE_dynamic_cast (TAO_UIOP_Profile *, other_profile);

  ACE_ASSERT (op->object_key_.length () < UINT_MAX);

  return this->object_key_ == op->object_key_
    && ACE_OS::strcmp (this->rendezvous_point_, op->rendezvous_point_) == 0
    && this->version_ == op->version_;
}

CORBA::ULong
TAO_UIOP_Profile::hash (CORBA::ULong max,
                        CORBA::Environment &env)
{
  CORBA::ULong hashval;

  env.clear ();

  // Just grab a bunch of convenient bytes and hash them; could do
  // more (rendezvous_point, full key, exponential hashing)
  // but no real need  to do so except if performance requires a more
  // costly hash.

  hashval = this->object_key_.length () *
    ACE_OS::atoi (this->rendezvous_point_);  // FIXME:  Is this valid?
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
  if (length < (ACE_OS::strlen (rendezvous_point_) + 1))
    return -1;

  ACE_OS::strcpy (buffer, this->rendezvous_point_);

  return 0;
}

const char *
TAO_UIOP_Profile::rendezvous_point (const char *r)
{
  if (this->rendezvous_point_)
    {
      delete [] this->rendezvous_point_;
      this->rendezvous_point_ = 0;
    }

  if (r)
    {
      ACE_NEW_RETURN (this->rendezvous_point_,
                      char[ACE_OS::strlen (r) + 1],
                      0);
      ACE_OS::strcpy (this->rendezvous_point_, r);
    }

  return this->rendezvous_point_;
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

  if (this->rendezvous_point_)
    {
      delete [] this->rendezvous_point_;
      this->rendezvous_point_ = 0;
    }

  if (src.rendezvous_point_)
    {
      ACE_NEW_RETURN (this->rendezvous_point_,
                      char[ACE_OS::strlen (src.rendezvous_point_) + 1],
                      *this);
      ACE_OS::strcpy (this->rendezvous_point_, src.rendezvous_point_);
    }

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
                  ACE_OS::strlen (this->rendezvous_point_) +
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
                   this->rendezvous_point_,
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

  if (TAO_debug_level > 0)
    {
      char *buf = (char *) cdr.rd_ptr (); // ptr to first buffer
      ACE_HEX_DUMP ((LM_DEBUG,
                     (const char*)buf,
                     encap_len,
                     "\n"));
    }

  // Read and verify major, minor versions, ignoring UIOP
  // profiles whose versions we don't understand.
  // FIXME:  Version question again,  what do we do about them for this
  //         protocol?

  if (!(cdr.read_octet (this->version_.major)
        && this->version_.major == TAO_UIOP_Profile::DEF_UIOP_MAJOR
        && cdr.read_octet (this->version_.minor)
        && this->version_.minor <= TAO_UIOP_Profile::DEF_UIOP_MINOR))
  {
    ACE_DEBUG ((LM_DEBUG,
                "detected new v%d.%d UIOP profile\n",
                this->version_.major,
                this->version_.minor));
    return -1;
  }

  if (this->rendezvous_point_)
    {
      delete [] this->rendezvous_point_;
      this->rendezvous_point_ = 0;
    }

  // Get rendezvous_point
  if (cdr.read_string (this->rendezvous_point_) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "error decoding UIOP rendezvous_point"));
      return -1;
    }

  this->object_addr_.set (this->rendezvous_point_);

  // ... and object key.

  if ((cdr >> this->object_key_) == 0)
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

//   ACE_DEBUG ((LM_DEBUG,
//               "UIOP_Profile --- r point: <%s>\n",
//               this->rendezvous_point_));

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

  // UNSIGNED LONG, number of succeeding bytes in the
  // encapsulation.  We don't actually need to make the
  // encapsulation, as nothing needs stronger alignment than
  // this longword; it guarantees the rest is aligned for us.

  CORBA::ULong rendezvous_pointlen = 0;
  if (this->rendezvous_point_ != 0)
    rendezvous_pointlen = ACE_OS::strlen (this->rendezvous_point_);

  CORBA::ULong encap_len =
    1                              // byte order
    + 1                            // version major
    + 1                            // version minor
    + 1                            // pad byte
    + 4                            // sizeof (strlen)
    + rendezvous_pointlen + 1      // strlen + null
    + (~rendezvous_pointlen & 0x3) // optional pad short
    + 4                            // sizeof (key length)
    + this->object_key_.length (); // key length.
  stream.write_ulong (encap_len);

  // CHAR describing byte order, starting the encapsulation
  stream.write_octet (TAO_ENCAP_BYTE_ORDER);

  // UIOP::TAO_IOP_Version, two characters (version 1.0) padding
  stream.write_char (this->version_.major);
  stream.write_char (this->version_.minor);

  // STRING rendezvous_pointname from profile
  stream.write_string (this->rendezvous_point_);

//   ACE_DEBUG ((LM_DEBUG,
//               "UIOP_Profile::encode -- rendezvous point: <%s>\n",
//               this->rendezvous_point_));

  // OCTET SEQUENCE for object key
  stream << this->object_key_;

  return 1;
}

#endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
