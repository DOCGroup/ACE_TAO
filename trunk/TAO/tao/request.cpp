// @(#)request.cpp	1.6 95/09/24
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of Dynamic Invocation Interface

#include "tao/corba.h"

// {77420085-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Request,
0x77420085, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG
CORBA_Request::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));

  return refcount_++;
}

ULONG
CORBA_Request::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->lock_, 0));

    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

TAO_HRESULT
CORBA_Request::QueryInterface (REFIID riid,
			       void **ppv)
{
  *ppv = 0;

  if (IID_CORBA_Request == riid || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

  (void) AddRef ();
  return TAO_NOERROR;
}

// Reference counting for DII Request object

void
CORBA::release (CORBA::Request_ptr req)
{
  if (req)
    req->Release ();
}

CORBA::Boolean
CORBA::is_nil (CORBA::Request_ptr req)
{
  return (CORBA::Boolean) req == 0;
}


// DII Request class implementation

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
			      const CORBA::Char *op,
			      CORBA::NVList_ptr args,
			      CORBA::NamedValue_ptr result,
			      CORBA::Flags flags)
  : args_ (args),
    result_ (result),
    flags_ (flags),
    refcount_ (1)
{
  target_ = CORBA::Object::_duplicate (obj);
  opname_ = CORBA::string_copy (op);
}

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
			      const CORBA::Char *op)
  : flags_ (0),
    refcount_ (1)
{
  target_ = CORBA::Object::_duplicate (obj);
  opname_ = CORBA::string_copy (op);

  args_ = new CORBA::NVList;
  result_ = new CORBA::NamedValue;
  result_->AddRef();
}

CORBA_Request::~CORBA_Request (void)
{
  assert (refcount_ == 0);

  CORBA::release (target_);
  CORBA::string_free ((CORBA::String) opname_);
  CORBA::release (args_);
  CORBA::release (result_);
}

// The public DII interfaces:  normal and oneway calls.
//
// NOTE that using DII, programmers can get the special behaviour of
// discarding the response for normal calls.  This doesn't change the
// semantics of any OMG-IDL interface, it just streamlines control
// flow in some exotic situations.

void
CORBA_Request::invoke (void)
{
  STUB_Object *stub;

  if (target_->QueryInterface (IID_STUB_Object,
			       (void **) &stub) != TAO_NOERROR)
    {
      env_.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return;
    }

  stub->do_dynamic_call ((char *) opname_,
			 CORBA::B_TRUE,
			 args_,
			 result_,
			 flags_,
			 exceptions_,
			 env_);
  stub->Release ();
}

void
CORBA_Request::send_oneway (void)
{
  STUB_Object *stub;

  if (target_->QueryInterface (IID_STUB_Object,
			       (void **) &stub) != TAO_NOERROR)
    {
      env_.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));
      return;
    }

  stub->do_dynamic_call ((char *) opname_,
			 CORBA::B_TRUE,
			 args_,
			 result_,
			 flags_,
			 exceptions_,
			 env_);
  stub->Release ();
}
