// @(#)request.cpp	1.6 95/09/24
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of Dynamic Invocation Interface
//
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/stub.h"
#include "tao/cdr.h"
#include "tao/nvlist.h"

// {77420085-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Request,
0x77420085, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG __stdcall
CORBA_Request::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));
 
  return refcount_++;
}
 
ULONG __stdcall
CORBA_Request::Release (void)
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
CORBA_Request::QueryInterface (REFIID riid,
			       void **ppv)
{
  *ppv = 0;
 
  if (IID_CORBA_Request == riid || IID_IUnknown == riid)
    *ppv = this;
 
  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);
 
  (void) AddRef ();
  return NOERROR;
}

// Reference counting for DII Request object

void
CORBA_release (CORBA_Request_ptr req)
{
  if (req)
    req->Release ();
}

CORBA_Boolean
CORBA_is_nil (CORBA_Request_ptr req)
{
  return (CORBA_Boolean) req == 0;
}


// DII Request class implementation

CORBA_Request::CORBA_Request (CORBA_Object_ptr obj,
			      const CORBA_Char *op,
			      CORBA_NVList_ptr args,
			      CORBA_NamedValue_ptr result,
			      CORBA_Flags flags) 
  : _args (args),
    _result (result),
    _flags (flags),
    refcount_ (1)
{
  _target = CORBA_Object::_duplicate (obj);
  _opname = CORBA_string_copy (op);
}

CORBA_Request::CORBA_Request (CORBA_Object_ptr obj,
			      const CORBA_Char *op) 
  : _flags (0),
    refcount_ (1)
{
  _target = CORBA_Object::_duplicate (obj);
  _opname = CORBA_string_copy (op);

  _args = new CORBA_NVList;
  _result = new CORBA_NamedValue;
}

CORBA_Request::~CORBA_Request (void)
{
  assert (refcount_ == 0);

  CORBA_release (_target);
  CORBA_string_free ((CORBA_String)_opname);
  CORBA_release (_args);
  CORBA_release (_result);
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

  if (_target->QueryInterface (IID_STUB_Object,
			       (void **) &stub) != NOERROR) 
    {
      _env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
      return;
    }

  stub->do_dynamic_call ((char *)_opname,
			 CORBA_B_TRUE,
			 _args,
			 _result,
			 _flags,
			 _exceptions,
			 _env);
  stub->Release ();
}

void
CORBA_Request::send_oneway (void)
{
  STUB_Object *stub;

  if (_target->QueryInterface (IID_STUB_Object,
			       (void **) &stub) != NOERROR) 
    {
      _env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
      return;
    }

  stub->do_dynamic_call ((char *)_opname,
			 CORBA_B_TRUE,
			 _args,
			 _result,
			 _flags,
			 _exceptions,
			 _env);
  stub->Release ();
}
