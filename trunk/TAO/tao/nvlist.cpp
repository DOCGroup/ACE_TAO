// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    nvlist.cpp
//
// = AUTHOR
//     @ (#)nvlist.cpp
//     1.6 95/11/04
//
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     All Rights Reserved
//     and
//     Aniruddha Gokhale (additions, missing operations)
//
// ============================================================================

// Implementation of Named Value List and NamedValue classes

#include "tao/corba.h"


// =Methods on class NamedValue


// COM's IUnknown support

// {77420087-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_NamedValue,
0x77420087, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

CORBA::ULong
CORBA_NamedValue::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));

  return refcount_++;
}

CORBA::ULong
CORBA_NamedValue::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->lock_, 0));

    ACE_ASSERT (this != 0);

    if (--this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

TAO_HRESULT
CORBA_NamedValue::QueryInterface (REFIID riid,
                                  void **ppv)
{
  *ppv = 0;

  if (IID_CORBA_NamedValue == riid || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

 (void) AddRef ();
  return TAO_NOERROR;
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
  if (this->name_)
    {
      CORBA::string_free (this->name_);
      this->name_ = 0;
    }
  // the Any will be destroyed on its own as we hold an instance and not a
  // pointer to Any
}


// =Methods on class NVList

// COM's IUnknown support

// {77420088-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_NVList,
0x77420088, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

CORBA::ULong
CORBA_NVList::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));

  return refcount_++;
}

CORBA::ULong
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

TAO_HRESULT
CORBA_NVList::QueryInterface (REFIID riid,
                              void **ppv)
{
  *ppv = 0;

  if (IID_CORBA_NVList == riid || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

 (void) AddRef ();
  return TAO_NOERROR;
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
  // destroy each NamedValue element
  for (CORBA::ULong i = 0; i < this->max_; i++)
    (&this->values_[i])->~CORBA_NamedValue ();

  if (this->values_)
    ACE_OS::free ((char *)values_);

  this->values_ = 0;
  this->len_ = this->max_ = 0;
}

// add an element and just initialize its flags
CORBA::NamedValue_ptr
CORBA_NVList::add (CORBA::Flags flags,
                   CORBA::Environment &env)
{
  CORBA::ULong len = this->len_; // next slot

  // call the helper to allocate a NamedValue element (if necessary)
  if (!this->add_element (flags, env))
    return 0;

  // now initialize the fields
  this->values_[len].flags_ = flags;
  return &this->values_[len];
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA_NVList::add_item (const char *name,
                        CORBA::Flags flags,
                        CORBA::Environment &env)
{
  CORBA::ULong len = this->len_; // next slot

  // call the helper to allocate a NamedValue element (if necessary)
  if (!this->add_element (flags, env))
    return 0;

  // now initialize the fields
  this->values_[len].flags_ = flags;
  this->values_[len].name_ = CORBA::string_dup (name);
  return &this->values_[len];
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA_NVList::add_value (const char *name,
                         const CORBA::Any &value,
                         CORBA::Flags flags,
                         CORBA::Environment &env)
{
  CORBA::ULong len = this->len_; // next slot

  // call the helper to allocate a NamedValue element (if necessary)
  if (!this->add_element (flags, env))
    return 0;

  // now initialize the fields
  this->values_[len].flags_ = flags;
  this->values_[len].name_ = CORBA::string_dup (name); // make a copy

  if (ACE_BIT_ENABLED (flags, CORBA::IN_COPY_VALUE))
    // IN_COPY_VALUE means that the parameter is not "borrowed" by
    // the ORB, but rather that the ORB copies its value.
    //
    // Initialize the newly allocated memory using a copy
    // constructor that places the new "Any" value at just the right
    // place, and makes a "deep copy" of the data.
    (void) new (&this->values_[len].any_) CORBA::Any (value);
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
    (void) new (&this->values_[len].any_) CORBA::Any (value.type (),
                                                      (void *)value.value (),
                                                      CORBA::B_FALSE); // does
                                                                       // not
                                                                       // own

  // return pointer to the newly inserted member
  return &this->values_[len];
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA_NVList::add_item_consume (char *name,
                                CORBA::Flags flags,
                                CORBA::Environment &env)
{
  CORBA::ULong len = this->len_; // next slot

  // call the helper to allocate a NamedValue element (if necessary)
  if (!this->add_element (flags, env))
    return 0;

  // now initialize the fields
  this->values_[len].flags_ = flags;
  this->values_[len].name_ = name; // we consume it
  return &this->values_[len];
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA_NVList::add_value_consume (char * /*name*/,
                                 CORBA::Any * /*value*/,
                                 CORBA::Flags /*flags*/,
                                 CORBA::Environment &/*env*/)
{
  // not implemented because we need to see how we can consume the value
  // One soln is to make the any_ member of NamedValue to be a Any_ptr or
  // Any_var
  // XXXASG- TODO
  return 0;
}

//CORBA::Status
void
CORBA_NVList::remove (CORBA::ULong /*n*/, CORBA::Environment &/*env*/)
{
  // not implemented
  // XXXASG - TODO

}

// Helper method
CORBA::Boolean
CORBA_NVList::add_element (CORBA::Flags flags, CORBA::Environment &env)
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
  CORBA::ULong len = this->len_++; // len_ points to the next available
                                   // slot. Access is by zero based indexing

  // Extend the array with an _initialized_ element ... relying on
  // zeroed memory to be sufficiently initialized.
  //

  if (this->values_ == 0) // list was created as an empty list
    {
      this->values_ = (CORBA::NamedValue_ptr)
        ACE_OS::calloc (this->len_, sizeof (CORBA::NamedValue));
      this->max_ = this->len_;
    }
  else if (len >= max_) // need reallocation
    {
      // reallocate a larger buffer
      this->values_ = (CORBA::NamedValue_ptr) ACE_OS::realloc
        ((char *)this->values_, sizeof (CORBA::NamedValue) * this->len_);
      // now zero the elements that have been newly allocated
      (void) ACE_OS::memset (&this->values_[this->max_], 0,
                             sizeof (values_[this->max_]) * (this->len_ - this->max_));
      this->max_ = this->len_; // set the new maximum size
    }
  if (!this->values_)
    {
      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
      return 0;
    }

  return 1; // success
}
