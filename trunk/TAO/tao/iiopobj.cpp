// @ (#)iiopobj.cpp     1.9 95/11/04
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// IIOP Bridge:         CORBA::Object operations
//
// Some CORBA::Object and other operations are specific to this IIOP
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#include "tao/corba.h"

IIOP::Profile::Profile (const IIOP::Profile &src)
  : iiop_version (src.iiop_version),
    port (src.port)
{
  assert (src.iiop_version.major == MY_MAJOR);
  assert (src.iiop_version.minor == MY_MINOR);

  // Free up our host if we've got one.
  if (this->host)
    ACE_OS::free (host);
  
  this->host = ACE_OS::strdup (src.host);

  object_key.length = src.object_key.length;
  object_key.maximum = src.object_key.length;

  //  object_key.buffer = (CORBA::Octet *) ACE_OS::malloc (object_key.maximum);
  this->object_key.buffer = new CORBA::Octet [object_key.maximum];

  (void) ACE_OS::memcpy (this->object_key.buffer,
                         src.object_key.buffer,
                         this->object_key.length);
  this->set_object_addr ();
}

IIOP::Profile::Profile (const IIOP::Version &v,
                        const char *h,
                        const CORBA::UShort p,
                        const TAO_opaque &key)
  : iiop_version (v),
    port (p)
{
  this->host = ACE_OS::strdup (h);
  this->object_key.length = key.length;
  this->object_key.maximum = key.length;

  //  object_key.buffer = (CORBA::Octet *) ACE_OS::malloc (object_key.maximum);
  this->object_key.buffer = new CORBA::Octet [object_key.maximum];

  (void) ACE_OS::memcpy (this->object_key.buffer,
                         key.buffer,
                         this->object_key.length);
  this->set_object_addr ();
}

// Quick'n'dirty hash of objref data, for partitioning objrefs into sets
//
// NOTE that this must NOT go across the network!

CORBA::ULong
IIOP_Object::hash (CORBA::ULong max,
                   CORBA::Environment &env)
{
  CORBA::ULong hashval;

  env.clear ();

  // Just grab a bunch of convenient bytes and hash them; could do
  // more (hostname, full key, exponential hashing) but no real need
  // to do so except if performance requires a more costly hash.

  hashval = profile.object_key.length * profile.port;
  hashval += profile.iiop_version.minor;

  if (profile.object_key.length >= 4)
    {
      hashval += profile.object_key.buffer [1];
      hashval += profile.object_key.buffer [3];
    }

  return hashval % max;
}

// Expensive comparison of objref data, to see if two objrefs
// certainly point at the same object. (It's quite OK for this to
// return FALSE, and yet have the two objrefs really point to the same
// object.)
//
// NOTE that this must NOT go across the network!

CORBA::Boolean
IIOP_Object::is_equivalent (CORBA::Object_ptr other_obj,
                            CORBA::Environment &env)
{
  IIOP::Profile *body, *body2;
  IIOP_Object *other_iiop_obj;

  env.clear ();

  if (CORBA::is_nil (other_obj) == CORBA::B_TRUE
      || other_obj->QueryInterface (IID_IIOP_Object,
                                    (void **) &other_iiop_obj) != NOERROR)
    return CORBA::B_FALSE;
  CORBA::release (other_obj);

  // Compare all the bytes of the object address -- must be the same

  body = &profile;
  body2 = &other_iiop_obj->profile;

  ACE_ASSERT (body->object_key.length < UINT_MAX);

  return body->object_key.length == body2->object_key.length
    && ACE_OS::memcmp (body->object_key.buffer,
                       body2->object_key.buffer,
                       (size_t) body->object_key.length) == 0
    && body->port == body2->port
    && ACE_OS::strcmp (body->host, body2->host) == 0
    && body->iiop_version.minor == body2->iiop_version.minor
    && body->iiop_version.major == body2->iiop_version.major;
}

// For COM -- IUnknown operations

// {A201E4C3-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IIOP_Object,
0xa201e4c3, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG __stdcall
IIOP_Object::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->IUnknown_lock_, 0));

  return ++this->refcount_;
}

ULONG __stdcall
IIOP_Object::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->IUnknown_lock_, 0));

    ACE_ASSERT (this != 0);

    if (--this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

// Note that (as of this writing) this is the only place all the
// interfaces to an "objref" come together:
//
// IUnknown ... this one
// STUB_OBJECT ... inherited by this one
// IIOP_OBJECT ... this one
//
// CORBA::Object ... contained within this; it delegates back
//      to this one as its "parent"

HRESULT __stdcall
IIOP_Object::QueryInterface (REFIID riid,
                             void **ppv)
{
  *ppv = 0;

  if (IID_IIOP_Object == riid
      || IID_STUB_Object == riid
      || IID_IUnknown == riid)
    *ppv = this;
  else if (IID_CORBA_Object == riid)
    *ppv = &base;

  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);

 (void) AddRef ();
  return NOERROR;
}

//TAO extensions
char *IIOP_Object::_get_name (CORBA::Environment &)
{
  char *name;

  name = CORBA::string_alloc (this->profile.object_key.length);
  ACE_OS::memset (name, '\0', this->profile.object_key.length+1);
  ACE_OS::memcpy (name, this->profile.object_key.buffer,
                  this->profile.object_key.length);
  return name;
}

// Constructor
// It will usually be used by the _bind call
IIOP_Object::IIOP_Object (const char *host,
                          const CORBA::UShort port,
                          const char *objkey,
                          char *repository_id)
  : STUB_Object (repository_id),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
  // If the repository ID (typeID) is NULL, it will make narrowing
  // rather expensive, though it does ensure that type-safe narrowing
  // code gets thoroughly excercised/debugged!  Without a typeID, the
  // _narrow will be required to make an expensive remote "is_a" call.

  // we set this to use IIOP version 1.0
  this->profile.iiop_version.major = IIOP::MY_MAJOR;
  this->profile.iiop_version.minor = IIOP::MY_MINOR;

  // set the profile information
  this->profile.host = ACE_OS::strdup (host);
  this->profile.port = port;

  // set the obj key in the profile info
  this->profile.object_key.buffer = (u_char *) CORBA::string_copy (objkey);
  this->profile.object_key.length = ACE_OS::strlen (objkey);
  this->profile.object_key.maximum = this->profile.object_key.length;
}

IIOP_Object::~IIOP_Object (void)
{
  assert (refcount_ == 0);
  delete this->fwd_profile_;
}

IIOP_Object::IIOP_Object (char *repository_id)
  : STUB_Object (repository_id),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
}

IIOP_Object::IIOP_Object (char *repository_id,
			  const IIOP::Profile &a_profile)
  : STUB_Object (repository_id),
    profile (a_profile),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
}

// Constructor.  It will usually be used by the server side.
IIOP_Object::IIOP_Object (char *repository_id,
                          const ACE_INET_Addr &addr, const
                          char *objkey)
  : STUB_Object (repository_id),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
  // set up an IIOP object
  char tempname [MAXHOSTNAMELEN]; // to hold the host name

  // setup the profile information
  this->profile.iiop_version.major = IIOP::MY_MAJOR;
  this->profile.iiop_version.minor = IIOP::MY_MINOR;
  ACE_OS::memset (tempname, '\0', MAXHOSTNAMELEN);
  (void) addr.get_host_name (tempname, MAXHOSTNAMELEN); // retrieve the host
                                                        // name
  this->profile.host = ACE_OS::strdup (tempname);
  this->profile.port = addr.get_port_number ();
  this->profile.object_key.length = ACE_OS::strlen (objkey);
  this->profile.object_key.maximum = this->profile.object_key.length;
  this->profile.object_key.buffer = (u_char *) CORBA::string_copy (objkey);
}
