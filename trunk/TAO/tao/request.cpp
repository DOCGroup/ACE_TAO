// @(#)request.cpp	1.6 95/09/24
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of Dynamic Invocation Interface

#include "tao/corba.h"

CORBA::ULong
CORBA_Request::AddRef (void)
{
  return refcount_++;
}

CORBA::ULong
CORBA_Request::Release (void)
{
  {
    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
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

  CORBA::release (this->target_);
  CORBA::string_free ((CORBA::String) this->opname_);
  this->opname_ = 0;
  CORBA::release (this->args_);
  CORBA::release (this->result_);
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
  STUB_Object *stub = this->target_->_get_parent ();
  stub->AddRef ();

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
  STUB_Object *stub = this->target_->_get_parent ();
  stub->AddRef ();

  stub->do_dynamic_call ((char *) opname_,
			 CORBA::B_TRUE,
			 args_,
			 result_,
			 flags_,
			 exceptions_,
			 env_);
  stub->Release ();
}
