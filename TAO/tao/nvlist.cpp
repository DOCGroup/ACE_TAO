// @ (#)nvlist.cpp      
// 1.6 95/11/04
// 
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of Named Value List

#include "tao/corba.h"

// COM's IUnknown support

// {77420087-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_NamedValue, 
0x77420087, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG __stdcall
CORBA_NamedValue::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));
 
  return refcount_++;
}
 
ULONG __stdcall
CORBA_NamedValue::Release (void)
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
 
HRESULT __stdcall
CORBA_NamedValue::QueryInterface (REFIID riid,
                                  void **ppv)
{
  *ppv = 0;
 
  if (IID_CORBA_NamedValue == riid || IID_IUnknown == riid)
    *ppv = this;
 
  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);
 
 (void) AddRef ();
  return NOERROR;
}

// Reference counting for DII Request object

void
CORBA::release (CORBA::NamedValue_ptr nv)
{
  if (nv)
    nv->Release ();
}

CORBA::Boolean
CORBA::is_nil (CORBA::NamedValue_ptr nv)
{
  return (CORBA::Boolean) nv == 0;
}

CORBA_NamedValue::~CORBA_NamedValue (void)
{
  if (_name)
    CORBA::string_free ((CORBA::String) _name);
}

// COM's IUnknown support

// {77420088-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_NVList,
0x77420088, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

ULONG __stdcall
CORBA_NVList::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));
 
  return refcount_++;
}
 
ULONG __stdcall
CORBA_NVList::Release (void)
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
 
HRESULT __stdcall
CORBA_NVList::QueryInterface (REFIID riid,
                              void **ppv)
{
  *ppv = 0;
 
  if (IID_CORBA_NVList == riid || IID_IUnknown == riid)
    *ppv = this;
 
  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);
 
 (void) AddRef ();
  return NOERROR;
}

// Reference counting for DII Request object

void
CORBA::release (CORBA::NVList_ptr nvl)
{
  if (nvl)
    nvl->Release ();
}

CORBA::Boolean
CORBA::is_nil (CORBA::NVList_ptr nvl)
{
  return (CORBA::Boolean) nvl == 0;
}

CORBA_NVList::~CORBA_NVList (void)
{
  for (u_int i = 0; i < len_; i++)
    (&values_[i])->~CORBA_NamedValue ();

  if (values_)
    ACE_OS::free ((char *)values_);
  values_ = 0;
  len_ = max_ = 0;
}

CORBA::NamedValue_ptr
CORBA_NVList::add_value (const CORBA::Char *name,
                         const CORBA::Any &value,
                         CORBA::Flags flags,
                         CORBA::Environment &env)
{
  env.clear ();

  if (ACE_BIT_DISABLED (flags, CORBA::ARG_IN | CORBA::ARG_OUT | CORBA::ARG_INOUT))
    {
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
      return 0;
    }

  // We track "len_" and "max_" like sequences do; mixing the
  // "add_arg" and nvlist[i] style accessors produces undefined
  // behaviour.
  u_int len = len_++;

  // Extend the array with an _initialized_ element ... relying on
  // zeroed memory to be sufficiently initialized.
  //
  // XXX report malloc failures as errors -- how?

  if (values_ == 0) 
    {
      values_ = (CORBA::NamedValue_ptr)
        calloc (len_, sizeof (CORBA::NamedValue));
      max_ = len_;
    } 
  else if (len >= max_) 
    {
      values_ = (CORBA::NamedValue_ptr) ACE_OS::realloc ((char *)values_,
                                                        sizeof (CORBA::NamedValue) * len_);
      (void) ACE_OS::memset (&values_[max_], 0,
                             sizeof (values_[max_]) * (len_ - max_));
      max_ = len_;
    }
  assert (values_ != 0);

  values_[len]._flags = flags;
  values_[len]._name = CORBA::string_copy (name);

  if (ACE_BIT_ENABLED (flags, CORBA::IN_COPY_VALUE))
    // IN_COPY_VALUE means that the parameter is not "borrowed" by
    // the ORB, but rather that the ORB copies its value.
    //
    // Initialize the newly allocated memory using a copy
    // constructor that places the new "Any" value at just the right
    // place, and makes a "deep copy" of the data.
    (void) new (&values_[len]._any) CORBA::Any (value);
  else 
    // The normal behaviour for parameters is that the ORB "borrows"
    // their memory for the duration of calls.
    //
    // Initialize the newly allocated "Any" using a normal
    // constructor that places the new "Any" value at just the right
    // place, yet doesn't copy the memory (except for duplicating
    // the typecode).
    //
    // NOTE: DSI has yet to be updated so that it's OK to use such
    // application-allocated memory.  It needs at least a "send the
    // response now" call.
    //
    (void) new (&values_[len]._any) CORBA::Any (value.type (),
                                                (void *)value.value (), CORBA::B_FALSE);

  return &values_[len];
}
