// $Id$

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

  // Enough room as to print a <void *>.
  // @@ The following "32" should not be a magic #...
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
    this->object_key[i] = key[i];

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
      // Special case.
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
      const char *tempaddr = addr.get_host_addr ();
      if (tempaddr == 0)
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
      // Special case.
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
      const char *tempaddr = addr.get_host_addr ();
      if (tempaddr == 0)
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
    if (rhs[i] != lhs[i])
      return 0;

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

  // Compare all the bytes of the object address -- must be the same.

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

    if (--this->refcount_ > 0)
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
TAO_HRESULT
IIOP_Object::QueryInterface (REFIID riid,
                             void **ppv)
{
  *ppv = 0;

  if (IID_IIOP_Object == riid
      || IID_STUB_Object == riid
      || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

 (void) AddRef ();
  return TAO_NOERROR;
}

// TAO extensions
TAO_ObjectKey*
IIOP_Object::key (CORBA::Environment &)
{
  return new TAO_ObjectKey (this->profile.object_key);
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
    refcount_ (1),
    fwd_profile_ (0)
{
}

// THREADING NOTE: Code below this point is of course thread-safe (at
// least on supported threaded platforms), so the caller of these
// routines need only ensure that the data being passed in is not
// being modified by any other thread.
//
// As an _experiment_ (to estimate the performance cost) remote calls
// are currently deemed "cancel-safe".  That means that they can be
// called by threads when they're in asynchronous cancellation mode.
// The only effective way to do this is to disable async cancellation
// for the duration of the call.  There are numerous rude interactions
// with code generators for C++ ... cancellation handlers just do
// normal stack unwinding like exceptions, but exceptions are purely
// synchronous and sophisticated code generators rely on that to
// generate better code, which in some cases may be very hard to
// unwind.

class TAO_Synchronous_Cancellation_Required
  // = TITLE
  //     Stick one of these at the beginning of a block that can't
  //     support asynchronous cancellation, and which must be
  //     cancel-safe.
  //
  // = EXAMPLE
  //     somefunc()
  //     {
  //       TAO_Synchronous_Cancellation_Required NOT_USED;
  //       ...
  //     }
{
public:
  // These should probably be in a separate inline file, but they're
  // only used within this one file right now, and we always want them
  // inlined, so here they sit.
  TAO_Synchronous_Cancellation_Required (void)
    {
#if !defined (VXWORKS)
      ACE_OS::thr_setcanceltype (THR_CANCEL_DEFERRED, &old_type_);
#endif /* ! VXWORKS */
    }

  ~TAO_Synchronous_Cancellation_Required (void)
    {
#if !defined (VXWORKS)
      int dont_care;
      ACE_OS::thr_setcanceltype(old_type_, &dont_care);
#endif /* ! VXWORKS */
    }
private:
  int old_type_;
};

// "Stub interpreter" for static stubs.  IDL compiler (or human
// equivalent thereof :-) should just dump a read-only description of
// the call into "calldata" and do varargs calls to this routine,
// which does all the work.

void
IIOP_Object::do_static_call (CORBA::Environment &env,   // exception reporting
                             const TAO_Call_Data *info, // call description
                             ...)                       // ... any parameters

{
  TAO_Synchronous_Cancellation_Required NOT_USED;

  TAO_GIOP_Invocation call (this,
                            info->opname,
                            info->is_roundtrip);

  // We may need to loop through here more than once if we're
  // forwarded to some other object reference.
  //
  // NOTE: A quality-of-service policy may be useful to establish
  // here, specifically one controlling how many times the call is
  // reissued before failing the call on the assumption that something
  // is broken.
  //
  // NOTE: something missing is a dynamic way to change the policy of
  // whether to issue LocateRequest messages or not.  This code uses a
  // simple, fixed policy: never use LocateRequest messages.
  //
  for (;;)
    {
      // Start the call by constructing the request message header.

      env.clear ();
      call.start (env);

      if (env.exception ())
        {
          dexc (env, "do_static_call, start request message");
          return;
        }

      // Now, put all "in" and "inout" parameters into the request
      // message body.
      //
      // Some "inout" data have an extra level of indirection,
      // specified by the language mapping's memory allocation
      // policies ... the indirection only shows up here when it's
      // needed later for allocating "out" memory, otherwise there's
      // just one indirection.

      u_int i;
      const TAO_Param_Data *pdp;
      va_list param_vector;

      va_start (param_vector, info);

      for (i = 0, pdp = info->params;
           i < info->param_count;
           i++, pdp++)
        {
          void *ptr = va_arg (param_vector, void *);

          if (pdp->mode == PARAM_IN)
            call.put_param (pdp->tc, ptr, env);
          else if (pdp->mode == PARAM_INOUT)
            {
              if (pdp->value_size == 0)
                call.put_param (pdp->tc, ptr, env);
              else
                call.put_param (pdp->tc, *(void **)ptr, env);
            }

          if (env.exception ())
            {
              dexc (env, "do_static_call, put request parameter");
              return;
            }
        }
      va_end (param_vector);

      // Make the call ... blocking for response if needed.  Note that
      // "oneway" calls can't return any exceptions except system
      // ones.

      TAO_GIOP_ReplyStatusType status;
      CORBA::ExceptionList exceptions;

      exceptions.length = exceptions.maximum = info->except_count;
      exceptions.buffer = (CORBA::TypeCode_ptr *) info->excepts;

      status = call.invoke (exceptions, env);

      exceptions.buffer = 0;            // don't free it

      if (env.exception ())
        {
          dexc (env, "do_static_call, invoke");
          return;
        }
      if (!info->is_roundtrip
          || status == TAO_GIOP_SYSTEM_EXCEPTION
          || status == TAO_GIOP_USER_EXCEPTION)
        return;

      // Now, get all the "return", "out", and "inout" parameters from
      // the response message body.

      if (status == TAO_GIOP_NO_EXCEPTION)
        {
          va_start (param_vector, info);
          for (i = 0, pdp = info->params;
               i < info->param_count;
               i++, pdp++)
            {
              void *ptr = va_arg (param_vector, void *);

              // if it is an inout parameter, it would become necessary to
              // first release the "in" memory
              if (pdp->mode == PARAM_INOUT)
                {
                  // @@ - add others as we test each case
                  // (ASG) will do 03/22/98.
                  switch (pdp->tc->kind (env))
                    {
                    case CORBA::tk_string:
                      {
                        CORBA::string_free (*(char **)ptr);
                        *(char **)ptr = 0;
                      }
                      break;
                    default:
                      break;
                    }
                }
              if (pdp->mode == PARAM_RETURN
                  || pdp->mode == PARAM_OUT
                  || pdp->mode == PARAM_INOUT)
                {
                  // The language mapping's memory allocation policy
                  // says that some data is heap-allocated.  This
                  // interpreter is told about the relevant policy by
                  // whoever built the operation description (e.g. the
                  // IDL compiler) so it doesn't have to know the
                  // policy associated with a particular language
                  // binding (e.g. C/C++ differ, and C++ even has
                  // different policies for different kinds of
                  // structures).
                  if (pdp->value_size == 0)
                    call.get_value (pdp->tc, ptr, env);
                  else
                    {
                      // assert (value_size == tc->size());
                      *(void **)ptr = new CORBA::Octet [pdp->value_size];
                      call.get_value (pdp->tc, *(void **)ptr, env);
                    }

                  if (env.exception ())
                    {
                      dexc (env, "do_static_call, get reply parameter");
                      return;
                    }
                }
            }
          va_end (param_vector);
          return;
        }

      // ... or maybe this request got forwarded to someplace else; send
      // the request there instead.
      assert (status == TAO_GIOP_LOCATION_FORWARD);
    }
}

// DII analogue of the above.  Differs in how the vararg calling
// convention is implemented -- DII doesn't use the normal call stack
// with its implicit typing, but iinstead uses heap-based arguments
// with explicit typing.

void
IIOP_Object::do_dynamic_call (const char *opname,               // operation name
                              CORBA::Boolean is_roundtrip,      // results required?
                              CORBA::NVList_ptr args,           // parameters
                              CORBA::NamedValue_ptr result,     // result
                              CORBA::Flags flags,               // per-call flag (one!)
                              CORBA::ExceptionList &exceptions, // possible user exceptions
                              CORBA::Environment &env)          // exception reporting
{
  TAO_Synchronous_Cancellation_Required NOT_USED;

  TAO_GIOP_Invocation call (this, opname, is_roundtrip);

  // Loop as needed for forwarding; see above.

  for (;;)
    {
      // Start the call by constructing the request message header.
      env.clear ();
      call.start (env);

      if (env.exception ())
        {
          dexc (env, "do_static_call, start request message");
          return;
        }

      // Now, put all "in" and "inout" parameters into the request
      // message body.

      u_int i;

      for (i = 0; i < args->count (); i++)
        {
          CORBA::NamedValue_ptr value = args->item (i, env);

          if (value->flags () == CORBA::ARG_IN
              || value->flags () == CORBA::ARG_INOUT)
            {
              call.put_param (value->value ()->type (),
                              (void *) value->value ()->value (), env);
              if (env.exception ())
                {
                  dexc (env, "do_dynamic_call, put request parameter");
                  return;
                }
            }
        }

      // Make the call ... blocking for response if needed.  Note that
      // "oneway" calls can't return any exceptions except system ones.

      TAO_GIOP_ReplyStatusType status;

      status = call.invoke (exceptions, env);
      if (env.exception ())
        {
          dexc (env, "do_dynamic_call, invoke");
          return;
        }
      if (!is_roundtrip
          || status == TAO_GIOP_SYSTEM_EXCEPTION
          || status == TAO_GIOP_USER_EXCEPTION)
        return;

      // Now, get all the "return", "out", and "inout" parameters from the
      // response message body ... return parameter is first, the rest are
      // in the order defined in the IDL spec (which is also the order that
      // DII users are required to use).

      if (status == TAO_GIOP_NO_EXCEPTION)
        {
          if (result != 0)
            {
              // If caller didn't set OUT_LIST_MEMORY flag, allocate
              // memory for return value ...

              if (!(flags & CORBA::OUT_LIST_MEMORY))
                {
                  CORBA::TypeCode_ptr tcp;
                  size_t size;

                  tcp = result->value ()->type ();
                  size = tcp->size (env);
                  dexc (env, "do_dynamic_call, get result size");

                  if (size != 0)
                    {
                      void *ptr = new CORBA::Octet [size];

                      tcp->AddRef ();
                      result->value ()->replace (tcp, ptr,
                                                 CORBA::B_TRUE, env);
                      dexc (env, "do_dynamic_call, set result mem");
                    }
                }

              call.get_value (result->value ()->type (),
                              (void *) result->value ()->value (), env);
            }

          for (i = 0; i < args->count (); i++)
            {
              CORBA::NamedValue_ptr     value = args->item (i, env);

              if (value->flags () == CORBA::ARG_OUT
                  || value->flags () == CORBA::ARG_INOUT)
                {
                  // If caller didn't set OUT_LIST_MEMORY flag, allocate
                  // memory for this parameter ...
                  if (!(flags & CORBA::OUT_LIST_MEMORY))
                    {
                      CORBA::TypeCode_ptr tcp;
                      size_t size;

                      tcp = value->value ()->type ();
                      size = tcp->size (env);
                      dexc (env, "do_dynamic_call, get param size");

                      if (size != 0)
                        {
                          void *ptr = new CORBA::Octet [size];

                          tcp->AddRef ();
                          value->value ()->replace (tcp, ptr,
                                                    CORBA::B_TRUE, env);
                          dexc (env, "do_dynamic_call, set result mem");
                        }
                    }

                  call.get_value (value->value ()->type (),
                                  (void *) value->value ()->value (), env);
                  if (env.exception ())
                    {
                      dexc (env, "do_dynamic_call, get response parameter");
                      return;
                    }
                }
            }
          return;
        }

      // ... or maybe this request got forwarded to someplace else.
      assert (status == TAO_GIOP_LOCATION_FORWARD);
    }
}
