// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================

#include "tao/corba.h"

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                                    const char *object_key)
  : host_(0),
    port_(0),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(DEF_IIOP_MAJOR, DEF_IIOP_MINOR), 
    object_addr_ (addr),
    hint_(0)
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
  : host_(0),
    port_(0),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(DEF_IIOP_MAJOR, DEF_IIOP_MINOR), 
    object_key_ (object_key),
    object_addr_ (addr),
    hint_(0)
{
  this->set(addr);
  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                                    const Version& version,
                                    const char *object_key)
  : host_(0),
    port_(0),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(version), 
    object_addr_ (addr),
    hint_(0)
{
  this->set(addr);
  int l = ACE_OS::strlen (object_key);
  this->object_key_.length (l);

  for (int i = 0; i < l; ++i)
    this->object_key_[i] = object_key[i];

  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                                    const Version& version,
                                    const TAO_ObjectKey& object_key)
  : host_(0),
    port_(0),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(version),
    object_key_ (object_key),
    object_addr_ (addr),
    hint_(0)
{
  this->set(addr);
  this->create_body ();
}

TAO_IIOP_Profile::TAO_IIOP_Profile (const char* host,
                                    CORBA::UShort port,
                                    const TAO_ObjectKey& object_key)
  : host_(0),
    port_(port),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(DEF_IIOP_MAJOR, DEF_IIOP_MINOR), 
    object_key_(object_key),
    object_addr_(port, host),
    hint_(0)
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
                                    const Version& version,
                                    const TAO_ObjectKey& object_key)
  : host_(0),
    port_(port),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(version),
    object_key_(object_key),
    object_addr_(port, host),
    hint_(0)
{

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
    hint_(0)
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
    hint_(0)
{
  
  ACE_NEW (this->host_,
           char[ACE_OS::strlen (pfile.host_) + 1]);
  ACE_OS::strcpy (this->host_, pfile.host_);
  hint_ = pfile.hint_;

}

TAO_IIOP_Profile::TAO_IIOP_Profile (const Version &version)
  : host_(0),
    port_(0),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(version),
    hint_(0)
{
}

TAO_IIOP_Profile::TAO_IIOP_Profile (void)
  : host_(0),
    port_(0),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    version_(DEF_IIOP_MAJOR, DEF_IIOP_MINOR), 
    hint_(0)
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
    if (addr.get_host_name (temphost, sizeof temphost) != 0)
      return -1;

    temphost2 = temphost;
  }

  ACE_NEW_RETURN (this->host_,
                  char[ACE_OS::strlen (temphost2) + 1],
                  -1);
  ACE_OS::strcpy (this->host_, temphost2);

  return 0;

}

TAO_IIOP_Profile:: ~TAO_IIOP_Profile ()
{
  delete [] this->host_;
  this->host_ = 0;
}

CORBA::ULong
TAO_IIOP_Profile::tag (void)
{
  return this->tag_;
}

TAO_Transport *
TAO_IIOP_Profile::transport (void)
{
  // do I need to do a dynamic cast here?
  if (hint_) {
    return hint_->transport ();
  }
  return 0;
}

const TAO_ObjectKey & 
TAO_IIOP_Profile::object_key (void) const
{
  return this->object_key_;
}

TAO_ObjectKey & 
TAO_IIOP_Profile::object_key (TAO_ObjectKey& objkey)
{
  this->object_key_ = objkey;
  return this->object_key_;
}

TAO_ObjectKey *
TAO_IIOP_Profile::_key (CORBA::Environment &env)
{
  return new TAO_ObjectKey (this->object_key_);
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
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  if (!(cdr.read_octet (this->version_.major)
      && this->version_.major == TAO_IIOP_Profile::DEF_IIOP_MAJOR
      && cdr.read_octet (this->version_.minor)
      && this->version_.minor <= TAO_IIOP_Profile::DEF_IIOP_MINOR))
  {
    ACE_DEBUG ((LM_DEBUG, "detected new v%d.%d IIOP profile",
                          this->version_.major,
                          this->version_.minor));
    return 0;
  }

  if (this->host_) {
    delete [] this->host_;
    this->host_ = 0;
  }
   
  // Get host and port
  if (cdr.decode (CORBA::_tc_string,
                  &this->host_,
                  0,
                  env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || !cdr.read_ushort (this->port_))
  {
    env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
    ACE_DEBUG ((LM_DEBUG, "error decoding IIOP host/port"));
    return -1;
  }

  this->object_addr_.set (this->port_, this->host_);

  // ... and object key.

  // @@ This is a hack. This code was moved from encode.cpp
  //    but it is not clear to me what is going on.  So I have
  //    passed a reference to continue_decoding into this method
  //    continue_decoding is used in IIOP_Object::decode ()
  continue_decoding = cdr.decode (TC_opaque,
                                  &this->object_key_,
                                  0,
                                  env) == CORBA::TypeCode::TRAVERSE_CONTINUE;

  if (cdr.length () != 0)
  {
    env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
    ACE_DEBUG ((LM_DEBUG,
                "%d bytes out of %d left after IIOP profile data\n",
                cdr.length (), encap_len));
    return -1;
  }
  return 1;
}
  
int 
TAO_IIOP_Profile::parse_string (const char *string,
                                CORBA::Environment &env)
{

  if (!string || !*string)
    return 0;

  // Remove the "N.N//" prefix, and verify the version's one
  // that we accept
  
  if (isdigit (string [0]) && isdigit (string [2]) && string [1] == '.'
     && string [3] == '/' && string [4] == '/')
  {
    this->version_.set_version ((char) (string [0] - '0'), (char) (string [2] - '0'));
    string += 5;
  }
  else
  {
    env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
  }

  if (this->version_.major != TAO_IIOP_Profile::DEF_IIOP_MAJOR
   || this->version_.minor  > TAO_IIOP_Profile::DEF_IIOP_MINOR)
  {
    env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
    return -1;
  }

  // Pull off the "hostname:port/" part of the objref
  // Copy the string because we are going to modify it...
  CORBA::String_var copy = CORBA::string_dup (string);

  char *start = copy.inout ();
  char *cp = ACE_OS::strchr (start, ':');

  if (cp == 0)
  {
    env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
    return -1;
  }

  if (this->host_)
  {
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
    env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
    CORBA::string_free (this->host_);
    return -1;
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

const TAO_opaque& 
TAO_IIOP_Profile::body (void) const
{
  return this->body_;
}

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

  TAO_IIOP_Profile *iiop_profile =
                   ACE_dynamic_cast (TAO_IIOP_Profile *, other_profile);

  ACE_ASSERT (iiop_profile->object_key_.length () < UINT_MAX);

  return (  this->tag_ == iiop_profile->tag_
         && this->object_key_ == iiop_profile->object_key_
         && this->port_ == iiop_profile->port_
         && ACE_OS::strcmp (this->host_, iiop_profile->host_) == 0
         && this->version () == iiop_profile->version ());
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

ACE_INET_Addr&
TAO_IIOP_Profile::object_addr (const ACE_INET_Addr *addr)
{
  if (addr != 0)
    this->object_addr_ = *addr;
  else if (this->host_)
    this->object_addr_.set (this->port_, this->host_);
  return this->object_addr_;
}

ACE_INET_Addr&
TAO_IIOP_Profile::object_addr (void)
{
  return this->object_addr_;
}

char *
TAO_IIOP_Profile::addr_to_string(void)
{
  static char s[MAXHOSTNAMELEN + MAXHOSTNAMELEN];
  ACE_OS::sprintf (s, "%s:%d",
                   this->host_, port_);
  return s;
}

char *
TAO_IIOP_Profile::host (void)
{
  return this->host_;
}

char *
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

CORBA::UShort
TAO_IIOP_Profile::port (void)
{
  return this->port_;
}

CORBA::UShort
TAO_IIOP_Profile::port (CORBA::UShort p)
{
  return this->port_ = p;
}

Version *
TAO_IIOP_Profile::version (void)
{
  return &this->version_;
}

Version *
TAO_IIOP_Profile::version (Version *v)
{
  this->version_ = *v;
  return &this->version_;
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

TAO_Client_Connection_Handler *&
TAO_IIOP_Profile::hint(void)
{
  return this->hint_;
}

TAO_Profile *
TAO_IIOP_Profile::_nil (void)
{
  return (TAO_IIOP_Profile *)0;
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

// TAO_IIOP_Profile::decode (TAO_InputCDR &str)
// {
//   
