// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/IIOP_Profile.h"
#include "tao/GIOP.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"
#include "tao/debug.h"

ACE_RCSID(tao, IIOP_Profile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Profile.i"
#endif /* __ACE_INLINE__ */

static const char *prefix_ = "iiop:";

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                                    const char *object_key)
  : host_ (0),
    port_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_IIOP_MAJOR, DEF_IIOP_MINOR),
    object_key_ (),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set(addr);
  int l = ACE_OS::strlen (object_key);
  this->object_key_.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key_[i] = object_key[i];

  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                                    const TAO_ObjectKey& object_key)
  : host_ (0),
    port_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_IIOP_MAJOR, DEF_IIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set(addr);
  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                                    const TAO_IOP_Version& version,
                                    const char *object_key)
  : host_ (0),
    port_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (version),
    object_key_ (),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set(addr);
  int l = ACE_OS::strlen (object_key);
  this->object_key_.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key_[i] = object_key[i];

  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                                    const TAO_IOP_Version& version,
                                    const TAO_ObjectKey& object_key)
  : host_ (0),
    port_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (version),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set(addr);
  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const char* host,
                                    CORBA::UShort port,
                                    const TAO_ObjectKey& object_key)
  : host_ (0),
    port_ (port),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_IIOP_MAJOR, DEF_IIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (port, host),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  if (host)
  {
    ACE_NEW (this->host_,
             char[ACE_OS::strlen (host) + 1]);
    ACE_OS::strcpy (this->host_, host);
  }

  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const char* host,
                                    CORBA::UShort port,
                                    const TAO_ObjectKey& object_key,
				    const ACE_INET_Addr& addr)
  : host_ (0),
    port_ (port),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_IIOP_MAJOR, DEF_IIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  if (host)
  {
    ACE_NEW (this->host_,
             char[ACE_OS::strlen (host) + 1]);
    ACE_OS::strcpy (this->host_, host);
  }

  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const char* host,
                                    CORBA::UShort port,
                                    const TAO_IOP_Version& version,
                                    const TAO_ObjectKey& object_key)
  : host_ (0),
    port_ (port),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_IIOP_MAJOR, DEF_IIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (port, host),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  ACE_UNUSED_ARG (version);

  ACE_NEW (this->host_,
           char[ACE_OS::strlen (host) + 1]);
  ACE_OS::strcpy (this->host_, host);

  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const TAO_IIOP_Profile *pfile)
  : host_(0),
    port_(pfile->port_),
    tag_(pfile->tag_),
    body_(pfile->body_),
    version_(pfile->version_),
    object_key_(pfile->object_key_),
    object_addr_(pfile->object_addr_),
    hint_(0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  ACE_NEW (this->host_,
           char[ACE_OS::strlen (pfile->host_) + 1]);
  ACE_OS::strcpy (this->host_, pfile->host_);
  hint_ = pfile->hint_;

}

TAO_IIOP_Profile::TAO_IIOP_Profile (const TAO_IIOP_Profile &pfile)
  : host_(0),
    port_(pfile.port_),
    tag_(pfile.tag_),
    body_(pfile.body_),
    version_(pfile.version_),
    object_key_(pfile.object_key_),
    object_addr_(pfile.object_addr_),
    hint_(0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  ACE_NEW (this->host_,
           char[ACE_OS::strlen (pfile.host_) + 1]);
  ACE_OS::strcpy (this->host_, pfile.host_);
  hint_ = pfile.hint_;

}

TAO_IIOP_Profile::TAO_IIOP_Profile (const TAO_IOP_Version &version)
  : host_ (0),
    port_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (version),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const char *string, CORBA::Environment &env)
  : host_ (0),
    port_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_IIOP_MAJOR, DEF_IIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  parse_string (string, env);
}

TAO_IIOP_Profile::TAO_IIOP_Profile (void)
  : host_ (0),
    port_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_IIOP_MAJOR, DEF_IIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
}

int
TAO_IIOP_Profile::set (const ACE_INET_Addr& addr)
{
  char temphost[MAXHOSTNAMELEN + 1];
  const char *temphost2 = 0;

  this->port_ = addr.get_port_number();

  if (TAO_ORB_Core_instance ()->orb_params ()->use_dotted_decimal_addresses ())
    {
      temphost2 = addr.get_host_addr ();
      if (temphost2 == 0)
        return -1;
    }
  else
    {
      if (addr.get_host_name (temphost,
                              sizeof temphost) != 0)
        return -1;

      temphost2 = temphost;
    }

  ACE_NEW_RETURN (this->host_,
                  char[ACE_OS::strlen (temphost2) + 1],
                  -1);
  ACE_OS::strcpy (this->host_, temphost2);

  return 0;

}

TAO_IIOP_Profile::~TAO_IIOP_Profile (void)
{
  assert (this->refcount_ == 0);

  delete [] this->host_;
  this->host_ = 0;
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_IIOP_Profile::parse (TAO_InputCDR& cdr,
                         CORBA::Boolean &continue_decoding,
                         CORBA::Environment &env)
{
  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring IIOP
  // profiles whose versions we don't understand.
  //
  // @@ Fred: if we find a version like 1.5 we are supposed to handle
  // it, i.e. read the fields we know about and ignore the rest!
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  if (!(cdr.read_octet (this->version_.major)
        && this->version_.major == TAO_IIOP_Profile::DEF_IIOP_MAJOR
        && cdr.read_octet (this->version_.minor)
        && this->version_.minor <= TAO_IIOP_Profile::DEF_IIOP_MINOR))
  {
    ACE_DEBUG ((LM_DEBUG,
                "detected new v%d.%d IIOP profile",
                this->version_.major,
                this->version_.minor));
    return 0;
  }

  if (this->host_)
    {
      delete [] this->host_;
      this->host_ = 0;
    }

  // Get host and port
  if (cdr.read_string (this->host_) == 0
      || cdr.read_ushort (this->port_) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "error decoding IIOP host/port"));
      return -1;
    }

  this->object_addr_.set (this->port_, this->host_);

  // ... and object key.

  if ((cdr >> this->object_key_) == 0)
    return -1;

  if (cdr.length () != 0 && TAO_debug_level)
    {
      // If there is extra data in the profile we are supposed to
      // ignore it, but print a warning just in case...
      ACE_DEBUG ((LM_DEBUG,
                  "%d bytes out of %d left after IIOP profile data\n",
                  cdr.length (),
                  encap_len));
    }
  if (cdr.good_bit ())
    return 1;

  return -1;
}

int
TAO_IIOP_Profile::parse_string (const char *string,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (!string || !*string)
    return 0;

  // Remove the "N.N//" prefix, and verify the version's one
  // that we accept

  if (isdigit (string [0])
      && isdigit (string [2])
      && string [1] == '.'
      && string [3] == '/'
      && string [4] == '/')
    {
      // @@ This may fail for non-ascii character sets [but take that
      // with a grain of salt]
      this->version_.set_version ((char) (string [0] - '0'),
                                  (char) (string [2] - '0'));
      string += 5;
    }
  else
    ACE_THROW_RETURN (CORBA::MARSHAL (), 0);

  if (this->version_.major != TAO_IIOP_Profile::DEF_IIOP_MAJOR
      || this->version_.minor  > TAO_IIOP_Profile::DEF_IIOP_MINOR)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
    }

  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy = CORBA::string_dup (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, ':');

  if (cp == 0)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
    }

  if (this->host_)
    {
      // @@ You are setting this->host_ using CORBA::string_alloc() a
      // couple of lines below, you should then use CORBA::string_free()
      // to release it!  In general use a single form of memory
      // allocation for a field/variable to avoid new/free() and
      // malloc/delete() mismatches.
      // Ohh, and if you are going to use CORBA::string_alloc() &
      // friends you may consider using CORBA::String_var to manage
      // the memory automatically (though there may be forces that
      // suggest otherwise).
      delete [] this->host_;
      this->host_ = 0;
    }

  this->host_ = CORBA::string_alloc (1 + cp - start);
  for (cp = this->host_; *start != ':'; *cp++ = *start++)
    continue;

  *cp = 0; start++; // increment past :

  cp = ACE_OS::strchr (start, '/');

  if (cp == 0)
    {
      CORBA::string_free (this->host_);
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
    }

  this->port_ = (CORBA::UShort) ACE_OS::atoi (start);

  this->object_addr_.set (this->port_, this->host_);

  start = ++cp;  // increment past the /

  TAO_POA::decode_string_to_sequence (this->object_key_, start);

  return 1;
}

//  TAO_InputCDR cdr (body.get_buffer (), body.length (), body[0]);
//
// @@
//  CORBA::Boolean byte_order;
//  cdr >> CORBA::Any::to_boolean (byte_order);
//  cdr >> this->version_.major;
//  cdr >> this->version_.minor;
//  cdr >> this->host_ ();
//  cdr >> this->port_;
//  cdr >> this->object_key_;
//  this->object_addr_.set(this->port_, this->host_);
// }

void
TAO_IIOP_Profile::create_body (void)
{
  TAO_OutputCDR cdr;
}
// @@
//   cdr << TAO_ENCAP_BYTE_ORDER;
//   cdr << this->version_.major;
//   cdr << this->version_.minor;
//   cdr << this->host_;
//   cdr << this->port_;
//   cdr << this->object_key_;
//
//   // Create a copy by making a temporary TAO_opaque object.
//   // @@ TODO the CDR stream may contain several fragments, need to
//   // merge them...
//   this->body_ = TAO_opaque (cdr.length (), // length
//                             cdr.length (), // maximum
//                             cdr.buffer (), // buffer
//                             0);            // not own
// }

CORBA::Boolean
TAO_IIOP_Profile::is_equivalent (TAO_Profile *other_profile,
                                 CORBA::Environment &env)
{
  env.clear ();

  if (other_profile->tag () != TAO_IOP_TAG_INTERNET_IOP)
    return 0;

  TAO_IIOP_Profile *op =
    ACE_dynamic_cast (TAO_IIOP_Profile *, other_profile);

  ACE_ASSERT (op->object_key_.length () < UINT_MAX);

  return this->port_ == op->port_
    && this->object_key_ == op->object_key_
    && ACE_OS::strcmp (this->host_, op->host_) == 0
    && this->version_ == op->version_;
}

CORBA::ULong
TAO_IIOP_Profile::hash (CORBA::ULong max,
                        CORBA::Environment &env)
{
  CORBA::ULong hashval;

  env.clear ();

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

char *
TAO_IIOP_Profile::addr_to_string(void)
{
  static char s[MAXHOSTNAMELEN + MAXHOSTNAMELEN];
  ACE_OS::sprintf (s, "%s:%d",
                   this->host_, port_);
  return s;
}

const char *
TAO_IIOP_Profile::host (const char *h)
{
  if (this->host_)
    {
      delete [] this->host_;
      this->host_ = 0;
    }

  if (h)
    {
      ACE_NEW_RETURN (this->host_,
                      char[ACE_OS::strlen (h) + 1],
                      0);
      ACE_OS::strcpy (this->host_, h);
    }

  return this->host_;
}

void
TAO_IIOP_Profile::reset_hint (void)
{
  if (this->hint_)
    {
      this->hint_->cleanup_hint ();
      this->hint_ = 0;
    }
}

TAO_IIOP_Profile &
TAO_IIOP_Profile::operator= (const TAO_IIOP_Profile &src)
{
  this->version_ = src.version_;

  this->object_key_ = src.object_key_;

  this->object_addr_.set (src.object_addr_);

  this->port_ = src.port_;

  if (this->host_)
    {
      delete [] this->host_;
      this->host_ = 0;
    }

  if (src.host_)
    {
      ACE_NEW_RETURN (this->host_,
                      char[ACE_OS::strlen (src.host_) + 1],
                      *this);
      ACE_OS::strcpy (this->host_, src.host_);
    }

  return *this;
}

// Memory managment

CORBA::ULong
TAO_IIOP_Profile::_incr_refcnt (void)
{
  // OK, think I got it.  When this object is created (guard) the
  // lock is automatically acquired (refcount_lock_).  Then when
  // we leave this method the destructir for guard is called which
  // releases the lock!
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
TAO_IIOP_Profile::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // refcount is 0, so delete us!
  // delete will call our ~ destructor which in turn deletes stuff.
  delete this;
  return 0;
}

CORBA::String
TAO_IIOP_Profile::to_string (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  CORBA::String_var key;
  TAO_POA::encode_sequence_to_string (key.inout(),
                                      this->object_key ());

  u_int buflen = (ACE_OS::strlen (::prefix_) +
                  1 /* major # */ + 1 /* minor # */ +
                  2 /* double-slash separator */ +
                  ACE_OS::strlen (this->host_) +
                  1 /* colon separator */ +
                  5 /* port number */ +
                  1 /* slash separator */ +
                  ACE_OS::strlen (key) +
                  1 /* zero terminator */);

  CORBA::String buf = CORBA::string_alloc (buflen);

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "%s%c.%c//%s:%d/%s",
                   ::prefix_,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->host_,
                   this->port_,
                   key.in ());
  return buf;
}

const char *
TAO_IIOP_Profile::prefix (void)
{
  return ::prefix_;
}

int
TAO_IIOP_Profile::encode (TAO_OutputCDR &stream) const
{
  // UNSIGNED LONG, tag for this protocol profile;
  // @@ it seems like this is not a good separation of concerns, why
  // do we write the TAG here? That's generic code and should be
  // handled by the object reference writer (IMHO).
  stream.write_ulong (TAO_IOP_TAG_INTERNET_IOP);

  // UNSIGNED LONG, number of succeeding bytes in the
  // encapsulation.  We don't actually need to make the
  // encapsulation, as nothing needs stronger alignment than
  // this longword; it guarantees the rest is aligned for us.

  CORBA::ULong hostlen = ACE_OS::strlen ((char *) this->host_);
  CORBA::ULong encap_len =
    1                              // byte order
    + 1                            // version major
    + 1                            // version minor
    + 1                            // pad byte
    + 4                            // sizeof (strlen)
    + hostlen + 1                  // strlen + null
    + (~hostlen & 01)              // optional pad byte
    + 2                            // port
    + ( hostlen & 02)              // optional pad short
    + 4                            // sizeof (key length)
    + this->object_key_.length (); // key length.
  stream.write_ulong (encap_len);

  // CHAR describing byte order, starting the encapsulation
  stream.write_octet (TAO_ENCAP_BYTE_ORDER);

  // IIOP::TAO_IOP_Version, two characters (version 1.0) padding
  stream.write_char (this->version_.major);
  stream.write_char (this->version_.minor);

  // STRING hostname from profile
  stream.write_string (this->host_);

  // UNSIGNED SHORT port number
  stream.write_ushort (this->port_);

  // OCTET SEQUENCE for object key
  stream << this->object_key_;

  return 1;
}
