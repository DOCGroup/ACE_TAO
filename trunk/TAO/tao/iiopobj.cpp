// @ (#)iiopobj.cpp     1.9 95/11/04
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// IIOP Bridge:         CORBA::Object operations
//
// Some CORBA::Object and other operations are specific to this IIOP
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/stub.h"
#include "tao/iiopobj.h"

#if !defined (__ACE_INLINE__)
#  include "iiopobj.i"
#endif /* __ACE_INLINE__ */
#endif /* 0 */

#include "tao/corba.h"

IIOP::ProfileBody::ProfileBody (const IIOP::ProfileBody &src)
  : iiop_version (src.iiop_version),
    port (src.port)
{
  assert (src.iiop_version.major == MY_MAJOR);
  assert (src.iiop_version.minor == MY_MINOR);

  host = ACE_OS::strdup (src.host);

  object_key.length = object_key.maximum = src.object_key.length;

  //  object_key.buffer = (CORBA::Octet *) ACE_OS::malloc (object_key.maximum);
  object_key.buffer = new CORBA::Octet [object_key.maximum];

 (void) ACE_OS::memcpy (object_key.buffer,
                         src.object_key.buffer,
                         object_key.length);
}

IIOP::ProfileBody::ProfileBody (const IIOP::Version &v,
                                const CORBA::String &h,
                                const CORBA::UShort &p,
                                const TAO_opaque &key)
  : iiop_version (v),
    port (p)
{
  host = ACE_OS::strdup (h);
  
  object_key.length = object_key.maximum = key.length;

  //  object_key.buffer = (CORBA::Octet *) ACE_OS::malloc (object_key.maximum);
  object_key.buffer = new CORBA::Octet [object_key.maximum];

 (void) ACE_OS::memcpy (object_key.buffer, key.buffer,
                         object_key.length);
}

// Quick'n'dirty hash of objref data, for partitioning objrefs into sets
//
// NOTE that this must NOT go across the network!

CORBA::ULong
IIOP_Object::hash (CORBA::ULong max,
                   CORBA::Environment &env)
{
  CORBA::ULong   hashval;

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
  IIOP::ProfileBody *body, *body2;
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
    && ACE_OS::strcmp ((char *) body->host, (char *) body2->host) == 0
    && body->iiop_version.minor == body2->iiop_version.minor
    && body->iiop_version.major == body2->iiop_version.major;
}

// For COM -- IUnknown operations

// {A201E4C3-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IIOP_Object,
0xa201e4c3, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

#if 0
// Added by BRM: 2/21/97
// IID_STUB_Object and IID_CORBA::Object were not being defined.
// Need a central place for all of these macros.

#if 0 //defined(WIN32)
EXTERN_C ACE_Svc_Export GUID IID_STUB_Object = 
{0xa201e4c7, 0xf258, 0x11ce, {0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98} };
#else
DEFINE_GUID(IID_STUB_Object,
0xa201e4c7, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);
#endif /* 0 */

// {A201E4C2-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA::Object,
0xa201e4c2, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// End - Added by BRM: 2/21/97 IID_STUB_Object and IID_CORBA::Object
// were not being defined.  Need a central place for all of these
// macros.
#endif /* 0 */

ULONG __stdcall
IIOP_Object::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));

  return ++refcount_;
}

ULONG __stdcall
IIOP_Object::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, mon, this->lock_, 0));

    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return refcount_;
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
CORBA::String IIOP_Object::_get_name (CORBA::Environment &)
{
  return (CORBA::String) this->profile.object_key.buffer;
}
