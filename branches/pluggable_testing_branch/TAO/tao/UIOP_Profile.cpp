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

TAO_UIOP_Profile::TAO_UIOP_Profile (const ACE_UNIX_Addr& addr,
                                    const char *object_key)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set (addr);
  int l = ACE_OS::strlen (object_key);
  this->object_key_.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key_[i] = object_key[i];

  this->create_body ();
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const ACE_UNIX_Addr& addr,
                                    const TAO_ObjectKey& object_key)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set (addr);
  this->create_body ();
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const ACE_UNIX_Addr& addr,
                                    const TAO_IOP_Version& version,
                                    const char *object_key)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (version),
    object_key_ (),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set (addr);
  int l = ACE_OS::strlen (object_key);
  this->object_key_.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key_[i] = object_key[i];

  this->create_body ();
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const ACE_UNIX_Addr& addr,
                                    const TAO_IOP_Version& version,
                                    const TAO_ObjectKey& object_key)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (version),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  this->set (addr);
  this->create_body ();
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char* rendezvous_point,
                                    const TAO_ObjectKey& object_key)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (rendezvous_point),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  if (rendezvous_point)
  {
    ACE_NEW (this->rendezvous_point_,
             char[ACE_OS::strlen (rendezvous_point) + 1]);
    ACE_OS::strcpy (this->rendezvous_point_, rendezvous_point);
  }

  this->create_body ();
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char* rendezvous_point,
                                    const TAO_ObjectKey& object_key,
                                    const ACE_UNIX_Addr& addr)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  if (rendezvous_point)
  {
    ACE_NEW (this->rendezvous_point_,
             char[ACE_OS::strlen (rendezvous_point) + 1]);
    ACE_OS::strcpy (this->rendezvous_point_, rendezvous_point);
  }

  this->create_body ();
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char* rendezvous_point,
                                    const TAO_IOP_Version& version,
                                    const TAO_ObjectKey& object_key)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (object_key),
    object_addr_ (rendezvous_point),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  ACE_UNUSED_ARG (version);

  ACE_NEW (this->rendezvous_point_,
           char[ACE_OS::strlen (rendezvous_point) + 1]);
  ACE_OS::strcpy (this->rendezvous_point_, rendezvous_point);

  this->create_body ();
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const TAO_UIOP_Profile *pfile)
  : TAO_Profile (pfile->tag ()),
    rendezvous_point_(0),
    body_(pfile->body_),
    version_(pfile->version_),
    object_key_(pfile->object_key_),
    object_addr_(pfile->object_addr_),
    hint_(0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  ACE_NEW (this->rendezvous_point_,
           char[ACE_OS::strlen (pfile->rendezvous_point_) + 1]);
  ACE_OS::strcpy (this->rendezvous_point_, pfile->rendezvous_point_);
  hint_ = pfile->hint_;

}

TAO_UIOP_Profile::TAO_UIOP_Profile (const TAO_UIOP_Profile &pfile)
  : TAO_Profile (pfile.tag ()),
    rendezvous_point_(0),
    body_(pfile.body_),
    version_(pfile.version_),
    object_key_(pfile.object_key_),
    object_addr_(pfile.object_addr_),
    hint_(0),
    // what about refcount_lock_ (),
    refcount_ (1)
{

  ACE_NEW (this->rendezvous_point_,
           char[ACE_OS::strlen (pfile.rendezvous_point_) + 1]);
  ACE_OS::strcpy (this->rendezvous_point_, pfile.rendezvous_point_);
  hint_ = pfile.hint_;

}

TAO_UIOP_Profile::TAO_UIOP_Profile (const TAO_IOP_Version &version)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (version),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
}

TAO_UIOP_Profile::TAO_UIOP_Profile (const char *string,
                                    CORBA::Environment &env)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
{
  parse_string (string, env);
}

TAO_UIOP_Profile::TAO_UIOP_Profile (void)
  : TAO_Profile (TAO_IOP_TAG_UNIX_IOP),
    rendezvous_point_ (0),
    body_ (),
    version_ (DEF_UIOP_MAJOR, DEF_UIOP_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1)
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

//   ACE_DEBUG ((LM_DEBUG,
//               "UIOP_Profile::set -- \n"
//               "   temp_rendezvous_point: <%s>\n"
//               "   rendezvous_point:      <%s>\n",
//               temp_rendezvous_point,
//               this->rendezvous_point_));
              
  return 0;  // Success
}

TAO_UIOP_Profile::~TAO_UIOP_Profile (void)
{
  assert (this->refcount_ == 0);

  delete [] this->rendezvous_point_;
  this->rendezvous_point_ = 0;
}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_UIOP_Profile::parse (TAO_InputCDR& cdr,
                         CORBA::Boolean &continue_decoding,
                         CORBA::Environment &env)
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
                "detected new v%d.%d UIOP profile",
                this->version_.major,
                this->version_.minor));
    return 0;
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
TAO_UIOP_Profile::parse_string (const char *string,
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

  if (this->version_.major != TAO_UIOP_Profile::DEF_UIOP_MAJOR ||
      this->version_.minor  > TAO_UIOP_Profile::DEF_UIOP_MINOR)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
    }

  // Pull off the "rendezvous_pointname:" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy = CORBA::string_dup (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, ':');

  if (cp == 0)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
    }

  if (this->rendezvous_point_)
    {
      // @@ You are setting this->rendezvous_point_ using CORBA::string_alloc() a
      // couple of lines below, you should then use CORBA::string_free()
      // to release it!  In general use a single form of memory
      // allocation for a field/variable to avoid new/free() and
      // malloc/delete() mismatches.
      // Ohh, and if you are going to use CORBA::string_alloc() &
      // friends you may consider using CORBA::String_var to manage
      // the memory automatically (though there may be forces that
      // suggest otherwise).
      delete [] this->rendezvous_point_;
      this->rendezvous_point_ = 0;
    }

  this->rendezvous_point_ = CORBA::string_alloc (1 + cp - start);
  for (cp = this->rendezvous_point_; *start != ':'; *cp++ = *start++)
    continue;

  *cp = 0; start++; // increment past :

  cp = ACE_OS::strchr (start, '/');

  if (cp == 0)
    {
      CORBA::string_free (this->rendezvous_point_);
      ACE_THROW_RETURN (CORBA::MARSHAL (), -1);
    }

  this->object_addr_.set (this->rendezvous_point_);

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
//  cdr >> this->rendezvous_point_;
//  cdr >> this->object_key_;
//  this->object_addr_.set (this->rendezvous_point_);
// }

void
TAO_UIOP_Profile::create_body (void)
{
  TAO_OutputCDR cdr;
}
// @@
//   cdr << TAO_ENCAP_BYTE_ORDER;
//   cdr << this->version_.major;
//   cdr << this->version_.minor;
//   cdr << this->rendezvous_point_;
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

char *
TAO_UIOP_Profile::addr_to_string(void)
{
  static char s[MAXPATHLEN + MAXPATHLEN];
  ACE_OS::sprintf (s, "%s",
                   this->rendezvous_point_);
  return s;
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

// Memory managment

CORBA::ULong
TAO_UIOP_Profile::_incr_refcnt (void)
{
  // OK, think I got it.  When this object is created (guard) the
  // lock is automatically acquired (refcount_lock_).  Then when
  // we leave this method the destructir for guard is called which
  // releases the lock!
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
TAO_UIOP_Profile::_decr_refcnt (void)
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
TAO_UIOP_Profile::to_string (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  CORBA::String_var key;
  TAO_POA::encode_sequence_to_string (key.inout(),
                                      this->object_key ());

  u_int buflen = (ACE_OS::strlen (::prefix_) +
                  1 /* major # */ + 1 /* minor # */ + 1 /* decimal point */ +
                  2 /* double-slash separator */ +
                  ACE_OS::strlen (this->rendezvous_point_) +
                  1 /* slash separator */ +
                  ACE_OS::strlen (key) +
                  1 /* zero terminator */);

  CORBA::String buf = CORBA::string_alloc (buflen);

  static const char digits [] = "0123456789";

  ACE_OS::sprintf (buf,
                   "%s%c.%c//%s/%s",
                   ::prefix_,
                   digits [this->version_.major],
                   digits [this->version_.minor],
                   this->rendezvous_point_,
                   key.in ());
  return buf;
}

const char *
TAO_UIOP_Profile::prefix (void)
{
  return ::prefix_;
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

  CORBA::ULong rendezvous_pointlen = ACE_OS::strlen ((char *) this->rendezvous_point_);
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
