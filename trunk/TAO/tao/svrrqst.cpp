// @(#)svrrqst.cpp	1.9 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of the Dynamic Server Skeleton Interface

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/svrrqst.h"
#include "tao/nvlist.h"
#include "tao/debug.h"
#endif

#include "tao/corba.h"

// {77420086-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IIOP_ServerRequest,
0x77420086, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {4B48D881-F7F0-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_ServerRequest,
0x4b48d881, 0xf7f0, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

#if !defined (__ACE_INLINE__)
#  include "svrrqst.i"
#endif /* __ACE_INLINE__ */

IIOP_ServerRequest::~IIOP_ServerRequest (void)
{
  ACE_ASSERT (refcount_ == 0);

  if (params_)
    CORBA::release (params_);
  if (retval_)
    delete retval_;
  if (exception_)
    delete exception_;
}
 
ULONG __stdcall
IIOP_ServerRequest::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));
 
  ACE_ASSERT (refcount_ > 0);
  return refcount_++;
}
 
ULONG __stdcall
IIOP_ServerRequest::Release (void)
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
 
HRESULT __stdcall
IIOP_ServerRequest::QueryInterface (REFIID riid,
				    void **ppv)
{
  ACE_ASSERT (refcount_ > 0);
  *ppv = 0;
 
  if (IID_IIOP_ServerRequest == riid
      || IID_CORBA_ServerRequest == riid
      || IID_IUnknown == riid)
    *ppv = this;
 
  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);
 
 (void) AddRef ();
  return NOERROR;
}

// Unmarshal in/inout params, and set up to marshal the appropriate
// inout/out/return values later on.

void __stdcall
IIOP_ServerRequest::params (CORBA::NVList_ptr list,
			    CORBA::Environment &env)
{
  env.clear ();

  // Save params for later use when marshaling the reply.
  this->params_ = list;

  // Then unmarshal each "in" and "inout" parameter.
  for (u_int i = 0; i < list->count (); i++) 
    {
      CORBA::NamedValue_ptr nv = list->item (i);

      if (ACE_BIT_DISABLED (nv->flags (), CORBA::ARG_IN | CORBA::ARG_INOUT))
	continue;

      // First, make sure the memory into which we'll be unmarshaling
      // exists, and is the right size.
      //
      // NOTE: desirable to have a way to let the dynamic
      // implementation routine preallocate this data, for
      // environments where DSI is just being used in lieu of a
      // language mapped server-side API and the size is really
      // knowable in advance.

      CORBA::Any_ptr any = nv->value ();
      CORBA::TypeCode_ptr tc = any->type ();

      tc->AddRef ();
      ACE_NEW (value, char [tc->size (env)]);

      void *value;
      any->replace (tc, value, CORBA::B_TRUE, env);

      // Decrement the refcount of "tc".
      //
      // The earlier AddRef is needed since Any::replace () releases
      // the typecode inside the Any.  Without the dup, the reference
      // count can go to zero, and the typecode would then be deleted.
      //
      // This Release ensures that the reference count is correct so
      // the typecode can be deleted some other time.

      tc->Release ();

      // Then just unmarshal the value.
      (void) incoming_->decode (tc, value, 0, env);
    }

  // If any data is left over, it'd be context values ... else error.
  // We don't support context values, so it's always an error.

  if (incoming_->bytes_remaining () != 0) 
    {
      dmsg1 ("params (), %d bytes remaining (error)", 
	     incoming_->bytes_remaining ());
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
    }
}

// Store the result value.  There's either an exception, or a result,
// but not both of them.  Results (and exceptions) can be reported
// only after the parameter list has been provided (maybe empty).

void __stdcall
IIOP_ServerRequest::result (CORBA::Any_ptr value,
			    CORBA::Environment &env)
{
  env.clear ();

  if (!params_ || retval_ || exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else
    retval_ = value;
    
  // XXX send the message now!
}

// Store the exception value.

void __stdcall
IIOP_ServerRequest::exception (CORBA::ExceptionType type,
			       CORBA::Any_ptr value,
			       CORBA::Environment &env)
{
  if (!params_ || retval_ || exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else 
    {
      env.clear ();
      exception_ = value;
      ex_type_ = type;
    }

  // XXX send the message now!
}

// Invocation attributes.

CORBA::String __stdcall
IIOP_ServerRequest::op_name (void)
{
  return opname_;
}

CORBA::Object_ptr __stdcall
IIOP_ServerRequest::target (void)
{
  // XXX implement me!!  Code from TCP_OA exists ...
  return 0;
}

CORBA::Principal_ptr __stdcall
IIOP_ServerRequest::caller (void)
{
  // XXX ... return client's principal
  return 0;
}

CORBA::ORB_ptr __stdcall
IIOP_ServerRequest::orb (void)
{
  return orb_;
}

CORBA::BOA_ptr __stdcall
IIOP_ServerRequest::oa (void)
{
  return boa_;
}
