// This may look like C, but it's really -*- C++ -*-
// $Id$

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/LSOCK_Profile.h"
#include "tao/GIOP.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"
#include "tao/debug.h"

ACE_RCSID(tao, LSOCK_Profile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/LSOCK_Profile.i"
#endif /* __ACE_INLINE__ */

static const char *prefix_ = "lsock:";

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const ACE_UNIX_Addr& addr,
                                      const char *object_key)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_LSOCK_MAJOR, DEF_LSOCK_MINOR),
    object_key_ (),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
  this->set (addr);
  int l = ACE_OS::strlen (object_key);
  this->object_key_.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key_[i] = object_key[i];

  this->create_body ();
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const ACE_UNIX_Addr& addr,
                                      const TAO_ObjectKey& object_key)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_LSOCK_MAJOR, DEF_LSOCK_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
  this->set (addr);
  this->create_body ();
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const ACE_UNIX_Addr& addr,
                                      const TAO_IOP_Version& version,
                                      const char *object_key)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (version),
    object_key_ (),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
  this->set (addr);
  int l = ACE_OS::strlen (object_key);
  this->object_key_.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key_[i] = object_key[i];

  this->create_body ();
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const ACE_UNIX_Addr& addr,
                                      const TAO_IOP_Version& version,
                                      const TAO_ObjectKey& object_key)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (version),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
  this->set (addr);
  this->create_body ();
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const char* rendezvous_point,
                                      const TAO_ObjectKey& object_key)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_LSOCK_MAJOR, DEF_LSOCK_MINOR),
    object_key_ (object_key),
    object_addr_ (rendezvous_point),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{

  if (rendezvous_point)
  {
    ACE_NEW (this->rendezvous_point_,
             char[ACE_OS::strlen (rendezvous_point) + 1]);
    ACE_OS::strcpy (this->rendezvous_point_, rendezvous_point);
  }

  this->create_body ();
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const char* rendezvous_point,
                                      const TAO_ObjectKey& object_key,
                                      const ACE_UNIX_Addr& addr)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_LSOCK_MAJOR, DEF_LSOCK_MINOR),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{

  if (rendezvous_point)
  {
    ACE_NEW (this->rendezvous_point_,
             char[ACE_OS::strlen (rendezvous_point) + 1]);
    ACE_OS::strcpy (this->rendezvous_point_, rendezvous_point);
  }

  this->create_body ();
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const char* rendezvous_point,
                                      const TAO_IOP_Version& version,
                                      const TAO_ObjectKey& object_key)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_LSOCK_MAJOR, DEF_LSOCK_MINOR),
    object_key_ (object_key),
    object_addr_ (rendezvous_point),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
  ACE_UNUSED_ARG (version);

  ACE_NEW (this->rendezvous_point_,
           char[ACE_OS::strlen (rendezvous_point) + 1]);
  ACE_OS::strcpy (this->rendezvous_point_, rendezvous_point);

  this->create_body ();
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const TAO_LSOCK_Profile *pfile)
  : rendezvous_point_(0),
    tag_(pfile->tag_),
    body_(pfile->body_),
    version_(pfile->version_),
    object_key_(pfile->object_key_),
    object_addr_(pfile->object_addr_),
    hint_(0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{

  ACE_NEW (this->rendezvous_point_,
           char[ACE_OS::strlen (pfile->rendezvous_point_) + 1]);
  ACE_OS::strcpy (this->rendezvous_point_, pfile->rendezvous_point_);
  hint_ = pfile->hint_;

}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const TAO_LSOCK_Profile &pfile)
  : rendezvous_point_(0),
    tag_(pfile.tag_),
    body_(pfile.body_),
    version_(pfile.version_),
    object_key_(pfile.object_key_),
    object_addr_(pfile.object_addr_),
    hint_(0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{

  ACE_NEW (this->rendezvous_point_,
           char[ACE_OS::strlen (pfile.rendezvous_point_) + 1]);
  ACE_OS::strcpy (this->rendezvous_point_, pfile.rendezvous_point_);
  hint_ = pfile.hint_;

}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const TAO_IOP_Version &version)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (version),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (const char *string,
                                      CORBA::Environment &env)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_LSOCK_MAJOR, DEF_LSOCK_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
  parse_string (string, env);
}

TAO_LSOCK_Profile::TAO_LSOCK_Profile (void)
  : rendezvous_point_ (0),
    tag_ (TAO_IOP_TAG_INTERNET_IOP),
    body_ (),
    version_ (DEF_LSOCK_MAJOR, DEF_LSOCK_MINOR),
    object_key_ (),
    object_addr_ (),
    hint_ (0),
    // what about refcount_lock_ (),
    refcount_ (1),
    forward_to_ (0)
{
}

int
TAO_LSOCK_Profile::set (const ACE_UNIX_Addr& addr)
{
  char temp_rendezvous_point[MAXPATHLEN + 1];
  const char *temp_rendezvous_point2 = 0;

//   if (TAO_ORB_Core_instance ()->orb_params ()->use_dotted_decimal_addresses ())
//     {
//       temp_rendezvous_point2 = addr.get_rendezvous_point_addr ();
//       if (temp_rendezvous_point2 == 0)
//         return -1;
//     }
//   else
//     {
      if (addr.get_rendezvous_point_name (temprendezvous_point,
                                          sizeof (temp_rendezvous_point)) != 0)
        return -1;

      temp_rendezvous_point2 = temp_rendezvous_point;
//     }

  ACE_NEW_RETURN (this->rendezvous_point_,
                  char[ACE_OS::strlen (temp_rendezvous_point2) + 1],
                  -1);
  ACE_OS::strcpy (this->rendezvous_point_, temp_rendezvous_point2);

  return 0;

}

TAO_LSOCK_Profile::~TAO_LSOCK_Profile (void)
{
  assert (this->refcount_ == 0);

  delete [] this->rendezvous_point_;
  this->rendezvous_point_ = 0;

  if (forward_to_)
    {
      delete forward_to_;
    }

}

// return codes:
// -1 -> error
//  0 -> can't understand this version
//  1 -> success.
int
TAO_LSOCK_Profile::parse (TAO_InputCDR& cdr,
                          CORBA::Boolean &continue_decoding,
                          CORBA::Environment &env)
{
  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring LSOCK
  // profiles whose versions we don't understand.
  // FIXME:  Version question again,  what do we do about them for this
  //         protocol?

  if (!(cdr.read_octet (this->version_.major)
        && this->version_.major == TAO_LSOCK_Profile::DEF_LSOCK_MAJOR
        && cdr.read_octet (this->version_.minor)
        && this->version_.minor <= TAO_LSOCK_Profile::DEF_LSOCK_MINOR))
  {
    ACE_DEBUG ((LM_DEBUG,
                "detected new v%d.%d LSOCK profile",
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
      ACE_DEBUG ((LM_DEBUG, "error decoding LSOCK rendezvous_point"));
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
                  "%d bytes out of %d left after LSOCK profile data\n",
                  cdr.length (),
                  encap_len));
    }
  if (cdr.good_bit ())
    return 1;

  return -1;
}

int
TAO_LSOCK_Profile::parse_string (const char *string,
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
    ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO), 0);

  if (this->version_.major != TAO_LSOCK_Profile::DEF_LSOCK_MAJOR
      || this->version_.minor  > TAO_LSOCK_Profile::DEF_LSOCK_MINOR)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO), -1);
    }

  // Pull off the "rendezvous_pointname:" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy = CORBA::string_dup (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, ':');

  if (cp == 0)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO), -1);
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
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO), -1);
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
TAO_LSOCK_Profile::create_body (void)
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
TAO_LSOCK_Profile::is_equivalent (TAO_Profile *other_profile,
                                  CORBA::Environment &env)
{
  env.clear ();

  if (other_profile->tag () != TAO_IOP_TAG_INTERNET_IOP)
    return 0;

  TAO_LSOCK_Profile *op =
    ACE_dynamic_cast (TAO_LSOCK_Profile *, other_profile);

  ACE_ASSERT (op->object_key_.length () < UINT_MAX);

  return this->object_key_ == op->object_key_
    && ACE_OS::strcmp (this->rendezvous_point_, op->rendezvous_point_) == 0
    && this->version_ == op->version_;
}

CORBA::ULong
TAO_LSOCK_Profile::hash (CORBA::ULong max,
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
TAO_LSOCK_Profile::addr_to_string(void)
{
  static char s[MAXPATHLEN + MAXPATHLEN];
  ACE_OS::sprintf (s, "%s",
                   this->rendezvous_point_);
  return s;
}

const char *
TAO_LSOCK_Profile::rendezvous_point (const char *r)
{
  if (this->rendezvous_point_)
    {
      delete [] this->rendezvous_point_;
      this->rendezvous_point_ = 0;
    }

  if (h)
    {
      ACE_NEW_RETURN (this->rendezvous_point_,
                      char[ACE_OS::strlen (r) + 1],
                      0);
      ACE_OS::strcpy (this->rendezvous_point_, r);
    }

  return this->rendezvous_point_;
}

void
TAO_LSOCK_Profile::reset_hint (void)
{
  if (this->hint_)
    {
      this->hint_->cleanup_hint ();
      this->hint_ = 0;
    }
}

TAO_LSOCK_Profile &
TAO_LSOCK_Profile::operator= (const TAO_LSOCK_Profile &src)
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
TAO_LSOCK_Profile::_incr_refcnt (void)
{
  // OK, think I got it.  When this object is created (guard) the
  // lock is automatically acquired (refcount_lock_).  Then when
  // we leave this method the destructir for guard is called which
  // releases the lock!
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
TAO_LSOCK_Profile::_decr_refcnt (void)
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


void
TAO_LSOCK_Profile::forward_to (TAO_MProfile *mprofiles)
{
  // we assume ownership of the profile list!!
  if (forward_to_)
    delete this->forward_to_;

  ACE_NEW (this->forward_to_,
           TAO_MProfile (mprofiles));

}

TAO_MProfile *
TAO_LSOCK_Profile::forward_to (void)
{
  TAO_MProfile *temp;

  ACE_NEW_RETURN (temp,
                  TAO_MProfile (this->forward_to_),
                  0);
  return temp;
}

CORBA::String
TAO_LSOCK_Profile::to_string (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  CORBA::String_var key;
  TAO_POA::encode_sequence_to_string (key.inout(),
                                      this->object_key ());

  u_int buflen = (ACE_OS::strlen (::prefix_) +
                  1 /* major # */ + 1 /* minor # */ +
                  2 /* double-slash separator */ +
                  ACE_OS::strlen (this->rendezvous_point_) +
                  1 /* colon separator */ +
                  5 /* port number */ +
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
TAO_LSOCK_Profile::prefix (void)
{
  return ::prefix_;
}

int
TAO_LSOCK_Profile::encode (TAO_OutputCDR &stream) const
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

  CORBA::ULong rendezvous_pointlen = ACE_OS::strlen ((char *) this->rendezvous_point_);
  CORBA::ULong encap_len =
    1                              // byte order
    + 1                            // version major
    + 1                            // version minor
    + 1                            // pad byte
    + 4                            // sizeof (strlen)
    + rendezvous_pointlen + 1                  // strlen + null
    + (~rendezvous_pointlen & 01)              // optional pad byte
    + 2                            // port
    + ( rendezvous_pointlen & 02)              // optional pad short
    + 4                            // sizeof (key length)
    + this->object_key_.length (); // key length.
  stream.write_ulong (encap_len);

  // CHAR describing byte order, starting the encapsulation
  stream.write_octet (TAO_ENCAP_BYTE_ORDER);

  // LSOCK::TAO_IOP_Version, two characters (version 1.0) padding
  stream.write_char (this->version_.major);
  stream.write_char (this->version_.minor);

  // STRING rendezvous_pointname from profile
  stream.write_string (this->rendezvous_point_);

  // OCTET SEQUENCE for object key
  stream << this->object_key_;

  return 1;
}

#endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
