
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

int
IIOP::Profile::set (const char *h,
                    const CORBA::UShort p,
                    const ACE_INET_Addr *addr)
{
  this->iiop_version.major = IIOP::MY_MAJOR;
  this->iiop_version.minor = IIOP::MY_MINOR;

  if (this->host)
    {
      delete [] this->host;
      this->host = 0;
    }

  this->port = p;

  if (h)
    {
      ACE_NEW_RETURN (this->host,
                      char[ACE_OS::strlen(h) + 1],
                      -1);
      ACE_OS::strcpy (this->host, h);
    }

  this->object_addr (addr);
  return 0;
}

int
IIOP::Profile::set (const char *h,
                    const CORBA::UShort p,
                    const char *key,
                    const ACE_INET_Addr *addr)
{
  if (this->set (h, p, addr) == -1)
    return -1;

  // Enough room as to print a <void*>
  const int bufs = 32;
  char buffer[bufs];
  if (key == 0)
    {
      // @@ (IRFAN) Object key generation
      // Use <this> as the key...
      ACE_OS::sprintf (buffer, "0x%024.24x", this);
      key = buffer;
    }

  int l = ACE_OS::strlen (key);
  this->object_key.length (l);

  for (int i = 0; i < l; ++i)
    {
      this->object_key[i] = key[i];
    }
  return 0;
}

int
IIOP::Profile::set (const char *h,
                    const CORBA::UShort p,
                    const TAO_opaque &key,
                    const ACE_INET_Addr *addr)
{
  if (this->set (h, p, addr) == -1)
    return -1;

  this->object_key = key;

  return 0;
}

IIOP::Profile::Profile (const IIOP::Profile &src)
  : host (0)
{
  (void) this->set (src.host,
                    src.port,
                    src.object_key,
                    &src.object_addr_);
}

int
IIOP::Profile::set (const ACE_INET_Addr &addr,
                    const char *key)
{
  // Set up an IIOP Profile to hold the host name.

  if (addr.get_ip_address () == INADDR_ANY)
    {
      // Special case
      char temphost[MAXHOSTNAMELEN + 1];
      if (addr.get_host_name (temphost, sizeof(temphost)) != 0)
        return -1;
      else
        return this->set (temphost,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
  else
    {
      const char* tempaddr = 0;
      if ((tempaddr = addr.get_host_addr ()) == 0)
        return -1;
      else
        return this->set (tempaddr,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
}

int
IIOP::Profile::set (const ACE_INET_Addr &addr,
                    const TAO_opaque &key)
{
  // Set up an IIOP Profile to hold the host name.

  if (addr.get_ip_address () == INADDR_ANY)
    {
      // Special case
      char temphost[MAXHOSTNAMELEN + 1];
      if (addr.get_host_name (temphost, sizeof(temphost)) != 0)
        return -1;
      else
        return this->set (temphost,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
  else
    {
      const char* tempaddr = 0;
      if ((tempaddr = addr.get_host_addr ()) == 0)
        return -1;
      else
        return this->set (tempaddr,
                          addr.get_port_number (),
                          key,
                          &addr);
    }
}

IIOP::Profile::Profile (const char *h,
                        const CORBA::UShort p,
                        const char *key)
  : host (0)
{
  (void) this->set (h, p, key);
}

IIOP::Profile::Profile (const char *h,
                        const CORBA::UShort p,
                        const char *key,
                        const ACE_INET_Addr &addr)
  : host (0)
{
  (void) this->set (h, p, key, &addr);
}

IIOP::Profile::Profile (const ACE_INET_Addr &addr,
                        const char *key)
  : host (0)
{
  (void) this->set (addr, key);
}

IIOP::Profile::Profile (const ACE_INET_Addr &addr,
                        const TAO_opaque &key)
  : host (0)
{
  (void) this->set (addr, key);
}

// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
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

  hashval = profile.object_key.length () * profile.port;
  hashval += profile.iiop_version.minor;

  if (profile.object_key.length () >= 4)
    {
      hashval += profile.object_key [1];
      hashval += profile.object_key [3];
    }

  return hashval % max;
}

int operator==(const TAO_opaque& rhs,
	       const TAO_opaque& lhs)
{
  if (rhs.length () != lhs.length ())
    return 0;

  for (CORBA::ULong i = 0;
       i < rhs.length ();
       ++i)
    {
      if (rhs[i] != lhs[i])
	return 0;
    }
  return 1;
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
                                    (void **) &other_iiop_obj) != TAO_NOERROR)
    return CORBA::B_FALSE;
  CORBA::release (other_obj);

  // Compare all the bytes of the object address -- must be the same

  body = &profile;
  body2 = &other_iiop_obj->profile;

  ACE_ASSERT (body->object_key.length () < UINT_MAX);

  return body->object_key == body2->object_key
    && body->port == body2->port
    && ACE_OS::strcmp (body->host, body2->host) == 0
    && body->iiop_version.minor == body2->iiop_version.minor
    && body->iiop_version.major == body2->iiop_version.major;
}

// For COM -- IUnknown operations

// {A201E4C3-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IIOP_Object,
0xa201e4c3, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG
IIOP_Object::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->IUnknown_lock_, 0));

  return ++this->refcount_;
}

ULONG
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

TAO_HRESULT
IIOP_Object::QueryInterface (REFIID riid,
                             void **ppv)
{
  *ppv = 0;

  if (IID_IIOP_Object == riid
      || IID_STUB_Object == riid
      || IID_TAO_IUnknown == riid)
    *ppv = this;
  else if (IID_CORBA_Object == riid)
    *ppv = &base;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

 (void) AddRef ();
  return TAO_NOERROR;
}

//TAO extensions
const char *
IIOP_Object::_get_name (CORBA::Environment &)
{
  // @@ We need access to the underlying buffer.
  return (const char *) &(this->profile.object_key[0]);
}

// It will usually be used by the _bind call.
//
// Note that if the repository ID (typeID) is NULL, it will make
// narrowing rather expensive, though it does ensure that type-safe
// narrowing code gets thoroughly exercised/debugged!  Without a
// typeID, the _narrow will be required to make an expensive remote
// "is_a" call.

IIOP_Object::IIOP_Object (const char *host,
                          const CORBA::UShort port,
                          const char *objkey,
                          char *repository_id)
  : STUB_Object (repository_id),
    profile (host, port, objkey),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
}

// Constructor.  It will usually be used by the server side.
IIOP_Object::IIOP_Object (char *repository_id,
                          const ACE_INET_Addr &addr,
                          const char *objkey)
  : STUB_Object (repository_id),
    profile (addr, objkey),
    base (this),
    refcount_ (1),
    fwd_profile_ (0)
{
}
